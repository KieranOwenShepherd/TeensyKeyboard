//HARDWARE STUFF
#define CPU_4MHz        0x02

#define NUMBER_OF_COLUMNS 8
#define NUMBER_OF_ROWS    6
#define NUMBER_OF_BUTTONS 42

#define BUTTON_ACTIVATION_DELAY 20
#define BUTTON_DEACTIVATION_DELAY 20

const byte decoderPins[3] = {
  12,11,10};
const byte ledRowPins[NUMBER_OF_ROWS] = {
  22,21,20,1,3,2};
const byte buttonRowPins[NUMBER_OF_ROWS] = {
  14,15,16,18,19,17};


//----------------------USER PREFERENCES SETTINGS---------------------------

int settings_brightness = 100;




//---------------------BUTTON TO button_number IMPLEMENTATION-------------------------------
//single buttons are given a button_number. So too are combinations of buttons, 
//which can be found with the combination of two single button_numbers
//also corresponds to the location of the active matrix

//these correspond to the physical key locations and led locations according to my mapping
#define BUTTON_ESC   35
#define BUTTON_1     36
#define BUTTON_3     37
#define BUTTON_5     38
#define BUTTON_7     39
#define BUTTON_9     40
#define BUTTON_0     41
#define BUTTON_BS    42
#define BUTTON_HRT   1

#define BUTTON_TB    27
#define BUTTON_W     28
#define BUTTON_E     29
#define BUTTON_R     30
#define BUTTON_T     31
#define BUTTON_U     32
#define BUTTON_I     33
#define BUTTON_O     34

#define BUTTON_ENT   3

#define BUTTON_A     19
#define BUTTON_S     20
#define BUTTON_D     21
#define BUTTON_F     22
#define BUTTON_G     23
#define BUTTON_H     24
#define BUTTON_J     25
#define BUTTON_L     26

#define BUTTON_LS    11
#define BUTTON_Z     12
#define BUTTON_X     13
#define BUTTON_C     14
#define BUTTON_V     15
#define BUTTON_N     16
#define BUTTON_M     17
#define BUTTON_GT    18
#define BUTTON_RS    2

#define BUTTON_LC    4
#define BUTTON_LA    5
#define BUTTON_FN    6
#define BUTTON_MT    7
#define BUTTON_SP    8
#define BUTTON_RA    9
#define BUTTON_RC    10

#define LED_R  8
#define LED_G  8
#define LED_B  8

#define BUTTON_NONE  0

#define BUTTON_FIRST BUTTON_HRT
#define BUTTON_LAST  43

#define BUTTON_REGULARKKEYS_START BUTTON_Z
#define BUTTON_REGULARKKEYS_END   43

//two keys beside each other  [_][x][x][_] 
//two keys with a gap between [_][x][_][x][_]
//two keys with a gap between [_][x][_][_][x][_]
//two keys with a gap between [_][x][_][_][_][x][_]
//a regular key and the spacebar

//this converts a buttons column and row to a button number
byte button_rowcol_to_button_number(byte column, byte row){
  byte n = row * NUMBER_OF_COLUMNS + column - 4;
  if(n < 8){
    return (n);
  }
  return (n - 1);
}

byte button_number_to_row(byte button_number){
  if(button_number < 8){
    return (button_number+ 4)/NUMBER_OF_COLUMNS ;
  }
  else{
    return (button_number+5)/NUMBER_OF_COLUMNS;
  }

}

byte button_number_to_col(byte button_number){
  if(button_number < 8){
    return (button_number+ 4)%NUMBER_OF_COLUMNS ;
  }
  else{
    return (button_number+5)%NUMBER_OF_COLUMNS;
  }
}

boolean isControlModifier(byte abutton){
  if(abutton == BUTTON_RC){
    return true;
  }
  if(abutton == BUTTON_RA){
    return true;
  }
  if(abutton == BUTTON_RS){
    return true;
  }
  if(abutton == BUTTON_LC){
    return true;
  }
  if(abutton == BUTTON_LA){
    return true;
  }
  if(abutton == BUTTON_LS){
    return true;
  }
  return false;
}

//takes two button_numbers and gives out the button_number
//returns BUTTON_NONE if the combination is invalid.
//order not important, but if the button numbers are the same, it returns it straight back
//combinations start at REGULAR_KEYS, meaning that certain buttons get banned from combinations including BUTTON_ENT BUTTON_HRT and the control modifiers
int btnCombinationToButtonNumber(byte button_1, byte button_2){
  byte button_number1;
  byte button_number2;

  if(button_1 == button_2){
    return button_1;
  }
  else if(button_1 < button_2){
    button_number1 = button_1;
    button_number2 = button_2;
  }
  else{
    button_number1 = button_2;
    button_number2 = button_1;
  }
  //buttons banned to be in combinations
  if(isControlModifier(button_1) || isControlModifier(button_2)){
    return BUTTON_NONE;
  }

  //indexing scheme!
  //start at the end of the single buttons
  int index = BUTTON_LAST;

  //is button1 the space key?
  if(button_number1 == BUTTON_SP && button_number2 >= BUTTON_REGULARKKEYS_START){
    return index + button_number2 - BUTTON_REGULARKKEYS_START;
  }
  index += BUTTON_REGULARKKEYS_END - BUTTON_REGULARKKEYS_START;

  //is it the metakey
  if(button_number1 == BUTTON_MT && button_number2 >= BUTTON_REGULARKKEYS_START){
    return index + button_number2 - BUTTON_REGULARKKEYS_START;
  }
  index += BUTTON_REGULARKKEYS_END - BUTTON_REGULARKKEYS_START;

  //if it's the function key
  if(button_number1 == BUTTON_FN && button_number2 >= BUTTON_REGULARKKEYS_START){
    return index + button_number2 - BUTTON_REGULARKKEYS_START;
  }
  index += BUTTON_REGULARKKEYS_END - BUTTON_REGULARKKEYS_START;

  //now we check for same row    
  //now, the distance between the two buttons (maximum 3)
  byte distance = button_number2-button_number1;
  byte number_of_rows = 4; //we only check 4 rows, row 0 an row 1 not included

  for(byte x = 1; x <= 3; x++){
    byte rowlength = 8-x;
    if(button_number_to_row(button_number1) == button_number_to_row(button_number2) && distance == x){
      return button_number_to_col(button_number1) + (button_number_to_row(button_number1)-2)*rowlength + index; 
    }
    index += rowlength*number_of_rows;
  }

  //now we check for same column
  //lets not implement this just now--

  //and if all else has failed return an invalid combination
  return BUTTON_NONE;
}


//given a button returns the next button in the sequence
byte button_next(byte button_number){
  return button_number+1;
}

//-------------------LED IMPLEMENTATION VARIABLES------------------------------

byte active_column = 0;
byte ledpwm = 0;
int color = 0;  //controls the three colored leds
byte ledDisplay[NUMBER_OF_COLUMNS][NUMBER_OF_ROWS] = {
  {
    0,0,0,0,0,0  }
  ,  
  {
    0,0,0,0,0,0  }
  ,
  {
    0,0,0,0,0,0  }
  ,
  {
    0,0,0,0,0,0  }
  ,
  {
    0,0,0,0,0,0  }
  ,
  {
    0,0,0,0,0,0  }
  ,
  {
    0,0,0,0,0,0  }
  ,
  {
    0,0,0,0,0,0  }
};  //the brightness of each led


//-------------------LED IMPLEMENTATION ------------------------------   

byte buttonHeatmatrix[42][42];

void setLedOn(byte which){
  ledDisplay[button_number_to_col(which)][button_number_to_row(which)] = 100;

} 
void clearLeds(){
  for(byte x = BUTTON_FIRST; x < BUTTON_LAST; x++){
    ledDisplay[button_number_to_col(x)][button_number_to_row(x)] = 0;
  }
}                
byte getButtonHeat(byte button, byte layout){
  return buttonHeatmatrix[button][layout];
}


//-----------------KEYSTROKES KEYBOARD SERIAL COMMUNICATION-----------------
//it's convenient to be able to store and send common combinations of keys (ctrl+a etc)

//contains up to 4 simultaneous keypresses; up to 1 regular key and up to three control modifiers
typedef struct KeyComb{
  int regularkey;
  byte controlmodflags;
} 
KeyComb;

KeyComb last_keycombination_sent;
byte    current_active_control_modifiers = 0;

//send a combination of keystrokes
#define CMF_LEFT_SHIFT    (1 << 0)  // 000001
#define CMF_LEFT_ALT      (1 << 1)  // 000010
#define CMF_LEFT_CTRL     (1 << 2)  // 000100
#define CMF_RIGHT_SHIFT   (1 << 3)  // 001000
#define CMF_RIGHT_ALT     (1 << 4)  // 010000
#define CMF_RIGHT_CTRL    (1 << 5)  // 100000

void sendKeystrokeCombination(struct KeyComb *keystosend){
  //hit the control modifiers
  int flags = keystosend->controlmodflags;
  if(flags != 0){
    Keyboard.releaseAll();
    if((flags & CMF_LEFT_SHIFT) != 0){
      Keyboard.press(KEY_LEFT_SHIFT);
    }
    if((flags & CMF_LEFT_CTRL) != 0){
      Keyboard.press(KEY_LEFT_CTRL);
    }
    if((flags & CMF_LEFT_ALT) != 0){
      Keyboard.press(KEY_LEFT_ALT);
    }
    if((flags & CMF_RIGHT_SHIFT) != 0){
      Keyboard.press(KEY_RIGHT_SHIFT);
    }
    if((flags & CMF_RIGHT_CTRL) != 0){
      Keyboard.press(KEY_RIGHT_CTRL);
    }
    if((flags & CMF_RIGHT_ALT) != 0){
      Keyboard.press(KEY_RIGHT_ALT);
    }
    
  }
  //hit the regular key
  Keyboard.press(keystosend->regularkey);

  //now, remember what we've just sent as long as it wasnt too complex
  if(keystosend->controlmodflags == 0){
    //remember active control modifiers
    last_keycombination_sent.controlmodflags = current_active_control_modifiers;
    last_keycombination_sent.regularkey = keystosend->regularkey;
    if(keystosend->regularkey == KEY_LEFT_CTRL){
      current_active_control_modifiers |= CMF_LEFT_CTRL;
    }
    if(keystosend->regularkey == KEY_LEFT_ALT){
      current_active_control_modifiers |= CMF_LEFT_ALT;
    }
    if(keystosend->regularkey == KEY_LEFT_SHIFT){
      current_active_control_modifiers |= CMF_LEFT_SHIFT;
    }
    if(keystosend->regularkey == KEY_RIGHT_CTRL){
      current_active_control_modifiers |= CMF_RIGHT_CTRL;
    }
    if(keystosend->regularkey == KEY_RIGHT_ALT){
      current_active_control_modifiers |= CMF_RIGHT_ALT;
    }
    if(keystosend->regularkey == KEY_RIGHT_SHIFT){
      current_active_control_modifiers |= CMF_RIGHT_SHIFT;
    }
  }
}

void releaseKeystrokeCombination(struct KeyComb *keystorelease){
  //release the regular key
  Keyboard.release(keystorelease->regularkey);
  //I shouldn't need to release others, they'll be figured out just before other keys, or on all release
  int flags = keystorelease->controlmodflags;
  if(flags != 0){
    Keyboard.releaseAll();
  }
  
  if(keystorelease->regularkey == KEY_LEFT_CTRL){
    current_active_control_modifiers &= !CMF_LEFT_CTRL;
  }
  if(keystorelease->regularkey == KEY_LEFT_ALT){
    current_active_control_modifiers &= !CMF_LEFT_ALT;
  }
  if(keystorelease->regularkey == KEY_LEFT_SHIFT){
    current_active_control_modifiers &= !CMF_LEFT_SHIFT;
  }
  if(keystorelease->regularkey == KEY_RIGHT_CTRL){
    current_active_control_modifiers &= !CMF_RIGHT_CTRL;
  }
  if(keystorelease->regularkey == KEY_RIGHT_ALT){
    current_active_control_modifiers &= !CMF_RIGHT_ALT;
  }
  if(keystorelease->regularkey == KEY_RIGHT_SHIFT){
    current_active_control_modifiers &= !CMF_RIGHT_SHIFT;
  }
}


//----------------------KEY LAYOUTS IMPLEMENTATION-------------------------------

#define NUMBER_OF_KEYASSIGNMENTS         200
#define NUMBER_OF_ACTIVE_KEYSTROKES      256

#define NO_LAYOUT                        0

typedef struct KeyAssignment{
  KeyComb keycombination; //the keys to send
  byte    button; //the button, which when pressed results in the keys being sent
  byte    layout; //which layout this is a part of, if it is non valid, this is 0
} 
KeyAssignment;

//All the stored layouts
KeyComb default_layout[256];
KeyAssignment keyassignments[NUMBER_OF_KEYASSIGNMENTS];   //stores all the key assignments, in no specific order

KeyComb*      active_keystrokes[NUMBER_OF_ACTIVE_KEYSTROKES]; //stores the keystrokes corresponding to the buttons, for quick access

byte          latest_layout;

boolean recordKeyAssignment(struct KeyAssignment *ka){
  //find the first empty key assignments
  //while we traverse through, if I find an alternate, overwrite it
  for(int x = 0; x < NUMBER_OF_KEYASSIGNMENTS; x++){
    if(keyassignments[x].layout == 0 || (keyassignments[x].layout == ka->layout && keyassignments[x].button == ka->button)){
      //record here
      keyassignments[x] = *ka;
      return true;
    }
  }
  return false;
}

//loads a layout into the quick access keystrokes
void loadlayout(byte layout_midbutton){

  //first clear the current layout
  for(int x = 0; x < 256; x++){
    active_keystrokes[x] = NULL;
  }
  //second load up the new layout
  if(layout_midbutton == 0){
    //the default layout is a special case
    for(int x = 0; x < 256; x++){
      if(default_layout[x].regularkey != 0){
        active_keystrokes[x] = &default_layout[x];
      }
    }
  }
  else{//custom layouts
    for(byte x = 0; x < NUMBER_OF_KEYASSIGNMENTS; x++){
      if(keyassignments[x].layout == layout_midbutton){
        active_keystrokes[keyassignments[x].button] = &keyassignments[x].keycombination;
      }
    }
  }

  //set leds
  clearLeds();
  for(byte x = BUTTON_FIRST; x < BUTTON_LAST; x++){
    if(active_keystrokes[x] != NULL){
      setLedOn(x);
    }
  }

} 


void loaddefaultlayout(){
  loadlayout(0);
}

void changeLayout(byte new_layout){
  latest_layout = new_layout;
  loadlayout(new_layout);
  //set led stuff here maybe?
}

//defines the default layout, and loads other layouts from memory
//call this in initialisation
void layouts_init(){
  //clear everything initially
  for(int x = 0; x < 256; x++){
    default_layout[x].regularkey = 0;
  }
  for(int x = 0; x < NUMBER_OF_KEYASSIGNMENTS; x++){
    keyassignments[x].layout = 0;
  }

  default_layout[BUTTON_ESC].regularkey = KEY_ESC;
  default_layout[BUTTON_1].regularkey = KEY_1;
  default_layout[BUTTON_3].regularkey = KEY_3;
  default_layout[BUTTON_5].regularkey = KEY_5;
  default_layout[BUTTON_7].regularkey = KEY_7;
  default_layout[BUTTON_9].regularkey = KEY_9;
  default_layout[BUTTON_0].regularkey = KEY_0;
  default_layout[BUTTON_BS].regularkey = KEY_BACKSPACE;
  default_layout[BUTTON_HRT].regularkey = KEY_DELETE;

  default_layout[BUTTON_TB].regularkey = KEY_TAB;
  default_layout[BUTTON_W].regularkey = KEY_W;
  default_layout[BUTTON_E].regularkey = KEY_E;
  default_layout[BUTTON_R].regularkey = KEY_R;
  default_layout[BUTTON_T].regularkey = KEY_T;
  default_layout[BUTTON_U].regularkey = KEY_U;
  default_layout[BUTTON_I].regularkey = KEY_I;
  default_layout[BUTTON_O].regularkey = KEY_O;

  default_layout[BUTTON_ENT].regularkey = KEY_ENTER;

  default_layout[BUTTON_A].regularkey = KEY_A;
  default_layout[BUTTON_S].regularkey = KEY_S;
  default_layout[BUTTON_D].regularkey = KEY_D;
  default_layout[BUTTON_F].regularkey = KEY_F;
  default_layout[BUTTON_G].regularkey = KEY_G;
  default_layout[BUTTON_H].regularkey = KEY_H;
  default_layout[BUTTON_J].regularkey = KEY_J;
  default_layout[BUTTON_L].regularkey = KEY_L;

  default_layout[BUTTON_LS].regularkey = KEY_LEFT_SHIFT;
  default_layout[BUTTON_Z].regularkey = KEY_Z;
  default_layout[BUTTON_X].regularkey = KEY_X;
  default_layout[BUTTON_C].regularkey = KEY_C;
  default_layout[BUTTON_V].regularkey = KEY_V;
  default_layout[BUTTON_N].regularkey = KEY_N;
  default_layout[BUTTON_M].regularkey = KEY_M;
  default_layout[BUTTON_GT].regularkey = KEY_PERIOD;
  default_layout[BUTTON_RS].regularkey = KEY_RIGHT_SHIFT;

  default_layout[BUTTON_LC].regularkey = KEY_LEFT_CTRL;
  default_layout[BUTTON_LA].regularkey = KEY_LEFT_ALT;
  //default_layout[BUTTON_FN].regularkey = KEY;
  //default_layout[BUTTON_MT].regularkey = KEY;
  default_layout[BUTTON_SP].regularkey = KEY_SPACE;
  default_layout[BUTTON_RA].regularkey = KEY_RIGHT_ALT;
  default_layout[BUTTON_RC].regularkey = KEY_RIGHT_CTRL;

  default_layout[btnCombinationToButtonNumber(BUTTON_V,BUTTON_N)].regularkey = KEY_B;
  default_layout[btnCombinationToButtonNumber(BUTTON_C,BUTTON_V)].regularkey = KEY_B;
  default_layout[btnCombinationToButtonNumber(BUTTON_J,BUTTON_L)].regularkey = KEY_K;
  default_layout[btnCombinationToButtonNumber(BUTTON_H,BUTTON_J)].regularkey = KEY_K;
  default_layout[btnCombinationToButtonNumber(BUTTON_U,BUTTON_O)].regularkey = KEY_P;
  default_layout[btnCombinationToButtonNumber(BUTTON_I,BUTTON_O)].regularkey = KEY_P;
  default_layout[btnCombinationToButtonNumber(BUTTON_I,BUTTON_ENT)].regularkey = KEY_P;
  default_layout[btnCombinationToButtonNumber(BUTTON_TB,BUTTON_W)].regularkey = KEY_Q;
  default_layout[btnCombinationToButtonNumber(BUTTON_W,BUTTON_E)].regularkey = KEY_Q;
  default_layout[btnCombinationToButtonNumber(BUTTON_T,BUTTON_U)].regularkey = KEY_Y;
  default_layout[btnCombinationToButtonNumber(BUTTON_R,BUTTON_T)].regularkey = KEY_Y;

  default_layout[btnCombinationToButtonNumber(BUTTON_1,BUTTON_3)].regularkey = KEY_2;
  default_layout[btnCombinationToButtonNumber(BUTTON_3,BUTTON_5)].regularkey = KEY_4;
  default_layout[btnCombinationToButtonNumber(BUTTON_5,BUTTON_7)].regularkey = KEY_6;
  default_layout[btnCombinationToButtonNumber(BUTTON_7,BUTTON_9)].regularkey = KEY_8;

  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_W)].regularkey = KEY_UP_ARROW;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_S)].regularkey = KEY_DOWN_ARROW;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_A)].regularkey = KEY_LEFT_ARROW;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_D)].regularkey = KEY_RIGHT_ARROW;

  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_W)].regularkey = KEY_PAGE_UP;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_S)].regularkey = KEY_PAGE_DOWN;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_A)].regularkey = KEY_HOME;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_D)].regularkey = KEY_END;

  //Punctuation
  default_layout[btnCombinationToButtonNumber(BUTTON_M,BUTTON_GT)].regularkey = KEY_COMMA;
  //shortcut for questionmark
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_GT)].regularkey = KEY_SLASH;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_GT)].controlmodflags = CMF_LEFT_SHIFT;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_GT)].regularkey = KEY_COMMA;
  
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_M)].regularkey = KEY_COMMA;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_M)].regularkey = KEY_COMMA;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_N)].regularkey = KEY_SLASH;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_N)].regularkey = KEY_SLASH;
  
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_U)].regularkey = KEY_BACKSLASH;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_U)].regularkey = KEY_BACKSLASH;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_ENT)].regularkey = KEY_BACKSLASH;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_ENT)].regularkey = KEY_BACKSLASH;
 

  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_H)].regularkey = KEY_QUOTE;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_H)].regularkey = KEY_QUOTE;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_G)].regularkey = KEY_SEMICOLON;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_G)].regularkey = KEY_SEMICOLON;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_J)].regularkey = KEY_LEFT_BRACE;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_J)].regularkey = KEY_LEFT_BRACE;  
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_L)].regularkey = KEY_RIGHT_BRACE;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_L)].regularkey = KEY_RIGHT_BRACE;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_I)].regularkey = KEY_MINUS;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_I)].regularkey = KEY_MINUS;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_O)].regularkey = KEY_EQUAL;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_O)].regularkey = KEY_EQUAL;

  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_1)].regularkey = KEY_F1;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_3)].regularkey = KEY_F3;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_5)].regularkey = KEY_F5;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_7)].regularkey = KEY_F7;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_9)].regularkey = KEY_F9;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_0)].regularkey = KEY_F11;

  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_1)].regularkey = KEY_F2;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_3)].regularkey = KEY_F4;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_5)].regularkey = KEY_F6;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_7)].regularkey = KEY_F8;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_9)].regularkey = KEY_F10;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_0)].regularkey = KEY_F12;

  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_TB)].regularkey = KEY_TILDE;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_TB)].regularkey = KEY_TILDE;
  default_layout[btnCombinationToButtonNumber(BUTTON_FN,BUTTON_ESC)].regularkey = KEY_TILDE;
  default_layout[btnCombinationToButtonNumber(BUTTON_MT,BUTTON_ESC)].regularkey = KEY_TILDE;

  /*
KEY_INSERT 	 
   KEY_CAPS_LOCK
   KEY_SCROLL_LOCK
   KEY_NUM_LOCK 
   KEY_PRINTSCREEN	
   
   MOUSE_MIDDLE
   MOUSE_RIGHT
   MOUSE_LEFT
   
   KEY_LEFT_GUI	 	 
   KEY_RIGHT_GUI 
   
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
   KEYPAD_PERIOD*/

}


//-------------------------------STATE MACHINE-----------------------------------------


//STATE MACHINE VARIABLES
enum State {
  CUSTOM_LAYOUT,
  CHOOSE_RECORD_LOCATION,
  DEFAULT_LAYOUT,
  TEMP_DEFAULT_LAYOUT_CM,
  TEMP_DEFAULT_LAYOUT_1S,
  SYSTEM,
  BUTTON_NUMBER //a neat way to reassign buttons if i need to
};

State previous_state = DEFAULT_LAYOUT;
State state = DEFAULT_LAYOUT;

void changeState(byte newstate){
  previous_state = state;
  Keyboard.releaseAll();
  state = (State) newstate;
}

//--------EVENTS----------
//these events return true if the event was handled;

boolean subevent_sytsembuttonpress(byte button_number){
  switch(button_number)
  {
  case BUTTON_9:
    settings_brightness -= 10;
    return true;
  case BUTTON_0:
    settings_brightness += 10;
    return true;
  default:
    return false;
  }
}

boolean event_buttonpress(byte button_number){
  KeyComb* keycombination;
  KeyAssignment ka;
  if(button_number == BUTTON_NONE){
    return false;
  }

  switch(state)
  {
  case DEFAULT_LAYOUT:
    if(button_number == btnCombinationToButtonNumber(BUTTON_MT,BUTTON_FN)){
      //go into system mode
      changeState(SYSTEM);
      return true;
    }
    break;
  case CUSTOM_LAYOUT:

    //if it's one of the control modifiers, go to the temp default state
    if(isControlModifier(button_number)){
      changeState(TEMP_DEFAULT_LAYOUT_CM);
      loaddefaultlayout();
      ///need to actually activate the key still, so fall through
    }

    if(button_number == btnCombinationToButtonNumber(BUTTON_MT,BUTTON_FN)){
      //go into system mode
      changeState(SYSTEM);
      return true;
    }

    if(button_number == btnCombinationToButtonNumber(BUTTON_FN,BUTTON_SP)){
      //go into system mode
      changeState(CHOOSE_RECORD_LOCATION);
      return true;
    }
  case TEMP_DEFAULT_LAYOUT_CM:
    break;

  case SYSTEM:
    // we do something totally different depending on the button
    return subevent_sytsembuttonpress(button_number);
  case BUTTON_NUMBER:
    Keyboard.print(button_number);
    return true;    
  case CHOOSE_RECORD_LOCATION:
    if(button_number == BUTTON_ESC){
      changeState(CUSTOM_LAYOUT);
      return true;
    }
    ka = {
      last_keycombination_sent,button_number,latest_layout    };
    recordKeyAssignment(&ka);
    changeLayout(latest_layout);
    changeState(CUSTOM_LAYOUT);

    return true;
  default:
    return false;
  }
  //falling though to here means that we should send a keystroke
  keycombination = active_keystrokes[button_number];
  if(keycombination == NULL){ //test if it does something in this layout
    return false;
  }
  sendKeystrokeCombination(keycombination);
  return true;
}

boolean event_bufferedButtonPress(int button_number){
  KeyComb* keycombination; 
  if(button_number == BUTTON_NONE ){
    return false;
  } 

  switch(state)
  {
  case CUSTOM_LAYOUT:
  case DEFAULT_LAYOUT:
    if(button_number == btnCombinationToButtonNumber(BUTTON_MT,BUTTON_FN)){
      //go into system mode
      changeState(SYSTEM);
      return true;
    }
    if(button_number > btnCombinationToButtonNumber(BUTTON_Z,BUTTON_X)){
      return false;
    }
    break;
  default:
    return false;
  }

  keycombination = active_keystrokes[button_number];
  if(keycombination != NULL){
    //now, don't buffer if they're in the same row, this prevents rolling issues
    //if(button_number > row keys){
    sendKeystrokeCombination(keycombination);
    //}
    return true;
  }

  return false;

}

boolean event_btnallrelease(){
  switch(state)
  {
  case CUSTOM_LAYOUT:
    Keyboard.releaseAll();
    return true; 
  case SYSTEM:
    //return to the previous mode
    return true;
  default:
    return false;
  }
}

//a single button or button combination is released
boolean event_buttonrelease(byte button_number){

  if(button_number == BUTTON_NONE){
    return false;
  }

  KeyComb* keycombination;
  switch(state)
  {
  case CUSTOM_LAYOUT:
    break;
  case DEFAULT_LAYOUT:
    break;
  case SYSTEM:
    if(button_number == BUTTON_MT || button_number == BUTTON_FN){
      //changeState(previous_state);
    }

  case TEMP_DEFAULT_LAYOUT_CM:
    if(isControlModifier(button_number)){
      changeState(previous_state);
      changeLayout(latest_layout);
    }
    break;
  default:
    return false;
  }

  keycombination = active_keystrokes[button_number];

  if(keycombination == NULL){
    return false;
  }

  //now, release!
  releaseKeystrokeCombination(keycombination);
  return true;
}

//---------------------BUTTON IMPLEMENTATION VARIABLES--------------------------

byte last_button_state[NUMBER_OF_COLUMNS] = {
  0,0,0,0,0,0,0,0};
byte next_button_state[NUMBER_OF_COLUMNS] = {
  0,0,0,0,0,0,0,0};

byte new_activations = 0;
byte new_deactivations = 0;
int currently_active = 0;
unsigned long last_activation_time = 0;
unsigned long last_deactivation_time = 0;

//-----------------------------BUTTON AND LED CONTROLLERS--------------------------------------

byte columnorderhack(byte activecolumn){
  switch(activecolumn){
  case 0:
    return 1;
  case 1:
    return 3;
  case 2:
    return 2;
  case 3:
    return 6;
  case 4:
    return 0;
  case 5:
    return 4;
  case 6:
    return 5;
  case 7:
    return 7;
  default:
    return 0;
  }
}

void scan_next_row() { 
  //set leds low here, so theres no crossover

  //Sweep through to the next column
  active_column++;
  if(!(active_column < NUMBER_OF_COLUMNS)){
    active_column = 0;
  }

  digitalWrite(decoderPins[0], bitRead(columnorderhack(active_column),0));	// Set the decoder to the current column
  digitalWrite(decoderPins[1], bitRead(columnorderhack(active_column),1));
  digitalWrite(decoderPins[2], bitRead(columnorderhack(active_column),2));

  for(byte r=0; r<NUMBER_OF_ROWS; r++) {
    //REHIGH LEDs (that arent 0)
    if(ledDisplay[active_column][r] > 0){
      digitalWrite(ledRowPins[r],LOW);
    }
  }

  //READ BUTTONS
  //  look at each of the pin inputs, if it's high, that means a button is depressed
  //  if that's something new, record it        
  for (byte r=0; r<NUMBER_OF_ROWS; r++) {
    //if the next state is already different (unresolved) do nothing
    if(bitRead(last_button_state[active_column],r) == bitRead(next_button_state[active_column],r)){
      bitWrite(next_button_state[active_column],r, digitalRead(buttonRowPins[r])); //write the new bit
      if(bitRead(last_button_state[active_column],r) != bitRead(next_button_state[active_column],r)){ //now are they different?
        if(bitRead(next_button_state[active_column],r) == HIGH){
          new_activations++;
          currently_active++;
          last_activation_time = millis(); //reset the timeout
        }
        else{  
          new_deactivations++;
          currently_active--;
          last_deactivation_time = millis();
        }
      }
    } 
  } 
}

//LAST BUTTON STATE ONLY UPDATED here, discerns button states and calls the related event handlers
void handle_activations(){
  //check for and handle control modifier keys
  //look for 3 button combination presses to change states
  if(new_activations >= 3){
    //check for default
    if(bitRead(next_button_state[2] & next_button_state[3] & next_button_state[5],1)==HIGH){
      loaddefaultlayout();
      changeState(DEFAULT_LAYOUT);
      for(byte c = 0; c < NUMBER_OF_COLUMNS; c++){       
        last_button_state[c] = last_button_state[c] | next_button_state[c];
      }
      new_activations = 0;
      return;
    }
    //okay, now, check for 3 consecutive ones on the same row
    for(byte cx = 0; cx < 6; cx++){
      byte testing = next_button_state[cx] & next_button_state[cx+1] & next_button_state[cx+2]; //this is sluggish but oh well
      for(byte r = 2; r < NUMBER_OF_ROWS-1; r++){ //forget row 5 and row 6
        if(bitRead(testing,r) == HIGH){
          //found a triple!
          //switch layout based on what was pressed  
          byte layout = button_rowcol_to_button_number(cx+1,r);  
          changeLayout(layout);
          changeState(CUSTOM_LAYOUT);
          //now forget any other buttons that were activated at the same time 
          for(byte c = 0; c < NUMBER_OF_COLUMNS; c++){       
            last_button_state[c] = last_button_state[c] | next_button_state[c];
          }
          new_activations = 0;
          return;
        }
      } 
    }
  }

  //now look for 2 button combination presses  
  for(byte b = BUTTON_FIRST; b < BUTTON_LAST; b = button_next(b)){   //find the activation
    byte c = button_number_to_col(b);
    byte r = button_number_to_row(b);
    if(bitRead(last_button_state[c],r) == LOW && bitRead(next_button_state[c],r) == HIGH){
      boolean handled = false;   

      //check for combinations - only need to look forward, because backward ones should already be handled
      for(byte cb = b+1; cb < BUTTON_LAST && !handled; cb = button_next(cb)){
        byte check_c = button_number_to_col(cb);
        byte check_r = button_number_to_row(cb);
        if(bitRead(next_button_state[check_c],check_r) == HIGH && bitRead(last_button_state[check_c],check_r) == LOW){
          //potential candidate
          //Keyboard.print("active");
          handled = event_buttonpress(btnCombinationToButtonNumber(b, cb));
          //dont handle this guy later
          if(handled){
            bitSet(last_button_state[check_c],check_r);
            new_activations--;
          }
        }
      }

      //Check though the whole set for previous activated (for buffering effect)
      for(byte cb = BUTTON_FIRST; cb < BUTTON_LAST && !handled; cb = button_next(cb)){
        byte check_c = button_number_to_col(cb);
        byte check_r = button_number_to_row(cb);
        if(bitRead(last_button_state[check_c],check_r) == HIGH){
          handled = event_bufferedButtonPress(btnCombinationToButtonNumber(cb,b));
        }
      }

      //kay, doubles didn't work out, what about singles?
      if(!handled){
        handled = event_buttonpress(button_rowcol_to_button_number(c,r));
      }

      //now, if it's not handled here, something went wrong
      if(handled){
      }

      //check this one off our list
      bitSet(last_button_state[c],r);
      new_activations--;
    }
  }  

  //guarantee the count zero, this is sorta a hack
  new_activations = 0;
}

//calls the event handlers for button releases and multi button releases
void handle_deactivations(){
  //first if all buttons will be deactivated, I call the all release function
  /*if(currently_active <= 0){
   event_btnallrelease();
   new_deactivations = 0;
   return;
   }*/

  //now start releasing any combination with active buttons
  for(byte btn = BUTTON_FIRST; btn < BUTTON_LAST; btn = button_next(btn)){ //find the deactivation
    if(bitRead(last_button_state[button_number_to_col(btn)],button_number_to_row(btn)) == HIGH && bitRead(next_button_state[button_number_to_col(btn)],button_number_to_row(btn)) == LOW){

      //found a deactivation! 
      //now deactivate other combinations, need to check forward and backward
      for(byte checkbtn = BUTTON_FIRST; checkbtn < BUTTON_LAST; checkbtn = button_next(checkbtn)){
        if(bitRead(last_button_state[button_number_to_col(checkbtn)],button_number_to_row(checkbtn)) == HIGH){
          //a potential deactivation candidate
          event_buttonrelease(btnCombinationToButtonNumber(btn,checkbtn));
        }
      }

      //then deactivate this single button press
      event_buttonrelease(btn);
      bitClear(last_button_state[button_number_to_col(btn)],button_number_to_row(btn)); 
    } 
  } 
  new_deactivations = 0;
}

//-------------------------------MAIN LOOP----------------------------------------

void setup() {

  for(byte r = 0; r < NUMBER_OF_ROWS; r++){
    //led as output pins
    //analogWriteFrequency(ledRowPins[r],187500);
    pinMode(ledRowPins[r], OUTPUT);
    digitalWrite(ledRowPins[r],HIGH);

    // initialize the pushbutton pin as an input:       
    pinMode(buttonRowPins[r], INPUT);
  } 

  for(byte r = 0; r < 3; r++){
    pinMode(decoderPins[r], OUTPUT);  
  }  

  layouts_init();

  //set the beginning state
  loaddefaultlayout();

  Keyboard.begin();

}

unsigned long currentmicros = 0;
unsigned long led_pwmtimer = 0;

void loop(){
  currentmicros = micros();
  if(currentmicros - led_pwmtimer > 2048){
    led_pwmtimer = currentmicros;
    scan_next_row();
  }
  //second half of the pwm prcedure, turn off timed out leds
  for(byte r = 0; r < NUMBER_OF_ROWS; r++){
    if(currentmicros - led_pwmtimer  > ledDisplay[active_column][r])
    {
      digitalWrite(ledRowPins[r],HIGH);
    }
  }

  if(millis() - last_activation_time > BUTTON_ACTIVATION_DELAY && new_activations > 0){
    handle_activations(); 
  }

  if(millis() - last_deactivation_time > BUTTON_DEACTIVATION_DELAY && new_deactivations > 0){
    handle_deactivations();  
  }

  //handle events?

}




