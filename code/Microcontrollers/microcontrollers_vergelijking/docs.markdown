# 1. Microcontrollers

## 1.1 Inleiding

In dit onderzoek wordt een vergelijking gemaakt tussen verschillende microcontrollers op basis van een vooraf gedefinieerde set criteria. De methodologie die hierbij wordt toegepast is "Multi-criteria decision making" (Bonestroo, et al., 2018), waarmee we verschillende eigenschappen van de microcontrollers op een gestructureerde manier kunnen analyseren. Het onderzoek bestaat uit twee onderdelen: in het eerste onderdeel wordt er een  literatuuronderzoek uitgevoerd om inzicht te krijgen in de eigenschappen van de microcontrollers. In het tweede onderdeel worden metingen verricht door middel van labonderzoek, waarin specifieke criteria verder worden onderzocht.

## 1.2 Onderzoeksvraag

De hoofdvraag van dit onderzoek luidt:  
**"Wat zijn de voor een IoT-toepassing relevante eigenschappen van microcontrollers?"**

Deze vraag wordt beantwoord door verschillende microcontrollers met elkaar te vergelijken aan de hand van een set van vooraf gedefinieerde criteria. Aangezien de lijst van mogelijke eigenschappen van een microcontroller zeer uitgebreid kan zijn, wordt in dit onderzoek de focus gelegd op de volgende eigenschappen:

- Vcc
- Power consumption
- Clock
- RAM
- Flash
- #IO
- #PWM
- #Timers
- #UART
- #I2C
- #SPI
- #CAN
- #Wifi
- #ADC
- #DAC
- Crypto
- FPU

Deze eigenschappen zijn geselecteerd omdat ze direct van invloed kunnen zijn op de prestaties en efficiëntie van een microcontroller in een IoT-systeem.

## 1.3 Voorgeschreven microcontrollers

De microcontrollers die in dit onderzoek worden vergeleken, zijn afkomstig van de volgende ontwikkelborden.

- **Arduino Uno R3**
- **ATTiny45**
- **Lolin MCU V2 met ESP8266**
- **ESP32**
- **Raspberry Pi**

# 2. Wat zijn de eigenschappen van de microcontrollers?

## 2.1 Arduino Uno R3

|| *Arduino Uno R3* | *ATTiny45* | *Lolin MCU V2 met ESP8266* | *ESP32* | *Raspberry Pi 4 2GB* ||
|-------------------------|-------------------------------|---------------------------|--------------------------------|-----------------------------|-----------------------------|-------------------------------|
| **Vcc** |5V & 3.3V| 5V | |||| 
| **Power consumption** |98.4 mA| 0.1 - 5 mA | ||||
| **Clock** |16mHz|8mHz| ||||
| **RAM** |2Kb|256 bytes| ||||
| **Flash** |32Kb|4Kb| ||||
| **IO**|20|5| ||||
| **PWM** |6|3| ||||
| **Timers**|2 8-bit timers en 1 16-bit timer| 2 8-bit timers| ||||
| **UART**|pin 0 (rx) en pin 1 (tx). Ook via USB-kabel| Nee | ||||
| **I2C** |Ja|ja| ||||
| **SPI** |Ja| Ja| ||||
| **CAN** |Nee, externe modules wel mogelijk|Nee, externe modules wel mogelijk| ||||
| **Wifi**|nee, externe modules wel mogelijk|Nee, externe modules wel mogelijk| ||||
| **ADC** |6|3| ||||
| **DAC** |Nee|Nee| ||||
| **Crypto**|Nee|Nee| ||||
| **FPU** |Nee|Nee| ||||

# Geciteerde werken
Bonestroo, W., Meesters, M., Niels, R., Schagen, J., Henneke, L., & van Turnhout, K. (2018). ICT Research Methods. Amsterdam: HBO-i.

https://diyi0t.com/arduino-reduce-power-consumption/
https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
https://support.arduino.cc/hc/en-us/articles/9350537961500-Use-PWM-output-with-Arduino
https://ww1.microchip.com/downloads/en/devicedoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf

