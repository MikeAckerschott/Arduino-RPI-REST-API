void setupDistanceSensors() {}

void getDistances(int* distanceSensor1,
                  int* distanceSensor2) {
  *distanceSensor1 = analogRead(A0);
  *distanceSensor2 = analogRead(A1);
}

void updateSensorData() {
  int distanceSensor1 = 0;
  int distanceSensor2 = 0;
  getDistances(&distanceSensor1, &distanceSensor2);

  update_aggregate(&aggregate1, distanceSensor1);
  update_aggregate(&aggregate2, distanceSensor2);
  insert_buffer(buffer_1, distanceSensor1);
  insert_buffer(buffer_2, distanceSensor2);
}