
#define MAX 6

///////////////////// Variables
int meter_pin1[8]             = {2,3,4,5,6,7};
int meter_pin2[8]             = {8,9,10,11,12,13};
int START_BTN                 = A1;
int LED_RED                   = A3;
int LED_GREEN                 = A4;

int count                     = -1;
int del                       = 10;
int toggle                    = 0;
////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////// Core Functionality
///////////////////// Randomizing
long seed(int s){ randomSeed(s); }
long randomize(int min, int max){ return random(min, max); }
/////////////////////

///////////////////// POPULATE ARR WITH RANDOMS
void generate_64(int* arr, long n1, long n2, int max){
  for(int i = 0 ; i < max ; i++){
      int m = i < 3 ? i : i-3 ;
      int k = i < 3 ? n1 : n2 ;
      arr[i] = (int)((k & (int) pow(2, m)) != 0) ;	
    }
}
/////////////////////

///////////////////// Randoms Display
void outputRandoms(){
  
  int arr1[MAX] = {0}, arr2[MAX] = {0};
  
  generate_64(arr1, randomize(8, 15) - 8, randomize(8, 15) - 8, MAX) ;
  generate_64(arr2, randomize(8, 15) - 8, randomize(8, 15) - 8, MAX) ;
  
  for(int i = 0 ; i < MAX ; i++){
    analogWrite(meter_pin1[MAX-i-1], arr1[i] != 0 ? 1023 : 0);
  }

  for(int i = 0 ; i < MAX ; i++){
    analogWrite(meter_pin2[MAX-i-1], arr2[i] != 0 ? 1023 : 0);
  }
}
///////////////////// 

///////////////////// LED Control & Display
void LED(int mode){
  int val_R, val_G;
  switch(mode){
    case 0:                           // LED back to normal GREEN
      val_R = 255, val_G = 0;         break;
    case 1:                           // LED flashes RED/GREEN in alternance
      toggle = !toggle;
      val_R = toggle * 255, val_G = !toggle * 255; break;
    case 2:                           // LED swithes to RED
      val_R = 0, val_G = 255;         break;
  }
  analogWrite(LED_RED, val_R);
  analogWrite(LED_GREEN, val_G);
}
///////////////////// 
////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////// Setup & Loop Declarations
///////////////////// Setup
void setup() {
  
  seed(analogRead(A0));                   // Rendom Seed Setup to unread A0 pin

  pinMode(A1, INPUT_PULLUP);              // Start Button Pin Setup
  
  for(int i =  0 ; i < MAX ; i++){        // Binary Meters Pin Setup
    pinMode(meter_pin1[i], OUTPUT);
    pinMode(meter_pin2[i], OUTPUT);
  }
}
///////////////////// 



///////////////////// Loop
void loop() {
  
  int state_read = digitalRead(START_BTN);

  if(state_read == LOW){ // while start button is on pressed state
    
    LED(1);                       // Set LED to ACTION Mode
    
    outputRandoms();               // generates & displays randoms
    
    count = 40;                   // countdown settings for when unpressed
    del = 10;

  }else{ // is start button is on inactive state

    if( count > -1){ // starts countdown
      
      outputRandoms();             // keep on generating & displaying randoms
      
      LED(2);                     // Set LED to WAIT Mode
      
      count--;                    // decrement countdown + increase delay (slowing down effect)
      delay(del+=2);

    }else{                        // countdown is done -> back to inactive/ready state
      
      LED(0);                     // Reset LED to DEFAULT Mode
      
      del = 10;                   // reset countdown settings
      count = -1;

    }
  }
  delay(20);
}
/////////////////////
