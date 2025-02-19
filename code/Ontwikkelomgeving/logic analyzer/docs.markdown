h1. Week 01 arduino en logic analyser

*Vraag:* Wat is het verschil is het gedecodeerde TX-signaal als we in de Arduino-code print of println gebruiken?
*Antwoord:* Bij het gebruik van println worden 2 extra karakters (bytes) verstuurd, namelijk een carriage return en een line feed

*Vraag:* Als we de pulsbreedte van de LED-pin opvragen met de   - knop blijkt dit nooit exact 10 msec te zijn, maar net iets meer. Waarom klopt het niet?
*Antwoord:* Het uitvoeren van het omhoog of omlaag zetten van de LED-pin kost tijd, wat na de 10 msec sleep plaatsvindt. hierdoor is de pulsebreedte altijd hoger dan 10 msec

*Vraag:* Als we de breedte van een startbit opvragen met de   - knop, waarom staat er dan bij de frequentie/pulsbreedte niet altijd 9600 Hz, terwijl we dat wel ingesteld hebben in de Arduino? Ligt het misschien aan de meting?
*Antwoord:* De logic analyzer die gebruikt wordt is plusminus 10 euro en kan dus gevoelig zijn voor fouten. Ook de gebruikte arduino uno is aan de oudere kant. Ook kan de innerlijke klok van een arduino uno R3 lichtelijk achterlopen met de realiteit.

*Vraag:* Hoe lang duurt het na het omklappen van de LED-pin voordat het verzenden van het eerste bit van de Serial.print is gestart en kun je dit antwoord verklaren?
*Antwoord:* Dit duurt ongeveer 12 microseconden. voordat er data verstuurd kan worden moet het eerst in een buffer gestopt worden. dit kost tijd

*Vraag:* Wat gebeurt er als de pulsbreedte van de LED-pin inkorten, terwijl de bitrate van de TX hetzelfde blijft?
*Antwoord:* Er wordt meer data verzonden, aangezien de delay blocking is.

*Vraag:* Wat is de maximale knipperfrequentie als we de delay() en de println() weghalen?
*Antwoord:* 6750 nanoseconden voor een aan-en-uit cyclus

*Vraag:* Wat is de maximale knipperfrequentie als we gebruik maken van directe IO-poort-manipulatie zoals beschreven in https://www.arduino.cc/en/Reference/PortManipulation?
*Antwoord:* 500 nanoseconden voor een aan-en-uit cyclus
