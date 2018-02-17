#include "tiny_IRremote.h"   // the standard IR library does not work (it uses the TIMER0 which is also used for the delay function)
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3,4);

// the IR receiver is connected to PIN3
#define RECV_PIN 2

// defines the PIN where the temperature-sensor is connected
#define TEMP_SENSOR 5
// maximum temperature allowed
#define TEMP_THRESHOLD 60

// define two different outputs, so each output could be controlled seperatly (in this sketch both are controlled the same way)
#define OUT_0 0
#define OUT_1 1

// defines the parameters of this program (start value for the outputs, in- and decrease steps, ...)
#define CURR_START 0
#define CURR_MAX 250
#define CURR_MIN 0
#define CURR_INC_UP 1
#define CURR_INC_DW 1
#define CURR_STEP_UP 25
#define CURR_STEP_DW 25
#define DELAY_STEP_UP 25
#define DELAY_STEP_DW 25

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
// the current output value for OUT_1
volatile int curr = 0;

// the setup function runs once when you press reset or power the board
void setup() 
{
  mySerial.begin(9600);
  // activate IR receiver
  irrecv.enableIRIn(); // Start the receiver
  
  // when turned on the value is 0
  curr = CURR_START;
  // Define pins, currently only OUT_1 is used
  pinMode(OUT_0, OUTPUT);
  pinMode(OUT_1, OUTPUT);

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

/*
  TESTING!!!
  Gets the actual temperature value and if it is below the threshold value it returns true otherwise false
  If no temperature sensor is available, connect the data pin to ground; in this case the function returns true
*/
bool IsTemperatureOK()
{
  float TempVal = analogRead(TEMP_SENSOR);

  // no temperature sensor is connected (should not be done as this is a security feature, but someone might not want to have it ...)
  if (TempVal == 0)
  {
    return true;
  }
  
  TempVal = (TempVal / 255) * 5; // Converts analogRead value to Volts
  // Voltage -> Temperature calculation, see https://learn.adafruit.com/tmp36-temperature-sensor
  TempVal = (TempVal * 1000) - 500; 
  TempVal = TempVal / 10;
  
  return (TempVal < TEMP_THRESHOLD);
}

// the loop function runs over and over again forever
void loop() 
{
  // need a break, need a kitk ... ahm delay :)
  delay(DELAY_LOOP_RUN);

  if (irrecv.decode(&results)) 
  {
    mySerial.print("Received value: ");
    mySerial.println(results.value);
    mySerial.flush();
    
    // based on the pressed button the desired funcion should be done
    if (results.value == left) 
    {
      LightDownSlow();
    } else if (results.value == leftMid) 
    {
      curr -= CURR_STEP_DW;
    } else if (results.value == rightMid) 
    {
      curr += CURR_STEP_UP;
    } else if (results.value == right) 
    {
      LightUpSlow();
    }

    // Just to make sure that everything is in range
    if (curr < CURR_MIN) { curr = CURR_MIN; }
    if (curr > CURR_MAX) { curr = CURR_MAX; }
    
    // Output value to OUT_0 and OUT_1
    analogWrite(OUT_0, curr);
    analogWrite(OUT_1, curr);

    // ready for receiving the next value
    irrecv.resume(); // Receive the next value
  }
}
