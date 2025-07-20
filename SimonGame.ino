//declare each pin
const int RED = 10 , YELLOW = 9,BLUE = 8, GREEN = 7;
const int RED_BUTTON = 6 , YELLOW_BUTTON = 5,BLUE_BUTTON =4, GREEN_BUTTON =3;
const int SPEAKER = 13;

int sequence[100]; 
int button_pressed[100];
int correct = 0; //how many rounds the user has completed correctly.
int level = 1;
int delayTime = 1000; // Default for Easy

unsigned long lastInputTime; //to detect if the user is inactive for more than 5 seconds.
//Initial setup of the game environment 
void setup() {
  pinMode(RED, OUTPUT); 
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(SPEAKER, OUTPUT);
  
  Serial.begin(9600);
  randomSeed(analogRead(0));
  //Start generating random numbers based on the value read from analog pin A0, so that the random pattern is different every time the Arduino restarts."

  Serial.println("Choose difficulty: 1 (Easy), 2 (Medium), 3 (Hard)");

  while (Serial.available() == 0); //nothing has been typed yet
  //The semicolon ; means this is an empty loop.. it keeps checking until the user types something.
  int difficulty = Serial.parseInt();
  if (difficulty == 2) delayTime = 600;
  else if (difficulty == 3) delayTime = 300;
  else delayTime = 1000;

  Serial.print("Difficulty set to ");
  Serial.println(difficulty);
  delay(1000);
  
}

void loop() {
  generateSequence(); //Adds a new random LED to the current sequence.
  if (!showSequence()) gameOver();
  if (!getUserInput()) gameOver();

  correct++;
  Serial.print("Score: ");
  Serial.println(correct);
  delay(1000);
}
// Append a random number (0–3) to the sequence (each representing an LED).
void generateSequence() {
  sequence[correct] = random(0, 4); 
}

//Displays the sequence of LEDs to the user visually
bool showSequence() {
  for (int i = 0; i <= correct; i++) {
    lightLED(sequence[i]);
    delay(delayTime);
    turnOffLEDs();
    delay(200);
  }
  return true;
}
//Waits for the user to press the buttons to repeat the sequence.
bool getUserInput() {
  lastInputTime = millis(); //Start a timer
  for (int i = 0; i <= correct; i++) {
    while (true) {
      if (millis() - lastInputTime > 5000) return false; // Timeout
      int btn = readButton();
      if (btn != -1) {
        lightLED(btn);
        button_pressed[i] = btn;
        delay(300);
        turnOffLEDs();
        if (button_pressed[i] != sequence[i]) 
          return false;
        lastInputTime = millis(); // Reset timer after valid input
        break;
        //Check if user's input matches the correct one.If wrong game over.If right continue.
      }
    }
  }
  return true;
}
//Reads which button (if any) is pressed by the user.
int readButton() {
  if (digitalRead(RED_BUTTON) == LOW) return 0;
  if (digitalRead(BLUE_BUTTON) == LOW) return 1;
  if (digitalRead(YELLOW_BUTTON) == LOW) return 2;
  if (digitalRead(GREEN_BUTTON) == LOW) return 3;
  return -1; //no button is pressed.
}

void lightLED(int led) {
  int pin = getLEDPin(led);
  digitalWrite(pin, HIGH);
  tone(SPEAKER, 400 + led * 200, 150); // Different tone for each LED
}

void turnOffLEDs() {
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
  //Turns off all LEDs.
}
//Translates the LED index (0–3) to the physical pin number.
int getLEDPin(int led) {
  if (led == 0) return RED;
  if (led == 1) return BLUE;
  if (led == 2) return YELLOW;
  return GREEN; // led == 3
}
//Handles what happens when the user loses (by timeout or incorrect input).
void gameOver() {
  Serial.println("Game Over!");
  Serial.print("Final Score: ");
  Serial.println(correct);

  for (int i = 0; i < 2; i++) {
    tone(SPEAKER, 200, 300);
    turnOnAllLEDs();
    delay(300);
    turnOffLEDs();
    delay(300);
  }
  while (true); // Stop the game
}

void turnOnAllLEDs() {
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, HIGH);
   //Turns on all LEDs.
}