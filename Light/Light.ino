#include "tiny_IRremote.h"   // the standard IR library does not work (it uses the TIMER0 which is also used for the delay function)

// the IR receiver is connected to PIN3
#define RECV_PIN 2
// diagnostic LED, on-board, to signalize if an error has ocurred. - TO BE DONE!
#define DIAG_LED 4

// define two different outputs, so each output could be controlled seperatly (in this sketch both are controlled the same way)
#define OUT_0 0
#define OUT_1 1

// defines the parameters of this program (start value for the outputs, in- and decrease steps, ...)
#define CURR_START 0     // AnalogWrite value when startup 
#define CURR_MAX 250     // Maximum value for AnalogWrite (PWM to LED)
#define CURR_MIN 0       // Minimum value for AnalogWrite (PWM to LED)
#define CURR_INC_UP 1    // Step-up value (PWM to LED) 
#define CURR_INC_DW 1    // Step-down value (PWM to LED)
#define CURR_STEP_UP 25  // Increase value for Button fade-up 
#define CURR_STEP_DW 25  // Decrease value for Button fade-down
#define DELAY_STEP_UP 25 // Delay between fade-up (from current value to the new value, using steps given as CURR_STEP_UP)
#define DELAY_STEP_DW 25 // Delay between fade-down (from current value to the new value, using steps given as CURR_STEP_DW)

// defines the delay length for each loop() run 
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
// the current output value for the OUTs
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

  analogWrite(OUT_0, curr);
  analogWrite(OUT_1, curr);

  pinMode(DIAG_LED, OUTPUT);
  digitalWrite(DIAG_LED, LOW);
}

// FadeDown Function, increases light
// value: the value to fadeDown to (e.g. value = 25 --> outputs will be decreased from curr -> 25)
void FadeStepDown(int value)
{
  // if a process is already running or the lights are already on min --> exit
  if (runIt) { return; }
  
  //use new value, just in case it needs to be changed in future
  int ToValue = value;
  
  // check for limits
  if (ToValue < CURR_MIN) { ToValue = CURR_MIN; }
  if (ToValue > CURR_MAX) { ToValue = CURR_MAX; }
  
  runIt = true;
  
  while(curr > ToValue)
  {
    // change the time for delay and/or for increasing the value, to change the time until the min. brightness is reached
    delay(DELAY_STEP_DW);
    curr-=CURR_INC_DW;
    
    analogWrite(OUT_0, curr);
    analogWrite(OUT_1, curr);
  }
  runIt = false;  
}

// FadeUp Function, increases light
// value: the value to fadeUp to (e.g. value = 250 --> outputs will be increased from curr -> 250)
void FadeStepUp(int value)
{
  // if a process is already running or the lights are already on min --> exit
  if (runIt) { return; }
  
  //use new value, just in case it needs to be changed in future
  int ToValue = value;
  
  // check for limits
  if (ToValue < CURR_MIN) { ToValue = CURR_MIN; }
  if (ToValue > CURR_MAX) { ToValue = CURR_MAX; }
  
  runIt = true;
  
  while(curr < ToValue)
  {
    // change the time for delay and/or for increasing the value, to change the time until the max. brightness is reached
    delay(DELAY_STEP_UP);
    curr+=CURR_INC_UP;
    
    analogWrite(OUT_0, curr);
    analogWrite(OUT_1, curr);
  }
  runIt = false;
}

// the loop function runs over and over again forever
void loop() 
{
  // need a break, need a kitk ... ahm delay :)
  delay(DELAY_LOOP_RUN);
  // test for diagnostic LED, TBD: change LED on/off according to status
  digitalWrite(DIAG_LED, !digitalRead(DIAG_LED));
  if (irrecv.decode(&results)) 
  {
    // store current value, might be needed in future
    int newValue = curr;
    
    // based on the pressed button the desired funcion should be done
    if (results.value == left) 
    {
      // fade to minimum brightness
      FadeStepDown(CURR_MIN);
    } else if (results.value == leftMid) 
    {
      // subtract the step down value to the current value 
      newValue -= CURR_STEP_DW;
      FadeStepDown(newValue);
    } else if (results.value == rightMid) 
    {
      // add the step up value to the current value 
      newValue += CURR_STEP_UP;
      FadeStepUp(newValue);
    } else if (results.value == right) 
    {
      // fade to full brightness
      FadeStepUp(CURR_MAX);
    }

    // ready for receiving the next value
    irrecv.resume(); // Receive the next value
  }
}
