# Benötigte Bibliotheken:

1. Meine CAN-Bus Library hier auf Github
2. SimpleTimer Library, über die Arduino IDE zu finden

# Informationen

Code ist für ein CAN-Bus Interface mit 8MHz Quartz. Achtung, Spannung und Strom sind für eine Zero eingestellt (116,2V und 32A)! Bitte entsprechend anpassen.

Mit einem Poti auf A0 kann der Ladestrom stufenlos eingestellt werden. Statusnachrichten und Fehlermeldungen vom Lader werden auf der Konsole ausgegeben. Alternativ kann dafür ein Display verwendet werden.

## Benötigte Hardware:

Statt eines Arduino Nano kann auch ein beliebiger anderer Arduino mit SPI-Schnittstelle verwendet werden, z.B. Pro Mini, Uno oder Mega2560. Bestellung am besten bei Amazon oder Banggood, dort gibt es auch gute Starterkits und Zubehör. 

Die CAN-Bus Interfaces solltet ihr besser nicht woanders bestellen, ich habe oft fehlerhafte bekommen. Mit dem Lieferanten auf Amazon habe ich ausschließlich gute Erfahrungen gemacht. Es schadet auch nicht einen in Reserve zu haben falls ihr mal einen zerschießt.

Arduino Nano:
https://www.amazon.de/Elegoo-Entwicklerboard-Arduino-Atmega328P-CH340/dp/B0713ZRJLC/

oder:
https://www.banggood.com/3Pcs-ATmega328P-Nano-V3-Controller-Board-Compatible-Arduino-Improved-Version-p-983486.html

CAN-Bus Interfaces:
https://www.amazon.de/gp/product/B01IV3ZSKO

Alternativ funktioniert auch das Leonardo-Board von Hobbytronics sehr gut, dann reduziert sich auch die Lötarbeit:
http://www.hobbytronics.co.uk/leonardo-canbus

Zusätzlich wird folgendes benötigt:
-Lötkolben
-dünne Kabel

## Installation:

Die gesamte Library im .zip Format herunterladen und in die Arduino IDE importieren.

## Verwendung:

Am besten in die Beispiele schauen. Die Datei "Einstiegsbeispiel" enthält alle wichtigen Funktionen mit Beschreibungen.

## Nützliche Tutorials:

https://www.youtube.com/watch?v=cKf3MwjL2fc
https://www.youtube.com/watch?v=ZHaxv-cGZFI

# TCCharger-voltage-current-control

TCCharger voltage & current control
