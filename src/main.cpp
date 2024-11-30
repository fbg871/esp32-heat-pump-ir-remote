#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <random>
#include <IRremote.h>

// Display stuff
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// IR transmitter stuff
constexpr int SEND_PIN = 6;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  randomSeed(analogRead(0));

  // IR transmitter setup
  IrSender.begin(SEND_PIN);
}

/* 
  Raw pulse timings for reference, split up into how I think it should be grouped:

  88 87   - Frame start (timings in the 80ish range)
  []      - Frame data (low are values below 20, high are values above 20) - 145? bits
  110     - Frame end (timings in the 110 range)

  Then a new frame starts, repeating the same signal. Sometimes this one isn't fully captured.
*/

    // 87,88,10,33,11,32,11,32,11,32,11,11,10,11,11,32,11,11,11,
    // 10,11,11,11,11,10,11,11,32,11,32,11,11,11,32,11,11,11,10,
    // 12,10,11,10,11,11,11,11,11,32,11,32,11,32,11,32,11,32,12,
    // 31,12,31,11,33,11,10,12,10,11,10,12,10,11,10,11,11,11,11,
    // 11,10,12,10,11,32,11,10,12,32,11,10,11,11,11,10,12,10,11,
    // 10,12,10,11,10,12,10,11,11,11,10,12,10,11,32,11,32,11,32,
    // 12,10,11,10,11,11,11,10,11,11,11,11,10,11,11,11,11,10,11,
    // 32,11,11,11,11,10,11,11,32,12,31,11,11,11,109,

    // 88,87,11,32,11,32,11,33,10,33,10,11,11,10,11,33,10,12,10,
    // 11,11,11,10,11,11,11,10,33,10,33,10,12,10,33,10,11,11,11,
    // 10,12,10,11,10,12,10,11,10,34,10,33,10,33,10,33,10,34,9,
    // 34,9,34,9,34,9,12,10,12,10,12,9,12,10,12,9,12,10,12,9,12,
    // 10,12,10,33,10,12,9,34,9,12,10,12,10,12,9,12,10,12,9,12,
    // 10,12,9,12,10,12,10,12,9,12,10,33,10,34,9,34,9,13,8,13,9,
    // 13,8,13,9,13,8,13,9,13,9,13,8,13,9,34,9,13,8,13,9,13,9,35,
    // 8,35,8,13,8


void sendSignal() {
  // First chunk of the raw signal, with 0 omitted (which I suspect the receiver adds)
  uint16_t signal[] = {
    87,88,10,33,11,32,11,32,11,32,11,11,10,11,11,32,11,11,11,
    10,11,11,11,11,10,11,11,32,11,32,11,11,11,32,11,11,11,10,
    12,10,11,10,11,11,11,11,11,32,11,32,11,32,11,32,11,32,12,
    31,12,31,11,33,11,10,12,10,11,10,12,10,11,10,11,11,11,11,
    11,10,12,10,11,32,11,10,12,32,11,10,11,11,11,10,12,10,11,
    10,12,10,11,10,12,10,11,11,11,10,12,10,11,32,11,32,11,32,
    12,10,11,10,11,11,11,10,11,11,11,11,10,11,11,11,11,10,11,
    32,11,11,11,11,10,11,11,32,12,31,11,11,11,109
  };

  for (int i = 0; i < sizeof(signal) / sizeof(signal[0]); i++) {
    // boost it
    signal[i] *= 50;
  };


  // Not working, unsure why (IR LED doesn't light up at all when looking through phone camera)
  IrSender.sendRaw(signal, sizeof(signal) / sizeof(signal[0]), 38);
}

void loop() {
  if(Serial.available() > 0) {
    char received = Serial.read();
    if(received == 'S') {
      Serial.println("Sending signal");
      sendSignal();
    }
  }

  // Dump raw signal for inspection
  if (IrReceiver.decode()) {
    int rawSignalLength = IrReceiver.decodedIRData.rawDataPtr->rawlen;
    Serial.println("Raw data length: " + String(IrReceiver.decodedIRData.rawDataPtr->rawlen));

    Serial.println("Raw data: ");
    for (int i = 0; i < rawSignalLength; i++) {
      Serial.print(String(IrReceiver.decodedIRData.rawDataPtr->rawbuf[i]) + " ");
    }
  }
}
