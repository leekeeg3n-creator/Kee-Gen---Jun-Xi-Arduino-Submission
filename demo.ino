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

//music  is done by GEMINI
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

//JUNXI's part

char* normalQuestions[3] = {
  "Q1: can a mobile phone can call for help when facing diffciulty?",
  "Q2: when a mobile device have no battery, we should throw away?",
  "Q3: is 995 for ambulance and 999 for police call?"
};
int normalAnswers[3] = {1, 0, 1};

//KEE GEN's part
char* diffQuestions[2] = {
  "Q4: when receiving a random message, we should talk to them and follow their instruction?",
  "Q5: we should not expose our home address and personal ID and bank infomation online?"
};
int diffAnswers[2] = {0, 1};

//we dont know how to do the music so the music part all done by GEMINI
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

int getAnswer() {
  while (1) {
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
}

void checkAnswer(int userAnswer, int correctAnswer) {
  if (userAnswer == correctAnswer) {
    Serial.println(F("-> Correct!"));
    digitalWrite(LED_GREEN, HIGH);
    
    marks += 1;
    disp.display(marks);
    
    soundWin();
    
    digitalWrite(LED_GREEN, LOW);
  } else {
    Serial.println(F("-> Incorrect!"));
    digitalWrite(LED_RED, HIGH);
    
    soundWrong();
    
    digitalWrite(LED_RED, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  
  disp.init();
  disp.display(marks);

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
  disp.display(marks);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_YELLOW, LOW);

  Serial.println(F("Are you ready?"));
  
  while (digitalRead(K2_YELLOW) != 0);
  while (digitalRead(K2_YELLOW) == 0);

  Serial.println(F("Let's get started!"));
  delay(1000);

  do {
    marks = 0;
    disp.display(marks);

    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_YELLOW, LOW);

    Serial.println();
    Serial.println(F("--- NORMAL LEVEL ---"));

    for (int i = 0; i < 3; i++) {
      Serial.print(normalQuestions[i]);
      Serial.println(F(" (Yellow = YES, Blue = NO)"));
      int userAns = getAnswer();
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
    Serial.println(F(" (Yellow = YES, Blue = NO)"));
    int userAns = getAnswer();
    checkAnswer(userAns, diffAnswers[i]);
  }

  if (marks == 5) {
    Serial.println();
    Serial.println(F("CONGRATULATIONS! You got 5/5!"));
    
    victorySong();
    
    delay(3000);
  } else {
    Serial.println();
    Serial.println(F("Failed Difficult Level! Resetting to 0 and restarting entire quiz..."));
    delay(2000);
  }
}
