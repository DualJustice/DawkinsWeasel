#include "Arduino.h"
#include "HardwareSerial.h"


// Global settings:
static constexpr unsigned short BAUD_RATE = 9600;
static constexpr unsigned short SERIAL_BUFFER = 64;
static constexpr unsigned short ASCII_START = 32;
static constexpr unsigned short ASCII_ADDITIONAL = 95;

// Simulation settings:
static constexpr unsigned short P = 5; // (%)
static constexpr unsigned short GEN_COPIES = 100; // Maximum recommended value of 250, based on what model of Arduino you are using

static char *targetArr = new char[SERIAL_BUFFER];
static char *topArr = new char[SERIAL_BUFFER];
static char *currentGen = new char[SERIAL_BUFFER*GEN_COPIES];
static unsigned short strLen = 0;
static unsigned short genIndex = 0;
static unsigned short topArrIndex = 0;
static unsigned short previousScore = 0;
static unsigned short score = 0;


void inputLoop() {
	while(true) {
		if(Serial.available() > 0) {
			strLen = Serial.readBytesUntil('\n', targetArr, SERIAL_BUFFER);
			Serial.print("Target String: ");
			for(unsigned short i = 0; i < strLen; i += 1) {
				Serial.print(targetArr[i]);
			}

			Serial.println();
			break;

		} else {
			delay(100);
		}
	}
}


void initialGeneration() {
	for(unsigned short i = 0; i < strLen; i += 1) {
		topArr[i] = rand() % ASCII_ADDITIONAL + ASCII_START;
	}
}


void processGenerations() {
genIndex = 0;
topArrIndex = 0;
previousScore = 0;
score = 0;

	while(true) {
		topArrIndex = 0;
		previousScore = 0;

		for(unsigned short i = 0; i < GEN_COPIES; i += 1) {
			for(unsigned short j = 0; j < strLen; j += 1) {
				genIndex = (i*strLen) + j;
				if((rand() % 100 + 1) <= P) {
					currentGen[genIndex] = rand() % ASCII_ADDITIONAL + ASCII_START;

				} else {
					currentGen[genIndex] = topArr[j];
				}

				if(j == 0) {
					score = 0;
				}

				if(currentGen[genIndex] == targetArr[j]) {
					score++;
					if(score > previousScore) {
						previousScore = score;
						topArrIndex = i;
					}
				}
			}
		}

		if(previousScore == strLen) {
			for(unsigned short j = 0; j < strLen; j += 1) {
				Serial.print(currentGen[(topArrIndex*strLen) + j]);
			}

			Serial.println();
			break;

		} else {
			for(unsigned short j = 0; j < strLen; j += 1) {
				topArr[j] = currentGen[(topArrIndex*strLen) + j];
				Serial.print(topArr[j]);
			}

			Serial.println();
		}
	}
}


void askTargetString() {
	Serial.println("Input a Target String.");
	inputLoop();
	initialGeneration();
	processGenerations();
}


void setup() {
	Serial.begin(BAUD_RATE);
	while(!Serial) {
		delay(250);
	}

	srand(millis());
}


void loop() {
	askTargetString();
}