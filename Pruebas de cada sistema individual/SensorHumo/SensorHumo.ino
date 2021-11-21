int sensorHumoAnalogico = A0;
int sensorHumoDigital = 4;
void setup() {
  Serial.begin(115200);
  pinMode(sensorHumoDigital,INPUT);
}

void loop() {
  int medidaHumoAnalogico = analogRead(sensorHumoAnalogico);
  int medidaHumoDigital = digitalRead(sensorHumoDigital);
  Serial.println("Sensor de gas: ");
  Serial.println(medidaHumoAnalogico);
  Serial.println("Tipo de gas: ");
  Serial.println(medidaHumoDigital);
  if (medidaHumoAnalogico > 1000) {
    Serial.println("ALERTA GAS");
   }
  else {
    Serial.println("-----");
   }
}
