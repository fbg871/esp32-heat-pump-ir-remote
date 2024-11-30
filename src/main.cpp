#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <random>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  randomSeed(analogRead(0));

  // Clear the buffer
  display.clearDisplay();

  // Display "Hello World" 
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

  display.println(F("Hello World!"));

  display.display();

}

uint16_t encodeRGB565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0x1F) << 11) | ((green & 0x3F) << 5) | (blue & 0x1F);
}

int getRandomInt(int min, int max) {
  return random(min, max + 1);
}

void drawRandomShape() {
    int val = getRandomInt(0, 4);

    Serial.println(val);
    display.clearDisplay();

    int circleRadius = getRandomInt(10, 50);
    int circleX = getRandomInt(10, SCREEN_WIDTH - 10);
    int circleY = getRandomInt(10, SCREEN_HEIGHT - 10);

    display.fillCircle(circleX, circleY, 10, SSD1306_WHITE);


    // if (val == 0) {
    //   int circleRadius = getRandomInt(10, 50);
    //   int circleX = getRandomInt(10, SCREEN_WIDTH - 10);
    //   int circleY = getRandomInt(10, SCREEN_HEIGHT - 10);

    //   int circleRed = getRandomInt(0, 255);
    //   int circleGreen = getRandomInt(0, 255);
    //   int circleBlue = getRandomInt(0, 255);

    //   uint16_t circleColor = encodeRGB565(circleRed, circleGreen, circleBlue);
    //   display.drawCircle(circleX, circleY, circleRadius, circleColor);


    // } else if (val == 1) {
    //   // Triangle
    //   int triangleX1 = getRandomInt(10, SCREEN_WIDTH - 10);
    //   int triangleY1 = getRandomInt(10, SCREEN_HEIGHT - 10);
    //   int triangleX2 = getRandomInt(10, SCREEN_WIDTH - 10);
    //   int triangleY2 = getRandomInt(10, SCREEN_HEIGHT - 10);
    //   int triangleX3 = getRandomInt(10, SCREEN_WIDTH - 10);
    //   int triangleY3 = getRandomInt(10, SCREEN_HEIGHT - 10);

    //   int triangleRed = getRandomInt(0, 255);
    //   int triangleGreen = getRandomInt(0, 255);
    //   int triangleBlue = getRandomInt(0, 255);

    //   uint16_t triangleColor = encodeRGB565(triangleRed, triangleGreen, triangleBlue);
    //   display.drawTriangle(triangleX1, triangleY1, triangleX2, triangleY2, triangleX3, 
    //      triangleY3, triangleColor);

    // } else if (val == 2) {

    // } else if (val == 3) {

    // }

    display.display();


}
// variables to track x, y position of the shape
int x = 10;
int y = 10;
int circleRadius = 10;

int xDirection = 1;
int yDirection = 1;

void loop() {
  display.clearDisplay();

  if (x + xDirection > SCREEN_WIDTH - circleRadius || x + xDirection < 0 + circleRadius) {
    xDirection = xDirection * -1;
  }

  if(y + yDirection > SCREEN_HEIGHT - circleRadius || y + yDirection < 0 + circleRadius) {
    yDirection = yDirection * -1;
  }

  x += xDirection;
  y += yDirection;


  display.fillCircle(x, y, 10, SSD1306_WHITE);
  display.display();
  delay(10);
}
