# **MAWAQIT**

## Tout d'abord : 

Le panneau de contrôle de prière dans les mosquées est essentiel pour informer les fidèles des horaires de prière. 
Cependant, l'affichage de l'heure peut parfois être imprécis, avec des écarts de 2 à 3 minutes par rapport à l'heure réelle. 
Cette inexactitude est corrigée par MAWAQIT, un panneau de contrôle de prière qui garantit des horaires précis en temps réel.

Le défi d'assurer la performance, la fiabilité et la stabilité du système repose sur la sélection primordiale des composants matériels. Pour garantir la réception d'informations précises et , nous devons  :

- **-Choisir des composants adaptés pour assurer la précision de l'affichage et la fiabilité de la communication.**
- **-Collecter et filtrer les données avec rigueur afin d'éviter les erreurs d'affichage.**
- **-Configurer précisément les chiffres sur les écrans LED pour garantir des informations exactes.**

## Aperçu de l'ESP32

L'ESP32 est un microcontrôleur peu énergivore et économique, fabriqué par la société chinoise Espressif Systems. Il est conçu pour être utilisé dans une grande variété d'applications, telles que l'Internet des objets (IoT), les systèmes embarqués, les applications de contrôle industriel, les capteurs, les dispositifs de communication sans fil, etc.

## Aperçu du MAX7219

Le MAX7219 présente plusieurs avantages :
- Il permet le contrôle de plusieurs LEDs en série avec un seul circuit intégré.
- Il prend en charge plusieurs modes d'affichage, y compris des chiffres, des lettres et des symboles personnalisés.
- Il permet de contrôler la luminosité des affichages LED et de sélectionner le nombre de colonnes à afficher.
- Il est facile à programmer avec une simple interface série.
- Il peut être utilisé avec différents types d'affichages LED, tels que les affichages à sept segments et les matrices LED.
- Il offre une faible consommation d'énergie grâce à son mode veille.

## Format de Transmission du MAX7219

Le MAX7219 utilise un format de transmission de 16 bits de D0 à D15, représenté dans le tableau ci-dessous :

- Les huit premiers bits de D0 à D7 représentent les données. D0 étant le bit le moins significatif (LSB) et D7 le bit le plus significatif (MSB).
- Les quatre bits suivants de D8 à D11 sont utilisés pour les adresses (registre).
- Les quatre derniers bits de D12 à D15 restent cependant inutilisés.

## Récupération et Filtrage des Données

Après avoir connecté l'ESP32 au réseau WIFI, l'accès au site Web des horaires de prière de HABOUS permet de récupérer les données nécessaires. Cette récupération de données est facilitée par une fonction de filtrage des données implémentée dans notre code.

## Connexion et Contrôle du MAX7219

Nous enchaînons ensuite les 3 MAX7219 en série pour contrôler l'affichage des 6 prières, ce qui se traduit par 24 chiffres 7SEG. Chaque MAX7219 contrôle 8 chiffres puisqu'il fonctionne avec 16 bits.

<div style="display: flex; align-items: center;">
  <img src="https://github.com/user-attachments/assets/367254f1-6455-4fb1-9954-c1a4470fed6b" alt="Image 2" style="height: 200px; object-fit: contain;"/>
</div>


## Séquence d'Affichage des Prières

Les horaires de prière sont affichés de manière séquentielle, en commençant par AL FAJR et CHOROK, suivis d'ADOHR et AL ASR, et enfin AL MAGHRIB et AL ICHAA.

## Protocole Utilisé

Le SPI (Serial Peripheral Interface) est un protocole de communication de données utilisé pour connecter des dispositifs externes à un microcontrôleur. Nous l'avons utilisé entre le microcontrôleur et le MAX7219 pour contrôler efficacement les affichages LED.


