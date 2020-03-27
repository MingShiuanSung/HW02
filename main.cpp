#include "mbed.h"

// switch
DigitalIn  Switch(SW3);

DigitalOut redLED(LED1);

DigitalOut greenLED(LED2);

// seven-seg
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);

char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char disp[3] = {};

// FFT
Serial pc( USBTX, USBRX );

AnalogOut Aout(DAC0_OUT);

AnalogIn Ain(A0);

int sample = 2048;

int result;

float n = 0.0;

float ADCdata[2048];


void int_to_seg(int x) {
  int a = 0, b = 0, c = 0;

  if (x > 99) {
    a = x / 100;
    x -= a * 100;
    b = x / 10;
    x -= b * 10;
    c = x;
  }
  else if (x > 9) {
    b = x / 10;
    x -= b * 10;
    c = x;
  }
  else 
    c = x;

    disp[0] = table[a];
    disp[1] = table[b];
    disp[2] = table[c];
}


int main(){

  while(1){

    if( Switch == 1 ){

        greenLED = 0;

        redLED = 1;

        display = 0x00;

    }

    else{

      redLED = 0;

      greenLED = 1;

      for (int i = 0; i < sample; i++){

        ADCdata[i] = Ain;

        wait(1./sample);
      }

      // calculate frequency
      for (int i = 0; i < sample; i++)
        if (ADCdata[i] == 0.000 && ADCdata[i-1] != 0.000) n++;

      result = round(n*10.0/11.0);
      
      n = 0.0;

      // seven-seg-disp goes here
      int_to_seg(result);

      for (int i = 0; i < 3; ++i){

        display = disp[i];

        wait(1);
      }
      display[7] = 0x1;
      wait(1);
    
      // FFT analysis
      for (int i = 0; i < sample; i+=2){

        pc.printf("%1.3f\r\n", ADCdata[i]);

        wait(0.0001);
      }

      // display sine wave on picoscope
      for(float i=0; i<2*result; i+=0.05 ){

        Aout = 0.5 + 0.5*sin(i*3.14159);

        wait(1./float(result*40));
      }
    }


  }





}