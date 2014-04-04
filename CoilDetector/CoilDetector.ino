// Arduino based metal detector
// (C)Dzl july 2013
// http://dzlsevilgeniuslair.blogspot.dk/

// Connect search coil oscillator (20-200kHz) to pin 5

// For calibration send char 's' 


#define SET(x,y) (x |=(1<<y))		        //-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       	// |
#define CHK(x,y) (x & (1<<y))           	// |
#define TOG(x,y) (x^=(1<<y))            	//-+

unsigned long t0=0;         //-Last time
int t=0;                    //-time between ints
unsigned char tflag=0;      //-Measurement ready flag

//-Generate interrupt every 1000 oscillations of the search coil
SIGNAL(TIMER1_COMPA_vect)
{
  OCR1A+=1000;
  t=micros()-t0;
  t0+=t;
  tflag=1;
}

void setup()
{
  Serial.begin(9600);
  //-Set up counter1 to count at pin 5
  TCCR1A=0;
  TCCR1B=0x07;
  SET(TIMSK1,OCF1A);
  Serial.println("Ready>");
}

//-Float ABS
float absf(float f)
{
  if(f<0.0)
    return -f;
  else
    return f;
}


int   v0=0;  //-NULL value
float f=0;   //-Measurement value
byte cmd=0;


void loop()
{
  if(tflag)
  {
    if (Serial.available()){  //-Check NULL SW.
      cmd = Serial.read();
      if (cmd == 's'){ 
            v0=t;             //-Sample new null value (Calibration procedure)
            Serial.println("NULL value installed");
          }
    }      
    f=f*0.9+absf(t-v0)*0.1;   //-Running average over 10 samples (filtering)
    tflag=0;                  //-Reset flag
    Serial.println(f);        
  }
  
  
}
