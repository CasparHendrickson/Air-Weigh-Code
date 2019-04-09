

// include the library code:
#include <LiquidCrystal.h>

//Set up pins for LCD and the Buttons and Pots
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int LED_G_DONE = 8;
int LED_R_RUNNING = 13;//LED pins
int LED_W_REPLACE = 7;
int start_button = 6;
int reset_button = 9;//Buttons
int change_button = A0;
int speakerPin = 10;//Piezo out
bool reset_pressed= false;//Logic Control
bool test_run = false;//Previous state information
int numTones = 8;//Length of the tones list. I don't know if Arduino has a good len() function
int tones[] = {880, 880, 880, 988, 784, 784, 784, 784};
int pot = 2;
int pot_val = 0;
int testing_length;//Calculated from the pot
int waittime = 1000;//Not currently used, would map this to minutes based on the length of test
int change_button_val = 0;

/////////////////////////////////////////////////////////////////
void setup() {
  // Setup LCD and pin modes
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(LED_G_DONE, OUTPUT);
  pinMode(LED_R_RUNNING, OUTPUT);//Easy, make some lights be able to be turned on
  pinMode(LED_W_REPLACE, OUTPUT);
  pinMode(start_button, INPUT);//Buttons for pushing, input
  pinMode(reset_button, INPUT);
  pinMode(pot, INPUT);
  pinMode(change_button, INPUT);

}
//////////////////////////////////////////////////////////////////////////////
//Run this over and over while checking states of the button
void loop() {
  	change_button_val=digitalRead(change_button);
  if (change_button_val == 1){
    lcd.setCursor(2, 1);//Make the display in different places
    pot_val = analogRead(pot);
    testing_length = map(pot_val,0, 1023, 1, 15);//Map to differnet values, may change based on Roman's test
    lcd.print("Test Length");
    lcd.print(":");
    lcd.print(testing_length);//Specified length, again, might be based of of Roman's test at some point.
  	lcd.print(" ");
  }
    //Run this over and over to check for button states
  lcd.setCursor(0, 0);
  int state=digitalRead(start_button);
  int reset_buttonval = digitalRead(reset_button);
  //Read The stuff
  if (reset_buttonval ==1){
   reset_pressed=1;
  }



  if (state ==1 && test_run==0) {
    lcd.clear();
    timingfunc();
  }
  if (reset_buttonval == 1 && test_run==1) {
	lcd.clear();
    digitalWrite(LED_G_DONE,HIGH);
    reset_pressed=1;
    test_run=0;
    lcd.print("Place new Board");
    digitalWrite(LED_G_DONE, LOW);
    digitalWrite(LED_W_REPLACE, HIGH);

  }


}
///////////////////////////////////////////////////////////////////////////////
// This fucntion runs when the buttons above are pressed.

void timingfunc(){
  lcd.clear();
  for (int i = 0; i < 100000000000 && digitalRead(start_button) == HIGH; i++){
    if (i >testing_length){
      digitalWrite(LED_G_DONE, HIGH);
      lcd.clear();
      lcd.print("Done");
      song();
      lcd.clear();
      lcd.print("Please Remove");
      digitalWrite(LED_R_RUNNING, LOW);
      test_run = 1;//Change state for next button presses


      return;
    }
    lcd.setCursor(0,0);
    lcd.print("Elapsed");
    lcd.print(":");
    lcd.print(i);
    digitalWrite(LED_R_RUNNING, HIGH);
    digitalWrite(LED_W_REPLACE,LOW);
    lcd.setCursor(2,1);
  	lcd.print("Test Length");
    lcd.print(":");
    lcd.print(testing_length);//Specified length, again, might be based of of Roman's test at some point.
  	lcd.print(" ");
    delay(1000);

  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Sing a little song that lets the operator know that they are done with the test and can remove the board.

void song(){
    for (int i = 0; i < numTones; i++)
  {
    tone(speakerPin, tones[i]);
    delay(290);
  }
  noTone(speakerPin);
}
