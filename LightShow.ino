
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    6

#define LED_COUNT 93

const int speedPotentio = 0;
const int potentio1 = 1;
const int brightPotentio = 2;
const int changeButton = 2;
const int circleButton = 3;
int loopCounter = 0;

volatile byte state = 0;
volatile byte circleState = LOW;

//int gate = 2;
//int envelop = 4;
//int audio = 5;
//int total = 0;
//int average = 0;
//int divident = 1;
//int PIN_LED_OUT = 13;
//int previousSounds[] = {0, 0};

int ringStartingIndex[6] = {0, 32, 56, 72, 84, 92};
/*index where each ring starts: [0, 32, 56, 72, 84, 92]
   Ring 1 can be divided by 8 to access each corner
   Ring 2 every 6 is a corner
   Ring 3 every 4
   Ring 4 every 3
   Ring 5 every 2
   Ring 6 0 is the center
*/

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


//void soundISR()
//{
//  int pin_val;
//
//  pin_val = digitalRead(gate);
//  digitalWrite(PIN_LED_OUT, pin_val);
//}

// setup() function -- runs once at startup --------------------------------

void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(20); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.begin(9600);
  randomSeed(analogRead(5));
  pinMode(changeButton, INPUT_PULLUP);
  pinMode(circleButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(changeButton), toggle, RISING);
  attachInterrupt(digitalPinToInterrupt(circleButton), toggle2, RISING);
  //  pinMode(gate, INPUT);
  //  attachInterrupt(gate, soundISR, CHANGE);
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {
	switch(state){
		case 0:
			matrix(loopCounter);
			break;
		case 1:
			snail();
			break;
		case 2:
			spin();
			break;
		case 3:
			smileyFace();
			break;
		case 4:
			expand(0);
			break;
		case 5:
			collapse(0);
			break;
		case 6:
			beatPulse();
			break;
		}
	if(circleState == HIGH){
    //circle yeeeeet
    circleState = !circleState;
    Serial.println("Button pressed, go cirles!");
    collapse(1);
    expand(1);
	}
 //this makes it go back and forth between 0 and 1
 if(loopCounter == 0){
  loopCounter++;
 }
 else{
  loopCounter--;
 }
	

  // Fill along the length of the strip in various colors...
  //  colorWipe(strip.Color(255,   0,   0), 50); // Red
  //  colorWipe(strip.Color(  0, 255,   0), 50); // Green
  //  colorWipe(strip.Color(  0,   0, 255), 50); // Blue

  // Do a theater marquee effect in various colors...
  //  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
  //  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
  //  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness
  //
  //  rainbow(10);             // Flowing rainbow cycle along the whole strip
  //  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  //delay(25);
}


void spin()
{
	int line1[] = {92, 84, 72, 56, 32, 0};
	int line2[] = {92, 91, 70, 53, 28};
	int line3[] = {92, 90, 81, 68, 50, 24};
	int line4[] = {92, 89, 66, 47, 20};
	int line5[] = {92, 88, 78, 64, 44, 16};
	int line6[] = {92, 87, 62, 41, 12};
	int line7[] = {92, 86, 75, 60, 38, 8};
	int line8[] = {92, 85, 58, 35, 4};
	int allLines[] = { line1, line2, line3, line4, line5, line6, line7, line8 };
	int j = 4;
  
	for(int i = 0; i < sizeof(allLines)/ sizeof(*allLines); ++i){
		strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
    if(i % 2 == 0){
      lightUpSpecifics(allLines[i], 6);
      lightUpSpecifics(allLines[j], 6);
    }
    else{
      lightUpSpecifics(allLines[i], 5);
      lightUpSpecifics(allLines[j], 5);
    }
    ++j;
    if(j > 7)
      j = 0;
		delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
		strip.clear();
	}
}


/* Immitates light falling from top to bottom, like matrix where there is a wall of numbers (time controlled by potentiometer)
   Would be cool to find a way to not spaghetti code this one
*/
void matrix(int option)
{
  int IndexVertical[] = {4, 36, 37, 38, 39, 40, 12};	//7
  int IndexVertical1[] = {3, 35, 59, 60, 61, 41, 13};	//7
  int IndexVertical2[] = {2, 34, 58, 74, 75, 76, 62, 42, 14};	//9
  int IndexVertical3[] = {1, 33, 57, 73, 85, 86, 87, 77, 63, 43, 15};	//11
  int IndexVertical4[] = {0, 32, 56, 72, 84, 92, 88, 78, 64, 44, 16};	//11
  int IndexVertical5[] = {31, 55, 71, 83, 91, 90, 89, 79, 65, 45, 17};	//11
  int IndexVertical6[] = {30, 54, 70, 82, 81, 80, 66, 46, 18};	//9
  int IndexVertical7[] = {29, 53, 69, 68, 67, 47, 19};	//7
  int IndexVertical8[] = {28, 52, 51, 50, 49, 48, 20};	//7
  int v0, v1, v2, v3, v4, v5, v6, v7, v8;
  v0 = random(5, 7);
  v1 = random(5, 7);
  v2 = random(6, 9);
  v3 = random(7, 11);
  v4 = random(7, 11);
  v5 = random(7, 11);
  v6 = random(6, 9);
  v7 = random(5, 7);
  v8 = random(5, 7);
  strip.setPixelColor(IndexVertical[v0], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical1[v1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 15));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical2[v2], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 30));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical3[v3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 45));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical4[v4], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 60));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical5[v5], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 75));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical6[v6], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 90));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical7[v7], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 105));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical8[v8], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 120));
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
  strip.clear();

  v0 = random(4, 6);
  v1 = random(4, 6);
  v2 = random(5, 7);
  v3 = random(7, 9);
  v4 = random(7, 9);
  v5 = random(7, 9);
  v6 = random(5, 7);
  v7 = random(4, 6);
  v8 = random(4, 6);
  strip.setPixelColor(IndexVertical[v0], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical1[v1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 20));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical2[v2], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 40));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical3[v3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 60));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical4[v4], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 80));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical5[v5], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 100));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical6[v6], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 120));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical7[v7], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 140));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical8[v8], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 160));
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
  strip.clear();

  v0 = random(2, 4);
  v1 = random(2, 4);
  v2 = random(3, 5);
  v3 = random(4, 7);
  v4 = random(4, 7);
  v5 = random(4, 7);
  v6 = random(3, 5);
  v7 = random(2, 4);
  v8 = random(2, 4);
  strip.setPixelColor(IndexVertical[v0], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical1[v1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 30));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical2[v2], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 60));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical3[v3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 90));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical4[v4], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 120));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical5[v5], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 150));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical6[v6], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 180));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical7[v7], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 210));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical8[v8], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 240));
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
  strip.clear();

  v0 = random(0, 2);
  v1 = random(0, 2);
  v2 = random(1, 3);
  v3 = random(2, 4);
  v4 = random(2, 4);
  v5 = random(2, 4);
  v6 = random(1, 3);
  v7 = random(0, 2);
  v8 = random(0, 2);
  strip.setPixelColor(IndexVertical[v0], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical1[v1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 10));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical2[v2], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 20));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical3[v3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 30));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical4[v4], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 40));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical5[v5], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 50));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical6[v6], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 60));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical7[v7], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 70));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical8[v8], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 80));
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
  strip.clear();

  v0 = random(0, 2);
  v1 = random(0, 2);
  v2 = random(0, 2);
  v3 = random(0, 2);
  v4 = random(0, 2);
  v5 = random(0, 2);
  v6 = random(0, 2);
  v7 = random(0, 2);
  v8 = random(0, 2);
  strip.setPixelColor(IndexVertical[v0], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical1[v1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical2[v2], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical3[v3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 255));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical4[v4], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 255));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical5[v5], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 255));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical6[v6], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 255));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical7[v7], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  //	strip.show();
  //	delay(35);
  strip.setPixelColor(IndexVertical8[v8], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  if(option == 0)
  {
    lightUpARing(1, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
    lightUpARing(3, strip.Color(150, map(analogRead(potentio1), 0, 1023, 0, 255), 150));
    lightUpARing(5, strip.Color(0, map(analogRead(potentio1), 0, 1023, 0, 255), 255));  
  }
  else
  {
    lightUpARing(2, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
    lightUpARing(4, strip.Color(150, map(analogRead(potentio1), 0, 1023, 0, 255), 150));
    lightUpARing(6, strip.Color(0, map(analogRead(potentio1), 0, 1023, 0, 255), 255));
  }
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
  strip.clear();
  strip.show();

  delay(map(analogRead(speedPotentio), 0, 1023, 15, 400));
}

/* Creates a snailish pattern that goes to the center

*/
void snail()
{
  int ringStartIndexHorizontal[11] = {8, 38, 60, 75, 86, 92, 90, 81, 68, 50, 24};
  int ringStartIndexVertical[11] = {0, 32, 56, 72, 84, 92, 88, 78, 64, 44, 16};
  int sizeIndex = 10;	//0-10 is size 11 but we index starting at 0
  int q1 = 0, q2 = 1, q3 = 2, q4 = 3;
  for (int i = 0; i < 5; ++i) {	//5 for 5 iterations total, should make a total loop
	strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
    lightUpASection(ringStartIndexVertical[q1] + 1, ringStartIndexHorizontal[q1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
	strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
    lightUpASection(ringStartIndexHorizontal[q2] + 1, ringStartIndexVertical[sizeIndex - q2], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
	strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
    lightUpASection(ringStartIndexVertical[sizeIndex - q3] + 1, ringStartIndexHorizontal[sizeIndex - q3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
	strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
    lightUpASection(ringStartIndexHorizontal[sizeIndex - q4] + 1, ringStartIndexVertical[q4 + 1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));

    //    lightUpASection(ringStartIndexVertical[q3] + 1, ringStartIndexHorizontal[q3], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), map(analogRead(brightPotentio), 0, 1023, 0, 255)));
    //    lightUpASection(ringStartIndexHorizontal[q4] + 1, ringStartIndexVertical[sizeIndex - q4], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), map(analogRead(brightPotentio), 0, 1023, 0, 255)));
    //    lightUpASection(ringStartIndexVertical[sizeIndex - q1] + 1, ringStartIndexHorizontal[sizeIndex - q1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), map(analogRead(brightPotentio), 0, 1023, 0, 255)));
    //    lightUpASection(ringStartIndexHorizontal[sizeIndex - q2] + 1, ringStartIndexVertical[q2 + 1], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), map(analogRead(brightPotentio), 0, 1023, 0, 255)));

    q1 = (q1 + 1) % 5;
    q2 = (q2 + 1) % 5;
    q3 = (q3 + 1) % 5;
    q4 = (q4 + 1) % 5;
    delay(map(analogRead(speedPotentio), 0, 1023, 10, 200));
    strip.clear();
  }
}

/* Function that draws a smiley face, it will either grow and shrink, or move around
*/
void smileyFace()
{
  //normal size smiley face will have eyes on ring 5 and smile on ring 4
  //drawing eyes
  int smile[7] = {86, 90, 72, 73, 74, 82, 83};
  int smile2[] = {75, 81, 56, 57, 58, 59, 69, 70, 71};
  int smile3[] = {60, 68, 32, 33, 34, 35, 36, 52, 53, 54, 55};
  int smile4[] = {38, 50, 0, 1, 2, 3, 4, 5, 6, 26, 27, 28, 29, 30, 31};
  //TODO finish smiley face
  for (int j = 0; j < 2; ++j)
  {
    for (int i = 0; i < sizeof(smile) / sizeof(*smile); ++i) {
      strip.setPixelColor(smile[i], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
      if (j == 0)
        strip.show();
      delay(30);
    }
    if (j == 0)
      strip.clear();
    strip.show();
  }
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 0, 300));
  strip.clear();

  for (int j = 0; j < 2; ++j)
  {
    for (int i = 0; i < sizeof(smile2) / sizeof(*smile2); ++i) {
      strip.setPixelColor(smile2[i], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
      if (j == 0)
        strip.show();
      delay(25);
    }
    if (j == 0)
      strip.clear();
    strip.show();
  }
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 0, 300));
  strip.clear();

  for (int j = 0; j < 2; ++j)
  {
    for (int i = 0; i < sizeof(smile3) / sizeof(*smile3); ++i) {
      strip.setPixelColor(smile3[i], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
      if (j == 0)
        strip.show();
      delay(25);
    }
    if (j == 0)
      strip.clear();
    strip.show();
  }
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 0, 300));
  strip.clear();

  for (int j = 0; j < 2; ++j)
  {
    for (int i = 0; i < sizeof(smile4) / sizeof(*smile4); ++i) {
      strip.setPixelColor(smile4[i], strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
      if (j == 0)
        strip.show();
      delay(15);
    }
    if (j == 0)
      strip.clear();
    strip.show();
  }
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 0, 300));
  strip.clear();
}



/* Function that expends outwards from the center to the outer rings
*/
void expand(int option) {
	int delayAmount;
	if(option == 0){
		delayAmount = map(analogRead(speedPotentio), 0, 1023, 0, 250);
	}
	else{
		delayAmount = 50;
	}
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(6, strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(5, strip.Color(12, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(4, strip.Color(61, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(3, strip.Color(0, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(2, strip.Color(65, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(1, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
}


/* Function that acts as a transition and collapses to the center quickly ish
*/
void collapse(int option)
{
	int delayAmount;
	if(option == 0){
		delayAmount = map(analogRead(speedPotentio), 0, 1023, 0, 250);
	}
	else{
		delayAmount = 50;
	}
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(1, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(2, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(3, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(4, strip.Color(255, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(5, strip.Color(12, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
  strip.setBrightness(map(analogRead(brightPotentio), 0, 1023, 0, 255));
  lightUpARing(6, strip.Color(153, map(analogRead(potentio1), 0, 1023, 0, 255), 0));
  delay(delayAmount);
  strip.clear();
}

/* Function that receives an array of leds to light up, different than lightUpASection because that one lights up from start to end (range)
*  While this function lights up specific leds that are provided to us through an array with random colors using hue -> rgb
*/
void lightUpSpecifics(int ledArray[], int arrSize)
{   
	for(int i = 0; i < arrSize; ++i){
    int pixelHue = random(0-256) * random(0-256) + (map(analogRead(potentio1), 0, 1023, 0, 92) * 65536L / strip.numPixels());
		strip.setPixelColor(ledArray[i], strip.gamma32(strip.ColorHSV(pixelHue)));
	}
	strip.show();
}


/* Function that lights up specific leds given the start and end of what needs to be lit up, start is included, end is excluded
* There is no delay when lighting up a section, it just shows the result, not the progress
*/
void lightUpASection(int start, int end, uint32_t color)
{
  for (int i = start; i < end; ++i) {
    strip.setPixelColor(i, color);
    //delay(15);
  }
  strip.show();
}

/* Function that lights up an entire ring, needs a ring number to know which to light up, ring 1 is the bottom/biggest ring
*/
void lightUpARing(int ringNumber, uint32_t color)
{
  int ledAmount;
  int i;
  if (ringNumber == 1) {
    ledAmount = 32;
    i = 0;
  }
  else if (ringNumber == 2) {
    ledAmount = 24 + 32;
    i = 32;
  }
  else if (ringNumber == 3) {
    ledAmount = 16 + 24 + 32;
    i = 24 + 32;
  }
  else if (ringNumber == 4) {
    ledAmount = 12 + 16 + 24 + 32;
    i = 16 + 24 + 32;
  }
  else if (ringNumber == 5) {
    ledAmount = 8 + 12 + 16 + 24 + 32;
    i = 12 + 16 + 24 + 32;
  }
  else {
    ledAmount = 1 + 8 + 12 + 16 + 24 + 32;
    i = 92;
  }
  for (i; i < ledAmount; ++i) {
    strip.setPixelColor(i, color);
    //strip.show();
  }
  strip.show();
}

/* Function that increases and decreases brightness to immitate it pulsing to a beat
   Pontentiometer
*/
void beatPulse()
{
  //TODO, replace the wait time on pulsing by actual data from the song

  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, strip.Color(124, map(analogRead(potentio1), 0, 1023, 0, 255), 0));         //  Set pixel's color (in RAM)
  }
  strip.setBrightness(255);
  strip.show();                          //  Update strip to match
  Serial.println("Boom");
  delay(map(analogRead(speedPotentio), 0, 1023, 20, 400));
  strip.setBrightness(20);
  strip.show();
  delay(map(analogRead(speedPotentio), 0, 1023, 20, 400));
  strip.clear();
}



// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    int brightness = map(analogRead(potentio1), 0, 1023, 0, 255);
    strip.setBrightness(brightness);
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
void theaterChase(uint32_t color, int wait) {
  for (int a = 0; a < 10; a++) { // Repeat 10 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      int brightness = map(analogRead(potentio1), 0, 1023, 0, 255);
      strip.setBrightness(brightness);
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    int brightness = map(analogRead(potentio1), 0, 1023, 0, 255);
    strip.setBrightness(brightness);
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++) { // Repeat 30 times...
    for (int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      int brightness = map(analogRead(potentio1), 0, 1023, 0, 255);
      strip.setBrightness(brightness);
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void toggle() {
  state++;
  if(state >= 7){
    state = 0;
  }
}

void toggle2(){
  circleState = !circleState;
}
