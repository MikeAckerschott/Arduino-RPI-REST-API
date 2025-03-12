unsigned long interval = 0; // Default interval is 0
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    if (input.length() > 0 && input.toInt() > 0) {
      interval = input.toInt();
    } 
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.println(analogRead(A0));
  }
}
