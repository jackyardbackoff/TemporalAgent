/** 
PIA's DES242 MACHINE: metronomic-transcendential device

connecting the motor & little display to each other.
trying to mimic realistic pendulum movement. Just very lazily...
**/

// LIBRARIES
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>

// lcd pins, object & variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 little(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String countingWords[20] = {
  "one", "two", "three", "four", // englitch
  "tahi", "rua", "toru", "wha", // te reo
  "eins", "zwei", "drei", "vier", // german
  "akhat", "shta'im", "shalosh", "arba", // hebrew
  "0001", "0010", "0011", "0100"}; // binary
// index variable: display starts @ modulo over to "one".
int wordIndex = sizeof(countingWords) - 1; 

// servo obj. & variables
Servo myServo;
const int SERVO_PIN = 8;

int centre = 155;
int angle = 45;
int speed = 7;
int offset = 20;
int waitForChange = 490;
int count = 0;


void setup() {
  // initializing the serial & little display.
  Serial.begin(9600);

  little.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  little.clearDisplay();

  myServo.attach(SERVO_PIN);
  myServo.write(centre);

  Serial.println("START!");
}


void loop() {
  // move to LEFT from CENTER.
  for (int i = centre; i <= centre + angle; i++) { 
    moveServo(i);
    // change text before end of rotation - looks better!
    if (i == centre + angle - offset) {
      changeLittleScreen();
    }
  }
  delay(waitForChange); // delay to switch sides.
  
  // moving to CENTER from LEFT.
  for (int i = centre + angle; i >= centre; i--) { 
    moveServo(i);
  }

  // moving to RIGHT from CENTER.
  for (int i = centre; i >= centre - angle; i--) { 
    moveServo(i);
    
    if (i == centre - angle + offset) { 
      changeLittleScreen();
    }
  }
  delay(waitForChange); 
  
  // moving to CENTER from RIGHT.
  for (int i = centre - angle; i <= centre; i++) { 
    moveServo(i);
  }
}

void changeLittleScreen() {
  incrementWordIndex();
  count++;
  little.clearDisplay();

  // draw WHITE TEXT in the top-left corner
  little.setTextSize(3);             
  little.setTextColor(SSD1306_WHITE);        // Draw white text
  little.setCursor(0, 0);
  little.println(countingWords[wordIndex]);

  little.display();
}

void incrementWordIndex() {
  wordIndex = (wordIndex + 1) % ((sizeof(countingWords)) / sizeof(countingWords[0]));
}

void moveServo(int angle) {
  myServo.write(angle);
  delay(speed);
}