#include "tiny_IRremote.h"   // the standard IR library does not work (it uses the TIMER0 which is also used for the delay function)

// the IR receiver is connected to PIN3
#define RECV_PIN 2

// the PIN to which the temperature sensor is connected
// http://funwidelectronics.blogspot.co.at/2016/10/lm35gui.html
#define TP_PIN 5

#define DIAG_LED 4

// PINOUT: https://camo.githubusercontent.com/081b569122da2244ff7de8bae15eb56947d05cc8/687474703a2f2f6472617a7a792e636f6d2f652f696d672f50696e6f7574543835612e6a7067

float temperature=0.0f;
const float MAX_TEMP = 60;

// define two different outputs, so each output could be controlled seperatly (in this sketch both are controlled the same way)
#define OUT_0 0
#define OUT_1 1

#define CURR_START 0
#define CURR_MAX 250
#define CURR_MIN 0
#define CURR_INC_UP 1
#define CURR_INC_DW 1
#define CURR_STEP_UP 25
#define CURR_STEP_DW 25
#define DELAY_STEP_UP 25
#define DELAY_STEP_DW 25

#define DELAY_LOOP_RUN 50

// using library for receiving and processing IR signals
IRrecv irrecv(RECV_PIN);
decode_results results;

// fast-back button on the remote control
const unsigned long left = 551547150;
// play button on the remote control
const unsigned long leftMid = 551489010;
// pause button on the remote control
const unsigned long rightMid = 551509410;
// fast-forward button on the remote control
const unsigned long right = 551514510;

// if a process is already running this variable is set to true (e.g. while lightUpSlow is running)
volatile bool runIt = false;
// the current output value for OUT_1
volatile int curr = 0;

// the setup function runs once when you press reset or power the board
void setup()
{
  // activate IR receiver
  irrecv.enableIRIn(); // Start the receiver

  // when turned on the value is 0
  curr = CURR_START;
  // Define pins, currently only OUT_1 is used
  pinMode(OUT_0, OUTPUT);
  pinMode(OUT_1, OUTPUT);
  pinMode(DIAG_LED, OUTPUT);

  digitalWrite(DIAG_LED, LOW);
  analogWrite(OUT_0, curr);
  analogWrite(OUT_1, curr);
}

/*
  This method turns _on_ the Light slowly by increasing the brightness
*/
void LightUpSlow()
{
  // if a process is already running or the lights are already on max --> exit
  if (curr >= CURR_MAX || runIt) { return; }

  runIt = true;
  while(curr<CURR_MAX)
  {
    // change the time for delay and/or for increasing the value, to change the time until the max. brightness is reached
    delay(DELAY_STEP_UP);
    curr+=CURR_INC_UP;
    if (curr > CURR_MAX)
    {
      curr = CURR_MAX;
    }
    analogWrite(OUT_0, curr);
    analogWrite(OUT_1, curr);
  }
  runIt = false;
}

/*
  This method turns _off_ the Light slowly by decreasing the brightness
*/
void LightDownSlow()
{
  // if a process is already running or the lights are already on min --> exit
  if (curr <= CURR_MIN || runIt) { return; }

  runIt = true;
  while(curr>CURR_MIN)
  {
    // change the time for delay and/or for increasing the value, to change the time until the min. brightness is reached
    delay(DELAY_STEP_DW);
    curr-=CURR_INC_DW;
    if (curr < CURR_MIN)
    {
      curr = CURR_MIN;
    }
    analogWrite(OUT_0, curr);
    analogWrite(OUT_1, curr);
  }
  runIt = false;
}


// the loop function runs over and over again forever
void loop()
{
  if (!IsTemperatureOK())
  {
    WriteToOutput(0);
    digitalWrite(DIAG_LED, HIGH);
    delay(150);
    digitalWrite(DIAG_LED, LOW);
    delay(250);
    return;
  }
  
  // need a break, need a kitk ... ahm delay :)
  delay(DELAY_LOOP_RUN);
  
  if (irrecv.decode(&results))
  {
    switch(results.value)
    {
      case left:
        LightDownSlow();
        break;
      case leftMid:
        curr -= CURR_STEP_DW;
        WriteToOutput(curr);
        break;
      case rightMid:
        curr += CURR_STEP_UP;
        WriteToOutput(curr);
        break;
      case right:
        LightUpSlow();
        break;
      default:
        break;
    }
    // ready for receiving the next value
    irrecv.resume(); // Receive the next value
  }
}

bool IsTemperatureOK()
{
  temperature = analogRead(TP_PIN);
  temperature = (0.4887*temperature);
  return (temperature <= MAX_TEMP)
}

void WriteToOutput(int val)
{
  // Just to make sure that everything is in range
  if (val < CURR_MIN) { val = CURR_MIN; }
  if (val > CURR_MAX) { val = CURR_MAX; }

  // Output value to OUT_0 and OUT_1
  analogWrite(OUT_0, val);
  analogWrite(OUT_1, val);
}
