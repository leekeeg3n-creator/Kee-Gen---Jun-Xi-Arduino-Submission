//DEEE/FT/1B/22
//Lee kee gen , P2621762
//Li junxi , P2621423 






#include "RichShieldTM1637.h"

// Hardware Pin Definitions
#define BUZZER    3
#define LED_RED   4
#define LED_GREEN 5
#define LED_BLUE  6
#define LED_YELLOW 7
#define K1_BLUE   8
#define K2_YELLOW 9
#define CLK      10
#define DIO      11

TM1637 disp(CLK, DIO);

int marks = 0;
int totalQuestions = 5;

// Shortened text to save memory on Arduino Uno
String normalQuestions[3] = {
  "Q1: Can a phone call for help in difficulty?",
  "Q2: When phone has no battery, throw away?",
  "Q3: Is 995 for ambulance and 999 for police?"
};
int normalAnswers[3] = {1, 0, 1};

String diffQuestions[2] = {
  "Q4: Follow instructions from random messages?",
  "Q5: Should we avoid exposing ID & bank info online?"
};
int diffAnswers[2] = {0, 1};

// Custom tone function to prevent Timer 2 conflict with Rich Shield library
void playTone(int pin, int frequency, int durationMs) {
  if (frequency <= 0) return;
  long delayValue = 500000 / frequency;
  long numCycles = (long)frequency * durationMs / 1000;
  
  for (long i = 0; i < numCycles; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(pin, LOW);
    delayMicroseconds(delayValue);
  }
}

// Helper function to display [Seconds Left] on Left 2 digits and [Marks] on Right digit
void updateDisplay(int secondsLeft, int currentMarks) {
  int displayNum = (secondsLeft * 100) + currentMarks;
  disp.display(displayNum);
}

// Sound Effects
void playWinSound() {
  playTone(BUZZER, 523, 150); delay(50); // C5
  playTone(BUZZER, 659, 150); delay(50); // E5
  playTone(BUZZER, 784, 250); delay(250); // G5
}

void playWrongSound() {
  playTone(BUZZER, 250, 200); delay(50);
  playTone(BUZZER, 180, 400); delay(400);
}

void playVictorySong() {
  int notes[] = {523, 659, 784, 1047, 784, 1047, 784, 880, 784, 659, 523, 587, 659, 587};
  int durations[] = {200, 200, 200, 400, 200, 500, 200, 200, 200, 200, 300, 200, 200, 400};

  for (int i = 0; i < 14; i++) {
    digitalWrite(LED_GREEN, HIGH);
    playTone(BUZZER, notes[i], durations[i]);
    delay(40);
    digitalWrite(LED_GREEN, LOW);
  }
}

void startCountdownSequence() {
  Serial.println(F("Starting in 3..."));
  digitalWrite(LED_RED, HIGH);
  playTone(BUZZER, 262, 150);
  delay(800);
  digitalWrite(LED_RED, LOW);

  Serial.println(F("2..."));
  digitalWrite(LED_YELLOW, HIGH);
  playTone(BUZZER, 330, 150);
  delay(800);
  digitalWrite(LED_YELLOW, LOW);

  Serial.println(F("1... GO!"));
  digitalWrite(LED_GREEN, HIGH);
  playTone(BUZZER, 784, 300);
  delay(500);
  digitalWrite(LED_GREEN, LOW);
}

// Get user answer with background audio tick and 20s timeout
int getAnswer(int timeoutSeconds) {
  unsigned long startTime = millis();
  unsigned long timeoutMs = (unsigned long)timeoutSeconds * 1000;
  unsigned long lastTickTime = 0;

  int tickNotes[] = {262, 330, 392, 330}; // C4, E4, G4, E4
  int noteIdx = 0;

  while (millis() - startTime < timeoutMs) {
    // Calculate remaining seconds
    int secondsLeft = timeoutSeconds - ((millis() - startTime) / 1000);
    updateDisplay(secondsLeft, marks);

    // Play background sound tick every 400ms
    if (millis() - lastTickTime >= 400) {
      playTone(BUZZER, tickNotes[noteIdx], 50);
      noteIdx = (noteIdx + 1) % 4;
      lastTickTime = millis();
    }

    // Check Yellow Button (YES = 1)
    if (digitalRead(K2_YELLOW) == LOW) {
      delay(50); // debounce
      while (digitalRead(K2_YELLOW) == LOW); // wait for release
      return 1;
    }

    // Check Blue Button (NO = 0)
    if (digitalRead(K1_BLUE) == LOW) {
      delay(50); // debounce
      while (digitalRead(K1_BLUE) == LOW); // wait for release
      return 0;
    }
  }

  updateDisplay(0, marks);
  Serial.println(F("Time's up!"));
  return -1; // Timed out
}

void checkAnswer(int userAns, int correctAns) {
  if (userAns == correctAns) {
    Serial.println(F("-> Correct!"));
    digitalWrite(LED_GREEN, HIGH);
    marks = marks + 1;
    updateDisplay(0, marks);
    playWinSound();
    digitalWrite(LED_GREEN, LOW);
  } else {
    Serial.println(F("-> Incorrect or Timed Out!"));
    digitalWrite(LED_RED, HIGH);
    playWrongSound();
    digitalWrite(LED_RED, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  disp.init();
  updateDisplay(20, marks);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(K1_BLUE, INPUT_PULLUP);
  pinMode(K2_YELLOW, INPUT_PULLUP);
}

void loop() {
  marks = 0;
  updateDisplay(20, marks);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);

  Serial.println(F("Press Yellow Button to Start Quiz..."));
  while (digitalRead(K2_YELLOW) == HIGH); // Wait for press
  while (digitalRead(K2_YELLOW) == LOW);  // Wait for release

  startCountdownSequence();

  // --- NORMAL LEVEL ---
  do {
    marks = 0;
    updateDisplay(20, marks);

    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_YELLOW, LOW);

    Serial.println(F("\n--- NORMAL LEVEL ---"));

    for (int i = 0; i < 3; i++) {
      Serial.println(normalQuestions[i]);
      Serial.println(F("(Yellow = YES, Blue = NO) [20s limit]"));
      int userAns = getAnswer(20);
      checkAnswer(userAns, normalAnswers[i]);
    }

    if (marks < 3) {
      Serial.println(F("\nDid not get 3/3. Retrying Normal Level..."));
      delay(2000);
    }
  } while (marks < 3);

  Serial.println(F("\nPassed Normal Level! Moving to Difficult Level..."));
  delay(1500);

  // --- DIFFICULT LEVEL ---
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, HIGH);

  Serial.println(F("\n--- DIFFICULT LEVEL ---"));

  for (int i = 0; i < 2; i++) {
    Serial.println(diffQuestions[i]);
    Serial.println(F("(Yellow = YES, Blue = NO) [20s limit]"));
    int userAns = getAnswer(20);
    checkAnswer(userAns, diffAnswers[i]);
  }

  // --- FINAL RESULTS ---
  Serial.println(F("\n=============================="));
  Serial.print(F("Final Score: "));
  Serial.print(marks);
  Serial.print(F(" / "));
  Serial.println(totalQuestions);
  Serial.println(F("=============================="));

  if (marks == 5) {
    Serial.println(F("GRADE: EXCELLENT (PERFECT SCORE)"));
    playVictorySong();
    delay(3000);
  } else {
    Serial.println(F("GRADE: FAILED (Resetting to start...)"));
    delay(2000);
  }
}
