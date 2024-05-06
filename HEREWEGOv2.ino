

//SENSOR LUXMETER
#include <Wire.h>
#include <BH1750.h>
#include<SoftwareSerial.h>
BH1750 lightMeter(0x23);
//=========================

//SENSOR : ULTRASONIC
#define TRIG 2 //DIGITAL PINOUT
#define ECHO 3
//=========================

//SENSOR : TURBIDITY
//Nothing
//=========================

//SENSOR : pH & Temp
#include<OneWire.h>
#include<DallasTemperature.h>

SoftwareSerial sim900(10,11);

//Temp sensor stuffs
// sensor diletakkan di pin 2
#define ONE_WIRE_BUS 7
// setup sensor
OneWire oneWire(ONE_WIRE_BUS);
// berikan nama variabel,masukkan ke pustaka Dallas
DallasTemperature sensorSuhu(&oneWire);
//------------------
float temperaturenow();


//PhSensor init
#define SENSOR A1            //pH meter Analog output to Arduino Analog Input 0
#define OFFSET 0.00            //deviation compensate
#define SAMPLING_INTERVAL 20
#define PRINT_INTERVAL 800
#define ARRAY_LENGTH  40    //times of collection
int PH_ARRAY[ARRAY_LENGTH];   //Store the average value of the sensor feedback
int PH_ARRAY_INDEX = 0;
double AVERAGE_ARRAY(int* , int);

int timeguardian = 0;
//=========================



void setup() {
  Serial.begin(19200);
  sim900.begin(19200);
  //Sensor init : Temperature & pH
  sensorSuhu.begin();

  //Sensor init : Ultrasonic
  pinMode(TRIG, OUTPUT); // Initializing Trigger Output and Echo Input
  pinMode(ECHO, INPUT_PULLUP);

  //Sensor init : Lux
  Wire.begin();
  lightMeter.begin();

  //Sensor init : Turbidity
  //NaN
  pinMode(13, OUTPUT);

      
  

   /*//======= TEST SMS
       sim900.print("AT+CMGF=1\r");
       delay(8000);

      sim900.println("AT+CSCA=\"+62855000000\"");
      delay(8000);


       sim900.println("AT+CMGS=\"+6282143246664\"");
       delay(8000);


       sim900.println("HASLO ARDUINO BALEKNO SEWELAS YUTO");
       delay(8500);

       //End AT command with a ^Z, ASCII code 26
       sim900.println((char)26);
       delay(2000);
       sim900.println();


       Serial.println("should finish send sms now");
       // Give module time to send SMS
       delay(5000);
    //======END TESET */

}

void loop() {
  //Do your magic, all the functions return raw numbers, therefore, formatting is required
  /*
     Temperature      : temperaturenow(); float -> in Centigrade
     pH               : calculateph(<sampling amount>); float -> in voltage
     Distance         : sonarping(); int -> in CM
     Light Intensity  : luxsample(); float -> in lux
     Liquid Clarity   : turbidity(); float -> in Volt*/
  /*
    Serial.println("--- Sensor Addressing Example ---");
    Serial.print("Temperature : "); Serial.print(temperaturenow()); Serial.println(" C");
    Serial.print("Distance : "); Serial.print(sonarping()); Serial.println(" CM");
    Serial.print("Light Intensity : "); Serial.print(luxsample()); Serial.println(" lux");
    Serial.print("Liquid Clarity : "); Serial.print(liquidclarity()); Serial.println(" v");


  
    int sonarsample=0,pinger;


    for(pinger=0;pinger<=100;pinger++){
    if(sonarping() > 30)
      sonarsample+=1;
      else
      sonarsample+=0;
    delay(10);
    }

    if(sonarsample>=60){
    digitalWrite(13,HIGH);
    Serial.println("  >FLOW ON< ");}
    else{
    digitalWrite(13,LOW);
    Serial.println(" >FLOW OFF< ");}

    Serial.print("Water Level : "); Serial.println(sonarsample);


    //delay(1000);
  
*/
 
  while (Serial.available())
  {
    sim900.println(Serial.readString());
    sim900.print("\n");
    if(Serial.read() == 's')
      SendMessage();
  }

}

//===================Phone Functions=======================
void SendMessage()
{
  Serial.println("YUK KITA KIRIM PESAN");
  sim900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  sim900.println("AT+CMGS=\"+60XXXXXXXXX\"\r"); // Replace x with mobile number
  delay(1000);
  sim900.println("Banyune entek goblook");// The SMS text you want to send
  delay(100);
   sim900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


//============SENSOR FUNCTIONAL :PH and Temp==============

float temperaturenow()
{
  sensorSuhu.requestTemperatures();
  float suhu = sensorSuhu.getTempCByIndex(0);
  return suhu;
}

double AVERAGE_ARRAY(int* ARR, int NUMBER) {
  int i;
  int max, min;
  double AVG;
  long AMOUNT = 0;
  if (NUMBER <= 0) {
    Serial.println("ERROR!/n");
    return 0;
  }
  if (NUMBER < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < NUMBER; i++) {
      AMOUNT += ARR[i];
    }
    AVG = AMOUNT / NUMBER;
    return AVG;
  } else {
    if (ARR[0] < ARR[1]) {
      min = ARR[0]; max = ARR[1];
    }
    else {
      min = ARR[1]; max = ARR[0];
    }
    for (i = 2; i < NUMBER; i++) {
      if (ARR[i] < min) {
        AMOUNT += min;      //arr<min
        min = ARR[i];
      } else {
        if (ARR[i] > max) {
          AMOUNT += max;  //arr>max
          max =      AMOUNT += ARR[i];
        } else {
          AMOUNT += ARR[i]; //min<=arr<=max
        }
      }//if
    }//for
    AVG = (double)AMOUNT / (NUMBER - 2);
  }//if
  return AVG;
}

float calculateph(int samplecount)
{
  int sampling = 0;
  do {
    sampling += 1;
    static unsigned long SAMPLING_TIME = millis();
    static unsigned long PRINT_TIME = millis();
    static float VOLTAGE;
    if (millis() - SAMPLING_TIME > SAMPLING_INTERVAL)
    {
      PH_ARRAY[PH_ARRAY_INDEX++] = analogRead(SENSOR);
      if (PH_ARRAY_INDEX == ARRAY_LENGTH)PH_ARRAY_INDEX = 0;
      VOLTAGE = AVERAGE_ARRAY(PH_ARRAY, ARRAY_LENGTH) * 5.0 / 1024;
      SAMPLING_TIME = millis();
    }
    if (millis() - PRINT_TIME > PRINT_INTERVAL)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
    {
      //Serial.print("VOLTAGE OUTPUT: ");
      //Serial.println(VOLTAGE,2);
      PRINT_TIME = millis();
      if (sampling == samplecount)
        return VOLTAGE;
    }
    delay(100);
  } while (sampling <= samplecount);

}

//=================================================================================
//=====================SENSOR FUNCTIONAL = Ultrasonic==============================
int sonarping()
{
  digitalWrite(TRIG, LOW); // Set the trigger pin to low for 2uS
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH); // Send a 10uS high to trigger ranging
  delayMicroseconds(20);

  digitalWrite(TRIG, LOW); // Send pin low again
  int distance = pulseIn(ECHO, HIGH, 26000); // Read in times pulse

  distance = distance / 58; //Convert the pulse duration to distance
  //You can add other math functions to calibrate it well

  return distance;
}

//=================================================================================
//===================== SENSOR FUNCTIONAL = Luxmeter ==============================
float luxsample() {
  float lux = lightMeter.readLightLevel(true);
  return lux;
}
//=================================================================================
//===================== SENSOR FUNCTIONAL = Turbidity =============================
float liquidclarity() {
  float sensor;
  sensor = analogRead(A0);

  return (sensor * (5.0 / 1024.0));
}
