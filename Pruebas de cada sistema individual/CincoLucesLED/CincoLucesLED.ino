int led1 = 16, led2 = 5, led3 = 4, led4 = 0, led5 = 2; //Inicio de variables de LED y puertos D0 a D4 del ESP8266
void encenderLuces(); //Inicialización de la función
void apagarLuces(); //Inicialización de la función
void setup() {
  //Declaración de los LEDs como OUTPUT
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
}

void loop() {
  //Código principal que se estará iterando
  encenderLuces();
  delay(5000);
  apagarLuces();
  delay(5000);
}
void encenderLuces() {
  //Función que enciende las luces
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  digitalWrite(led5,HIGH);
}
void apagarLuces() {
  //Función que apaga las luces
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  digitalWrite(led5,LOW);  
}
