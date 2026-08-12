//Li junxi, P2621423, DEEE/FT/1B/22, Group 3
//Lee kee gen, P2621762, 


//







#include "RichShieldTM1637.h"

#define BUZZER    3
#define LED_RED   4
#define LED_GREEN 5
#define LED_BLUE  6
#define LED_YELLOW 7
#define K1_BLUE   8
#define K2_YELLOW 9
#define CLK      10
#define DIO      11

#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784
#define A5 880
#define C6 1047

TM1637 disp(CLK, DIO);
int marks = 0;
int totalQuestions = 5;

char* normalQuestions[3] = {
  "Q1: can a mobile phone can call for help when facing diffciulty?",
  "Q2: when a mobile device have no battery, we should throw away?",
  "Q3: is 995 for ambulance and 999 for police call?"
};
int normalAnswers[3] = {1, 0, 1};

char* diffQuestions[2] = {
  "Q4: when receiving a random message, we should talk to them and follow their instruction?",
  "Q5: we should not expose our home address and personal ID and bank infomation online?"
};
int diffAnswers[2] = {0, 1};

void myTone(int pin, int frequency, int durationMs) {
  int delayValue = 500000 / frequency;
  int numCycles = (frequency / 10) * durationMs / 100;
  
  for (int i = 0; i < numCycles; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(pin, LOW);
    delayMicroseconds(delayValue);
  }
}

void soundWin() {
  myTone(BUZZER, C5, 150);
  delay(50);
  myTone(BUZZER, E5, 150);
  delay(50);
  myTone(BUZZER, G5, 250);
  delay(250);
}

void soundWrong() {
  myTone(BUZZER, 250, 200); 
  delay(50);
  myTone(BUZZER, 180, 400); 
  delay(400);
}

void victorySong() {
  int notes[] = {
    C5, E5, G5, C6, 
    G5, C6, 
    G5, A5, G5, E5, C5,
    D5, E5, D5,
    G5, A5, G5, E5, C5,
    D5, E5, C5,
    C5, D5, E5, F5, G5, A5, B4, C6
  };

  int durations[] = {
    150, 150, 150, 300, 
    150, 500, 
    200, 200, 200, 200, 300,
    200, 200, 400,
    200, 200, 200, 200, 300,
    200, 200, 400,
    150, 150, 150, 150, 200, 200, 200, 600
  };

  int totalNotes = sizeof(notes) / sizeof(notes[0]);

  for (int i = 0; i < totalNotes; i++) {
    digitalWrite(LED_GREEN, HIGH);
    myTone(BUZZER, notes[i], durations[i]);
    digitalWrite(LED_GREEN, LOW);
    
    delay(40);
  }
}

void showDisplay(int timeLeft, int score) {
  int displayValue = (timeLeft * 100) + score;
  disp.display(displayValue);
}

void startBeepSequence() {
  Serial.println(F("Starting in 3..."));
  digitalWrite(LED_RED, HIGH);
  myTone(BUZZER, C4, 150);
  delay(800);
  digitalWrite(LED_RED, LOW);

  Serial.println(F("2..."));
  digitalWrite(LED_YELLOW, HIGH);
  myTone(BUZZER, E4, 150);
  delay(800);
  digitalWrite(LED_YELLOW, LOW);

  Serial.println(F("1... GO!"));
  digitalWrite(LED_GREEN, HIGH);
  myTone(BUZZER, G5, 300);
  delay(500);
  digitalWrite(LED_GREEN, LOW);
}

int getAnswerWithMusic(int timeoutSeconds) {
  unsigned long startTime = millis();
  unsigned long timeoutMs = (unsigned long)timeoutSeconds * 1000;
  unsigned long lastNoteTime = 0;
  
  int timerNotes[] = {C4, E4, G4, E4};
  int noteIndex = 0;

  while (millis() - startTime < timeoutMs) {
    int secondsLeft = timeoutSeconds - ((millis() - startTime) / 1000);
    showDisplay(secondsLeft, marks);

    if (millis() - lastNoteTime >= 400) {
      myTone(BUZZER, timerNotes[noteIndex], 60);
      noteIndex = (noteIndex + 1) % 4;
      lastNoteTime = millis();
    }

    if (digitalRead(K2_YELLOW) == 0) {
      delay(50);
      while (digitalRead(K2_YELLOW) == 0);
      return 1;
    }
    if (digitalRead(K1_BLUE) == 0) {
      delay(50);
      while (digitalRead(K1_BLUE) == 0);
      return 0;
    }
  }
  
  showDisplay(0, marks);
  Serial.println(F("Time's up!"));
  return -1;
}

void checkAnswer(int userAnswer, int correctAnswer) {
  if (userAnswer == correctAnswer) {
    Serial.println(F("-> Correct!"));
    digitalWrite(LED_GREEN, HIGH);
    
    marks += 1;
    showDisplay(0, marks);
    
    soundWin();
    
    digitalWrite(LED_GREEN, LOW);
  } else {
    Serial.println(F("-> Incorrect or Timed Out!"));
    digitalWrite(LED_RED, HIGH);
    
    soundWrong();
    
    digitalWrite(LED_RED, LOW);
  }
}

void calculateResults() {
  int percentage = (marks * 100) / totalQuestions;
  
  Serial.println();
  Serial.println(F("=============================="));
  Serial.print(F("Final Score: "));
  Serial.print(marks);
  Serial.print(F(" / "));
  Serial.println(totalQuestions);
  
  Serial.print(F("Accuracy: "));
  Serial.print(percentage);
  Serial.println(F("%"));
  Serial.println(F("=============================="));
}

void setup() {
  Serial.begin(9600);
  
  disp.init();
  showDisplay(20, marks);

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
  showDisplay(20, marks);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);

  Serial.println(F("Press Yellow Button to Start Quiz..."));
  
  while (digitalRead(K2_YELLOW) != 0);
  while (digitalRead(K2_YELLOW) == 0);

  startBeepSequence();

  do {
    marks = 0;
    showDisplay(20, marks);

    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_YELLOW, LOW);

    Serial.println();
    Serial.println(F("--- NORMAL LEVEL ---"));

    for (int i = 0; i < 3; i++) {
      Serial.print(normalQuestions[i]);
      Serial.println(F(" (Yellow = YES, Blue = NO) [20s limit]"));
      int userAns = getAnswerWithMusic(20);
      checkAnswer(userAns, normalAnswers[i]);
    }

    if (marks < 3) {
      Serial.println();
      Serial.println(F("Did not get full marks (3/3). Retrying Normal Level..."));
      delay(2000);
    }

  } while (marks < 3);

  Serial.println();
  Serial.println(F("Passed Normal Level! Moving to Difficult Level..."));
  delay(1500);

  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, HIGH);

  Serial.println();
  Serial.println(F("--- DIFFICULT LEVEL ---"));

  for (int i = 0; i < 2; i++) {
    Serial.print(diffQuestions[i]);
    Serial.println(F(" (Yellow = YES, Blue = NO) [20s limit]"));
    int userAns = getAnswerWithMusic(20);
    checkAnswer(userAns, diffAnswers[i]);
  }

  calculateResults();

  if (marks == 5) {
    Serial.println(F("GRADE: EXCELLENT (PERFECT SCORE)"));
    victorySong();
    delay(3000);
  } else {
    Serial.println(F("GRADE: FAILED (Resetting to start...)"));
    delay(2000);
  }
}
