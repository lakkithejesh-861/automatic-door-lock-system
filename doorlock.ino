// Automatic Door Lock System using Arduino
// Author: Lakki Thejesh Kumar
// Project: Diploma Final Year Project - ECE
// State Board of Technical Education, Andhra Pradesh

#include <Keypad.h>
#include <Servo.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Pin definitions
const int SERVO_PIN    = 11;
const int GREEN_LED    = 12;
const int RED_LED      = 13;
const int BUZZER_PIN   = 10;

// Password
String correctPassword = "1234";
String enteredPassword = "";
int wrongAttempts = 0;
const int MAX_ATTEMPTS = 3;
bool isLocked = true;

Servo lockServo;

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lockServo.attach(SERVO_PIN);
  lockDoor();
  Serial.println("Door Lock System Ready");
  Serial.println("Enter password:");
}

void loop() {
  if (wrongAttempts >= MAX_ATTEMPTS) {
    triggerAlarm();
    return;
  }

  char key = keypad.getKey();

  if (key) {
    if (key == '#') {
      // Submit password
      checkPassword();
    } else if (key == '*') {
      // Clear entered password
      enteredPassword = "";
      Serial.println("Cleared. Enter password:");
    } else {
      enteredPassword += key;
      Serial.print("*");
    }
  }
}

void checkPassword() {
  Serial.println("");
  if (enteredPassword == correctPassword) {
    Serial.println("Access Granted!");
    wrongAttempts = 0;
    unlockDoor();
    delay(5000);
    lockDoor();
  } else {
    wrongAttempts++;
    Serial.print("Wrong Password! Attempts left: ");
    Serial.println(MAX_ATTEMPTS - wrongAttempts);
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER_PIN, 500, 500);
    delay(1000);
    digitalWrite(RED_LED, LOW);
  }
  enteredPassword = "";
}

void unlockDoor() {
  isLocked = false;
  lockServo.write(90);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  Serial.println("Door Unlocked!");
}

void lockDoor() {
  isLocked = true;
  lockServo.write(0);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  Serial.println("Door Locked.");
}

void triggerAlarm() {
  Serial.println("ALERT! Too many wrong attempts!");
  for (int i = 0; i < 10; i++) {
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER_PIN, 1000, 200);
    delay(300);
    digitalWrite(RED_LED, LOW);
    delay(300);
  }
  wrongAttempts = 0;
  Serial.println("System Reset. Enter password:");
}
