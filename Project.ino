#include <DHT.h>
#include <HCSR04.h>

//PINS LCD
#define LCD_SDA 6
#define LCD_SCL 7
//PINS L298N
#define enableA 10
#define IN1 9
#define IN2 8
//PINS HC-SR04
#define TRIG 13
#define ECHO 12
//PINS DHT-11
#define DOUT A0

UltraSonicDistanceSensor distanceSensor(TRIG, ECHO);
DHT dht(DOUT, DHT11);

void setup()
{
    Serial.begin(115200);
    pinMode(LCD_SCL, OUTPUT);
    pinMode(LCD_SDA, OUTPUT);
    pinMode(enableA ,OUTPUT);
    pinMode(IN1 ,OUTPUT);
    pinMode(IN2 ,OUTPUT);
    pinMode(TRIG ,OUTPUT);
    pinMode(ECHO ,INPUT);
    pinMode(DOUT ,INPUT);
    dht.begin();
}

void loop()
{  
    float distance = distanceSensor.measureDistanceCm();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
    Serial.print(distance);
    Serial.print(",");
    Serial.print(h);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(hic);
    Serial.print("\n");
    delay(100);

    if (Serial.available())
    {
        int motorSpeed = Serial.read();
    }
    
}