//El sensor DHT11 no es tan preciso, por lo que los valores no son tan cercanos a los reales, pero sirve para el propósito de este proyecto
#include <DHT.h> //Se incluyen las librerías del sensor DHT
#define DHTTYPE DHT11 //Se define el tipo de sensor DHT (en este caso es un DHT11, se reconoce por el color azul)
#define DHTPIN 13 //Se define el pin correspondiente (D7)
DHT dht(DHTPIN,DHTTYPE); //A la variable dht se le atribuyen el pin y el tipo
void setup() {
  Serial.begin(9600); //Se abre el monitor serial
  dht.begin(); //Se inicializa al sensor en el monitor serial
}

void loop() {
  delay(1000); //Tiempo para que el sensor comience a funcionar
  float humidity = dht.readHumidity(); //Variable que guarda la humedad 
  float temperature = dht.readTemperature(); //Variable que guarda la temperatura
  //Impresión de las lecturas en el monitor serial
  Serial.println("Humidity is: ");
  Serial.println(humidity);
  Serial.println("Temperature is: ");
  Serial.println(temperature);
}
