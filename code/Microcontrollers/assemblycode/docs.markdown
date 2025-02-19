- **Over hoeveel clockcycles per seconde beschikt de Arduino Uno?**  
  De Arduino Uno R3 beschikt over 16.000.000 clockcycles per seconde.

- **Wat is de relatie tussen assembly code en clockcycles?**  
  Elke assembly-instructie kost een aantal clockcycles voor de Arduino Uno om uit te voeren. Minder assembly-instructies betekent minder clockcycles en dus snellere uitvoering van het programma.

- **Als je programmeert schrijf je geen assembly code. Wie genereert deze code en waarom?**  
  De **compiler** zet de C-code om naar assembly. Dit gebeurt omdat de processor zelf geen C-code begrijpt, maar wel assembly-instructies.

- **Assembly code instructies worden uitgevoerd door welk deel van de processor in onderstaande afbeelding?**  
  De **CU** (Control Unit) voert de assembly-instructies uit. Voor berekeningen wordt de **ALU** (Arithmetic Logic Unit) gebruikt.

- **Waarom resulteert dezelfde code niet in dezelfde assembly code als je je target verandert (en opnieuw compileert) van bv een Arduino UNO -> NodeMcu ESP8266?**  
  Omdat een andere target een andere hardwarearchitectuur heeft, waardoor de instructies naar de Control Unit anders moeten zijn. Anders was van target wisselen ook niet nodig.

- **De code in het bestand firmware.lst is te lezen voor mensen en wordt ook wel …… genoemd.**  
  De code in het bestand `firmware.lst` is te lezen voor mensen en wordt ook wel **dissassembly** genoemd.

- **Wordt de code in firmware.lst ook zo geïnterpreteerd door de processor? En zo nee; hoe dan wel?**  
  Nee, de daadwerkelijke code die de processor uitvoert is niet leesbaar voor mensen. Het is in machinecode. De processor leest en interpreteert de machinecode direct.

- **Waarom verandert er maar een klein beetje assembly code als je de code in je sketch verandert? Waarvoor dient de rest en waar komt het vandaan?**  
  Omdat een groot deel van de assemblycode wordt gebruikt voor het ondersteunen van de functionaliteit in de **Arduino.h** header, waarmee de hardware van de Arduino aangestuurd wordt om bijvoorbeeld pinflips te kunnen uitvoeren.

- **Als ik een println() toevoeg komt er ineens veel meer in de listing te staan. Hoe komt dat?**  
  Serieel printen kost veel clockcycles, waardoor er dus ook veel extra assemblycode gegenereerd wordt. Als er niet geprint wordt in de code, zorgt de compiler er dynamisch voor dat deze library en de benodigde **Serial struct** niet meegecompileerd worden, wat het aantal assemblycode aanzienlijk verlaagt.
