#include "Arduino.h"
#include "HardwareSerial.h"


// Global settings:
static constexpr unsigned short BAUD_RATE = 9600;
static constexpr unsigned short ASCII_START = 32;
static constexpr unsigned short ASCII_ADDITIONAL = 95;

// Simulation settings:
static constexpr float P = 5; // (%)
static constexpr unsigned short GEN_COPIES = 100;

static unsigned short strLen = 0;


String inputLoop() {
	String userInput;

	while(true) {
		if(Serial.available() > 0) {
			userInput = Serial.readStringUntil('\n');
			Serial.print("Target String: ");
			Serial.println(userInput);
			break;
		} else {
			delay(100);
		}
	}

	strLen = userInput.length();
	return userInput;
}


unsigned short* convertStringToIntArr(String targetString) {
	unsigned short *targetIntArr = new unsigned short[strLen];
	unsigned short i = 0;

	Serial.print("Target Array: ");
	while(i < strLen) {
		targetIntArr[i] = (unsigned short)(targetString[i]);
		Serial.print(targetIntArr[i]);
		i++;
	}

	Serial.println();
	return targetIntArr;
}


unsigned short* initialGeneration() {
	unsigned short *gen1 = new unsigned short[strLen];
	unsigned short i = 0;

	while(i < strLen) {
		gen1[i] = rand() % ASCII_ADDITIONAL + ASCII_START;
		i++;
	}

	return gen1;
}


void processGenerations(unsigned short* topArr, unsigned short* targetArr) {
	unsigned short newGenSize = strLen*GEN_COPIES;
	unsigned short *currentGen = new unsigned short[newGenSize];
	unsigned short genIndex = 0;
	unsigned short topArrIndex = 0;
	unsigned short previousScore = 0;
	unsigned short score = 0;

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

	delete[] currentGen;
}


void askTargetString() {
	Serial.println("Input a Target String.");
	String targetString = inputLoop();
	unsigned short *targetIntArr = convertStringToIntArr(targetString);
	unsigned short *gen1 = initialGeneration();
	processGenerations(gen1, targetIntArr);

	delete[] targetIntArr;
	delete[] gen1;
}


void setup() {
	Serial.begin(BAUD_RATE);
	while(!Serial) {
		delay(250);
	}
}


void loop() {
	srand(millis());
	askTargetString();
}