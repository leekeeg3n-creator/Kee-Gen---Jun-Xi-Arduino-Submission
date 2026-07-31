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

 while (digitalRead(K2 != 0))
{
  cout << "Please press the yellow button to get ready.";
}

digitalWrite(BLUE_LED, HIGH);
cout << "You are now in the normal level (BLUE LED TURNED ON)\n";
cout << "Let's get started!\n";
cout << "1) Should you tell someone your banking OTP if they ask over the phone?\n";
cout << "     Yellow Button - Yes\n";
cout << "     Blue Button - No\n";

if (yellow_button == 0)

  

}
