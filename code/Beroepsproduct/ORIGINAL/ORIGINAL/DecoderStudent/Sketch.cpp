/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/*
  Decoder for train signals and gates
*/
#include <Servo.h>
//Beginning of Auto generated function prototypes by Atmel Studio
void setupLightDefinitions();
void setupLightPins();
void setupGatePins();
void allLightsOff();
void toggleLightsTest();
void toggleGatesTest();
void testLights();
void setSignal(byte signal, byte aspect);
void blinkLights();
void moveGates();
//End of Auto generated function prototypes by Atmel Studio



const byte NROFSIGNALS = 2;
const byte NROFLIGHTSPERSIGNAL = 3;
const byte NROFASPECTS = 3;
const byte NROFGATES = 2;
const byte NROFCOUNTRIES = 2;

enum lamp_t {OFF, ON, BLINK};
enum light_t {RED, YELLOW, GREEN};
enum lightsRunTestStatus_t {LIGHTSTESTING, LIGHTSRUNNING};
enum aspect_t {DRIVE, BRAKE, STOP};
enum country_t {D, NL};
lamp_t LIGHTS [NROFCOUNTRIES][NROFASPECTS][NROFLIGHTSPERSIGNAL] ;
byte LIGHTPINS[NROFSIGNALS][NROFLIGHTSPERSIGNAL];
byte signalStatus[NROFSIGNALS];
lightsRunTestStatus_t lightsRunTestStatus;
byte blinkStatus;

enum gate_t {OPEN, CLOSED, OPENING, CLOSING};
enum gatesRunTestStatus_t {GATETESTING, GATERUNNING};
byte GATEPINS[NROFGATES] = {9, 8};
const byte POTENTIOPIN = 2;
gatesRunTestStatus_t gatesRunTestStatus;
country_t country;
gate_t gateStatus[NROFGATES];

byte testSignalNumber;
byte testLightNumber;
unsigned long previousBlinkMillis = 0;
unsigned long previousTestBlinkMillis = 0;
byte TESTLIGHTINTERVAL = 250;
const int BLINKINTERVAL = 500;
unsigned int previousGateMillis = 0;

byte gatePosition[NROFGATES];
const int GATEMAXPOSITION = 180;
const byte GATEINCREMENT = 2;
const int MINGATEINTERVAL = 5;
const int MAXGATEINTERVAL = 500;

int gateInterval = 200;

Servo myservo[NROFGATES];

void setupLightDefinitions() {
  LIGHTPINS[0][RED] = 13;
  LIGHTPINS[0][YELLOW] = 12;
  LIGHTPINS[0][GREEN] = 11;
  LIGHTPINS[1][RED] = 7;
  LIGHTPINS[1][YELLOW] = 6;
  LIGHTPINS[1][GREEN] = 5;

  LIGHTS[NL][STOP][RED] = ON;
  LIGHTS[NL][STOP][YELLOW] = OFF;
  LIGHTS[NL][STOP][GREEN] = OFF;
  LIGHTS[NL][BRAKE][RED] = OFF;
  LIGHTS[NL][BRAKE][YELLOW] = ON;
  LIGHTS[NL][BRAKE][GREEN] = OFF;
  LIGHTS[NL][DRIVE][RED] = OFF;
  LIGHTS[NL][DRIVE][YELLOW] = OFF;
  LIGHTS[NL][DRIVE][GREEN] = ON;

  LIGHTS[D][STOP][RED] = ON;
  LIGHTS[D][STOP][YELLOW] = OFF;
  LIGHTS[D][STOP][GREEN] = OFF;
  LIGHTS[D][BRAKE][RED] = OFF;
  LIGHTS[D][BRAKE][YELLOW] = ON;
  LIGHTS[D][BRAKE][GREEN] = BLINK;
  LIGHTS[D][DRIVE][RED] = OFF;
  LIGHTS[D][DRIVE][YELLOW] = OFF;
  LIGHTS[D][DRIVE][GREEN] = ON;

  blinkStatus = HIGH;
}

//initialize digital pins as output and set signal and lights to aspect STOP
void setupLightPins() {
  for (byte signal = 0; signal < NROFSIGNALS; signal++) {
    for (byte light = 0; light < NROFLIGHTSPERSIGNAL; light++) {
      pinMode(LIGHTPINS[signal][light], OUTPUT);
      digitalWrite(LIGHTPINS[signal][light], LIGHTS[country][light][STOP]);
    }
    signalStatus[signal] = STOP;
  }
}

//initialize gate pins for servo and position to 0
void setupGatePins() {
  for (byte gate = 0; gate < NROFGATES; gate++) {
    gatePosition[gate] = 0;
    gateStatus[gate] = CLOSED;
    myservo[gate].attach(GATEPINS[gate]);
    myservo[gate].write(gatePosition[gate]);
  }
}

// initialize system; at startup no test
void setup() {
  setupLightDefinitions();
  setupLightPins();
  TESTLIGHTINTERVAL = 1000;
  lightsRunTestStatus = LIGHTSRUNNING;
  gatesRunTestStatus = GATERUNNING;
  Serial.begin(9600, SERIAL_7E2);
  Serial.setTimeout(500);
}

void allLightsOff() {
  for (byte signal = 0; signal < NROFSIGNALS; signal++) {   //switch all lights off not in student version. Not specified
    for (int light = 0; light < NROFLIGHTSPERSIGNAL; light++) {
      digitalWrite(LIGHTPINS[signal][light], OFF);
    }
  }
}

void toggleLightsTest() {
  lightsRunTestStatus = (lightsRunTestStatus == LIGHTSTESTING) ? LIGHTSRUNNING : LIGHTSTESTING;
  testSignalNumber = 0;
  testLightNumber = 0;
  allLightsOff();
}

void toggleGatesTest() {
  gatesRunTestStatus = (gatesRunTestStatus == GATETESTING) ? GATETESTING : GATERUNNING;
  for (byte gate = 0; gate < NROFGATES; gate++) {
    gateStatus[gate] = OPENING;
  }
}

//in main loop testLights is called frequently to blink all lights of all signals one after the other
void testLights() {
  long previousTestBlinkMillis;
  if ((millis() - previousTestBlinkMillis) >= TESTLIGHTINTERVAL) {
    previousTestBlinkMillis = millis();
    digitalWrite(LIGHTPINS[testSignalNumber][testLightNumber], LOW);
    testLightNumber++;
    if (testLightNumber >= NROFLIGHTSPERSIGNAL) {
      testLightNumber = 0;
      testSignalNumber++;
      if (testSignalNumber >= NROFSIGNALS) {
        testSignalNumber = 0;
      }
    }
    digitalWrite(LIGHTPINS[testSignalNumber][testLightNumber], HIGH);
  }
}

//set certain signal to new aspect and set lights in that signal accordingly
void setSignal(byte signal, byte aspect) {
  signalStatus[signal] = aspect;
  for (int light = 0; light <= NROFLIGHTSPERSIGNAL; light++) {
    if ((LIGHTS[country][aspect][light] == ON) || (LIGHTS[country][aspect][light] == OFF)) {
      digitalWrite(LIGHTPINS[signal][light], LIGHTS[country][light][aspect]);
    }
  }
}

void blinkLights() {
  long currentBlinkMillis = millis();
  if (currentBlinkMillis - previousBlinkMillis >= BLINKINTERVAL) {
    previousBlinkMillis = currentBlinkMillis;
    for (byte signal = 0; signal < NROFSIGNALS; signal++) {
      for (byte light = 0; light < NROFLIGHTSPERSIGNAL; light++) {
        if (LIGHTS[country][signalStatus[signal]][light] == BLINK) {
          digitalWrite(LIGHTPINS[signal][light], blinkStatus);
        }
      }
    }
    blinkStatus = (blinkStatus == HIGH) ? LOW : HIGH;
  }
}

void moveGates() {
  long currentMillis = millis();
  if ((currentMillis - previousGateMillis) >= gateInterval) {
    previousGateMillis = currentMillis;
    for (byte gate = 0; gate < NROFGATES; gate++) {
      if (gateStatus[gate] == OPENING) {
        gatePosition[gate] = (gatePosition[gate] + GATEINCREMENT);
        if (gatePosition[gate] > GATEMAXPOSITION) {
          if (gatesRunTestStatus == GATERUNNING) {
            gateStatus[gate] = OPEN;
          } else
            gateStatus[gate] = CLOSING;
        } else {
          myservo[gate].write(gatePosition[gate]);
        }
      } else if (gateStatus[gate] == CLOSING) {
        gatePosition[gate] = (gatePosition[gate] - GATEINCREMENT);
        if (gatePosition[gate] < 0) {
          if (gatesRunTestStatus == GATERUNNING) {
            gateStatus[gate] = CLOSED;
          } else
            gateStatus[gate] = OPENING;
        } else {
          myservo[gate].write(gatePosition[gate]);
        }
      }
    }
  }
}

void loop() {
  if (Serial.available() > 0) {
    String inputCommand = Serial.readString();
    byte firstLetterCommand = inputCommand[0];
    byte signalOrGateNumber = inputCommand[1] - 'O' - 1;
    switch (firstLetterCommand) {
      case 'T': toggleLightsTest();
        break;
      case 'G': toggleGatesTest();
        break;
      case 'N': country = NL;
      case 'D': country = D;
        break;
      case 'R': setSignal(signalOrGateNumber, BRAKE);
        break;
      case 'A': setSignal(signalOrGateNumber, DRIVE);
        break;
      case 'H': setSignal(signalOrGateNumber, STOP);
        break;
      case 'O': gateStatus[signalOrGateNumber] = OPENING;
        break;
      case 'S': gateStatus[signalOrGateNumber] = CLOSING;
        break;
    }
  }
  if (lightsRunTestStatus = LIGHTSTESTING) {
    testLights();
  }
  moveGates();
  blinkLights();
  gateInterval = map(analogRead(POTENTIOPIN), 0, 1023, MINGATEINTERVAL, MAXGATEINTERVAL);
}
