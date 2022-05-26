
#define TERMO A1
#define T_OP A0
#define HUMEDAD A2
#define RESIST_SERIE 10000
#define R_ROOM 9980.47      // resistencia de termómetro a 25° C
#define ROOM_T 298.15      // temperatura ambiente en Kelvin
#define BETA 3455          // parámetro BETA de termómetro
#define Kp 2               // ganancia directamente proporcional a error3
#define Ki 5               // reducir error final sin aumentar mucho sobrepaso3
#define Kd 1               // incrementar tiempo de respuesta2
#include <PID_v1.h>        // librería de PID
#include <math.h>

// liberías para manipular PCD8544
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// pines de PCD a arduino: SCLK = 7, DIN = 6, D/C = 5, CS = 4, RST = 3
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

int t_op;
float vr;
float r_termo;

// PID
double setPoint, temperatura, salida;  // Variables de lazo realimentado del PID
PID incubadora(&temperatura, &salida, &setPoint, Kp, Ki, Kd, DIRECT);  


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  // INICIALIZAR VARIABLES DE PID
  int t_op = analogRead(T_OP);
  setPoint = map(t_op, 0, 1023, 0, 80);  // Mapeo señal potenciómetro a temp. operación
 
  // Temperatura termistor
  vr = analogRead(TERMO)*5.0/1023.0;
  r_termo = vr*RESIST_SERIE/(5-vr);
  float temp = 1.0/ROOM_T + (1.0/BETA)*log(r_termo/R_ROOM);
  temp = 1/temp;
  temperatura = temp - 273.15;

  // Activar PID
  incubadora.SetMode(AUTOMATIC);

  // Activar display
  display.begin();

 
 
}

void loop() {
  // put your main code here, to run repeatedly:

  // Temperatura operación
  t_op = analogRead(T_OP);
  setPoint = map(t_op, 0, 1023, 0, 80);
  Serial.println(setPoint);
  delay(500);
  //

  // Sensor humedad
  int humid = analogRead(HUMEDAD);
  humid = map(humid, 0, 1023, 0, 100);
  Serial.print("humedad: "); Serial.println(humid);
  delay(500);

  // Resistencia termistor
  vr = analogRead(TERMO)*5.0/1023.0;
  r_termo = vr*RESIST_SERIE/(5-vr);
  delay(500);
 
  // uso de ecuación Steinhart simplificada
  float temp = 1.0/ROOM_T + (1.0/BETA)*log(r_termo/R_ROOM);
  temp = 1/temp;
  temperatura = temp - 273.15;
  Serial.println(temperatura);
  delay(500);

  // Algoritmo PID
  incubadora.Compute();
  Serial.print("out: "); Serial.println(salida);
  delay(300);

  // Imprimir en el LCD
  display.clearDisplay(); //clears display each time loop starts over
  display.setCursor(0,0);
  display.print("T op.: "); //this will be written on the LCD
  display.println(setPoint);
  display.print("Control: "); //this will be written on the LCD
  display.println(salida);
  display.print("T sensed: "); //this will be written on the LCD
  display.println(temperatura);
  display.print("Humedad: "); //this will be written on the LCD
  display.println(humid);
  display.display();

  // LEDs alerta
  if (temperatura <= 30){
    digitalWrite(2, HIGH);  // azul
  } else {digitalWrite(2, LOW);}
  if (temperatura >= 80){
    digitalWrite(8, HIGH);
  } else {digitalWrite(8, LOW);}
}
