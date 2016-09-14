#include "FastLED.h"

#include "buttons.h"

#include "plasma.cpp"
#include "stripes.cpp"
#include "fire.cpp"
#include "descent.cpp"
#include "solid.cpp"
#include "noise.cpp"

// These are my specific LED strings
#define LED_PIN     17
#define CHIPSET     WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    300  // Changed from 400 (not enough memory.

// Reasonable for battery-powered jacket - use 300 if powering from only USB cable
#define MAX_POWER 300   // Was 500
#define BRIGHTNESS  64 // Was 150

// Sketches are tuned for this speed.  TODO: consistent framerate for sketches inside Animation class
#define FRAMES_PER_SECOND 18

// The actual LEDs and their X/Y positions
CRGB leds[NUM_LEDS];
struct { int x; int y; } position[NUM_LEDS];




// This is such an ugly function, but it's how I create 400 X,Y positions
#define HALF 5
#define HALF_Y 6
void get_xy_jacket(int i, int *x, int *y) {
  int h = 12;
  int baseNumber = 255;
  if ( (i/25)%2 == 0 ) {
    *x = 10 + (i-(i/25*25))  * 255 / 25 - HALF;
  } else {
    *x = 255 - (i-(i/25*25))  * 255 / 25 - HALF;
  }
//  if ( i < 25 ) {
//    *x += 10;
//  }
  *y = (h * (i/25)) + HALF_Y;
//  Serial.print("i=");
//  Serial.print(i);
//  Serial.print(", x=");
//  Serial.print(*x);
//  Serial.print(", y=");
//  Serial.println(*y);
}

void old_get_xy_jacket(int i, int *x, int *y) {
  int h = 12;
  
  if (i < 25) {
    *x = i * 255 / 25 + HALF;
    *y = HALF_Y;
  } else if (i < 50) {
    *x = 255 - (i-25)  * 255 / 25 - HALF;
    *y = h + HALF_Y;
  } else if (i < 75) {
    *x = (i - 50) * 255 / 25 + HALF;
    *y = h * 2 + HALF_Y;
  } else if (i < 100) {
    *x = 255 - (i-75)  * 255 / 25 - HALF;
    *y = h * 3 + HALF_Y;
  } else if (i < 125) {
    *x = (i - 100) * 255 / 25 + HALF;
    *y = h * 4 + HALF_Y;
  } else if (i < 150) {
    *x = 255 - (i-125)  * 255 / 25 - HALF;
    *y = h * 5 + HALF_Y;
  } else if (i < 175) {
    *x = (i - 150) * 255 / 25 + HALF;
    *y = h * 6 + HALF_Y;
  } else if (i < 200) {
    *x = 255 - (i-175)  * 255 / 25 - HALF;
    *y = h * 7 + HALF_Y;
  } else if (i < 224) {
    *x = (i - 200) * 255 / 24 + HALF;
    *y = h * 8 + HALF_Y;
    if (i >= 203 && i <= 210)
      *x += HALF;
  } else if (i < 248) {
    *x = 255 - (i - 224) * 255 / 24 - HALF;
    *y = h * 9 + HALF_Y;
    if (i < 238)
      *x -= HALF*2;
  } else if (i < 272) {
    *x = (i - 248)  * 255 / 24 + HALF;
    *y = h * 10 + HALF_Y;
    if (i < 260)
      *x += HALF;
  } else if (i < 296) {
    *x = 255 - (i - 272) * 255 / 24 - HALF;
    *y = h * 11 + HALF_Y;
  } else if (i < 300) {
    *x = (i - 296) * 10 + 10;
    *y = h * 12 + HALF_Y;
  } else if (i < 310) {
    *x = (i - 300) * 11 + 70;
    *y = h * 13 + HALF_Y;
  } else if (i < 320) {
    *x = 165 - (i - 310) * 11;
    *y = h * 14 + HALF_Y;
  } else if (i < 329) {
    *x = (i - 320) * 127 / 11 + 75;
    *y = h * 15 + HALF_Y;
  } else if (i < 338) {
    *x = 160 - (i - 329) * 127 / 11;
    *y = h * 16 + HALF_Y;
  } else if (i < 348) {
    *x = (i - 338) * 127 / 11 + 72;
    *y = h * 17 + HALF_Y;
  } else if (i == 348) {
    *x = 180;
    *y = h * 16 + HALF_Y;
  } else if (i == 349) {
    *x = 180;
    *y = h * 15 + HALF_Y;
  } else if (i < 355) {
    *x = (i - 350) * 64 / 5 + 192;
    *y = h * 13 + HALF_Y;
  } else if (i < 360) {
    *x = 252 - (i - 355) * 64 / 4;
    *y = h * 14 + HALF_Y;
  } else if (i < 365) {
    *x = (i - 360) * 64 / 5 + 192;
    *y = h * 15 + HALF_Y;
  } else if (i < 369) {
    *x = 247 - (i - 365) * 55 / 4;
    *y = h * 16 + HALF_Y;
  } else if (i < 373) {
    *x = (i - 369) * 45 / 4 + 205;
    *y = h * 17 + HALF_Y;
  } else if (i < 375) {
    *x = 235 - (i - 373) * 15;
    *y = h * 18 + HALF_Y;
  } else if (i == 375) {
    *x = 53;
    *y = h * 12 + HALF_Y;
  } else if (i < 381) {
    *x = 60 - (i - 375) * 60 / 5 + HALF;
    *y = h * 13 + HALF_Y;
  } else if (i < 386) {
    *x = (i - 381) * 255 / 22 + HALF;
    *y = h * 14 + HALF_Y;
  } else if (i < 391) {
    *x = 55 - (i - 386) * 60 / 5;
    *y = h * 15 + HALF_Y;
  } else if (i < 395) {
    *x = (i - 391) * 255 / 22 + HALF;
    *y = h * 16 + HALF_Y;
  } else if (i < 398) {
    *x = 40 - (i - 395) * 40 / 3;
    *y = h * 17 + HALF_Y;
  } else {
    *x = -1;
    *y = 255;
  }
//  Serial.print("i=");
//  Serial.print(i);
//  Serial.print(", x=");
//  Serial.print(*x);
//  Serial.print(", y=");
//  Serial.println(*y);
}


// Current, previous animations and transition counter
Animation *anim;
Animation *lastAnim;
fract8 animTransition;

#define TIME_CYCLE 20000
#define TIME_PAUSED ((unsigned long)-1)
unsigned long nextAnimationTime = 0;
int state = 1; // Start state

void NextAnimation(int pin)
{
  state = leaveState(state);
  Serial.print("next animation state: ");
  Serial.println(state);
}

void Pause(int pin)
{
  if (nextAnimationTime == TIME_PAUSED) {
    Serial.println("unpause");
    nextAnimationTime = millis();
  } else {
    Serial.println("pause");
    nextAnimationTime = TIME_PAUSED;
  }
}


int leaveState(int i) {
//  Serial.print(", ");
//  Serial.print(state);
  i = (i+1) % 10;
//  Serial.println("leaveState: ");
//  Serial.println(i);
  return enterState( i );
}

int enterState(int i) {
  switch (i) {
    case 0:
      beginAnimation(new Noise());
      break;
    case 1:
      beginAnimation(new Plasma());
      break;
    case 2:
      beginAnimation(new Fire());
      break;
    case 3:
      beginAnimation(new Descent(true));
      break;
    case 4:
      beginAnimation(new Noise());
      break;
    case 5:
      beginAnimation(new Fire());
      break;
    case 6:
      beginAnimation(new Plasma());
      break;
    case 7:
      beginAnimation(new Descent(true));
      break;
    case 8:
      beginAnimation(new Fire());
      break;
    case 9:
      beginAnimation(new Plasma());
      break;
  }
  return i;
}




FWB Button;



void setup() { 
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  
  // Teensy needs this?
  pinMode(13, OUTPUT);
   set_max_power_indicator_LED( 13); 
  
  Button.Configure(12, PULL_UP);
  Button.OnClick = NextAnimation;
  Button.OnLongPress = Pause;
  
  // Set up the LED string and appropriate power config
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  set_max_power_in_volts_and_milliamps( 5, MAX_POWER);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither( false );
  
  Serial.begin(9600);
  Serial.print("started State = ");
//  Serial.println(state);

  // Precalc all posiitons
  for (int i = 0; i < NUM_LEDS; i++) {  
    int x,y;
    old_get_xy_jacket(i, &position[i].x, &position[i].y);

    /* Debug positions:*/
    Serial.print("#"); 
    Serial.print(i);
    Serial.print(": ");
    Serial.print(position[i].x);
    Serial.print(",");
    Serial.print(position[i].y);
    Serial.println();
    int oldX = position[i].x;
    int oldY = position[i].y;
    get_xy_jacket(i, &position[i].x, &position[i].y);
    if ( oldX != position[i].x || oldY != position[i].y) {
      Serial.print("DIFF #"); 
      Serial.print(i);
      Serial.print(": ");
      Serial.print(position[i].x);
      Serial.print(",");
      Serial.print(position[i].y);
      Serial.println();
    }
  }
}



void loop() {
  unsigned long now = millis();
  unsigned long next = now + (1000 / FRAMES_PER_SECOND);

  Button.CheckBP();
  if (now >= nextAnimationTime) {
    NextAnimation(0);
    nextAnimationTime = now + TIME_CYCLE;
  }

  // Cycle current and previous animations
  // Continue fading between current animations
  if (anim)
    anim->loop(now);
  if (lastAnim) {
    lastAnim->loop(now);
    animTransition = qsub8(animTransition, 3);
    if (!animTransition) {
      delete lastAnim;
      lastAnim = (Animation *)0;
    }
  }
  
  // Map from the virtual matrix to the actual LEDs, blending previous
  for(int i = 0; i < NUM_LEDS; i++) {
   if (anim)
     leds[i] = anim->at(position[i].x, position[i].y);
   if (lastAnim)
     leds[i] = blend(leds[i], lastAnim->at(position[i].x, position[i].y), animTransition);
  }

  // Try to maintain a consistent framerate regardless of how expensive calculation is
  show_at_max_brightness_for_power();
  now = millis();
  if (next > now)
    delay_at_max_brightness_for_power(next - now);
  
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());
}

void beginAnimation(Animation *newAnim) {
  // If there's an existing previous transition, let it continue fading away.
  if (!lastAnim) {
    lastAnim = anim;
    animTransition = 255;
  } else {
    delete anim;
  }

  // Start up the new animation
  anim = newAnim;
  newAnim->start();
}


