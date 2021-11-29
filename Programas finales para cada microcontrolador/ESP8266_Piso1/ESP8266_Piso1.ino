#include "EspMQTTClient.h"
#include <Servo.h>
Servo servoPuerta;
int led1 = 16, led2 = 5, led3 = 4, led4 = 0; //Inicialización de las variables de los LEDs en los puertos D0-D3

float distance, duration;
#define echopin D5
#define trigpin D6
EspMQTTClient client(
  "GalaxyNote9c9cf",
  "muhs6150",
  "driver.cloudmqtt.com",  // MQTT Broker server ip
  "luocawzu",   // Can be omitted if not needed
  "Xk0OITCDjYHq",   // Can be omitted if not needed
  "ESP8266_Piso1",     // Client name that uniquely identify your device
   18598              // The MQTT port, default to 1883. this line can be omitted
);
void encenderLuces();
void apagarLuces();
void ultrasonicoServo();
void setup()
{
  Serial.begin(115200);
  client.enableDebuggingMessages();
  client.enableHTTPWebUpdater(); 
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(trigpin,OUTPUT);
  servoPuerta.attach(2);
}

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  //client.subscribe("mytopic/test", [](const String & payload) {
    //Serial.println(payload);
  //});

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  //client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
    //Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  //});

  // Publish a message to "mytopic/test"
  client.publish("/piso1", "ESP8266 Piso 1 Funcionando"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(2 * 1000, []() {
    client.publish("/piso1", "ESP8266 Piso 1 Funcionando (1)");
  });
}

void loop()
{
  client.loop();
  encenderLuces();
  ultrasonicoServo();
  delay(1000);
}
void encenderLuces() {
  //Función que enciende las luces
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  client.publish("/piso1/luces", "Luces del piso 1 encendidas.");
}
void apagarLuces() {
  //Función que apaga las luces
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  client.publish("/piso1/luces", "Luces del piso 1 apagadas.");
}
void ultrasonicoServo() {
  digitalWrite(trigpin,LOW);
  delay(2);
  digitalWrite(trigpin,HIGH);
  delay(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distance = ((duration/2)/29);
  if (distance<10) {
    servoPuerta.write(150);  
    client.publish("/piso1/puerta", "La puerta está abierta.");
  }  
  else {
    servoPuerta.write(1);
    client.publish("/piso1/puerta", "La puerta está cerrada.");
  }
}
