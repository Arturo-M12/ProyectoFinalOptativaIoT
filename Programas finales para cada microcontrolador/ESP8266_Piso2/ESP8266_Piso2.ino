//Este es el código para el microcontrolador ESP8266 que se utiliza en el piso 2
#include "EspMQTTClient.h" //Librería de MQTT Client para la transmisión y recepción de datos
#include <Servo.h> //Librería de servomotores
#include <DHT.h> //Librería del sensor DHT de temperatura y humedad
#define DHTTYPE DHT11 //Se define el tipo de sensor DHT (en este caso es un DHT11, se reconoce por el color azul)
#define DHTPIN D7 //Se define el pin correspondiente (D7)
DHT dht(DHTPIN,DHTTYPE); //A la variable dht se le atribuyen el pin y el tipo
Servo servoElevador; //Se declara a servoElevador como una variable de tipo Servo
int led1 = 16, led2 = 5, led3 = 4, led4 = 0; //Inicialización de las variables de los LEDs en los puertos D0-D3
int sensorHumoAnalogico = A0; //Declaración e inicialización del sensor de humo en el pin A0
int buzzerAlerta = 14; //Declaración e inicialización del buzzer de alerta en el pin D5
int ledAlerta = 12; //Declaración e inicialización del LED rojo de alerta en el pin D6
//En esta sección se incluyen los datos de la conexión WiFi, del Broker y el nombre de la tarjeta:
EspMQTTClient client(
  "GalaxyNote9c9cf", //Nombre de la red WiFi
  "muhs6150", //Contraseña de la red WiFi
  "driver.cloudmqtt.com", //IP del server del Broker MQTT
  "luocawzu", //Usuario de MQTT 
  "Xk0OITCDjYHq", //Contraseña de MQTT  
  "ESP8266_Piso2", //Nombre que tendrá el microcontrolador    
   18598          //El puerto MQTT   
);
void luces(); //Declaración de la función de las luces
void funcionDHT(); //Declaración de la función del sensor de temperatura y humedad
void humo(); //Declaración de la función del sensor MQ2 para medir niveles de humo
void motorElevador(); //Declaración de la función para que el motor del elevador suba o baje
void setup()
{
  Serial.begin(115200); //Inicio del monitor serial con un baudio de 115200
  client.enableDebuggingMessages(); //Función de MQTT
  client.enableHTTPWebUpdater(); //Función de MQTT
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline"); //Mensaje que se imprime en caso de que el microcontrolador pierda la señal
  //Declaración de los LEDs como OUTPUT:
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  servoElevador.attach(2); //Se inicializa el servomotor del elevador en el pin D4
  dht.begin(); //Función que inicializa el funcionamiento del DHT11
  pinMode(buzzerAlerta,OUTPUT); //Declaración del buzzer de alerta como output
  pinMode(ledAlerta,OUTPUT); //Declaración del led de alerta como output
}
//Función que se utilzia para probar la conexión exitosa del microcontrolador:
void onConnectionEstablished()
{
  client.publish("/piso2", "Piso 2 funcionando"); //Primer mensaje
  client.executeDelayed(2 * 1000, []() { //Delay
    client.publish("/piso2", "Piso 2 funcionado (1)"); //Segundo mensaje
  });
}
void loop() //Función que se repite infinitamente en el microcontrolador
{
  client.loop(); //Función MQTT
  luces(); //Función de luces
  funcionDHT(); //Función sensor de temperatura y humedad
  humo(); //Función sensor humo
  motorElevador(); //Función servomotor elevador
  delay(2000); //Delay de un segundo
}
//Función para controlar las luces mediante Node Red Dashboard y visualizar su estado mediante suscripción y publicación
void luces() {
  client.subscribe("/piso2/luces/estatus", [](const String & estatusLuces) {
    if (estatusLuces == "LOW") {
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      digitalWrite(led3,LOW);
      digitalWrite(led4,LOW);
      client.publish("/piso2/luces", "Luces del piso 2 apagadas.");
    }
    else if (estatusLuces == "HIGH") {
      digitalWrite(led1,HIGH);
      digitalWrite(led2,HIGH);
      digitalWrite(led3,HIGH);
      digitalWrite(led4,HIGH);
      client.publish("/piso2/luces", "Luces del piso 2 encendidas.");
    }
    else {}
    Serial.println(estatusLuces);
    });
}
//Función para que el sensor DHT11 lea la temperatura y humedad relativa
void funcionDHT() {
  float humedad = dht.readHumidity(); //Variable que guarda la humedad 
  float temperatura = dht.readTemperature(); //Variable que guarda la temperatura  
  char temperaturaString[8];
  dtostrf(temperatura, 1, 2, temperaturaString);
  char humedadString[8];
  dtostrf(humedad, 1, 2, humedadString);
  client.publish("/piso2/quirofano/DHT11/temperatura", temperaturaString);
  client.publish("/piso2/quirofano/DHT11/humedad", humedadString);
}
//Función para que el sensor de humo mida la cantidad de humo en el ambiente
void humo() {
  int medidaHumoAnalogico = analogRead(sensorHumoAnalogico);
  char humoAnalogicoString[8];
  dtostrf(medidaHumoAnalogico,1,2,humoAnalogicoString);
  client.publish("/piso2/pasillo/humo", humoAnalogicoString);
  if (medidaHumoAnalogico > 200) {
    tone(buzzerAlerta,1000);
    digitalWrite(ledAlerta,HIGH);
   }
  else {
    noTone(buzzerAlerta);
    digitalWrite(ledAlerta,LOW);
   }
}
//Función para que el elevador se mueva dependiendo de los inputs realizados en Node Red Dashboard
void motorElevador() {
  client.subscribe("/piso2/elevador", [](const String & payload) {
    Serial.println(payload);
    int resultado = payload.toInt();
    int pos = map(resultado, 1, 100, 0, 180);
    servoElevador.write(pos);
  });
}
