/*   Neue Weihnachtsbaumsteuerung
 *    by Dietmar Nikolai
 *    E-Mail:  dietmar-nikolai@t-online.de
 *    Version 1.0 von 15.02.2024
 *    Grundidee und Kit von AZ-Delivery
 *      AZDelivery DIY LED Weihnachtsbaum Kit: Weihnachtsbaum Elektronik Bausatz zum Löten - 
 *      Lötbausatz für einen drehenden Weihnachtsbaum mit LEDs und USB-Anschluss
 *    soweit von mir steht das Ganze unter GPL-Lizenz
 *    geschrieben mit Arduino 1.8.19
 *    bei Arduino 2.2.1 wird kein Programmer gefunden
 *    empirisch
 */


#define EIN HIGH
#define AUS LOW
#define motor_pin 6
#define baum_pin 8
#define grund_pin 7
#define test_pin  4
#define motorread_pin 3
#define baumread_pin 2
#define grundread_pin 1
#define loop_baum 9
#define loop_grund 10
#define zeit_pin 5
bool test = false;
bool zeit = true;
bool timer_on = false;
bool loopbaum = false;
bool loopgrund = false;
bool grundhoch = true;
bool baumhoch = true;
int grundzaehler = 0;
int baumzaehler = 0;
int grundzaehlerindex = 0;
int baumzaehlerindex = 0;
int loop_index[] = {1,2,3,4,5,6};
long yy = 0;
unsigned long s0 = 0,s1 = 21600,s2 = 64800,s3 = 93600,s4 = 172760, xx = 0;  // zeitaufteilung für 24 Stunden, 172800 halbe Sekunden - 20s = 40 delay-ausgleich
// 0 -an 3h - 4h aus -4h an - aus bis 24h gesamt
//unsigned long s0 = 0,s1 = 21600,s2 = 22200,s3 = 22800,s4 = 172800, xx = 21000;    // Timertest

void ausgabe_loopbaum(){

if (baumhoch) {
    baumzaehlerindex = map( analogRead(baumread_pin),0,1023,0,5);
     baumzaehler = baumzaehler + loop_index[baumzaehlerindex];
    if (baumzaehler > 250){
     baumhoch = false;
     baumzaehler = 250;
    }
}
else {
   baumzaehlerindex = map( analogRead(baumread_pin),0,1023,0,5);
     baumzaehler = baumzaehler - loop_index[baumzaehlerindex];
    if (baumzaehler < 20){
     baumhoch = true;
     baumzaehler = 20;    
       }
    }
    analogWrite(baum_pin,baumzaehler);
}

void ausgabe_loopgrund(){

if (grundhoch) {
    grundzaehlerindex = map( analogRead(grundread_pin),0,1023,0,5);
     grundzaehler = grundzaehler + loop_index[grundzaehlerindex];
    if (grundzaehler > 250){
     grundhoch = false;
     grundzaehler = 250;
    }
}
else {
   grundzaehlerindex = map( analogRead(grundread_pin),0,1023,0,5);
     grundzaehler = grundzaehler - loop_index[grundzaehlerindex];
    if (grundzaehler < 20){
     grundhoch = true;
     grundzaehler = 20;    
       }
   }
    analogWrite(grund_pin,grundzaehler);
 
}

void alles_on(){
      
      if (!digitalRead(loop_baum)){
        ausgabe_loopbaum();
      }
      else {
         yy = 0;
         yy = analogRead(baumread_pin);
        analogWrite(baum_pin, map(yy,0, 1023, 20, 255));
         }

      if (!digitalRead(loop_grund)){
        ausgabe_loopgrund();
      }
      else {
        yy = 0;
        yy = analogRead(grundread_pin);
        analogWrite(grund_pin, map(yy,0, 1023, 20, 255));
      }
      
      yy = 0; 
      yy = analogRead(motorread_pin);
      analogWrite(motor_pin, map(yy,0, 1023, 600, 1010));
       
}

void setup() {
  
  delay(1000);
  pinMode(motor_pin, OUTPUT);
  pinMode(baum_pin, OUTPUT);
  pinMode(grund_pin, OUTPUT);
  digitalWrite(motor_pin, LOW);
  pinMode(grund_pin, OUTPUT);
  digitalWrite(motor_pin, LOW);
   pinMode(test_pin, OUTPUT);
  digitalWrite(test_pin, LOW);
  digitalWrite(baum_pin, LOW);
  digitalWrite(grund_pin, LOW);
  pinMode(motorread_pin, INPUT);
  pinMode(baumread_pin, INPUT);
  pinMode(grundread_pin, INPUT);
  pinMode(loop_baum, INPUT);
  pinMode(loop_grund, INPUT);
  pinMode(zeit_pin, INPUT);
  zeit = digitalRead(zeit_pin);


  /* Versuche mit Frequenzen der PWMs
   * haben sich eher negativ ausgewirkt
   * ATTiny84 mit 1MHz internal bessere Wahl
   * Pin 6 Motor 250Hz  einstellbar von ca. 1,4 bis 4 U/min
   * Pin 7 und 8 Grund und Baum 63Hz

  --------------------------------------------------------------------------
   TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(WGM11);
   TCCR1B = _BV(CS10) | _BV(WGM12); 
  ----------------------------------------------------------------------------
  TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(WGM11);
  TCCR1B = _BV(CS10) | _BV(WGM12);
     TCCR1A = _BV(COM1A1) | _BV(WGM10) | _BV(WGM11); 
     TCCR1B = _BV(CS11) | _BV(WGM12) | _BV(WGM13);
 
  --------------------------------------------------------------------------------  
    TCCR1A = (1<<COM1A1) + (1<<WGM11); 
    TCCR1B = (1<<WGM13) + (1<<WGM12) + (1<<CS10); // prescaler = none; 
    ICR1 = 15999;
    OCR1A = 3999;
    DDRB |= (1<<PB1);

  --------------------------------------------------------------------------------------------
  TCCR1A = (1<<COM1A1) + (1<<COM1B1) + (1<<WGM11); 
  TCCR1B = (1<<WGM13) + (1<<WGM12) + (1<<CS10); // prescaler = 1 (none) 
  ICR1 = 15999;
  OCR1A = 3999;
  OCR1B = 11999;
  DDRB |= (1<<PB1)|(1<<PB2);
  -------------------------------------------------------------------------------------------------------------  
   DDRB = 1 << DDB0;                       
  TCCR0A = 1 << COM0A1 | 1 << WGM01 | 1 << WGM00;
  TCCR0B = 1 << CS00;
  ---------------------------------------------------------------------------------
     TCCR0A = _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);  // | _BV(WGM10)
     TCCR0B = _BV(CS01) | _BV(WGM02); 
  -----------------------------------------------------------------------------
  */
}

void loop() {
       if (((xx >= 0) && (xx< s1)) || (( xx>= s2) && ( xx< s3))){
        timer_on = true;
       }
       else timer_on = false;
       
      if ((!zeit) && (timer_on)) {
        alles_on();
    
       }
     else if (zeit){
     alles_on();
     }
  
      else {
      digitalWrite(grund_pin, LOW);
      digitalWrite(baum_pin, LOW);
      digitalWrite(motor_pin, LOW);
      }
  
    if ( test) {
      test = false;
      digitalWrite(test_pin, LOW);  
    }
    else {
      test = true;
      digitalWrite(test_pin, HIGH);
    }

    xx++;
    if (xx > s4) xx = 0;
   delay(517);  // ergibt fast genau 500 ms
   delayMicroseconds(900);
}
