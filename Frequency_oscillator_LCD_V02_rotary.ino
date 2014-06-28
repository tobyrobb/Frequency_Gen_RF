/* T Robb 22.7.13

the way it works , is we read the buttons for up and down, we use that to increment or decrement a number which we later
use to set an increment or decrement amount, ie 10hz, 1khz, 1mhz etc
then if the left or right buttons are pushed we go up or down in frequency byt the incrment amount

*/


// Include the library code

#include <ooPinChangeInt.h> // necessary for the rotary encoder 
#include <AdaEncoder.h>
#include <stdio.h>
#include <dds.h>
#include <LiquidCrystal.h>

#define RESET A3
#define data_pin 13
#define load_pin 8
#define clock_pin A1
#define buttonPin 11
#define speakerPin 4      //Just in case we are using the keypad with the speaker
#define ENCA_a 5    //Rotary encoder pins
#define ENCA_b 6    //Rotary encoder pins
#define relay 2

#define clock_hz 120000000LL
#define calibrationValue -0.0400000 // this is a value we change to calibrate our particular chip more accurately

  

// chip, data_pin, load_pin, clock_pin, clock_hz
dds ddschip(DDS9850, data_pin, load_pin, clock_pin, clock_hz);  // set my dds up with 120mhz onboard crystal
AdaEncoder encoderA = AdaEncoder('a', ENCA_a, ENCA_b);      //define the instance of the rotary encoder
LiquidCrystal lcd(7, 12, A0, A2, 9, 10);       //define the instance of the LCD

// some variables to use in our program
long toFrequency = 14070000;
long currentFrequency;
long maxFrequency = 40000000;
long minFrequency = 0;
int incrementNumber = 6;
int maxprogramnumber = 6;  // dont forget to increase the menu numbers here!!
int programnumber = 1;           
int8_t clicks=0;  // Rotary encoder state

void setup()
 
{
  Serial.begin(9600);
  Serial.println("Beginning Setup");
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T.Robb    V0.1b  ");    //Print a little message
  lcd.setCursor(0, 1);
  lcd.print("  DDS Sine wave  ");
  delay(2000);
  // setup pins
  pinMode(RESET, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(load_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH); 
  
  ddschip.calibrate(calibrationValue);  // this is a value we change to calibrate our particular chip more accurately
  ddschip.setfrequency(toFrequency);
  lcd.clear();
 
}
 
void loop()
{
    Serial.println("Main LOOP");
// Lets check the rotary encoder
   AdaEncoder *thisEncoder=NULL;
   thisEncoder=AdaEncoder::genie();
  if (thisEncoder != NULL) {
    // Something must be happening with the rotary encoder if not NULL
    clicks=thisEncoder->query();
    if (clicks > 0) {
      Serial.println(" CW");
       upFrequency();
    }
    if (clicks < 0) {
      Serial.println(" CCW");
  downFrequency();
    }
  }
  
  //check the buttons
  if(!digitalRead(buttonPin)){
  tone(speakerPin, 900, 75);
  incrementNumber++;
  delay(200);
  } 
    
  if(toFrequency >= maxFrequency){(toFrequency = maxFrequency);}
  if(toFrequency <= minFrequency){(toFrequency = minFrequency);}
  ddschip.setfrequency(toFrequency);
  currentFrequency = toFrequency;
 

switch(incrementNumber){
        
        case 0:
          Serial.println("increment amount is 1hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By    1hz");  
          break;
         
        case 1:
          Serial.println("increment amount is 10hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By   10hz ");  
          break;
        
        case 2:
          Serial.println("increment amount is 100hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By  100hz ");  
          break;
        
        case 3:
          Serial.println("increment amount is 1 000hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By   1khz"); 
          break;
 
        case 4:
          Serial.println("increment amount is 10 000hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By  10khz");  
          break;
 
        case 5:
          Serial.println("increment amount is 100 000hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By 100khz");  
          break;
      
        case 6:
          Serial.println("increment amount is 1 000 000hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By   1Mhz");  
          break;
      
        default:  
          Serial.println("increment amount is 100hz");
          lcd.setCursor(0, 0);
          lcd.print("Change By  100hz ");  
          break;
      }

  lcd.setCursor(0, 1);
  lcd.print("Freq is ");    //Print to lcd
  lcd.setCursor(8, 1);
  lcd.print(currentFrequency);
  
  Serial.println(incrementNumber);  // temporary for debuggin delete me
  
  Serial.print("Current Frequency is set to :");
  Serial.println(currentFrequency);
 
  if(incrementNumber > 6){incrementNumber = 0;}          // this is where the menu goes around and around
  if(incrementNumber < 0){incrementNumber = 6;}

  delay(100);
  lcd.clear();

}


void upFrequency()
{
  Serial.println("Going UP Frequency");
  switch(incrementNumber){
  
    case 0: 
         toFrequency = (toFrequency + 1);
         break;
         
    case 1: 
         toFrequency = (toFrequency + 10);
         break;

    case 2: 
         toFrequency = (toFrequency + 100);
         break;

    case 3: 
         toFrequency = (toFrequency + 1000);
         break;

    case 4: 
         toFrequency = (toFrequency + 10000);
         break;

    case 5: 
         toFrequency = (toFrequency + 100000);
         break;

   
    case 6:
         toFrequency = (toFrequency + 1000000);
         break;
    
    default: 
         toFrequency = (toFrequency + 10);
         break;
 }

}


void downFrequency()
{
  
  Serial.println("Going DOWN Frequency");
  switch(incrementNumber){
  
    case 0: 
         toFrequency = (toFrequency - 1);
         break;
         
    case 1: 
         toFrequency = (toFrequency - 10);
         break;

    case 2: 
         toFrequency = (toFrequency - 100);
         break;

    case 3: 
         toFrequency = (toFrequency - 1000);
         break;

    case 4: 
         toFrequency = (toFrequency - 10000);
         break;

    case 5: 
         toFrequency = (toFrequency - 100000);
         break;

    case 6:
         toFrequency = (toFrequency - 1000000);
         break;
    
    default: 
         toFrequency = (toFrequency - 10);
         break;
 }
}

