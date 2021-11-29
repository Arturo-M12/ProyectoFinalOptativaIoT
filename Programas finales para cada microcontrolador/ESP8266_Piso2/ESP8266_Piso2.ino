#include "EspMQTTClient.h"
#include <Servo.h>
#include <DHT.h>
#define DHTTYPE DHT11 //Se define el tipo de sensor DHT (en este caso es un DHT11, se reconoce por el color azul)
#define DHTPIN D7 //Se define el pin correspondiente (D7)
DHT dht(DHTPIN,DHTTYPE); //A la variable dht se le atribuyen el pin y el tipo
Servo servoElevador;
int led1 = 16, led2 = 5, led3 = 4, led4 = 0; //Inicialización de las variables de los LEDs en los puertos D0-D3
int sensorHumoAnalogico = A0;
int buzzerAlerta = 14;
int ledAlerta = 12;
EspMQTTClient client(
  "INFINITUMA76C", //GalaxyNote9c9cf
  "3114793511", //muhs6150
  "driver.cloudmqtt.com",
  "luocawzu",  
  "Xk0OITCDjYHq",  
  "ESP8266_Piso2",     
   18598             
);
void encenderLuces();
void apagarLuces();
void funcionDHT();
void humo();
void motorElevador();
/*void callback(char* topic, byte* payload, unsigned int length) {
  String string;  
  for (int i = 0; i < length; i++) {
    string += ((char)payload[1]);
  }
  if (topic = "/piso2/elevador") {
    int resultado = string.toInt();
    int pos = map(resultado, 1, 100, 0, 180);
    servoElevador.write(pos);
    delay(15);
  }
} */
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
  pinMode(buzzerAlerta,OUTPUT);
  pinMode(ledAlerta,OUTPUT);
}

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  //client.subscribe("/piso2/elevador", [](const String & payload) {
  //Serial.println(payload);
  //});

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  //client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
    //Serial.println("(From wildcard) topic: " + topic + ", payload: " + payload);
  //});

  // Publish a message to "mytopic/test"
  client.publish("/piso2", "Piso 2 funcionando"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  client.executeDelayed(5 * 1000, []() {
    client.publish("/piso2", "Piso 2 funcionado (1)");
  });
}

void loop()
{
  client.loop();
  encenderLuces();
  funcionDHT();
  humo();
  motorElevador();
  delay(100);
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
  float humedad = dht.readHumidity(); //Variable que guarda la humedad 
  float temperatura = dht.readTemperature(); //Variable que guarda la temperatura  
  char temperaturaString[8];
  dtostrf(temperatura, 1, 2, temperaturaString);
  char humedadString[8];
  dtostrf(humedad, 1, 2, humedadString);
  client.publish("/piso2/quirofano/DHT11/temperatura", temperaturaString);
  client.publish("/piso2/quirofano/DHT11/humedad", humedadString);
  delay(5000);
}
void humo() {
  int medidaHumoAnalogico = analogRead(sensorHumoAnalogico);
  char humoAnalogicoString[8];
  dtostrf(medidaHumoAnalogico,1,2,humoAnalogicoString);
  client.publish("/piso2/pasillo/humo", humoAnalogicoString);
  if (medidaHumoAnalogico > 1000) {
    tone(buzzerAlerta,1000);
    digitalWrite(ledAlerta,HIGH);
    delay(1000);
    noTone(buzzerAlerta);
    digitalWrite(ledAlerta,LOW);
   }
  else {
    noTone(buzzerAlerta);
    digitalWrite(ledAlerta,LOW);
   }
}
void motorElevador() {
  client.subscribe("/piso2/elevador", [](const String & payload) {
    Serial.println(payload);
    int resultado = payload.toInt();
    int pos = map(resultado, 1, 100, 0, 180);
    servoElevador.write(pos);
    delay(15);
  });
}
