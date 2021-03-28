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
static String userInput;
static unsigned short *targetIntArr;
static unsigned short *topArr;
static unsigned short newGenSize = strLen*GEN_COPIES;
static unsigned short *currentGen;
static unsigned short genIndex = 0;
static unsigned short topArrIndex = 0;
static unsigned short previousScore = 0;
static unsigned short score = 0;

//static unsigned short myArray = new unsigned short[MAX_ARRAY_LENGTH];


String inputLoop() {
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
	targetIntArr = new unsigned short[strLen];

	Serial.print("Target Array: ");
	for(unsigned short i = 0; i < strLen; i += 1) {
		targetIntArr[i] = (unsigned short)(targetString[i]);
		Serial.print(targetIntArr[i]);
	}

	Serial.println();
	return targetIntArr;
}


unsigned short* initialGeneration() {
	topArr = new unsigned short[strLen];

	for(unsigned short i = 0; i < strLen; i +=1) {
		topArr[i] = rand() % ASCII_ADDITIONAL + ASCII_START;
	}

	return topArr;
}


void processGenerations(unsigned short* topArr, unsigned short* targetArr) {
newGenSize = strLen*GEN_COPIES;
currentGen = new unsigned short[newGenSize];
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
	String targetString = inputLoop();
	targetIntArr = convertStringToIntArr(targetString);
	topArr = initialGeneration();
	processGenerations(topArr, targetIntArr);
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

	delete[] targetIntArr;
	delete[] topArr;
	delete[] currentGen;
}