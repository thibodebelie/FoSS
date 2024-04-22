#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//PINS L298N
#define enableA 10
#define IN1 9
#define IN2 8
//PINS DHT-11
#define DOUT A0
//PINS Potentiometer
#define POTPIN A1
//PINS Button
#define BTNPIN 3

#define TESTLED 2

LiquidCrystal_I2C lcd(0x27,  16, 2);
DHT dht(DOUT, DHT11);

int lastButtonState = 0;
int buttonState = 0; // variable for reading the pushbutton status
int btnVal = 0; // variable to hold the led stat

void setup()
{
    Serial.begin(115200);
    pinMode(enableA ,OUTPUT);
    pinMode(IN1 ,OUTPUT);
    pinMode(IN2 ,OUTPUT);
    pinMode(BTNPIN, INPUT);
    pinMode(POTPIN, INPUT);
    pinMode(DOUT ,INPUT);
    pinMode(TESTLED, OUTPUT);
    dht.begin();
    lcd.init();
    lcd.backlight();
}

void loop()
{  
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
    float potVal = analogRead(POTPIN);

    lastButtonState = buttonState;
    buttonState = digitalRead(BTNPIN); // read the state of the pushbutton value

    if (buttonState == HIGH && lastButtonState == LOW) { // check if the pushbutton is pressed. If it is, the buttonState is HIGH
        btnVal = !btnVal; // toggle the ledState
        digitalWrite(TESTLED, btnVal); // turn LED on or off based on the ledState
        delay(10); // delay to debounce the switch
    }

    Serial.print(h);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(hic);
    Serial.print(",");
    Serial.print(potVal);
    Serial.print(",");
    Serial.print(btnVal);

    if (Serial.available())
    {
        String read = Serial.readString();
        String motorPWM = getValue(read, ';', 0);
        String state = getValue(read, ';', 1);
        analogWrite(enableA, motorPWM.toInt());
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(state.toInt() ? "Automatisch" : "Manueel" );
        lcd.setCursor(0,1);
        lcd.print(motorPWM.toFloat()/255*100);
        lcd.print("%");
    }
    
    Serial.print("\n");
    delay(100);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}