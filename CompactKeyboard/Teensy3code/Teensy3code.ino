//INITIALIZE
//populate the data from memory
#define NUMBER_OF_COLUMNS 8
#define NUMBER_OF_ROWS    6

//define the buttons and where they are
#define BUTTON_1  
#define BUTTON_3
#define BUTTON_5
#define BUTTON_7
#define BUTTON_9
#define BUTTON_0
#define BUTTON_W
#define BUTTON_E
#define BUTTON_R
#define BUTTON_T
#define BUTTON_U
#define BUTTON_I
#define BUTTON_O
#define BUTTON_
#define BUTTON_
#define BUTTON_
#define BUTTON_
#define BUTTON_
#define BUTTON_
#define BUTTON_
//reserved keys
#define BUTTON_ESC
#define BUTTON_LTAB
//modifier keys
#define BUTTON_LCTRL
#define BUTTON_LSHIFT
#define BUTTON_LALT
#define BUTTON_RCTRL
#define BUTTON_RSHIFT
#define BUTTON_RALT
//metakeys
#define BUTTON_LMETA
#define BUTTON_RMETA



byte active_column = 0;
byte actions_need_resolving = 0;

byte display_transition;

byte new_activation = 0;
byte new_deactivation = 0;

button_state
next_button_state



//CODE TO RECOGNIZE BUTTONS AND REFRESH LEDS
//this interracts with the hardware of the buttons and LEDs
scan_next_row() {
        //Sweep through to the next column
        active_column++;
        if(!(active_column < NUMBER_OF_COLUMNS)){
          active_column = 0;
        }
        
        //NOTE might need to set leds low here, so theres no crossover
    	pin_write(decoderPins[0], bitRead(active_column,0));	// Set the decoder to the current column
    	pin_write(decoderPins[1], bitRead(active_column,1));
    	pin_write(decoderPins[2], bitRead(active_column,2));
    
        //UPDATE LEDs and the new state
        //  look at each of the pin inputs, if it's high, that means a button is depressed
        //  if that's something new, record it
        //  then set LED pins
    	for (byte r=0; r<NUMBER_OF_ROWS; r++) {
                //check for a new activation
                if(pin_read(rowPins[r]) & ~bitRead(next_button_state[active_column],r){
                  bitWrite(next_button_state[active_column]),r,1) //wirte it high
                  new_activations++;
                  last_activation_time = millis(); //reset the timeout
                }
                //check for new deactivation, they only count if the delay has expired
                if(millis() - last_activation_time > BUTTON_ACTIVATION_DELAY){
                  if(~pin_read(rowPins[r]) & bitRead(next_button_state[active_column],r){
                    bitWrite(next_button_state[active_column]),r,0) //wirte it low
                    new_deactivations++;
                  }
                }
                
                //update LEDs
                analogWrite(ledRows[r],bitRead(ledDisplay[active_column]))//set LED
    	}
}

//MAIN LOOP CODE
//run the button sweeping code for the active column
//anything actionable? Pass it on to the button handler
        if(millis() - last_activation_time > BUTTON_RESOLVE_DELAY){
          //now, if anyhing has changed, we can take action
          //call the handler
        }
//any buttons to be released?
//update display matrix


//BUTTON PRESS HANDLER
//this is basically a state machine when a keypress happens this gets called
//handles single button presses, simlutaneous button presses and button holds
//works out what action the key press should take based on the current states
//these actions might be to send keystrokes via usb, change state, or change other variables
handle_activations(){
  //first handle modifier keys, these never change, and they can't be used in combos
  //and they always need to be sent first
    //found a match, send the keystroke
    //update the old state
    new_activations--;
    
  //simultaneous button presses
  if(new_activations > 1){
    //check this against the current double button presses
      //found a match, pass the appropriate key onto the key handler
      //update the old state with these buttons
      //decrease activations
  }
  
  //single button presses
  if(new_activations > 0){
    for(byte column = 0; column < NUMBER_OF_COLUMBS; columb++){
      //check this columb for changes, ie. new buttons pressed
      if(next_button_state[columb] != next_button_state[columb]){
        //now we check for presses
        if(){
          //now if we find a press, we check the map for the button it needs
        }
      }
    }
  }
  
  //single button holds

  //change states, release all pressed keys 
  display_trasition = DISPLAY_TRANSITION_TIME;
}

//BUTTON RELEASE HANDLER
handle_deactivations(){
  
}

//DISPLAY CONTROLLER
//controls what is in the led array, which is used by the scan function to update LEDs
if(display_transition > 0){
  //transition scheme
  //switch states fadeout
}
//if nothing new is happening, and there's no transitioning, then do nothing

//KEY SENDER
//press new outputs, release old ones, remember what was prssed down 
//remember what was sent last also
Keyboard.press();
Keyboard.release();
Mouse.press();
Mouse.release();
Keyboard.releaseAll();

//IDEA: REMEMBER THIS
//after activating, the next set of keys sent is remembered, and can be repeated at a single buton press



MOUSE_MIDDLE
MOUSE_RIGHT
MOUSE_LEFT

KEY_LEFT_CTRL	 
KEY_LEFT_SHIFT	 
KEY_LEFT_ALT	 
KEY_LEFT_GUI	 
KEY_RIGHT_CTRL	 
KEY_RIGHT_SHIFT	 
KEY_RIGHT_ALT	 
KEY_RIGHT_GUI
	 
KEY_UP_ARROW	 
KEY_DOWN_ARROW	 
KEY_LEFT_ARROW	 
KEY_RIGHT_ARROW	

KEY_PAGE_UP	 
KEY_PAGE_DOWN	 
KEY_HOME	 
KEY_END

KEY_BACKSPACE	
KEY_TAB	  
KEY_ESC	 
KEY_INSERT 	 
KEY_DELETE
KEY_ENTER
KEY_ESC	 
KEY_CAPS_LOCK
	
KEY_MINUS
KEY_EQUAL
KEY_LEFT_BRACE
KEY_RIGHT_BRACE
KEY_BACKSLASH
KEY_SLASH
KEY_SEMICOLON
KEY_QUOTE
KEY_TILDE
KEY_COMMA
KEY_PERIOD

KEY_SCROLL_LOCK
KEY_NUM_LOCK 
KEY_PRINTSCREEN

KEY_F1	 
KEY_F2	 
KEY_F3	 
KEY_F4	 
KEY_F5	 
KEY_F6	 
KEY_F7	 
KEY_F8	 
KEY_F9	 
KEY_F10	 
KEY_F11	 
KEY_F12	 

KEY_1
KEY_2
KEY_3
KEY_4
KEY_5
KEY_6
KEY_7
KEY_8
KEY_9
KEY_0


KEY_A
KEY_B
KEY_C
KEY_D
KEY_E
KEY_F
KEY_G
KEY_H
KEY_I
KEY_J
KEY_K
KEY_L
KEY_M
KEY_N
KEY_O
KEY_P
KEY_Q
KEY_R
KEY_S
KEY_T
KEY_U
KEY_V
KEY_W
KEY_X
KEY_Y
KEY_Z

KEYPAD_SLASH
KEYPAD_ASTERIX
KEYPAD_MINUS
KEYPAD_PLUS
KEYPAD_ENTER
KEYPAD_1
KEYPAD_2
KEYPAD_3
KEYPAD_4
KEYPAD_5
KEYPAD_6
KEYPAD_7
KEYPAD_8
KEYPAD_9
KEYPAD_0
KEYPAD_PERIOD

