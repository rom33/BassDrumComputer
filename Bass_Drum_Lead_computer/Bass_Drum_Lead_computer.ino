//////////////////////////////////////////
//  3.5" TOUCH SCREEN Bass, Drum & lead   //
//          Sequencer Machine             //
//                                        //
//            rom3 01.2020                //
//                                        //
//                                        //
////////////////////////////////////////////
#include <SPI.h>
#include <TFT_HX8357_Due.h>
#include <Scheduler.h>
#include "Button.h"
#include "TouchScreen_kbv.h"

// *** card-reader
#include <SdFat.h>
SdFatSoftSpi<12, 11, 13> SD; //Bit-Bang on the Shield pins
#define CS_PIN 10
File file;

// *** display
TFT_HX8357_Due tft = TFT_HX8357_Due();

// *** vs1053
#define VS_XCS    46 // Control Chip Select Pin (for accessing SPI Control/Status registers)
#define VS_XDCS   48 // Data Chip Select / BSYNC Pin
#define VS_DREQ   50 // Data Request Pin: Player asks for more data
#define VS_RESET  53 // Reset is active low

// *** touch
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin
int TS_MINX = 890;
int TS_MAXX = 90;
int TS_MINY = 890;
int TS_MAXY = 214;
#define MINPRESSURE 25
TouchScreen_kbv myTouch = TouchScreen_kbv(XP, YP, XM, YM, 300);
TSPoint_kbv tp;

// *** rotary encoders
const int CLK[] = {22, 26, 30}; // CLK PIN rotary encoder
const int DT[] = {24, 28, 32}; // DT PIN rotary encoder
const int rotButton[] = {23, 25, 27};
int lastPosition[3], currentPosition[3];
int reverb[] = {12 , 12, 12};
int pan[] = {64, 64, 64};
int Vol[] = {100, 100, 100};
int rotMode[3];

// *** some MACROS
#define DRAW(Colour) if (tool ? color = TFT_BLUE : color = Colour);tft.fillRect(xDraw, yDraw, 5 , 7, color);
#define DrawValue(value,height) tft.fillRect(321,height,value,6,TFT_GREEN);tft.fillRect(321+value,height,127-value,6,TFT_BLUE);
#define Format(space) if(space<100&&space>=-9)tft.print(" ");if(space<10&&space>=0)tft.print(" ");
#define DrawValue(value,height) tft.fillRect(321,height,value,6,TFT_GREEN);tft.fillRect(321+value,height,127-value,6,TFT_BLUE);
#define WaitTouch do{tp=myTouch.getPoint();}while(tp.z<MINPRESSURE);
#define GetTouchPoints tp=myTouch.getPoint();xx=map(tp.x,TS_MINX,TS_MAXX,480,0);yy=map(tp.y,TS_MINY,TS_MAXY,320,0);

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

// *** button declaraions
Button ButtPat[] = {
  Button(123, 277, 30, 20, "1"),
  Button(158, 277, 30, 20, "2"),
  Button(193, 277, 30, 20, "3"),
  Button(228, 277, 30, 20, "4"),
  Button(123, 300, 30, 20, "A"),
  Button(158, 300, 30, 20, "B"),
  Button(193, 300, 30, 20, "C"),
  Button(228, 300, 30, 20, "D")
};
Button ButtInst[] = {
  Button(265, 300, 32, 20, "Drum"),
  Button(265, 277, 32, 20, "Bass"),
  Button(265, 254, 32, 20, "Leed"),
};
Button ButtInstPlay[] = {
  Button(300, 300, 15, 20, "P"),
  Button(300, 277, 15, 20, "P"),
  Button(300, 254, 15, 20, "P"),
};
Button LoopLen[] = {
  Button(425, 295, 55, 25, "1 - 2"),
  Button(425, 295, 55, 25, "1 - 3"),
  Button(425, 295, 55, 25, "1 - 4"),
};
Button NoteLen[] = {
  Button(0,0,0,0,""),
  Button(320, 300, 35, 20,  "1/16"),
  Button(320, 277, 35, 20,  "1/8"),
  Button(320, 254, 35, 20,  "1/4"),
  Button(320, 231, 35, 20,  "1/2"),
  Button(320, 208, 35, 20,  " 1"),
};

Button Rewind           = Button(0, 280, 30, 35, "<<");
Button StartStopButton  = Button(35, 280, 80, 35, "Start/Stop");
Button LoopButton       = Button(365, 295, 55, 25, "Loop");
Button SongButton       = Button(365, 280, 115, 25, "Song  Mode");
Button PatButton        = Button(365, 280, 115, 25, "Pat.  Mode");
Button CopyButton       = Button(365, 265, 55, 25,  "CP");
Button PasteButton      = Button(425, 265, 55, 25,  "PA");
Button ClearButton      = Button(365, 235, 55, 25,  "CL");
Button SaveButton       = Button(425, 235, 55, 25,  "SV");
Button TempMinusButton  = Button(365, 205, 55, 25,  "--");
Button TempPlusButton   = Button(425, 205, 55, 25,  "++");
Button Setup            = Button(325, 3, 115, 20,  "Setup");
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

// *** Key declrations
Button keys[] =
{
  Button (TFT_WHITE, 235),
  Button (TFT_BLACK, 216),
  Button (TFT_WHITE, 197),
  Button (TFT_BLACK, 178),
  Button (TFT_WHITE, 159),
  Button (TFT_BLACK, 140),
  Button (TFT_WHITE, 121),
  Button (TFT_WHITE, 102),
  Button (TFT_BLACK, 83),
  Button (TFT_WHITE, 64),
  Button (TFT_BLACK, 45),
  Button (TFT_WHITE, 26)
};
// *** variable
int potRead1, potRead2, potRead3, val, rX[8], rY[8];
int32_t clx, crx, cty, cby;
float px, py;
bool pressed;
unsigned long buttonColor[] = {TFT_BLACK, TFT_RED, TFT_MAGENTA, TFT_YELLOW, TFT_CYAN, TFT_GREEN};
byte toggle, patRow1, patRow2, patRow1Old, patRow2Old, instSelect, instSelectOld, loopLen, playtrack, noteLen;
bool play, tool, loopMode, bassPlay, drumPlay, leadPlay;
unsigned short instrument[4][13][17], bassSound = 32, leadSound = 1;
unsigned short instNoteLen[4][13][16][16];
unsigned long instNoteOff[4][13][16];
unsigned short interval = 200, rotInterval = 600, xDraw, yDraw, xx, yy, note, pat, nextPat, copyPat, stp, slope, slope2, slope3, touched;
unsigned short tick, tempo = 120;
unsigned short instSet[3][13] = {{35, 38, 44, 42, 43, 48, 47, 49, 56, 60, 61, 83}, {35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46}, {47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58}};
unsigned long color;
unsigned long currentMillis, previousMillis, currTime, prevTime;

void setup()  {
  /*
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

  // *** rotary encoders
  for (slope = 0; slope < 3; slope++) {
    pinMode (CLK[slope], INPUT);
    pinMode (DT[slope], INPUT);
    pinMode (rotButton[slope], INPUT);
    digitalWrite(rotButton[slope], true);
    digitalWrite(CLK[slope], true);
    digitalWrite(DT[slope], true);
  }
  // *** touch
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  // *** vs1053
  pinMode(VS_DREQ, INPUT);
  pinMode(VS_XCS, OUTPUT);
  pinMode(VS_XDCS, OUTPUT);
  pinMode(VS_RESET, OUTPUT);
  // *** vs1053
  digitalWrite(VS_XCS, true); //Deselect Control
  digitalWrite(VS_XDCS, true); //Deselect Data
  digitalWrite(VS_RESET, false);
  delayMicroseconds(10);
  digitalWrite(VS_RESET, true);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(50000); //Set SPI bus speed to 50K

  // *** Use serial for debugging
  //  Serial.begin(9600);
  //  Serial.println("VS1053 Shield Example");

  // *** bank select
  //  VSLoadUserCode();
  //  VSWriteRegister(0x1e03, 0xff, 0xff);
  talkMIDI(0xB9, 0, 0x7F);  //Bank select drums. midi cannel 10
  talkMIDI(0xB9, 0x07, Vol[2]);//0x07 is channel message, set channel volume to near max (127)

  talkMIDI(0xB8, 0, 0x00); //Default bank GM1
  talkMIDI(0xC8, bassSound, 0); //Set instrument number. 0xC8 is a 1 data byte command
  talkMIDI(0xB8, 0x07, Vol[1]);//0x07 is channel message, set channel volume to near max (127)

  talkMIDI(0xB7, 0, 0x00); //Default bank GM1
  talkMIDI(0xC7, leadSound, 0); //Set instrument number. 0xC7 is a 1 data byte command
  talkMIDI(0xB7, 0x07, Vol[0]);//0x07 is channel message, set channel volume to near max (127)

  // *** display begin
  tft.init();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.fillScreen(TFT_BLUE);
  welcomeScreen();

  // *** card-reader begin
  SD.begin(CS_PIN);

  readCalibrate();
  patternScreen();
  for (pat = 0; pat < 16; pat++) {
    readPat();
  }
  pat = 16;
  nextPat = 0;
  drawPattern();
  pat = nextPat;
  drumPlay = true;
  loopLen = 1;
  noteLen = 1;

  // *** scheduler begin
  Scheduler.startLoop(readTouch);
}

void loop() {
  while (1) {
    VSWriteRegister(0x0B, analogRead(A8) / 16, analogRead(A9) / 16); // Master Vol control left right
    talkMIDI(0xB0, 0x0c, 127); // effect control 1 (sets global reverb decay)
    talkMIDI(0xB0, 0x26, analogRead(A10) / 8); // RPN LSB: 0 = bend range
    playNotes();
    delay(250 * 60 / tempo);
  }
}
void playNotes() {
  if (!play) {
    talkMIDI(0xB7, 0x7b, 127); //all notes channel 1 off
    talkMIDI(0xB8, 0x7b, 127); //all notes channel 2 off
    DrawOrNot();
    return;
  }
  if (tick > 15) {
    if (loopMode) {
      nextPat += 1;
      patRow1Old = patRow1;
      patRow1 += 1;
      if (patRow2 != patRow2Old) {
        nextPat = 0 + patRow2 * 4;
        patRow1 = 0;
        ButtPat[patRow2Old + 4].draw(tft, buttonColor[1]);
        ButtPat[patRow2 + 4].draw(tft, buttonColor[0]);
        patRow2Old = patRow2;
      }
      if (nextPat > loopLen + patRow2 * 4) {
        nextPat = patRow2 * 4;
        patRow1 = 0;
      }
      ButtPat[patRow1Old].draw(tft, buttonColor[1]);
      ButtPat[patRow1].draw(tft, buttonColor[0]);
    }
    DrawOrNot();
    tick = 0;
    stp = 15;
  }
  if (tick > 0)
  {
    stp = tick - 1;
  }
  drawRec();
  for (slope = 0; slope < 12; slope++) {
    if((instNoteOff[0][slope][pat] >> tick) & (1)||(instNoteOff[0][slope][pat] >> 16 + tick) & (1)){
      noteOff(9, instSet[0][slope], 0);
    }
    if (drumPlay && (instrument[0][slope][pat] >> tick) & (1)) {
      noteOn(9, instSet[0][slope], 40 + (((instrument[0][12][pat] >> tick) & (1)) * 20));
    }
    if((instNoteOff[1][slope][pat] >> tick) & (1)||(instNoteOff[1][slope][pat] >> 16 + tick) & (1)){
      noteOff(8, instSet[1][slope], 0);
    }
    if (bassPlay && (instrument[1][slope][pat] >> tick) & (1)) {
      noteOn(8, instSet[1][slope], 40 + (((instrument[1][12][pat] >> tick) & (1)) * 20));
    }
    if((instNoteOff[2][slope][pat] >> tick) & (1)||(instNoteOff[2][slope][pat] >> 16 + tick) & (1)){
      noteOff(7, instSet[2][slope], 0);
    }
    if (leadPlay && (instrument[2][slope][pat] >> tick) & (1)) {
      noteOn(7, instSet[2][slope], 40 + (((instrument[2][12][pat] >> tick) & (1)) * 20));
    }
  }
  tick += 1;
}
