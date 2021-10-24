// include the library code:
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
#include <Arduino_HTS221.h>
#include <RTClib.h>

// initialize the library with the numbers of the interface pins
Adafruit_LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_Millis rtc;
    
//*****************************************************************
//intializes all the variables
int i;
float temp = 0;
float hum = 0;
int pause = 5000;
//^^^^^^^^^^^^^^^^^^^^^^^^^
//Moister variables
int moi[4] = {0, 0, 0, 0};
//^^^^^^^^^^^^^^^^^^^^^^^^
//defining moister pins
#define MPin1 A0
#define MPin2 A1
#define MPin3 A2
#define MPin4 A3
int moiPin[4] = {MPin1, MPin2, MPin3, MPin4};

//^^^^^^^^^^^^^^^^^^^^^^^^^
//defining relays
#define rly1 6
#define rly2 7
#define rly3 8
#define rly4 9
int relay[4] = {rly1, rly2, rly3, rly4};
//^^^^^^^^^^^^^^^^^^^^^^^^^
//*****************************************************************
//setup
void setup()
{
  // begins serial monitor and lets me know if it fails
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);

  // following line sets the RTC to the date & time this sketch was compiled
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  lcd.print("Hello World!");
  if (!HTS.begin())
  {
    lcd.print("HTS.ERROR");
    while (1);
  }
  // set up the LCD's number of rows and columns:

  // Print a message to the LCD.

  relaySetup();

}
//*****************************************************************
//sets the pinmodes to outputs and turns them all off
void relaySetup()
{
  for (i = 0; i < 4; i++) {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], HIGH);
  }
}
//*****************************************************************
void dateTime ()
{
  DateTime now = rtc.now();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print('/');
  lcd.print(now.year(), DEC);
  lcd.print(' ');
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
}
//*****************************************************************
void checkE()
{
  lcd.clear();
  lcd.setCursor(0, 0);

  temp = HTS.readTemperature(FAHRENHEIT);
  lcd.print("Temp is " );
  lcd.print(temp);
  Serial.print("temperature is ");
  Serial.println(temp);

  lcd.setCursor(0, 1);
  hum = HTS.readHumidity();
  lcd.print("humidity: ");
  lcd.print(hum);
  Serial.print("humdity: ");
  Serial.println(hum);
}
//*****************************************************************
//prints moister levels
void checkMoister(int i)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  int b = i + 1;
  moi[i] = int(analogRead(moiPin[i]));
  lcd.print("Moisture " + String(b) + ": " + String(moi[i]));
  Serial.print("Moiature 1: ");
  Serial.println(moi[i]);
}
//*****************************************************************
void feed(int i)
{
  DateTime now = rtc.now();
  if (now.hour() == 15 /*&& now.minute() == 15*/)
  {  
      if (moi[i] > 500)
    {
      digitalWrite(relay[i], LOW);
      delay(1000);
      digitalWrite(relay[i], HIGH);
      delay(1000);
    }
  }
}
//*****************************************************************
void loop()
{
  dateTime();
  delay(pause);
  for (i = 0; i < 4; i++)
  {
    checkMoister(i);
    delay(pause);
    feed(i);
    delay(pause);
  }
  

  checkE();
  delay(pause);
}
//*****************************************************************
