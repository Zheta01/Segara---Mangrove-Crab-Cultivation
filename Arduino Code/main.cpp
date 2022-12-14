#include <Arduino.h>
#include <dht.h> 
#include <Adafruit_Sensor.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Robojax_WCS.h>
#include <SoftwareSerial.h>


unsigned long int routine1 = 0;
String data;


SoftwareSerial Serial2(3, 2); //pin 3,2 arduino sebagai pin RX,TX

//sensor dht
dht DHT;
int DHTpin = 5;
String kelembamanDHT;
String suhuDHT;

//sensor HY-SRF05
int echopin = 6;
int trigpin = 7;
String jarak;

//sensor DS18B20
OneWire pin_DS18B20(4);
DallasTemperature DS18B20(&pin_DS18B20);
String suhuAir;

// sensor Salinitas
int Salinitaspin = A0;
float salin[20];
float teg[30];
float rata_rata_teg=0;
float Salinitas=0;
String konduktivitas;

//Sensor WCS1800
#define MODEL 12 //model sensor yang digunakan (WCS1800)
#define SENSOR_PIN A2 //pin yang digunakan
#define SENSOR_VCC_PIN 10
#define ZERO_CURRENT_LED_PIN 12

#define ZERO_CURRENT_WAIT_TIME 5000 //tunggu 5detik untuk mengukur arus yang lewat
#define CORRECTION_VLALUE 164 //satuan mA
#define MEASUREMENT_ITERATION 100 
#define VOLTAGE_REFERENCE  5000.0 //5V
#define BIT_RESOLUTION 10
#define DEBUT_ONCE true
String arus;

Robojax_WCS sensor(
          MODEL, SENSOR_PIN, SENSOR_VCC_PIN, 
          ZERO_CURRENT_WAIT_TIME, ZERO_CURRENT_LED_PIN,
          CORRECTION_VLALUE, MEASUREMENT_ITERATION, VOLTAGE_REFERENCE,
          BIT_RESOLUTION, DEBUT_ONCE           
          );

void bacaArus(){
  // Serial.println(sensor.getModel());
  arus = sensor.getCurrent();
  Serial.print("Arus : ");
  Serial.println(arus);
  Serial.println(" ");
  delay(1000);
}

void bacadht(){
  DHT.read22(DHTpin);
  suhuDHT = DHT.temperature;
  kelembamanDHT = DHT.humidity;
  Serial.print("Suhu Ruangan: ");
  Serial.println(suhuDHT);
  delay(1000);
  Serial.print("Kelembaman : ");
  Serial.println(kelembamanDHT);
  delay(1000);
};

void bacaJarak(){
  digitalWrite(trigpin, LOW);
  delayMicroseconds(8);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(8);
  digitalWrite(trigpin, LOW);
  float duration = pulseIn(echopin, HIGH);
  jarak = duration/58;
  Serial.print("Jarak : ");
  Serial.println(jarak);
  delay(1000);
};

void SuhuAir(){
  DS18B20.setResolution(10);
  DS18B20.requestTemperatures();
  suhuAir = DS18B20.getTempCByIndex(0);
  Serial.print("Suhu air celcius : ");
  Serial.println(suhuAir);
  delay(1000);
}

void bacaSalinitas(){
  for ( int i=0; i<30; i++){
      int val = analogRead(Salinitaspin);
      teg[i] = val;
    }
    rata_rata_teg = (teg[0] + teg[1] + teg[2] + teg[3] + teg[4] + teg[5] + teg[6] + teg[7] + teg[8] + teg[9]
    +teg[10] + teg[11] + teg[12] + teg[13] + teg[14] + teg[15] + teg[16] + teg[17] + teg[18] + teg[19]+ teg[20] + teg[21] + teg[22] + teg[23] + teg[24] + teg[25] + teg[26] + teg[27] + teg[28] + teg[29])/30;
    delay(10);


     Salinitas=0.0000119*exp(0.019*rata_rata_teg);

     for (int x=0; x<20; x++){
      float rata_salinitas = Salinitas;
      salin[x]= rata_salinitas;
     }
     konduktivitas = (salin[0] + salin[1] + salin[2] + salin[3] + salin[4] + salin[5] + salin[6] + salin[7] + salin[8] + salin[9] + salin[10] + 
     salin[11] + salin[12] + salin[13] + salin[14] + salin[15] + salin[16] + salin[17] + salin[18] + salin[19])/20;
    delay(10);
    Serial.print("Salinitas : ");
    Serial.println(konduktivitas);
    delay(1000);
}

void setup(){
    sensor.start();
    DS18B20.begin();
    pinMode(trigpin, OUTPUT);
    pinMode(echopin, INPUT);
    Serial.begin(115200);
    
  Serial2.begin(115200);
}

void loop(){
 data = " ";

if(millis() >= routine1){
    bacadht();
    bacaJarak();
    bacaSalinitas();
    SuhuAir();
    bacaArus();
  }
  // data="Suhu Ruangan :"+suhuDHT+"<br>Kelembaman: "+kelembamanDHT+"<br>Jarak: "+jarak+"<br>Salinitas: "+Salinitas+"<br>Suhu air: "+suhuAir+"<br>Arus: "+arus;
 
  data += "<script>setTimeout(()=>{window.location.reload(1);},5000);</script>";
  data += "<h1>DATA ARDUINO</h1> ";

  data += "Suhu Ruangan : ";
  data += suhuDHT;
 
  data += "<br>Kelembaman: ";
  data += kelembamanDHT;

  data += "<br>Jarak  : ";
  data += jarak;
  
  data += "<br>Salinitas: ";
  data += Salinitas;

  data += "<br>Suhu Air: ";
  data += suhuAir;

  data += "<br>Arus: ";
  data += arus;

   while(Serial2.available()>0){
    int i=Serial2.parseInt();
    // Serial2.println(data);
    Serial2.print(data);
    if( i== 1){
      Serial.print("dapat data");
    }
  } 
}



