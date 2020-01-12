////////////////////////////////////////////
//    3.5" TOUCH SCREEN Bass & Drum       //
//              Machine                   //
//                                        //
//            rom3 08.2019                //
//                                        //
//                                        //
////////////////////////////////////////////
#include <SPI.h>
#include <TFT_HX8357_Due.h>
#include <Scheduler.h>
#include "Button.h"
#include "Key.h"
#include "TouchScreen_kbv.h"

#include <SdFat.h>
SdFatSoftSpi<12, 11, 13> SD; //Bit-Bang on the Shield pins
#define CS_PIN 10

#define VS_XCS    46 // Control Chip Select Pin (for accessing SPI Control/Status registers)
#define VS_XDCS   48 // Data Chip Select / BSYNC Pin
#define VS_DREQ   50 // Data Request Pin: Player asks for more data
#define VS_RESET  53 // Reset is active low

#define DRAW(Colour) if (tool ? color = TFT_BLUE : color = Colour);tft.fillRect(xx + 3, yy + 4, 5 , 7, color);
#define GetTouchPoints tp=myTouch.getPoint();x=map(tp.x,TS_MINX,TS_MAXX,480,0);y=map(tp.y,TS_MINY,TS_MAXY,320,0);
#define WaitTouch do{tp=myTouch.getPoint();}while(tp.z<MINPRESSURE);
#define Format(space) if(space<100&&space>=-9)tft.print(" ");if(space<10&&space>=0)tft.print(" ");
#define DrawValue(value,height) tft.fillRect(321,height,value,6,TFT_GREEN);tft.fillRect(321+value,height,127-value,6,TFT_BLUE);

TFT_HX8357_Due tft = TFT_HX8357_Due();

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

int TS_MINX = 890;
int TS_MAXX = 90;
int TS_MINY = 890;
int TS_MAXY = 214;
#define MINPRESSURE 35

TouchScreen_kbv myTouch = TouchScreen_kbv(XP, YP, XM, YM, 300);
TSPoint_kbv tp;

File file;

#define buttonColor TFT_RED
#define buttonTouched TFT_BLACK
int touched;

const int CLK1 = 22;  // CLK PIN rotary encoder
const int rotButton1 = 23;
const int rotButton2 = 25;
const int rotButton3 = 27;
const int DT1 = 24;  // DT PIN rotary encoder
const int CLK2 = 26; 
const int DT2 = 28;
const int CLK3 = 30; 
const int DT3 = 32;
int lastPosition1, currentPosition1;
int lastPosition2, currentPosition2;
int lastPosition3, currentPosition3;
int reverb1 =12 ,reverb2 = 12,reverb3 = 12;
int pan1=64,pan2=64,pan3=64;
int potRead1,potRead2,potRead3,val;
bool pressed = false;
int rotMode1,rotMode2,rotMode3;

int rX[8], rY[8], col;
unsigned short int x, y, xx, yy, tempo = 120, shift, inst, Vol1 = 100, Vol2= 100, Vol3 = 100, buff, soundPatch, leadPatch, offStep;
unsigned short pattern[300];
unsigned short instrument[13][16];
unsigned short bassNoteLen[16][16];
unsigned long  bassNoteOff[16][16];
unsigned short channel, bank, note;
unsigned short patch[] = {35, 38, 44, 42, 43, 48, 47, 49, 56, 60, 61, 83};
unsigned short bass[] = {35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,0};
unsigned short slope, slope2, pat = 0, nextPat = 0, copyTo, color, color1, pos, posOld, posx, posy;
unsigned short mode = 0, xxOld = 0, yyOld = 0, yyy;
short tick = -1,noteLen;
bool play = false, copyPat = false, tool, testIt = false, tst = true;
unsigned long currTime, prevTime, currentMillis, previousMillis = 0, curMillis, prevMillis = 0;
const long interval = 250;
float px, py;
int32_t clx, crx, cty, cby;

String drumSounds[] = {"High Q               ","Slap                 ","Scratch Push[EXC 7]  ","Scratch Pull [EXC 7] ","Sticks               ","Square Click         ","Metronome Click      ", 
                       "Metronome Bell       ","Acoustic Bass Drum   ","Bass Drum            ","Side Stick           ","Acoustic Snare       ","Hand Clap            ","Electric Snare       ", 
                       "Low Floor Tom        ","Closed Hi-hat [EXC 1]","High Floor Tom       ","Pedal Hi-hat [EXC 1] ","Low Tom              ","Open Hi-hat [EXC 1]  ","Low-Mid Tom          ", 
                       "High Mid Tom         ","Crash Cymbal 1       ","High Tom             ","Ride Cymbal 1        ","Chinese Cymbal       ","Ride Bell            ","Tambourine           ", 
                       "Splash Cymbal        ","Cowbell              ","Crash Cymbal 2       ","Vibra-slap           ","Ride Cymbal 2        ","High Bongo           ","Low Bongo            ",
                       "Mute Hi Conga        ","Open Hi Conga        ","Low Conga            ","High Timbale         ","Low Timbale          ","High Agogo           ","Low Agogo            ", 
                       "Cabasa               ","Maracas              ","Short Whistle [EXC 2]","Long Whistle [EXC 2] ","Short Guiro [EXC 3]  ","Long Guiro [EXC 3]   ","Claves               ", 
                       "Hi Wood Block        ","Low Wood Block       ","Mute Cuica [EXC 4]   ","Open Cuica [EXC 4]   ","Mute Triangle [EXC 5]","Open Triangle [EXC 5]","Shaker               ", 
                       "Jingle bell          ","Bell tree            ","Castanets            ","Mute Surdo [EXC 6]   ","Open Surdo [EXC 6]   ", ""};
                       
String Sounds[] = {"Acoustic Grand Piano ","Bright Acoustic Piano","Electric Grand Piano ","Honky-tonk Piano     ","Electric Piano 1     ","Electric Piano 2     ","Harpsichord          ",
                   "Clavi                ","Celesta              ","Glockenspiel         ","Music Box            ","Vibraphone           ","Marimba              ","Xylophone            ",
                   "Tubular Bells        ","Dulcimer             ","Drawbar Organ        ","Percussive Organ     ","Rock Organ           ","Church Organ         ","Reed Organ           ",
                   "Accordion            ","Harmonica            ","Tango Accordion      ","AcousticGuitar(nylon)","AcousticGuitar(steel)","ElectricGuitar(jazz) ","ElectricGuitar(clean)",
                   "ElectricGuitar(muted)","Overdriven Guitar    ","Distortion Guitar    ","Guitar Harmonics     ","Acoustic Bass        ","Electric Bass(finger)","Electric Bass(pick)  ",
                   "Fretless Bass        ","Slap Bass 1          ","Slap Bass 2          ","Synth Bass 1         ","Synth Bass 2         ","Violin               ","Viola                ",
                   "Cello                ","Contrabass           ","Tremolo Strings      ","Pizzicato Strings    ","Orchestral Harp      ","Timpani              ","String Ensembles 1   ",
                   "String Ensembles 2   ","Synth Strings 1      ","Synth Strings 2      ","Choir Aahs           ","Voice Oohs           ","Synth Voice          ","Orchestra Hit        ",
                   "Trumpet              ","Trombone             ","Tuba                 ","Muted Trumpet        ","French Horn          ","Brass Section        ","Synth Brass 1        ",
                   "Synth Brass 2        ","Soprano Sax          ","Alto Sax             ","Tenor Sax            ","Baritone Sax         ","Oboe                 ","English Horn         ",
                   "Bassoon              ","Clarinet             ","Piccolo              ","Flute                ","Recorder             ","Pan Flute            ","Blown Bottle         ",
                   "Shakuhachi           ","Whistle              ","Ocarina              ","Square Lead (Lead 1) ","Saw Lead (Lead)      ","Calliope Lead(Lead 3)","Chiff Lead (Lead 4)  ",
                   "Charang Lead (Lead 5)","Voice Lead (Lead 6)  ","Fifths Lead (Lead 7) ","Bass + Lead (Lead 8) ","New Age (Pad 1)      ","Warm Pad (Pad 2)     ","Polysynth (Pad 3)    ",
                   "Choir (Pad 4)        ","Bowed (Pad 5)        ","Metallic (Pad 6)     ","Halo (Pad 7)         ","Sweep (Pad 8)        ","Rain (FX 1)          ","Sound Track (FX 2)   ",
                   "Crystal (FX 3)       ","Atmosphere (FX 4)    ","Brightness (FX 5)    ","Goblins (FX 6)       ","Echoes (FX 7)        ","Sci-fi (FX 8)        ","Sitar                ",
                   "Banjo                ","Shamisen             ","Koto                 ","Kalimba              ","Bag Pipe             ","Fiddle               ","Shanai               ",
                   "Tinkle Bell          ","Agogo                ","Pitched Percussion   ","Woodblock            ","Taiko Drum           ","Melodic Tom          ","Synth Drum           ",
                   "Reverse Cymbal       ","Guitar Fret Noise    ","Breath Noise         ","Seashore             ","Bird Tweet           ","Telephone Ring       ","Helicopter           ",
                   "Applause             ","Gunshot              "};

Button StartStopButton  = Button(356, 280, 80, 35, "Start/Stop");
Button Rewind           = Button(320, 280, 30, 35, "<<");
Button SongButton       = Button(320, 250, 115, 25, "Song  Mode");
Button PatButton        = Button(320, 250, 115, 25, "Pat.  Mode");
Button CopyButton       = Button(320, 220, 55, 25,  "CP");
Button PasteButton      = Button(380, 220, 55, 25,  "PA");
Button ClearButton      = Button(320, 190, 55, 25,  "CL");
Button SaveButton       = Button(380, 190, 55, 25,  "SV");
Button TempMinusButton  = Button(320, 160, 55, 25,  "--");
Button TempPlusButton   = Button(380, 160, 55, 25,  "++");
Button holeNote         = Button(440, 160, 40, 25,  " 1");
Button halfNote         = Button(440, 190, 40, 25,  "1/2");
Button quaterNote       = Button(440, 220, 40, 25,  "1/4");
Button eighthNote       = Button(440, 250, 40, 25,  "1/8");
Button sixteenthNote    = Button(440, 280, 40, 25,  "1/16"); 
//Button VolMinusButton   = Button(320, 36, 55, 40, "--");
//Button VolPlusButton    = Button(380, 36, 55, 40, "++");
Button Setup            = Button(320, 3, 115, 20,  "Setup");
Button ScrollUp         = Button(320, 220, 55, 25, "Up");
Button ScrollDown       = Button(380, 220, 55, 25, "Dn");
Button BackButton       = Button(380, 270, 55, 40, "Back");
Button SaveSetup        = Button(320, 270, 55, 40, "Save");
Button ScreenCalibrate  = Button(40, 40, 400, 40,  "Screen Calibrate");
Button DrumSetup        = Button(40, 140, 400, 40, "Setup Drums");
Button SoundSetup       = Button(40, 190, 400, 40, "Setup Sound");
Button LeadSetup        = Button(40, 240, 400, 40, "Setup Lead");
Button SoundBank1       = Button(320, 180, 55, 40, "Bk.1");
Button SoundBank2       = Button(380, 180, 55, 40, "Bk.2");
Button Calibrate        = Button(240, 180, 15, 15, "");

Key keys[] = 
{
  Key (TFT_WHITE,245),
  Key (TFT_BLACK,226),
  Key (TFT_WHITE,207),
  Key (TFT_BLACK,188),
  Key (TFT_WHITE,169),
  Key (TFT_BLACK,150),
  Key (TFT_WHITE,131),
  Key (TFT_WHITE,112),
  Key (TFT_BLACK,93),
  Key (TFT_WHITE,74),
  Key (TFT_BLACK,55),
  Key (TFT_WHITE,36),
};

void setup() {/*
//"15UL", tells the PLL what multiplier value to use
//13UL = 84MHz,14UL = 90MHZ,15UL = 96MHz,16UL = 102MHz,17UL = 108MHz,18UL = 114MHz, 19UL = 120MHz 
#define SYS_BOARD_PLLAR (CKGR_PLLAR_ONE | CKGR_PLLAR_MULA(13UL) | CKGR_PLLAR_PLLACOUNT(0x3fUL) | CKGR_PLLAR_DIVA(1UL))
#define SYS_BOARD_MCKR ( PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)
//Set FWS according to SYS_BOARD_MCKR configuration
EFC0->EEFC_FMR = EEFC_FMR_FWS(4); //4 waitstate flash access
EFC1->EEFC_FMR = EEFC_FMR_FWS(4);
// Initialize PLLA to 114MHz
PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
while (!(PMC->PMC_SR & PMC_SR_LOCKA)) {}
PMC->PMC_MCKR = SYS_BOARD_MCKR;
while (!(PMC->PMC_SR & PMC_SR_MCKRDY)) {}
// Re-initialize some stuff with the new speed
SystemCoreClockUpdate();*/
  pinMode (CLK1,INPUT);
  pinMode (DT1,INPUT);
  pinMode (CLK2,INPUT);
  pinMode (DT2,INPUT);
  pinMode (CLK3,INPUT);
  pinMode (DT3,INPUT);
  pinMode (rotButton1,INPUT);
  pinMode (rotButton2,INPUT);
  pinMode (rotButton3,INPUT); 
  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XCS, OUTPUT);
  pinMode(VS_XDCS, OUTPUT);
  pinMode(VS_RESET, OUTPUT);

  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);  
  
  digitalWrite(VS_XCS, true); //Deselect Controltft.cursorToXY
  digitalWrite(VS_XDCS, true); //Deselect Data
  digitalWrite(VS_RESET, true);
  digitalWrite(CLK1, true);
  digitalWrite(DT1, true);
  digitalWrite(CLK2, true);   
  digitalWrite(DT2, true);
  digitalWrite(CLK3, true);
  digitalWrite(DT3, true);   
  digitalWrite(rotButton1, true);
  digitalWrite(rotButton2, true);
  digitalWrite(rotButton3, true);     
  delay(20);
  digitalWrite(VS_RESET, false);
  delay(20);
  digitalWrite(VS_RESET, true);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);  
  SPI.setClockDivider(100000); //Set SPI bus speed to 100K
  SPI.transfer(0xFF); //Throw a dummy byte at the bus  

  VSLoadUserCode();  
//  VSWriteRegister(0x1e03, 0xff, 0xff);
  talkMIDI(0xB9, 0, 0x7F);  //Bank select drums. midi cannel 10
  talkMIDI(0xB9, 0x07, Vol3);//0x07 is channel message, set channel volume to near max (127)

  talkMIDI(0xB0, 0, 0x00); //Default bank GM1
  talkMIDI(0xC0, soundPatch, 0); //Set instrument number. 0xC0 is a 1 data byte command
  talkMIDI(0xB0, 0x07, Vol2);//0x07 is channel message, set channel volume to near max (127)

  talkMIDI(0xB1, 0, 0x00); //Default bank GM1
  talkMIDI(0xC1, leadPatch, 0); //Set instrument number. 0xC0 is a 1 data byte command
  talkMIDI(0xB1, 0x07, Vol1);//0x07 is channel message, set channel volume to near max (127)

//  Serial.begin(9600); //Use serial for debugging
//  Serial.println("VS1053 Shield Example");
  tft.init();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  
  SD.begin(CS_PIN);
  for (pat = 0; pat < 16; pat++) {
    readPat();
  }
  Scheduler.startLoop(readTouch);
}
void loop() {
  readSound();
  readLead();
  readSong();
  readSetup();
  readCalibrate();
  patternMode();
  pat = 0;
  drawPattern();
while(1){
  playNotes();
  delay(250 * 60 / tempo);
if(!play){
        talkMIDI(0xB0, 0x7b, 127); //all notes channel 1 off
        talkMIDI(0xB1, 0x7b, 127); //all notes channel 2 off
}        
//  if(bassNoteOff[tick][pat]!=0) noteOff(0, bassNoteOff[tick][pat], 127);
//  talkMIDI(0xB0, 0x7b, 127); //all notes channel 1 off
  talkMIDI(0xB1, 0x7b, 127); //all notes channel 2 off
  talkMIDI(0xB0, 0x0c, analogRead(A11)/8); // effect control 1 (sets global reverb decay)
  talkMIDI(0xB0, 0x26, analogRead(A10)/8); // RPN LSB: 0 = bend range
//  VSWriteRegister(0x1e03, analogRead(A9), 0);
//  VSWriteRegister(0x1e03, analogRead(A8)/8, analogRead(A9)/8);
  VSWriteRegister(0x0B, analogRead(A8)/16, analogRead(A9)/16); // Master Vol control left right
//  VSWriteRegister(0x02, analogRead(A8)/16, analogRead(A9)/16);
}
}
void patternMode() {
  tick = -1;
  drawRaster();
  tft.fillRect(12, 263, 14, 17, TFT_BLACK);
  tft.drawRect(11, 15, 16, 266, TFT_RED);
  tft.setCursor(10, 290 );
  tft.print("Play Pat:");
  tft.setCursor(10, 305 );
  tft.print("Next Pat:");
  for (x = 0; x < 16; x++) {
    tft.setCursor(16 + (x * 16), 269);
    tft.print(x, HEX);
  }
  drawPattern();
}
void playNotes() {
  if (!mode) {
    if (copyPat) {
      tft.setCursor(10, 290);
      tft.print("Copy");
    }
    else
    {
      tft.setCursor(10, 290 );
      tft.print("Play");
      tft.setCursor(70, 290);
      tft.print(pat, HEX);
      tft.setCursor(70, 305);
      tft.print(nextPat, HEX);
    }
    if (!play)  return;
    if ((tick + 1) % 4) tft.fillRect(455, 5, 20, 20 , TFT_GREEN);
    else tft.fillRect(455, 5, 20, 20 , TFT_BLUE);
    if (tick != -1) {
      tft.fillRect((tick) * 16 + 12, 263, 14, 17, TFT_BLUE);
      tft.drawRect((tick) * 16 + 11, 15, 16, 266, TFT_WHITE);
      tft.setCursor(16 + (tick * 16), 269);
      tft.print(tick, HEX);
    }
    tick += 1;
    if (tick == 16) {
      tick = 0;
      if ((pat != nextPat) && (!copyPat)) {
        pat = nextPat;
        talkMIDI(0xB0, 0x7b, 127); //all notes channel 1 off
        talkMIDI(0xB1, 0x7b, 127); //all notes channel 2 off
        drawPattern();
      }
    }
if(bassNoteOff[tick][pat]!=0) noteOff(0, bassNoteOff[tick][pat], 127);
    tft.fillRect((tick) * 16 + 12, 263, 14, 17, TFT_BLACK);
    tft.drawRect((tick) * 16 + 11, 15, 16, 266, TFT_RED);
    tft.setCursor(16 + (tick * 16), 269);
    tft.print(tick, HEX);
    for (slope = 0; slope < 12; slope++) {
      if ((instrument[slope][pat] >> tick) & (1)) {
        if (pat < 8) {
          channel = 9;
          note = patch[slope];
          }
        else if (pat > 7 && pat < 12) {
          channel = 0;
          note =bass[slope];
        }else
        {
          channel = 1;
          note = bass[slope]+12;
        }
          noteOn(channel, note, 40 + (((instrument[12][pat] >> tick) & (1)) * 20));
        }
    } return;
  }
  else {
    if (!play)  return;
    if ((tick + 1) % 4) tft.fillRect(455, 5, 20, 20 , TFT_GREEN);
    else tft.fillRect(455, 5, 20, 20 , TFT_BLUE);
    printSongPart();
    tick += 1;
    if (tick == 16) {
      tick = 0;
      pos += 1;
      if (pos == 300) pos = 0;
      drawSong();
    }
    for (slope = 0; slope < 12; slope++) {
      for (slope2 = 0; slope2 < 16; slope2++) {
        if (pattern[pos] & (1 << slope2)) {
          if ((instrument[slope][slope2] >> tick) & (1)) {
            if (slope2 < 8) {
              channel = 9;
              note = patch[slope];
//              talkMIDI(0xB0, 0x0c, reverb3);              
              }
            else if (slope2 > 7 && slope2 < 12) {
              channel = 0;
              note = bass[slope];
//              talkMIDI(0xB0, 0x0c, reverb2);
            }
            else{
          channel = 1;
          note = bass[slope]+12;
//          talkMIDI(0xB0, 0x0c, reverb1);              
            }
            noteOn(channel, note, 40 + ((instrument[12][slope2] >> tick) & (1) * 20));
          }
        }
      }
    }
  }
}
void printSongPart() {
  for (slope = 0; slope < 14; slope++) {
    tft.setCursor(275, slope * 19 + 20);
    tft.print(pos + 13 - slope);
  }
}
void songMode()
{
  drawRaster();
  pos = 1;
  printSongPart();
  tft.drawRect(11, 262, 256, 19, TFT_RED);
  tick = -1;
  for (x = 0; x < 16; x++) {
    tft.setCursor(16 + (x * 16), 285);
    tft.print(x, HEX);
  }
  drawSong();
}
