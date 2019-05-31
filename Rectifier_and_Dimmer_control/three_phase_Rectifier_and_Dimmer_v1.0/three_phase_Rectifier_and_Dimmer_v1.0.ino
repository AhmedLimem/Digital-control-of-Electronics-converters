
/*Dimmer Control
 
 Updated by Ahmed LIMEM_Tarek Ben Dahou_2AGE2

 to refer: dim<=limitRise, dim_per<=limitWave, Per<=period, dim_check()<=checking periodically
 Pins: analog-In()<=A0, Interupts-pins<=18(pin9-Ty); 3(pin12-Tx); 2(Square Wave), digital-Out-pins<=X(10,11); Y(7,8); Z(4,5)
 Requirements: potentiometer-1K

to improve: toggle output signals at least 30 khz - developpe PLL
method instead of the average methode - build file mono.h  
and call it in main to simplify the code 
 
 Changed zero-crossing detection to look for RISING edge only rather
 than rising and falling.  (originally it was chopping the positive and negative half
 of the AC square wave form). 
 
 Also changed the check() to turn on the Thyristor every half
 of the AC square wave form, leaving it on until the zero_cross_detect() turn's it off.
 
 Adapted from AC light control sketch by Robert Twomey 
https://codebender.cc/sketch:171819#AC-Dimmer-Timer%20Driven.ino
 
 */

#include  <TimerOne.h>          // Avaiable from http://www.arduino.cc/playground/Code/Timer1
#include <TimerThree.h>
#include <TimerFive.h> 
#include <TimerFour.h>

volatile int k=0;

volatile int i0x=0;               // Variable to use as a counter volatile as it is in an interrupt
volatile int l0x=0;
volatile int i1x=0; 
volatile int l1x=0;              
volatile int ex=0;              
volatile int fx=0;


volatile int i0y=0;               
volatile int l0y=0;
volatile int i1y=0; 
volatile int l1y=0; 
volatile int ey=0;              
volatile int fy=0;

volatile int i0z=0;               
volatile int l0z=0;
volatile int i1z=0; 
volatile int l1z=0; 


volatile boolean zero_cross11x=0;  // Boolean to store a "switch" to tell us if we have crossed zero
volatile boolean zero_cross12x=0;
volatile boolean zero_cross21x=0;
volatile boolean zero_cross22x=0;
volatile boolean zero_crossT0x=0;
volatile boolean zero_crossT1x=0;

volatile boolean zero_cross11y=0;
volatile boolean zero_cross12y=0;
volatile boolean zero_cross21y=0;
volatile boolean zero_cross22y=0;
volatile boolean zero_crossT0y=0;
volatile boolean zero_crossT1y=0;

volatile boolean zero_cross11z=0;
volatile boolean zero_cross12z=0;
volatile boolean zero_cross21z=0;
volatile boolean zero_cross22z=0;

volatile boolean th11=0;
volatile boolean th12=0;
volatile boolean th21=0;
volatile boolean th22=0;
volatile boolean th31=0;
volatile boolean th32=0;
                    
unsigned long dim = 0;  // Dimming level (0-dim_per)  0 = on, dim_per = 0ff
unsigned long dimT = 0;
unsigned long dim_per =0;
unsigned long dim_perT =0;
                    
int timeStep_toggle = 35;
int timeStep = 85;    //This is the delay-per-step in microseconds.
                      // It is calculated based on the number of steps you want.
  
// Realize that there are 2 zerocrossing per cycle. 
//This means to calculate timeStep: divide the length of one full half-wave of the power
// cycle (in microseconds) by the number of steps. 

int a =0;
long int time;
unsigned long T;
unsigned long Tt;
unsigned long Tmoy;
unsigned long Per[10];
int n=0;

void setup() { // Begin setup
// Set the pins as output
  DDRB |= 1 << DDB4; //I/O port PB4  // Output to Thyristor  
  DDRB |= 1 << DDB5; //I/O port PB5   
  DDRB |= 1 << DDB6; //I/O port PB6   
     
  DDRH |= 1 << DDH4; //I/O port PH4 
  DDRH |= 1 << DDH5; //I/O port PH5    
  DDRH |= 1 << DDH6; //I/O port PH6

  DDRG |= 1 << DDG5; //I/O port PG5    
  DDRE |= 1 << DDE3; //I/O port PE3 
  
  attachInterrupt(0, zero_cross_detect, RISING ); // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  attachInterrupt(1, zero_cross_detect3, RISING ); // Attach an Interupt (Int1) to Pin 3
  attachInterrupt(5, zero_cross_detect5, RISING ); // Attach an Interupt (Int5) to Pin 18
  //**  
  Timer1.initialize(timeStep);  // Initialize TimerOne library for the freq we need
  Timer3.initialize(timeStep_toggle);
  Timer5.initialize(timeStep_toggle);
  Timer4.initialize(timeStep_toggle);
  //**
  Timer1.attachInterrupt(dim_check, timeStep); 
  Timer3.attachInterrupt(dim_check3, timeStep_toggle); 
  Timer5.attachInterrupt(dim_check5, timeStep_toggle);  
  Timer4.attachInterrupt(dim_check4, timeStep_toggle); 
  //**  
  // Use the TimerOne Library to attach an interrupt
  // to the function we use to check to see if it is 
  // the right time to fire the Thyristor.  This function 
  // will now run every freqStep in microseconds.                                            
}
//void zero_cross_detect4() {

//}
void zero_cross_detect() {

 T=timeStep*k;  
  k=0;
   
  zero_cross11x = true;   // set the boolean to true to tell our dimming function that a zero cross has occured
  zero_cross12x = true;
  zero_cross21x = true;            
  zero_cross22x = true;
  zero_crossT0x = true;               
  zero_crossT1x = true;

  th11=false;
  th12=false;
  
  i0x=0;
  l0x=0;
  i1x=0;
  l1x=0;
  ex=0;
  fx=0;

  // turn off Thyristor 
  PORTB &= ~(1<<PORTB4);
  PORTB &= ~(1<<PORTB5);
  PORTB &= ~(1<<PORTB6);

}       

void zero_cross_detect3() {    
  zero_cross11y = true;     // set the boolean to true to tell our dimming function that a zero cross has occured
  zero_cross12y = true;
  zero_cross21y = true;            
  zero_cross22y = true;
  zero_crossT0y = true;               
  zero_crossT1y = true;

  th21=false;
  th22=false;

  i0y=0;
  l0y=0;
  i1y=0;
  l1y=0;
  ey=0;
  fy=0;

  // turn off Thyristor
  PORTH &= ~(1<<PORTH4);
  PORTH &= ~(1<<PORTH5);
  PORTH &= ~(1<<PORTH6);
  
} 

void zero_cross_detect5() {    
  zero_cross11z = true;            // set the boolean to true to tell our dimming function that a zero cross has occured
  zero_cross12z = true;               
  zero_cross21z = true;            
  zero_cross22z = true;

  th31=false;
  th32=false;
  
  i0z=0;
  l0z=0;
  i1z=0;
  l1z=0;
  
  // turn off Thyristor
  PORTG &= ~(1<<PORTG5);
  PORTE &= ~(1<<PORTE3);
}           

void dim_check() {  
  k++;
                  
  if(zero_cross11x == true) {              
    if(i0x>=dim) {  // Thyristor On propogation delay  
      th11=true;                   
      i0x=0;  // reset time step counter                         
      zero_cross11x = false; //reset zero cross detection
    } 
    else {
      i0x++; // increment time step counter                     
    }
  }                                
  if(zero_cross12x == true) {   
    if(l0x>=dim_per){
        th11=false;
        l0x=0;  // reset time step counter                          
        zero_cross12x = false;
        }
        else
        {l0x++;}                                  
  }
  
  if(zero_cross21x == true) {              
     if(i1x>=dim+dim_per) { // Thyristor On propogation delay       
     th12=true;     
     i1x=0;  // reset time step counter                         
     zero_cross21x = false; //reset zero cross detection
    } 
    else {
      i1x++; // increment time step counter                     
    }
  }
                               
  if(zero_cross22x == true) {   
    if(l1x>=2*dim_per){
        th12=false;
        l1x=0;  // reset time step counter                          
        zero_cross22x = false;
        }
        else
        {l1x++;}                                  
  }

  if(zero_crossT0x == true) {              
    if(ex>=dimT) {         
     PORTB |= 1 << PORTB6;       
     ex=0;                        
     zero_crossT0x = false;
    } 
    else {
      ex++;                    
    }
  } 
                                 
  if(zero_crossT1x == true) { 
     if(fx>=dim_perT){
       PORTB &= ~(1<<PORTB6); 
       fx=0;                     
       zero_crossT1x = false;
       }
       else
        {fx++;}                                  
  }
                  
  if(zero_cross11y == true) {              
    if(i0y>=dim) {  // Thyristor On propogation delay           
     th21=true;      
      i0y=0;  // reset time step counter                         
     zero_cross11y = false; //reset zero cross detection
    } 
    else {
      i0y++; // increment time step counter                     
    }
  }  
                                
  if(zero_cross12y == true) {   
    if(l0y>=dim_per){
        th21=false;
        //digitalWrite(AC_pin1y, LOW);
        l0y=0; // reset time step counter                         
        zero_cross12y = false;
        }
        else
        {l0y++;}                                  
  }

  if(zero_cross21y == true) {              
    if(i1y>=dim+dim_per) {  // Thyristor On propogation delay
    th22=true;       
     i1y=0;  // reset time step counter                         
     zero_cross21y = false; //reset zero cross detection
    } 
    else {
      i1y++; // increment time step counter                     
    }
  } 
                                 
  if(zero_cross22y == true) {   
    if(l1y>=2*dim_per){
        th22=false;
        l1y=0;  // reset time step counter                         
      zero_cross22y = false;
        }
        else
        {l1y++;}                                  
  }

  if(zero_crossT0y == true) {              
    if(ey>=dimT) {         
     PORTH |= 1 << PORTH6;      
     ey=0;                        
     zero_crossT0y = false;
    } 
    else {
      ey++;                    
    }
  } 
                                 
  if(zero_crossT1y == true) {   
    if(fy>=dim_perT){
       PORTH &= ~(1 << PORTH6); 
       fy=0;                     
       zero_crossT1y = false;
       }
       else
        {fy++;}                                  
  }
                   
  if(zero_cross11z == true) {              
    if(i0z>=dim) {  // Thyristor On propogation delay       
     th31=true;      
      i0z=0;  // reset time step counter                         
     zero_cross11z = false; //reset zero cross detection
    } 
    else {
      i0z++; // increment time step counter                     
    }
  } 
                               
  if(zero_cross12z == true) {   
    if(l0z>=dim_per){
        th31=false;
        l0z=0;  // reset time step counter                           
        zero_cross12z = false;
        }
        else
        {l0z++;}                                  
  }

  if(zero_cross21z == true) {              
    if(i1z>=dim+dim_per) { // Thyristor On propogation delay                
      th32=true;      
      i1z=0;  // reset time step counter                         
      zero_cross21z = false; //reset zero cross detection
    } 
    else {
      i1z++; // increment time step counter                     
    }
  }
                                  
  if(zero_cross22z == true) {   
    if(l1z>=2*dim_per){ 
        th32=false;
        l1z=0;  // reset time step counter
        zero_cross22z = false;
        }
        else
        {l1z++;}                                  
  }                                      
} 

// Turn on the Thyristor at the appropriate time

void dim_check3() {
 // turn on Thyristor-P1 
  if(th11 == true) {  
  PORTB =PORTB^_BV(4);
}else{PORTB &= ~(1<<PORTB4);} 

 if(th12 == true) {  
  PORTB =PORTB^_BV(5);
}else{PORTB &= ~(1<<PORTB5);} 
}
  
void dim_check4() {
  
 // turn on Thyristor-P2
 if(th21 == true) {  
  PORTH =PORTH^_BV(4);
}else{PORTH &= ~(1<<PORTH4);}

   if(th22 == true) {  
   PORTH =PORTH^_BV(5);
}else{PORTH &= ~(1<<PORTH5); }
}

void dim_check5() {

// turn on Thyristor-P3
 if(th31 == true) {  
   PORTG =PORTG^_BV(5);
}else{PORTG &= ~(1<<PORTG5); }

 if(th32 == true) {  
   PORTE =PORTE^_BV(3);
}else{PORTE &= ~(1<<PORTE3); } 
}


void loop() { 
                    
 if(millis() >time+50) {
  a = analogRead(A0);
  time=millis();
 }

 Per[9]=T;
Tt=Per[9];
for (n = 0 ; n < 9 ; n++){
   Tt=Tt+Per[n];
   Per[n]=Per[n+1];
   } 
Tmoy=floor(Tt/10);
dim_per=floor(Tmoy/(2*timeStep));
dimT=floor((120*dim_per)/180);
dim_perT=dimT+dim_per;
dim=map(a, 0, 1023, 3, dim_per);

}
