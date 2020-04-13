#include <Arduino.h>
//#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <CircularBuffer.h>

CircularBuffer<int, 30> buffer;
unsigned long time = 0;

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

int ox;
int oy;
int x;
int y;

//Draw x & y axis
void drawaxis() {
  tft.drawFastVLine(25, 20, 200, ILI9341_BLUE);
  tft.drawFastVLine(295, 20, 200, ILI9341_YELLOW);
  tft.drawFastHLine(25, 220, 270, ILI9341_WHITE); //horizontal line, 270 pix long (/30s= 9 pix per second increment)
}

void drawlabels () {
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  //draw time axis labels, 30 seconds
  for (int i = 1; i < 6; i++) {
    tft.setCursor((19 + (45*i)), 222);
    tft.println(5*i);
  }
  //draw pressure axis labels, 0-300 bar
  for (int i = 0; i < 11; i++) {
    tft.setCursor(5, (216 - (20*i)));
    tft.println(30*i);
  }
  //draw voltage axis labels, 19-24V
  for (int i = 0; i < 7; i++) {
    tft.setCursor(300, (216 - (40*i)));
    tft.println(19+i);
  }
}

void setup() {
  
  Serial.begin(9600);
  tft.begin();

  delay(1000);

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  drawaxis();
  drawlabels();

  //Fllowing code to draw basic graph with 52 values
  // for (int i= 0; i<= 52; i++) {

  //   x = 20+(5*i);
  //   y = random(25, 210);   
  //   if (i==0) {
  //     ox = x;
  //     oy = y;
  //   }

  //   tft.drawLine(ox, oy, x, y, ILI9341_RED);
  //   ox=x;
  //   oy=y;
  // }
}


void loop(void) 
{

	if (millis() - time >= 1000) {

    if (buffer.isFull() ==true) {
      for (int i = 0; i < buffer.size(); i++) {
      x = 25+(9*i);
      y = buffer[i];   
      if (i==0) {
        ox = x;
        oy = y;
      }

      tft.drawLine(ox, oy, x, y, ILI9341_BLACK);
      ox=x;
      oy=y;
      }
      drawaxis();
    }    
    int reading = random(25,210);
	  buffer.push(reading);
		time = millis();

		for (int i = 0; i < buffer.size(); i++) {
    x = 25+(9*i);
    y = buffer[i];   
    if (i==0) {
      ox = x;
      oy = y;
    }

    tft.drawLine(ox, oy, x, y, ILI9341_RED);
    ox=x;
    oy=y;
		}

  Serial.println(buffer.size());

	}


}