#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Adafruit_SSD1306.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_LIS3DH.h>

#include "RTClib.h"

RTC_DS3231 rtc;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_BMP280 bme; // I2C

int battery = 0.00;
int temp = 0;

int battery_flag = false;
int temp_flag = false;
int time_flag = false;

unsigned long update_millis = 0;

void setup()
{

  Serial.begin(115200);            // For Serial Monitor
  Serial.println("SNHS Watch v1"); //This prints one time at Arduino power-up,  or on Arduino reset.

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();

  if (!bme.begin())
  {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1)
      ;
  }

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  //  rtc.adjust(DateTime(2019, 10, 17, 18, 35, 0));
  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2019, 10, 3, 18, 5, 0));
  }
  update_millis = millis();
  bmp();
    battery_level();
    datetime();
}

void bmp()
{

  temp = bme.readTemperature();
  display.setTextColor(WHITE); // Draw white text
  display.setTextSize(1);      //

  display.setCursor(0, 0); //
  display.print(temp);
  display.print(" C");

  display.display();
}
void battery_level()
{

  battery = analogRead(A1) ;
 battery = map(battery, 0, 260, 0, 100);
  display.setCursor(90, 0); //

  display.print(battery);
  display.print(" %");

  display.display();
}
void datetime()
{
  DateTime now = rtc.now();

  display.setTextSize(2);    //
  display.setCursor(27, 20); //

  display.print(now.hour());
  display.print(F(":"));
  display.print(now.minute());

  display.setCursor(30, 45); //
  display.setTextSize(1);    //

  display.print(now.day());
  display.print(F(":"));
  display.print(now.month());
  display.print(F(":"));
  display.println(now.year());

  display.display();
}

void loop()
{

  if ((millis() - update_millis) >= 60000)
  {
    display.clearDisplay();
    display.display();

    bmp();
    battery_level();
    datetime();

    update_millis = millis();
  }
}
