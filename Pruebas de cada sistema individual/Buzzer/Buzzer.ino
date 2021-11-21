int buzzerAlerta = 12; //Declaración e inicialización de la variable en el puerto correspondiente (D6)
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzerAlerta,OUTPUT); //Se define al buzzer como output
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzerAlerta,1000); //Función tone para indicarle al buzzer que debe sonar a esa frecuencia
  delay(1000);
  noTone(buzzerAlerta); //Función noTone que hace que el sonido del buzzer se detenga
  delay(5000);
}
