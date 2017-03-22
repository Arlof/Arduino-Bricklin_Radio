#include <HID.h>
#include <EEPROM.h>

/* Bricklin Radio Testing

 Turns on and off a light emitting diode (LED) connected to a digital
 pin, without using the delay() function.  This means that other code
 can run at the same time without being interrupted by the LED code.

 The circuit:
 * LED attached from pin 13 to ground.
 * Motor Is High Side 12 V Base To Ard PIN 7 
 * ADC Not Used
 * 

 */

// constants won't change. Used here to set a pin number :
const int ledPin =  13;      // the number of the LED pin
const int motorPin = 7;     // Pin Connected To Base Of Transistor
const int hallPin = 10;      // Connect To ADC from Feedback Resistor


// Variables will change :
int ledState = LOW;             // ledState used to set the LED



// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change :
const long interval = 60000;           // interval at which to blink (milliseconds)
                                       // 60000 = to One Minute

static long currentTime = 350;
static int currentTimeMM = 46;
static int currentTimeH = 11;

void setup() 
{
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(hallPin, INPUT);
  digitalWrite(motorPin, LOW);      
  Serial.begin(9600);                 // Open Serial Port
  while (!Serial) ;
    
  // Set to Known Condition Input 0 
  while(digitalRead(hallPin) == HIGH)
    {
      digitalWrite(motorPin, HIGH);
    }
  digitalWrite(motorPin, LOW);

  int mem_status = EEPROM.read(1);
  Serial.println("MemTime: ");
  Serial.print(mem_status);

  if (mem_status == 88)
     {
      Serial.print("Valid Memory Found ");
      int mem_HH = EEPROM.read(2);
      int mem_MM = EEPROM.read(3);
      char curtime[10];
      sprintf(curtime, "%2d : %02d",mem_HH,mem_MM);      // Allows For Leading Zero In Minutes
      Serial.print("Memory Time: ");
      Serial.println(curtime);
     }
    else 
    {
      Serial.println("No Valid Memory");
    }
}

void store_time(int set, int hour, int mins)
{
  EEPROM.write(1, set);          // 88 = Key Code For Memory Stored
  EEPROM.write(2, hour);
  EEPROM.write(3, mins);
}

void clear_memory()
{
  EEPROM.write(1, 0);          // 88 = Key Code For Memory Stored
  EEPROM.write(2, 0);
  EEPROM.write(3, 0);
}

void advancemin()
{
  digitalWrite(motorPin, HIGH);
  Serial.println("MOTOR ON");
  //delay(500);                    //  Delay The First Read To Move Magnet
 while (digitalRead(hallPin) == LOW)
  {
    //printf ("moving low");
    //delay(10);  
  }
 Serial.println("Stage One Of Two");

 while (digitalRead(hallPin) == HIGH)
 {
    //printf( "moving high");
    //delay(10);
 }
  Serial.println("Stage Two Of Two Complete");
  digitalWrite(motorPin, LOW);
  Serial.println("MOTOR OFF");
}

void settime()
{
  


  if (currentTimeMM <= 59)
    {
      currentTimeMM++;          
    }
  if (currentTimeMM == 60)
  {
    currentTimeMM = 00;         // Turn Over The Hour Roll Back The Minutes
    currentTimeH ++;  
     if (currentTimeH == 13)
    {
      currentTimeH = 1;         // Set For 12 Hour Clock 
    }
  }
    char curtime[10];
    sprintf(curtime, "%2d : %02d",currentTimeH,currentTimeMM);      // Allows For Leading Zero In Minutes
    
    Serial.print("Current Time: ");
    Serial.println(curtime);

    store_time(88,currentTimeH,currentTimeMM);                    // Store New Time In Memory 
    
    }

void loop() 
{

  
//  motorPin = LOW;
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  
  //Serial.println(currentTime);
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
{
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) 
    {
      ledState = HIGH;
    } else 
    {
      ledState = LOW;
    }
    settime();
    advancemin();
    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }

    
}
