#include <WiFi.h>
#include <HTTPClient.h>
#include <iostream>
#include <string>
using namespace std;
#include <SPI.h>
#include <iomanip>

//const int CS_PIN = 5; // Chip select pin for the MAX7219

const int MAX7219_COUNT = 3; // Number of MAX7219 modules in the chain
const int CS_PINS[] = {5, 6, 7};  //Chip select pins for the MAX7219 modules


void CongigMAX7219() {
  // Send configuration commands to each MAX7219 module
  for (int i = 0; i < MAX7219_COUNT; i++) {
    sendCommand(i, 0x09, 0xFF); // Decode mode
    sendCommand(i, 0x0A, 0x0F); // Intensity: medium brightness
    sendCommand(i, 0x0B, 0x07); // Scan limit: all digits
    sendCommand(i, 0x0C, 0x01); // Shutdown: normal operation
    sendCommand(i, 0x0F, 0x00); // Display test: off
  }
}*/

/*const int CS_PIN = 5; // Chip select pin for the MAX7219
 
void CongigMAX7219(){
  sendCommand(0x09,  0xFF); // Decode mode
  sendCommand(0x0A, 0x0F); // Intensity: medium brightness
  sendCommand(0x0B, 0x07); // Scan limit: all digits
  sendCommand(0x0C, 0x01); // Shutdown: normal operation
  sendCommand(0x0F, 0x00); // Display test: off
}*/

//To control 8 digits using a MAX7219
void Affiche(int val[8]){
  for(int i=0 ;i<8;i++)
  drawDigit(i+1,  val[i]);
}
void Affichelocaltime(){
    drawDigit(1,  1);
    drawDigit(2,  0);
    drawDigit(3,  5);
    drawDigit(4,  7);
}

void Affiche(int val[8], int displayIndex) {
  // Send data for each digit to the corresponding MAX7219 module
  for (int i = 0; i < 8; i++) {
    drawDigit(i + 1, val[i], displayIndex);
  }
}



//To separate the tens and units and stock it in M
void afficheDigit(int T[6][2], int M[8]){
  int K=0;
  for(int i=0;i<6;i++){
    for(int j=0;j<2;j++){
      M[K]=T[i][j]/10;
      M[K+1]=T[i][j]%10;
      K=K+2;
    }
  }
}

const char* ssid = "Escanor sama";
const char* password = "12345689";
/*void sendCommand(byte address, byte data) {
  digitalWrite(CS_PIN, LOW); // Select the MAX7219
  SPI.transfer(address); // Send the address byte
  SPI.transfer(data); // Send the data byte
  digitalWrite(CS_PIN, HIGH); // Deselect the MAX7219
}
*/

void sendCommand(int displayIndex, byte address, byte data) {
  digitalWrite(CS_PINS[displayIndex], LOW); // Select the MAX7219
  SPI.transfer(address); // Send the address byte
  SPI.transfer(data); // Send the data byte
  digitalWrite(CS_PINS[displayIndex], HIGH); // Deselect the MAX7219
}

/*void drawDigit(byte digit, int value) {
  sendCommand(digit, value); // Set the segments of the digit based on the value
}*/

void drawDigit(byte digit, int value, int displayIndex) {
  sendCommand(displayIndex, digit, value); // Set the segments of the digit based on the value
}

// Function to transfer data from source MAX7219 to target MAX7219
void transferData(int sourceDisplayIndex, int targetDisplayIndex, byte digit, int value) {
  sendCommand(targetDisplayIndex, digit, value); // Set the segments of the digit on the target MAX7219
}


void separerHM(String *time,int (&T)[6][2]){
  for (int i = 0; i < 6; i++) {
        // Separate the Hours and min
        int pos = time[i].indexOf(":");
        T[i][0] = time[i].substring(0, pos).toInt(); // heures
        T[i][1] = time[i].substring(pos + 1).toInt(); // minutes
      //affiche
      cout << "Heures : " << setfill('0') << setw(2) << T[i][0] << endl;
      cout << "Minutes : " << setfill('0') << setw(2) <<  T[i][1] << endl;
 }
}

void setup() {
  Serial.begin(115200);
  delay(1000);SPI.begin(); // Initialize SPI communication
  SPI.begin(18, 19, 23, 5);
  SPI.setFrequency(10000000);
  SPI.setDataMode(SPI_MODE0);
  for(int i=0;i<3;i++){
  pinMode(CS_PINS[i], OUTPUT);
  } // Set the chip select pin as an output
  CongigMAX7219();
  delay(100);
 }
  

void loop(){
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected to WiFi!");

  // Make HTTP request
  HTTPClient http;
  String payload;
  http.begin("https://www.habous.gov.ma/prieres/horaire_hijri_fr.php?ville=1");
  int httpCode = http.GET();
  if (httpCode > 0) {
    payload = http.getString();
  }
  // Extract prayers for a specific day
     
  size_t start = payload.indexOf("<tr align=\"center\" class=\"cournt\" ");
  while (start != -1) {
        size_t end = payload.indexOf("</tr>", start);
        String trContent = payload.substring(start, end);
        
        // Found the desired day, extract prayers
        String prayerTags[8] = {"Hijri","Month","Alfajr", "Chourouq", "Dhuhr", "Asr", "Maghrib", "Ishae"};
        String prayers[8];
        size_t tdStart = trContent.indexOf("<td>");
        size_t tdEnd;
        for (int i = 0; i < 8; i++) {
            tdStart = trContent.indexOf("<td>", tdStart+3);
            tdEnd = trContent.indexOf("</td>", tdStart);
            prayers[i] = trContent.substring(tdStart + 4, tdEnd); // 4 is to skip "<td>"
        }
        // Print the prayers
        Serial.print(": ");
        for (int i = 0; i < 8; i++) {
            Serial.print(prayerTags[i]);
            Serial.print(": ");
            Serial.print(prayers[i]);
            Serial.print("  ");
        }
        Serial.println();
        String time[6];
        for (int i = 0; i < 6; i++){
          time[i]=prayers[i+2];
        }
        for (int i = 0; i < 6; i++) {
            Serial.print(prayerTags[i+2]);
            Serial.print(": ");
            Serial.print(time[i]);
            Serial.print("  ");
        }
       /* int T[6][2];
        separerHM(time, T);
        int M[8];
        
        afficheDigit(T,M);  
         Serial.print(" aa ");
        for (int i=0;i<24;i++){
          Serial.print(M[i]);
        }
        Affiche(M,2);     
    */
       int T[6][2];
       int targetDisplayIndex=0;
        separerHM(time, T);
        int M[24];
        afficheDigit(T,M);
        for (int i=0;i<24;i++){
          Serial.print(M[i]);
        }
       // Loop through the array in chunks of 8 and transfer to MAX7219 modules
        for (int i = 0; i < 24; i += 8) {
       // Extract 8 values from the array
        int chunk[8];
        for (int j = 0; j < 8; j++) {
        chunk[j] = M[i + j];
        }
    
    // Transfer the chunk to the target MAX7219 module
    Affiche(chunk, targetDisplayIndex);
    for(i=0;i<8;i++){
    transferData(0,1,i+1,chunk[i]);
    transferData(1,2,i+1,chunk[i]);    
    }
    
    // Wait for a short delay before transferring the next chunk
    delay(1000);
  }
    
        break;
        
        start = payload.indexOf("<tr align=\"center\" class=\"cournt\" style=\"border-bottom: 2px solid #69633F !important;\">", end);
  delay(3600);
}
}

