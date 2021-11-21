//El sensor ultrasónico sirve para medir distancia al emitir ondas ultrasónicas y medir el tiempo que tardan en regresar
float distance; //Variable para al distancia
float duration; //Variable para medir la duración entre la emisión de la onda y el regreso al sensor
#define echopin 16 //Definición del pin Echo (D0 Segunda placa)
#define trigpin 5 //Definición del pin Trig (D1 Segunda placa)
void setup() {
  Serial.begin(9600); //Inicio del monitor serial
  pinMode(echopin,INPUT); //El pin Echo sirve para medir la distancia y duración de la onda ultrasónica
  pinMode(trigpin,OUTPUT); //El pin Trig sirve para emitir la onda
}

void loop() {
  digitalWrite(trigpin,LOW);
  delay(2);
  digitalWrite(trigpin,HIGH);
  delay(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distance = ((duration/2)/29); //Conversión de la variable para que la medida quede en centímetros
  Serial.println(distance); //Impresión de la distancia en el monitor serial
  Serial.println("centímetros");
}
