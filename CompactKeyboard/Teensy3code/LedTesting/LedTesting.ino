#define NUMBER_OF_COLUMNS 8
#define NUMBER_OF_ROWS    6

byte decoderPins[3] = {10,11,12};
byte ledRows[NUMBER_OF_ROWS] = {3,4,5,22,20,21};  

byte ledDisplay[NUMBER_OF_COLUMNS][NUMBER_OF_ROWS] = {{100,255,255,100,100,100},
                                                       {100,100,255,100,100,100},
                                                       {100,100,100,100,100,100},
                                                       {100,100,255,100,100,100},
                                                       {100,100,255,100,100,100},
                                                       {100,100,255,100,100,100},
                                                       {100,100,100,100,100,100},
                                                       {100,100,255,100,100,100}};
 

byte active_column = 0;

void scan_next_row() {
        
        //Sweep through to the next column
        active_column++;
        if(!(active_column < NUMBER_OF_COLUMNS)){
          active_column = 0;
        }
        
  
        //NOTE might need to set leds low here, so theres no crossover
        for (byte r=0; r<NUMBER_OF_ROWS; r++) {
                analogWrite(ledRows[r],260);//set LED
    	}
    
    	digitalWrite(decoderPins[0], bitRead(active_column,0));	// Set the decoder to the current column
    	digitalWrite(decoderPins[1], bitRead(active_column,1));
    	digitalWrite(decoderPins[2], bitRead(active_column,2));
    
        //UPDATE LEDs and the new state
        //  look at each of the pin inputs, if it's high, that means a button is depressed
        //  if that's something new, record it
        //  then set LED pins
        
        //update LEDs
        for (byte r=0; r<NUMBER_OF_ROWS; r++) {
                analogWrite(ledRows[r],ledDisplay[active_column][r]);//set LED
    	}    
}

void setup() {
  // initialize the LED pin as an output:
  pinMode(14, INPUT);      
  pinMode(15, INPUT);   
  pinMode(16, INPUT);   
  pinMode(17, INPUT);   
  pinMode(18, INPUT);   
  pinMode(19, INPUT); 
    
  pinMode(4, OUTPUT); 
 for(byte r = 0; r < 6; r++){
   analogWriteFrequency(ledRows[r],4700000);
   pinMode(ledRows[r], OUTPUT);  
 } 
  // initialize the pushbutton pin as an input:
  for(byte r = 0; r < 3; r++){
   pinMode(decoderPins[r], OUTPUT);  
 }     
}

void loop(){
  scan_next_row();
  //for (byte r=0; r<NUMBER_OF_ROWS; r++) {
  //              analogWrite(ledRows[r],100);//set LED
  //}
}


