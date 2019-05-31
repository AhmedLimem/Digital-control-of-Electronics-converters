/*single phase dimmer Control
 
 Updated by Ahmed LIMEM_Tarek Ben Dahou_2AGE2
 Pins: analog-In()<=A0, Interrupt<=pin2, digital-Out<=pin10-11
 pins display LCD-I2C: SDA:A4 SCL:A5
*/

#include  <TimerOne.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //i2c pins 

volatile int i=0;               // Variable to use as a counter volatile as it is in an interrupt
volatile int j=0;
volatile int l=0;
volatile int p=0;
volatile int k=0;

volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
volatile boolean zero_cross1=0;
volatile boolean zero_cross3=0;
volatile boolean zero_cross4=0;
  
volatile boolean the1=0;  //Boolean variable to detect the passage of amortization delay
volatile boolean the2=0;  

unsigned long dim = 0;  //the limit of the counter to trigger the amortization of the thyristor
unsigned long dim_per =0;
int a =0;

int alpha;  //thyristor amortization delay in degrees

float freq;  
float freqx;

int TimeStep = 17;  // This is the delay-per-step in microseconds.

long int tm;
long int time;
unsigned long T;
unsigned long Tt;
unsigned long Tmoy;
unsigned long Per[10];
//unsigned long Per[100]; for systems with high inertia so the frequency varies slowly within 2 seconds for 50 Hz frequency
int n=0;

// int Xt=21; //the maximum number of toggling signal per cycle de control of thyristor
                     // Xt=(timeStep/te)*(2*dim_per)
                     //te is execution time of 1 toggle in microseconds
                     // execution frequency is 30KHz when we use arduino instructions
                     // execution frequency is 170KHz when we use direct port manipulation with PORTx registers


void setup(){

lcd.begin(16,2);
lcd.backlight();

// Set the thyristor pin as output
  DDRB |= 1 << DDB2; //I/O port PB2    
  DDRB |= 1 << DDB3; //I/O port PB3 
  
  attachInterrupt(0, zero_cross_detect, RISING ); // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(TimeStep);  // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, TimeStep);                                                   
}

void zero_cross_detect() {
  T=TimeStep*k; 
  k=0;

  zero_cross = true;   // set the boolean to true to tell our dimming function that a zero cross has occured
  zero_cross1 = true;
  the1 = false;
  i=0;
  l=0;
  PORTB &= ~(1<<PORTB2);  // turn off thyristor
  //set the Arduino pin Low  
         
  zero_cross3 = true;  // set the boolean to true to tell our dimming function that a zero cross has occured
  zero_cross4 = true;
  the2 = false;
  j=0;
  p=0;
  PORTB &= ~(1<<PORTB3);  // turn off thyristor 
  //set the Arduino pin Low 
}                                       


void dim_check() {  
  k++; 
                
  if(zero_cross == true) {              
    if(i>=dim) {  
     the1 = true;
      i=0;  // reset time step counter                         
     zero_cross = false; //reset zero cross detection
    } 
    else {
      i++; // increment time step counter                     
    }
  } 
                
if(zero_cross1 == true) {   
  if(l>=dim_per){
    
        the1 = false;
        l=0;  // reset time step counter                         
      zero_cross = false;
        }
        else
        {l++;}                                  
  }
  
// Toggle on the thyristor at the appropriate time    
  if(the1 == true) {  
  PORTB =PORTB^_BV(2);
}else{PORTB &= ~(1<<PORTB2);}


  if(zero_cross3 == true) {              
    if(j>=dim+dim_per) {  
      the2 = true;
      j=0;  // reset time step counter                         
      zero_cross3 = false; //reset zero cross detection
    } 
    else {
      j++; // increment time step counter                     
    }                                
  } 

if(zero_cross4 == true) {   
  if(p>=2*dim_per){
        the2 = false;
        p=0;  // reset time step counter                         
      zero_cross4 = false;
        }
        else
        {p++;}                                  
  }                                  
          
// Toggle on the thyristor at the appropriate time
 if(the2 == true) {  
   PORTB =PORTB^_BV(3);
}else{PORTB &= ~(1<<PORTB3); }
} 

 
void loop() {  

Per[9]=T;
Tt=Per[9];
for (n = 0 ; n < 9 ; n++)
{
   Tt=Tt+Per[n];
   Per[n]=Per[n+1]; 
 } 
Tmoy=floor(Tt/10);
dim_per=floor(Tmoy/(2*TimeStep));

 if(millis() >time+50){
  a = analogRead(A0);
  time=millis();
  }
dim=map(a, 0, 1023, 1, dim_per);

alpha=floor(dim*180/dim_per);
freqx=100000000/T;
freq=freqx/100 ;

if(millis() >tm+150){  //Delay used to give a dinamic effect
lcd.clear();

lcd.setCursor(8,0);
lcd.print(alpha);
lcd.setCursor(0,0); //we start writing from the first row first column
lcd.print("ALPHA ="); //16 characters poer line
lcd.setCursor(13,0);
lcd.print("deg");
lcd.setCursor(13,1);
lcd.print("Hz");
lcd.setCursor(8,1);
lcd.print(freq);
lcd.setCursor(0,1); //we start writing from the first row first column
lcd.print("freq  =");

tm=millis();}

}
