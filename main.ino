// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 16x32 RGB LED matrix:
// http://www.adafruit.com/products/420

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.


#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h" // Hardware-specific library
#include "math.h"

/** Define RGB matrix panel GPIO pins **/
#if defined (STM32F10X_MD)	//Core
	#define CLK D6
	#define OE  D7
	#define LAT A4
	#define A   A0
	#define B   A1
	#define C   A2
	#define D	A3		// Only used for 32x32 panels
#endif

#if defined (STM32F2XX)	//Photon
	#define CLK D6
	#define OE  D7
	#define LAT A4
	#define A   A0
	#define B   A1
	#define C   A2
	#define D	A3		// Only used for 32x32 panels
#endif
/****************************************/


RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
int sensor = A5;
double sensorValue;
String displayValue;

void setup() {
  pinMode(sensor, INPUT);

  Serial1.begin(9600);
  matrix.begin();

  // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color444(7, 7, 7));
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, 32, 16, matrix.Color444(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, 32, 16, matrix.Color444(7, 7, 0));
  delay(500);

  // draw an 'X' in red
  matrix.drawLine(0, 0, 31, 15, matrix.Color444(7, 0, 0));
  matrix.drawLine(31, 0, 0, 15, matrix.Color444(7, 0, 0));
  delay(500);

  // draw a blue circle
  matrix.drawCircle(7, 7, 7, matrix.Color444(0, 0, 7));
  delay(500);

  // fill a violet circle
  matrix.fillCircle(23, 7, 7, matrix.Color444(7, 0, 7));
  delay(500);

  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color444(0, 0, 0));

  // draw some text!
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high

  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color444(7,0,0));
  matrix.print('1');
  matrix.setTextColor(matrix.Color444(7,4,0));
  matrix.print('6');
  matrix.setTextColor(matrix.Color444(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color444(4,7,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color444(0,7,0));
  matrix.print('2');

  matrix.setCursor(1, 9);   // next line
  matrix.setTextColor(matrix.Color444(0,7,7));
  matrix.print('*');
  matrix.setTextColor(matrix.Color444(0,4,7));
  matrix.print('R');
  matrix.setTextColor(matrix.Color444(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color444(4,0,7));
  matrix.print("B");
  matrix.setTextColor(matrix.Color444(7,0,4));
  matrix.print("*");

  // whew!
}

void loop() {
  matrix.fillScreen(matrix.Color444(0, 0, 0));
  matrix.setCursor(1, 0);   // start at top left, with one pixel of spacing
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  matrix.setTextColor(matrix.Color444(7,0,4));
  sensorValue = analogRead(sensor);
  //displayValue = sensorValue.toString();
  matrix.print((sensorValue / 6.4) / 12);
  delay(200);
}
