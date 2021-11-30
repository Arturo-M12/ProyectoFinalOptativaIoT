//Este es el código para el microcontrolador ESP8266 que se utiliza en el piso 1 
#include "EspMQTTClient.h" //Librería de MQTT Client para la transmisión y recepción de datos
#include <Servo.h> //Librería de servomotores
Servo servoPuerta; //Se declara a servoPuerta como una variable de tipo Servo 
int led1 = 16, led2 = 5, led3 = 4, led4 = 0; //Inicialización de las variables de los LEDs en los puertos D0-D3
float distance, duration; //Declaración de las variables que serán utilizadas para medir la distancia con el ultrasónico
#define echopin D5 //Definición del pin para echo (recepción de las ondas ultrasónicas)
#define trigpin D6 //Definición del pin para trig (emisión de las ondas ultrasónicas)
//En esta sección se incluyen los datos de la conexión WiFi, del Broker y el nombre de la tarjeta:
EspMQTTClient client(
  "INFINITUMA76C", //GalaxyNote9c9cf //Nombre de la red WiFi
  "3114793511", //muhs6150 //Contraseña de la red WiFi
  "driver.cloudmqtt.com",  //IP del server del Broker MQTT
  "luocawzu",   //Usuario de MQTT
  "Xk0OITCDjYHq",   //Contraseña de MQTT
  "ESP8266_Piso1",     //Nombre que tendrá el microcontrolador
   18598              //El puerto MQTT
);
void luces(); //Declaración de la función de las luces 
void ultrasonicoServo(); //Declaración de la función del ultrasónico y el servomotor
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
  pinMode(echopin,INPUT); //Declaración del pin de echo como input para que lea los valores de las ondas ultrasónicas
  pinMode(trigpin,OUTPUT); //Declaración del pin de trig como output para que envíe las ondas ultrasónicas
  servoPuerta.attach(2); //Se inicializa el servomotor de la puerta en el pin D4
}
//Función que se utilzia para probar la conexión exitosa del microcontrolador:
void onConnectionEstablished()
{
  client.publish("/piso1", "ESP8266 Piso 1 Funcionando"); //Primer mensaje
  client.executeDelayed(2 * 1000, []() { //Delay
    client.publish("/piso1", "ESP8266 Piso 1 Funcionando (1)"); //Segundo mensaje
  });
}

void loop() //Función que se repite infinitamente en el microcontrolador
{
  client.loop(); //Función MQTT
  luces(); //Función de luces
  ultrasonicoServo(); //Función de ultrasónico y servomotor para la puerta
  delay(1000); //Delay de un segundo
}
//Función para controlar las luces mediante Node Red Dashboard y visualizar su estado mediante suscripción y publicación
void luces() {
  client.subscribe("/piso1/luces/estatus", [](const String & estatusLuces) {
    if (estatusLuces == "LOW") {
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      digitalWrite(led3,LOW);
      digitalWrite(led4,LOW);
      client.publish("/piso1/luces", "Luces del piso 1 apagadas.");
      delay(1000);
    }
    else if (estatusLuces == "HIGH") {
      digitalWrite(led1,HIGH);
      digitalWrite(led2,HIGH);
      digitalWrite(led3,HIGH);
      digitalWrite(led4,HIGH);
      client.publish("/piso1/luces", "Luces del piso 1 encendidas.");
      delay(1000);  
    }
    else {}
    Serial.println(estatusLuces);
    });
}
//Función para controlar la apertura y el cierre de la puerta con un servomotor mediante las medidas del sensor ultrasónico. Igualmente, el estatus se publica en Node Red Dashboard
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
