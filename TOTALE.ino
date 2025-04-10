#include <Wire.h>
#include <MPU6050_tockn.h> // pin 13 23 11 10 digitali
#include "SparkFunMPL3115A2.h" // pin A4 = SDA e A5=SCL 
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
//variabili sistema
float temperatuta_effettiva; 
int picco = 0;
int soglia = 0;
//giroscopio
MPU6050 mpu6050(Wire);
float Xangolo , Yangolo , Zangolo , temperatura_Mpu6050 , GyroX , GyroY , GyroZ , AccX , AccY , AccZ , GyroAngleX , GyroAngleY , GyroAngleZ;
long timer = 0;
//sensore altimetro
MPL3115A2 myPressure;
float altitude , pressure , temperature_MPL3115A2;
//salvataggio file
File myFile;
// Servo pin
int servopin1 = 9;  
int servopin2 = 8;
int servopin3 = 7;
int servopin4 = 6;
int servo = 5;
// Servo object 
Servo servo1;   
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo_paracadute;
//salvataggio dati
void salvataggio_dati(float c1, float c2,float c3, float c4,float c5, float c6,float c7, float c8,float c9, float c10,float c11, float c12,float c13,float c14, float c15) { 
  myFile = SD.open("roketlog.txt", FILE_WRITE);
  myFile.print(millis());
  myFile.print("\t");
  myFile.print(c1);
  myFile.print("\t");
  myFile.print(c2);
  myFile.print("\t");
  myFile.print(c3);
  myFile.print("\t");
  myFile.print(c4);
  myFile.print("\t");
  myFile.print(c5);
  myFile.print("\t");
  myFile.print(c6);
  myFile.print("\t");
  myFile.print(c7);
  myFile.print("\t");
  myFile.print(c8);
  myFile.print("\t");
  myFile.print(c9);
  myFile.print("\t");
  myFile.print(c10);
  myFile.print("\t");
  myFile.print(c11);
  myFile.print("\t");
  myFile.print(c12);
  myFile.print("\t");
  myFile.print(c13);
  myFile.print("\t");
  myFile.print(c14);
  myFile.print("\t");
  myFile.println(c15);
  myFile.close();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // inizializzazione sheda logger sd
  Serial.print("Initializing SD card...");
  if (!SD.begin()) {
    Serial.println("ERRORE SD CARD"); //errore compilazione codice SD
  }

  Serial.println("inizializzazione MPL3115A2 altimetro");
  myPressure.begin(); // Get sensor online altimetro
  Serial.println("inizializzazione MPL3115A2 altimetro completata");
  myFile.print("inizializzazione MPL3115A2 altimetro completata");
  Serial.println("inizializzazione MPU6050 giroscopio");
  mpu6050.begin(); // sensore online giroscopio
  Serial.println("inizializzazione MPU6050 giroscopio completata");
  

  //Configure the sensor MPL3115A2 altimetro
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags

  //Configure the sensor MPU6050 giroscopio
  mpu6050.calcGyroOffsets(true); //taratura
  Serial.println("non muovere ... ");
  myFile.print("inizializzazione MPU6050 giroscopio con taratura completata");

  //attach object to pin 
  servo1.attach(servopin1);   
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
  servo_paracadute.attach(servo);

  servo_paracadute.write(0); // posizione neutra servo paracadute
  //soglia picco paracadute scheda sd e serial
  Serial.print("soglia: ");
  Serial.println(soglia);
  myFile.println(" soglia: ");
  myFile.println(soglia);
}
void loop() {
  mpu6050.update();
  if(millis() - timer > 1){  //MPU6050 GIROSCOPIO
    Xangolo = mpu6050.getAngleX(); 
    Yangolo = mpu6050.getAngleY(); 
    Zangolo = mpu6050.getAngleZ(); 
    
    GyroX = mpu6050.getGyroX();
    GyroY = mpu6050.getGyroY();
    GyroZ = mpu6050.getGyroZ();

    AccX = mpu6050.getAccX();
    AccY = mpu6050.getAccY();
    AccZ = mpu6050.getAccZ();

    GyroAngleX = mpu6050.getGyroAngleX();
    GyroAngleY = mpu6050.getGyroAngleY();
    GyroAngleZ = mpu6050.getGyroAngleZ();

    //temperatura
    temperatura_Mpu6050 = mpu6050.getTemp();
    timer = millis();
  }
  // SENSORE MPL3115A2 Pressione e temperatura
  altitude = myPressure.readAltitude();
  pressure = myPressure.readPressure();
  temperature_MPL3115A2 = myPressure.readTemp();
  temperatuta_effettiva = (temperatura_Mpu6050 + temperature_MPL3115A2)/2 ; // temperatura effettiva

  if( Xangolo == -1<0< 1){                  
    servo2.write(90);    //set to 90 degrees
  }
  servo2.write(90 - Yangolo); //unstable angle
  
  if( Xangolo == 0){
    servo1.write(90); //set to 90 degrees
  }
  servo1.write(90 + Yangolo); //unstable angle

  //y-axis / pitch 
  if(Yangolo == 0){        
    servo3.write(90);//set to 90 degrees
  }
  servo3.write(90 +Xangolo); //unstable angle
    
  if(Yangolo == 0){
    servo4.write(90); //set to 90 degrees
  }
  servo4.write(90 - Xangolo);//unstable angle

  // comandi seriali
  if(Serial.available()>0){
    char seriale = Serial.read();
    
    if (seriale == '1'){ //seriale per temperatura
    Serial.println(seriale);
    Serial.print("\temperatura esterna : ");
    Serial.println(temperatuta_effettiva);
    }
    if(seriale == '2'){ //seriale per angolo x y z
      Serial.println(seriale);
      Serial.print("angleX : ");Serial.print(Xangolo); 
      Serial.print("\tangleY : ");Serial.print(Yangolo);
      Serial.println("\tangleZ : ");Serial.println(Zangolo);
      }
    if(seriale == '2'){ //seriale per altimetro
      Serial.println(seriale);
      Serial.print(" Altitudine(m):");
      Serial.print(altitude, 2);
      Serial.print(" Pressione(Pa):");
      Serial.println(pressure, 2);
      }
    if(seriale == '1000'){ //seriale per taratura
      Serial.println(seriale);
      Serial.println("taratura mpu6050");
      mpu6050.calcGyroOffsets(true); //taratura
      Serial.println("taratura mpu6050 effettuata");
    }
  }
  //rilevare apogeo 
  if (altitude > picco) {
    picco = altitude;  
  } 
  //apertura paracadute con scrittura su sd
  if (altitude <= soglia) {
    if (picco > soglia) {
      Serial.print(picco);
      myFile.println(picco);
      Serial.println("\t PICCO!");
      picco = 0;
      servo_paracadute.write(180); // modificare in base posizione servo
      myFile.println(" paracadute aperto : ");
    }  
  }
  salvataggio_dati(altitude,pressure,Xangolo, Yangolo,Zangolo,temperatuta_effettiva,GyroX,GyroY,GyroZ,AccX,AccY,AccZ,GyroAngleX,GyroAngleY,GyroAngleZ); //salvataggio dati
}
//soglia
void determina_soglia(){
  int altitudemax = 180; // modificare in base all apogeo
  soglia = altitude + (altitudemax - altitude)/2;
}
