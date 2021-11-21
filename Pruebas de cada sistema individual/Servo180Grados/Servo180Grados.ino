#include <Servo.h> //Incluir librería Servo de Arduino
Servo servoElevador; //Declaración de la variable de elevador como tipo Servo
void setup() {
  // put your setup code here, to run once:
  servoElevador.attach(16); //Se inicializa la variable en el puerto correspondiente (D5)
  servoElevador.write(0); //Se le ajustan los grados iniciales
}

void loop() {
  servoElevador.write(180); //Ajuste de grados
  delay(5000);
  servoElevador.write(90); //Ajuste de grados
  delay(5000);
  servoElevador.write(0); //Ajuste original de grados
  delay(5000);
}
