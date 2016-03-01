// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h" // Hardware-specific library
#include "math.h"
#include "string.h"

#if defined (STM32F2XX)	//Photon
	#define CLK D6
	#define OE  D7
	#define LAT A4
	#define A   A0
	#define B   A1
	#define C   A2
	#define D	A3		// Only used for 32x32 panels
#endif

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
int sensor = A5;
int switchPin = A3;
double sensorValue;
String displayValue;


void setup() {
  pinMode(sensor, INPUT);
	pinMode(switchPin, INPUT_PULLUP);

  Serial1.begin(9600);
  matrix.begin();

  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, 32, 16, matrix.Color333(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, 31, 15, matrix.Color333(7, 0, 0));
  matrix.drawLine(31, 0, 0, 15, matrix.Color333(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(7, 7, 7, matrix.Color333(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(23, 7, 7, matrix.Color333(7, 0, 7));
  delay(500);

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('1');
  matrix.setTextColor(matrix.Color333(7,4,0));
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(0,7,0));
  matrix.print('2');

  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color333(0,7,7));
  matrix.print('*');
  matrix.setTextColor(matrix.Color333(0,4,7));
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7));
  matrix.print("B");
  matrix.setTextColor(matrix.Color333(7,0,4));
  matrix.print("*");
	delay(200);

	Particle.subscribe("hook-response/get_message_data", printMessage, MY_DEVICES);
	Particle.publish("get_message_data");
  matrix.fillScreen(matrix.Color333(0, 0, 0));
	matrix.setTextWrap(false);
	matrix.setTextSize(2);
	delay(500);
  // whew!
}

char response[4000];
int  textX   = matrix.width(),
     textMin = sizeof(response) * -12,
     hue     = 0;

void printMessage(const char *name, const char *data) {
	//String resp = String(data);
	/*int i = 0;
	char ch;
	while (i < 3999 && data[i] != 0) {
		response[i++] = ch;
	}
	response[i] = '\0';*/
	strcpy(response, data);
}

void loop() {
	/*
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  matrix.setTextColor(matrix.Color333(7,0,4));
  sensorValue = analogRead(sensor);
  //displayValue = sensorValue.toString();
  matrix.print((sensorValue / 6.4) / 12);
  delay(200);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  matrix.setTextColor(matrix.Color333(7,0,4));
	Particle.publish("get_message_data");
	delay(5000);
	//
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  matrix.setTextColor(matrix.Color333(7,0,4));
	if (analogRead(switchPin) < 0.5) {
	  Particle.publish("get_message_data");
		delay(3000);
	  matrix.fillScreen(matrix.Color333(0, 0, 0));
	}
	else {
		matrix.print("Taylor");
		delay(3000);
	  matrix.fillScreen(matrix.Color333(0, 0, 0));
	}	*/

  // Clear background
  matrix.fillScreen(0);

  // Draw big scrolly text on top
  matrix.setTextColor(matrix.ColorHSV(255, 255, 255, true)); //this was ...(hue, 255, 255, true)
  matrix.setCursor(textX, 1);
  matrix.print(response);

  // Move text left (w/wrap), increase hue
  if((--textX) < textMin) textX = matrix.width();
  hue += 7;
  if(hue >= 1536) hue -= 1536;

  // Update display
  matrix.swapBuffers(false);
	delay(50);
}
