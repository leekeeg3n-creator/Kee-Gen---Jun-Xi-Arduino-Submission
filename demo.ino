#define RED_LED 4
#define GREEN_LED 5
#define BLUE_LED 6
#define YELLOW_LED 7
#define K2 9
#define K1 10

void setup() {
  // put your setup code here, to run once:
pinMode (RED_LED, OUTPUT);
pinMode (GREEN_LED, OUTPUT);
pinMode (BLUE_LED, OUTPUT);
pinMode (YELLOW_LED, OUTPUT);
pinMode (K2, INPUT_PULLUP);
pinMode (K1, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  cout << "Are you ready for the quiz? If yes, please press the yellow button!";

  if (digitalRead(K2 == 0))
  {
    cout << "Let's get started!";
  }
  else
  {
    cout << "What should we do when our mobile devices have no battery?"
    cout << "We should throw away and buy a new one.\n";
    cout << "yes(yellow button) or no (blue_button)";
  }

  if (digitalRead () )

  

}
