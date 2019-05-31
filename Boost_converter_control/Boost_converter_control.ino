/*single phase chopper Control
 
 Updated by Ahmed LIMEM_Tarek Ben Dahou_2AGE2
 Pins: analog-In()<=A0-A3, digital-Out<=pin10
 
*/

int a =0;
int b =0;
long int time;

float x;
unsigned long pwm;

void setup()
{
 // Serial.begin(9600);
  
pinMode(10, OUTPUT);
TCCR1A = _BV(COM1A1) | _BV(COM1B1) ; 
TCCR1B = _BV(WGM13) | _BV(CS11);
time=millis();

}

void loop(){


  if(millis() >time+500) {
  a = analogRead(A3);
if (  a > 30 )
 {ICR1 = a; }
else if (a < 30) { ICR1 = 30; } 
b = analogRead(A0);

if ((b > 105) && (b < 950))
{pwm = b;}
else if (b < 105) {pwm = 105;}
else if ( b> 950) {pwm=950;}

x = float(pwm);
x = x * ICR1;
x = x / 1023;
OCR1B = int(x);
time=millis();
}
 // Serial.println(a);
}
