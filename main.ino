// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include "Adafruit_mfGFX.h"   // Core graphics library
#include "RGBmatrixPanel.h" // Hardware-specific library
#include "math.h"
#include "speedometer.h"
#include "string.h"

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

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
int sensor = A5;
int switchPin = A3;
double sensorValue;
String displayValue;


void setup() {
	initialize_speed_list();
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
	delay(4000);
  // whew!
}

String response;
int  textX   = matrix.width(),
     textMin = sizeof(response) * -12,
     hue     = 0;
bool display_message = true;

float new_distance;
float old_distance;
float speed_list[30];
int speed_count = 0;

void initialize_speed_list()
{
	for (int count=0; count < 30; count++)
	{
		speed_list[count] = -1;
	}
	speed_count = 0;
}

float get_distance()
{
	return (analogRead(sensor)/6.4)/12;
}

void add_speed(float speed)
{
	if (speed_count == 30)
		initialize_speed_list();
	if (speed < MAX_SPEED)
	{
		speed_list[speed_count++] = speed;
	}
}

float ave_speed()
{
	if (speed_count == 0) return 0;
	float result = 0;
	for (int count = 0; count < speed_count; count++)
	{
		result += speed_list[count];
	}
	return result/speed_count;
}

void printMessage(const char *name, const char *data) {
	String resp(data);
	textMin = strlen(data) * -12;
	response = resp;
}

void loop() {
	// Clear background
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  new_distance = get_distance();
	display_message = (new_distance > MAX_THRESHOLD);

	if (display_message)
	{
		initialize_speed_list();
		old_distance = 0;
	  // Draw big scrolly text on top
	  matrix.setTextColor(matrix.ColorHSV(255, 255, 255, true)); //this was ...(hue, 255, 255, true)
		matrix.setTextSize(2);
	  matrix.setCursor(textX, 1);
	  matrix.print(response);

	  // Move text left (w/wrap), increase hue
	  if((--textX) < textMin) textX = matrix.width();

	  // Update display
	  matrix.swapBuffers(false);
	  delay(40);
	}
	else
	{
		if (old_distance == 0) {
			old_distance = MAX_THRESHOLD;
		}
		float speed = (abs(old_distance - new_distance) * (SEC_HOUR * (MILLI_SEC/TIMER_WAIT)))/FT_MILE;
		add_speed(speed);
		//float speed = (((abs(old_distance - new_distance))/FT_MILE)*SEC_HOUR)*(MILLI_SEC/TIMER_WAIT);
		matrix.setTextSize(1);
		matrix.setCursor(1, 1);
		char display_speed[10];
		sprintf(display_speed, "%.2f\nMPH", ave_speed());
		matrix.print(display_speed);
		old_distance = new_distance;
		delay(TIMER_WAIT);
	}
}
