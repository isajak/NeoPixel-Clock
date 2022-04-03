// Include the libraries
#include <RtcDS3231.h>
#include "U8glib.h"
#include <Wire.h> 
#include <Adafruit_NeoPixel.h>


// Create new SSD1306 I2C display with 128x64 resolution
U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NONE);  // I2C / TWI 

// Declare and initialise global GPIO pin constant for Neopixel ring
const byte neoPin = 10;

// Declare and initialise global constant for number of pixels
const byte neoPixels = 24;

// Declare and initialise variable for Neopixel brightness
byte neoBright = 5;
 
// Create new Neopixel ring object
Adafruit_NeoPixel ring = Adafruit_NeoPixel(neoPixels, neoPin, NEO_GRB);

// Create new RTC module
RtcDS3231<TwoWire> rtcModule(Wire);

// Create variables for time
byte hours;
byte minutes;
byte seconds;

// To keep track of seconds
long lastUpdate = millis();
long lastSecond = millis();

int currentSecond, currentMinute, currentHour;

void setup () {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


// Initialise the ring
  ring.begin();
  ring.setBrightness(neoBright); 
  ring.show();

// Retrieves and stores current value of millis()
  lastUpdate = millis();
  lastSecond = millis();
}

void loop() {

// If 1800000 milliseconds we update time again
 if ((millis() - lastUpdate) > 1800000) updateTime();

// Checks if 1000 milliseconds have passed since last update
 if ((millis() - lastSecond) > 1000)
 {
  // Sets all pixels to off to clear last positions
  ring.setPixelColor(currentMinute / 2.5, 0, 0, 0);
  ring.setPixelColor(currentHour * 2, 0, 0, 0);
  ring.show();

// Increases currentSecond by 1. When currentSecond reaches 59 it resets and add 1 to currentMinute.
// When currentMinute reaches 59 it resets and add 1 to currentHour which will go up to 12 before resetting.
  currentSecond++;
  if (currentSecond > 59)
  { currentSecond = 0;
    currentMinute++;
    if (currentMinute > 59)
    { currentMinute = 0;
      currentHour++;
       if (currentHour > 12) currentHour= 0;
    }
   }

// Sets currentMinute to green and currentHour to red
   ring.setPixelColor(currentMinute / 2.5, 0, 255, 0);
   ring.setPixelColor(currentHour * 2, 255, 0, 0);
   ring.show();
 }
}

// Reads from the rtc module to update time and stores the values.
void updateTime() {
   RtcDateTime now = rtcModule.GetDateTime();
   hours = now.Hour();
   minutes = now.Minute();
   seconds = now.Second();
   currentHour = now.Hour();
   if (currentHour > 12) currentHour = currentHour - 12;
   currentMinute = now.Minute();
   currentSecond = now.Second();
   lastUpdate = millis();
} 
// Prints time on lcd display
void draw(void) {
  // Set font to Helvetica size 24
  oled.setFont(u8g_font_helvB24);

  // Format and print time on display
  char timeString[10];
  sprintf(timeString, "%02u:%02u:%02u", hours, minutes, seconds);
  oled.setPrintPos(0, 45);
  oled.print(timeString);
  
}
