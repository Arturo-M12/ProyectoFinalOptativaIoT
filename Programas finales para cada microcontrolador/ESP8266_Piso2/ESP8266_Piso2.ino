#include "EspMQTTClient.h"
#include <Servo.h>
#include <DHT.h>
#define DHTTYPE DHT11 //Se define el tipo de sensor DHT (en este caso es un DHT11, se reconoce por el color azul)
#define DHTPIN D7 //Se define el pin correspondiente (D7)
DHT dht(DHTPIN,DHTTYPE); //A la variable dht se le atribuyen el pin y el tipo
Servo servoElevador;
int led1 = 16, led2 = 5, led3 = 4, led4 = 0; //Inicialización de las variables de los LEDs en los puertos D0-D3

EspMQTTClient client(
  "GalaxyNote9c9cf",
  "muhs6150",
  "driver.cloudmqtt.com",  // MQTT Broker server ip
  "luocawzu",   // Can be omitted if not needed
  "Xk0OITCDjYHq",   // Can be omitted if not needed
  "ESP8266_Piso2",     // Client name that uniquely identify your device
   18598              // The MQTT port, default to 1883. this line can be omitted
);
void encenderLuces();
void apagarLuces();
void funcionDHT();
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
  servoElevador.attach(2);
  dht.begin();
  
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
  client.publish("/piso2/luces", "Prueba luces"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("/piso2/luces", "Prueba luces 2");
  });
}

void loop()
{
  client.loop();
  encenderLuces();
  funcionDHT();
  delay(1000);
}
void encenderLuces() {
  //Función que enciende las luces
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  digitalWrite(led3,HIGH);
  digitalWrite(led4,HIGH);
  client.publish("/piso2/luces", "Luces del piso 2 encendidas.");
}
void apagarLuces() {
  //Función que apaga las luces
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);
  client.publish("/piso2/luces", "Luces del piso 2 apagadas.");
}
void funcionDHT() {
  float humidity = dht.readHumidity(); //Variable que guarda la humedad 
  float temperature = dht.readTemperature(); //Variable que guarda la temperatura  
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  client.publish("/piso2/quirofano/DHT11", tempString);
  client.publish("/piso2/quirofano/DHT11", humString);
}
