#include <Arduino.h>
#include <Bounce2.h>
#include <header.h>

#define GAME_LENGTH 5
#define LEDS_COUNT 3
#define UNSET -1

int sequence[GAME_LENGTH];
unsigned int ledPins[] = {8, 3, 2};
unsigned int buttonPins[] = {16, 14, 15};
volatile int playerSequence[GAME_LENGTH];
volatile int playerMove = 0;
Bounce buttons[3];

void setup()
{
  Serial.begin(9600);
  prepareLeds();
  prepareButtons();
}

void loop()
{
  generateSequence();
  bool isCorrect;
  for (int i = 1; i <= GAME_LENGTH; i++)
  {
    simonSays(i);
    playerTurn(i);
    delay(1000);
    isCorrect = checkPlayerSequence(i);
    if (!isCorrect)
    {
      gameLost();
      break;
    }
  }
  if (isCorrect)
  {
    gameWon();
  }
  playLedCheck();
}

void prepareLeds()
{
  setLedPins(OUTPUT);
  playLedCheck();
}

void prepareButtons()
{
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    buttons[i] = Bounce();
    buttons[i].attach(buttonPins[i], INPUT_PULLUP);
    buttons[i].interval(50);
  }
}

void setLedPins(int mode)
{
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    pinMode(ledPins[i], mode);
  }
}

void playLedCheck()
{
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(500);
  }
  delay(100);
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    digitalWrite(ledPins[i], LOW);
    delay(500);
  }
}
void generateSequence()
{
  Serial.print("Generating sequence: ");
  for (int i = 0; i < GAME_LENGTH; i++)
  {
    sequence[i] = random(0, LEDS_COUNT);
    Serial.print(sequence[i]);
    Serial.print(",");
  }
  Serial.println(" GOOD LUCK!");
}

void simonSays(int round)
{
  Serial.print("Round ");
  Serial.print(round);
  Serial.println(" starts!");
  for (int i = 0; i < round; i++)
  {
    Serial.print("Light: ");
    Serial.println(sequence[i]);
    int pin = sequence[i];
    digitalWrite(ledPins[pin], HIGH);
    delay(1000);
    digitalWrite(ledPins[pin], LOW);
    delay(500);
  }
}

void playerTurn(int round)
{
  int playerMove = 0;
  initPlayerTurn();
  Serial.print("PLAYER TURN!");
  Serial.println(playerSequence[round - 1]);
  while (playerSequence[round - 1] == UNSET)
  {
    readPlayerSequence();
  }
  delay(500);
  //resetuje wszystkie ledy bo powyzsza petla wychodzi po wcisnieciu ostatniego przycisku
  //i nie odpala sie procedura gaszenia
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    digitalWrite(ledPins[i], LOW);
  }
}

void initPlayerTurn()
{
  playerMove = 0;
  for (int i = 0; i < GAME_LENGTH; i++)
  {
    playerSequence[i] = UNSET;
  }
}

void readPlayerSequence()
{
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    buttons[i].update();
    int value = buttons[i].read();
    if (buttons[i].rose())
    {
      Serial.print("Button ");
      Serial.print(i);
      Serial.println(" pressed");
      digitalWrite(ledPins[i], HIGH);
      playerSequence[playerMove] = i;
      playerMove++;
    }
    else if (buttons[i].fell())
    {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

boolean checkPlayerSequence(int round)
{
  bool isCorrect = true;
  for (int i = 0; i < round; i++)
  {
    if (sequence[i] != playerSequence[i])
    {
      isCorrect = false;
    }
  }
  Serial.print("Player was correct?: ");
  Serial.print(isCorrect);
  return isCorrect;
}

void gameLost()
{
  int RXLED = 17;
  pinMode(RXLED, OUTPUT);
  for (int i = 0; i < 5; i++)
  {
    TXLED0;
    digitalWrite(RXLED, HIGH);
    delay(500);
    TXLED1;
    digitalWrite(RXLED, LOW);
    delay(500);
  }
}

void gameWon()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
    delay(500);
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], LOW);
    delay(500);
  }
}