#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <DHT.h>
#define DHTPIN 2 
#define DHTTYPE DHT11 
#define SensorPin A2          // the pH meter Analog output is connected with the Arduino’s Analog
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET); 
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  Serial.begin(9600);
   dht.begin();
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  pinMode(13,OUTPUT);  
  Serial.begin(9600);  
  Serial.println("Ready");    //Test the serial monitor
 display.clearDisplay();
  display.setCursor(0, 5);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" IoT Based Fish Prediction System ");
  display.display();
  delay(1000);
  
}
void loop()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  float ph=phValue-2;
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  
  
 
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW); 
 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


// turbidity



 // turbidity part
  int sensorValue = analogRead(A1);// turbidity to A1
  float turbidity = sensorValue * (5.0 / 1024.0);
 
  Serial.println ("Sensor Output (V):");
  Serial.println (turbidity);
  Serial.println();
  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C "));
  Serial.print(f);
  Serial.print(F("F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("C "));
  Serial.print(hif);
  Serial.println(F("F"));
  display.clearDisplay();
  
 
  display.setTextSize(2);
  display.setCursor(0, 5);
  display.print("pH: ");
  display.print(ph);
 
  
 
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("Tur: ");
  display.print(turbidity);
 
 
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print("Temp: ");
  display.print(t);
  display.display();
}
