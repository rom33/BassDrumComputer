////////////////////////////////////////////
//    3.5" TOUCH SCREEN Bass & Drum       //
//              Maschine                  //
//                                        //
//            rom3 08.2019                //
//                                        //
//                                        //
////////////////////////////////////////////
#include <SPI.h>
#include <TFT_HX8357_Due.h>
#include <Scheduler.h>
#include "Button.h"
#include "TouchScreen_kbv.h"

#include <SdFat.h>
SdFatSoftSpi<12, 11, 13> SD; //Bit-Bang on the Shield pins
#define CS_PIN 10

#define VS_XCS    46 // Control Chip Select Pin (for accessing SPI Control/Status registers)
#define VS_XDCS   48 // Data Chip Select / BSYNC Pin
#define VS_DREQ   50 // Data Request Pin: Player asks for more data
#define VS_RESET  52 //Reset is active low

#define DRAW if (tool ? color = TFT_BLUE : color = TFT_RED);tft.fillRect(xx + 3, yy + 4, 5 , 7, color);
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

const int CLK1 = 22;  // CLK PIN des Drehreglers auf den digitalen PIN 3
const int rotButton1 = 23;
const int rotButton2 = 25;
const int rotButton3 = 27;
const int DT1 = 24;  // DT PIN des Drehreglers auf den digitalen PIN 4
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

int rX[8], rY[8];
unsigned short int x, y, xx, yy, tempo = 120, shift, inst, Vol1 = 100, Vol2= 100, Vol3 = 100, buff, soundPatch, leadPatch;
unsigned short pattern[300], instrument[13][16], channel, bank, note;
unsigned short patch[] = {35, 38, 44, 42, 43, 48, 47, 49, 56, 60, 61, 83};
unsigned short bass[] = {35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46};
unsigned short slope, slope2, pat = 0, nextPat = 0, copyTo, color, color1, pos, posOld, posx, posy;
unsigned short mode = 0, xxOld = 0, yyOld = 0, yyy;
short tick = -1;
bool play = false, copyPat = false, tool, testIt = false;
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
Button CopyButton       = Button(320, 220, 55, 25, "CP");
Button PasteButton      = Button(380, 220, 55, 25, "PA");
Button ClearButton      = Button(320, 190, 55, 25, "CL");
Button SaveButton       = Button(380, 190, 55, 25, "SV");
Button TempMinusButton  = Button(320, 150, 55, 25, "--");
Button TempPlusButton   = Button(380, 150, 55, 25, "++");
//Button VolMinusButton   = Button(320, 36, 55, 40, "--");
//Button VolPlusButton    = Button(380, 36, 55, 40, "++");
Button Setup            = Button(320, 3, 115, 20, "Setup");
Button ScrollUp         = Button(320, 220, 55, 25, "Up");
Button ScrollDown       = Button(380, 220, 55, 25, "Dn");
Button BackButton       = Button(380, 270, 55, 40, "Back");
Button SaveSetup        = Button(320, 270, 55, 40, "Save");
Button ScreenCalibrate  = Button(40, 40, 400, 40, "Screen Calibrate");
Button DrumSetup        = Button(40, 140, 400, 40, "Setup Drums");
Button SoundSetup       = Button(40, 190, 400, 40, "Setup Sound");
Button LeadSetup        = Button(40, 240, 400, 40, "Setup Lead");
Button SoundBank1       = Button(320, 180, 55, 40, "Bk.1");
Button SoundBank2       = Button(380, 180, 55, 40, "Bk.2");
Button Calibrate        = Button(240, 180, 15, 15, "");

void setup() {
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
//  VSWriteRegister(0x1e03, 0, 0xff);
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
  talkMIDI(0xB0, 0x7b, 127); //all notes channel 1 off
  talkMIDI(0xB1, 0x7b, 127); //all notes channel 2 off
}
}
void DrawValuePan(int value,int height){
  tft.drawFastVLine(320+value, height, 6, TFT_WHITE);
  tft.drawFastVLine(321+value, height, 6, TFT_RED);
  tft.drawFastVLine(322+value, height, 6, TFT_WHITE);
}
void printCopyTo()
{
  tft.setCursor(10, 310 );
  tft.print("To      :");
  tft.setCursor(70, 310);
  tft.print(copyTo, HEX);
}

void patCopy() {
  for (slope = 0; slope < 13; slope++) {
    instrument[slope][copyTo] = instrument[slope][pat];
  }
  copyPat = false;
}
void savePat() {
  String str = String(pat) + "pat.txt";
  file = SD.open(str, FILE_WRITE);
  file.rewind();
  for (slope = 0; slope < 13; slope++) {
    file.println(instrument[slope][pat]);
  }
  file.close();
}
void saveSong() {
  file = SD.open("song.txt", FILE_WRITE);
  file.rewind();
  for (slope = 0; slope < 300; slope++) {
    file.println(pattern[slope]);
  }
  file.close();
}
void readLead() {
  file = SD.open("lead.txt");
  if(file){
  String test = file.readStringUntil('\n');
  leadPatch = test.toInt();
  file.close();
  talkMIDI(0xC1, leadPatch, 0); 
}
}
void readSound() {
  file = SD.open("sound.txt");
  if(file){
  String test = file.readStringUntil('\n');
  soundPatch = test.toInt();
  file.close();
  talkMIDI(0xC0, soundPatch, 0); 
}
}
void readPat()
{
  String str = String(pat) + "pat.txt";
  file = SD.open(str);
  if (file) {
    for (slope = 0; slope < 13; slope++) {
      String test = file.readStringUntil('\n');
      instrument[slope][pat] = test.toInt();
    }
  }
  file.close();
}
void readSong()
{
  file = SD.open("song.txt");
  if (file) {
    for (slope = 0; slope < 300; slope++) {
      String test = file.readStringUntil('\n');
      pattern[slope] = test.toInt();
    }
  }
  file.close();
}
void readSetup()
{
  file = SD.open("drumsounds.txt");
  if (file) {
    for (slope = 0; slope < 12; slope++) {
      String test = file.readStringUntil('\n');
      patch[slope] = test.toInt();
    }
  }
  file.close();
}
void readCalibrate() {
  String test;
  file = SD.open("calibrate.txt");
  if (file) {
    test = file.readStringUntil('\n');
    TS_MINX = test.toInt();
    test = file.readStringUntil('\n');
    TS_MAXX = test.toInt();
    test = file.readStringUntil('\n');
    TS_MINY = test.toInt();
    test = file.readStringUntil('\n');
    TS_MAXY = test.toInt();
    file.close();
  }else calibrate();
}
void drawPattern()
{
  for (x = 0; x < 16; x++) {
    for (slope2 = 0; slope2 < 13; slope2++) {
      tool = !((instrument[slope2][pat]) & (1 <<  x));
      xx = (x * 16) + 14;
      yy = 245 - (slope2 * 19);
      DRAW;
    }
  }
}

void drawSong()
{
  for (x = 0; x < 16; x++) {
    for (slope2 = 0; slope2 < 14; slope2++) {
      tool = !((pattern[slope2 + pos] >> x) & (1));
      xx = (x * 16) + 14;
      yy = 264 - (slope2 * 19);
      DRAW;
    }
  }
}
void drawRaster() {
  rotMode1 =rotMode2 = rotMode3 = 0;
  tft.fillScreen(TFT_BLUE);
  StartStopButton.draw(tft);
  TempMinusButton.draw(tft);
  TempPlusButton.draw(tft);
  tft.setCursor(320,27);
  tft.print("Drums");
  tft.setCursor(320,67);
  tft.print("Bass");
  tft.setCursor(320,107);
  tft.print("Melody"); 
  tft.drawRect(320,37,129,8,TFT_RED);
  tft.drawRect(320,47,129,8,TFT_WHITE);
  tft.fillRect(320,57,129,8,TFT_WHITE);
  tft.drawRect(320,77,129,8,TFT_RED);
  tft.drawRect(320,87,129,8,TFT_WHITE);
  tft.fillRect(320,97,129,8,TFT_WHITE);  
  tft.drawRect(320,117,129,8,TFT_RED);
  tft.drawRect(320,127,129,8,TFT_WHITE);
  tft.fillRect(320,137,129,8,TFT_WHITE);

  tft.setCursor(455,128);
  Format(reverb1);
  tft.print(reverb1);
  DrawValue(reverb1,128);
  tft.setCursor(455,88);
  Format(reverb2);
  tft.print(reverb2);
  DrawValue(reverb2,88);
  tft.setCursor(455,48);
  Format(reverb3);
  tft.print(reverb3);
  DrawValue(reverb3,48);  

  tft.setCursor(455,38);
  Format(Vol3);
  tft.print(Vol3);
  DrawValue(Vol3,38);
  tft.setCursor(455,78);
  Format(Vol2);
  tft.print(Vol2);
  DrawValue(Vol2,78);
  tft.setCursor(455,118);
  Format(Vol1);
  tft.print(Vol1);
  DrawValue(Vol1,118);   
  
  tft.setCursor(455,48);
  Format(reverb3);
  tft.print(reverb3);
  DrawValue(reverb3,48);
  tft.setCursor(455,88);
  Format(reverb2);
  tft.print(reverb2);
  DrawValue(reverb2,88);
  tft.setCursor(455,128);
  Format(reverb1);
  tft.print(reverb1);
  DrawValue(reverb1,128); 

  tft.setCursor(455,58);
  Format(pan3-64);
  tft.print(pan3-64);
  DrawValuePan(pan3,58);
  tft.setCursor(455,98);
  Format(pan2-64);
  tft.print(pan2-64);
  DrawValuePan(pan2,98);
  tft.setCursor(455,138);
  Format(pan1-64);
  tft.print(pan1-64);
  DrawValuePan(pan1,138); 
                 
  ClearButton.draw(tft);
  SaveButton.draw(tft);
  Rewind.draw(tft);
  Setup.draw(tft);
  tft.setCursor(360, 178 );
  tft.print(tempo);
  tft.setCursor(385, 178 );
  tft.print("bpm");
  if (!mode) {
    CopyButton.draw(tft);
    PasteButton.draw(tft);
    SongButton.draw(tft);
    drawKeys();
    tft.setCursor(275, 23 );
    tft.print("Acc");
  }
  else {
    PatButton.draw(tft);
    ScrollUp.draw(tft);
    ScrollDown.draw(tft);
  }
  tft.setCursor(15, 5 );
  tft.print("Bass & Drum-Computer with VS1053 DSP");
  for (int x = 0; x < 17; x++) {
    tft.drawFastVLine((x * 16) + 10, 14, 268, TFT_WHITE);
    tft.drawFastVLine((x * 16) + 11, 14, 268, TFT_WHITE);
  }
  for (int x = 14; x < 286; x += 19) {
    tft.drawFastHLine(10, x, 256, TFT_WHITE);
    tft.drawFastHLine(10, x + 1, 256, TFT_WHITE);
  }
}
void patternMode() {
  tick = -1;
  drawRaster();
  tft.fillRect(12, 263, 14, 17, TFT_BLACK);
  tft.drawRect(11, 15, 16, 266, TFT_RED);
  tft.setCursor(10, 290 );
  tft.print("Play Pat:");
  tft.setCursor(10, 310 );
  tft.print("Next Pat:");
/*  tft.setCursor(70, 290);
  tft.print(0);
  tft.setCursor(70, 310);
  tft.print(0);*/
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
      tft.setCursor(70, 310);
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
        drawPattern();
      }
    }
    tft.fillRect((tick) * 16 + 12, 263, 14, 17, TFT_BLACK);
    tft.drawRect((tick) * 16 + 11, 15, 16, 266, TFT_RED);
    tft.setCursor(16 + (tick * 16), 269);
    tft.print(tick, HEX);
    for (slope = 0; slope < 12; slope++) {
      if ((instrument[slope][pat] >> tick) & (1)) {
        if (pat < 8) {
          channel = 9;
          note = patch[slope];
//          talkMIDI(0xB0, 0x0c, reverb3);
          }
        else if (pat > 7 && pat < 12) {
          channel = 0;
          note =bass[slope];
//          talkMIDI(0xB0, 0x0c, reverb2);
        }else
        {
          channel = 1;
          note = bass[slope]+12;
//          talkMIDI(0xB0, 0x0c, reverb1);
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
            noteOn(channel, note, 40 + (((instrument[12][slope2] >> tick) & (1)) * 20));
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
void goSetup() {
  mode = false;
  play = false;
  tft.fillScreen(TFT_BLUE);
  ScreenCalibrate.draw(tft);
  DrumSetup.draw(tft);
  SoundSetup.draw(tft);
  LeadSetup.draw(tft);
  WaitTouch;
  GetTouchPoints;
  if (DrumSetup.contains(x, y)) {
    drumSetup();
  }
  if (SoundSetup.contains(x, y)) {
    soundSetup(0,"sound.txt");
  }
  if (LeadSetup.contains(x, y)) {
    soundSetup(1,"lead.txt");
  }  
  if(ScreenCalibrate.contains(x,y)){
     calibrate();
  }
}
void drumSetup() {
  tft.fillScreen(TFT_BLUE);
  BackButton.draw(tft);
  SaveSetup.draw(tft);
  for (xx = 0; xx < 62; xx += 2) {
    tft.drawFastHLine(0, xx * 5, 270, TFT_WHITE);
    tft.setCursor(4, xx * 5 + 2);
    tft.print(drumSounds[xx]);
    tft.setCursor(140, xx * 5 + 2);
    tft.print(drumSounds[xx + 1]);
  }
  tft.drawFastHLine(0, 310, 270, TFT_WHITE);
  tft.drawFastVLine(0, 0, 310, TFT_WHITE);
  tft.drawFastVLine(135, 0, 310, TFT_WHITE);
  tft.drawFastVLine(270, 0, 310, TFT_WHITE);
  drawKeys();
  for (slope = 0; slope < 12; slope++) {
    tft.setCursor(320, slope * 19 + 38);
    tft.print(drumSounds[patch[11 - slope] - 27]);
   }
  while (1) {
    WaitTouch;
    GetTouchPoints;
    if (SaveSetup.contains(x, y)) {
      file = SD.open("drumsounds.txt", FILE_WRITE);
      file.rewind();
      for (slope2 = 0; slope2 < 12; slope2++) {
        file.println(patch[slope2]);
      }
      file.close();
    }
    tft.setCursor(xxOld * 136 + 4, yyOld * 10 + 2);
    tft.print(drumSounds[yyOld * 2 + xxOld]);
    if (x > 0 && x < 135)xx = 0;
    if (x > 135 && x < 270)xx = 1;
    if (x > 270)xx = 2;
    if (xx != 2) {
      for (slope = 0; slope < 31; slope++) {
        if (y > slope * 10 && y < slope * 10 + 12) {
          yy = slope;
          break;
        }
      }
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(xx * 136 + 4, yy * 10 + 2);
      tft.print(drumSounds[yy * 2 + xx]);
      noteOn(9, yy * 2 + xx + 27, 60);
      xxOld = xx;
      yyOld = yy;
      treadWater();
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
    } else {
      for (slope = 0; slope < 12; slope++)
      {
        if ((y > (slope * 19) + 36) && (y < (slope * 19) + 55)) {
          yyy = slope;
          break;
        }
      }
      patch[11 - yyy] = yyOld * 2 + xxOld + 27;
      tft.setCursor(320, yyy * 19 + 38);
      tft.print(drumSounds[patch[11 - yyy] - 27]);
    }
    if (BackButton.contains(x, y))return;
    else treadWater();
  }
}
void soundSetup(int channel, String fileSave) {
  int Patch;
  tft.fillScreen(TFT_BLUE);
  BackButton.draw(tft);
  SaveSetup.draw(tft);
  SoundBank1.draw(tft);
  SoundBank2.draw(tft);
  for (xx = 0; xx < 62; xx += 2) {
    tft.drawFastHLine(0, xx * 5, 270, TFT_WHITE);
    tft.setCursor(4, xx * 5 + 2);
    tft.print(Sounds[xx]);
    tft.setCursor(140, xx * 5 + 2);
    tft.print(Sounds[xx + 1]);
  }
  tft.drawFastHLine(0, 310, 270, TFT_WHITE);
  tft.drawFastVLine(0, 0, 310, TFT_WHITE);
  tft.drawFastVLine(135, 0, 310, TFT_WHITE);
  tft.drawFastVLine(270, 0, 310, TFT_WHITE);
  drawKeys();
  tft.setCursor(320, 57);
  if(!channel ? Patch = soundPatch : Patch = leadPatch);
  tft.print(Sounds[Patch]);
  while (1) {
  testIt = 0;
    WaitTouch;
    GetTouchPoints;
    if (SoundBank1.contains(x, y)) {
      for (xx = 0; xx < 62; xx += 2) {
        tft.setCursor(4, xx * 5 + 2);
        tft.print(Sounds[xx]);
        tft.setCursor(140, xx * 5 + 2);
        tft.print(Sounds[xx + 1]);
        bank = 0;
        testIt = 1;
      }treadWater();
    }
    if (SoundBank2.contains(x, y)) {
      for (xx = 0; xx < 62; xx += 2) {
        tft.setCursor(4, xx * 5 + 2);
        tft.print(Sounds[xx + 62]);
        tft.setCursor(140, xx * 5 + 2);
        tft.print(Sounds[xx + 63]);
        bank = 1;
        testIt = 1;
      }treadWater();
    }
    if (BackButton.contains(x, y)) {
      return;
    }
    if (SaveSetup.contains(x, y)) {
      file = SD.open(fileSave, FILE_WRITE);
      file.rewind();
      file.println(Patch);
      file.close();
      testIt = 1;
    }
    if(!testIt){
    if (x > 0 && x < 135) xx = 0;
    if (x > 135 && x < 270) xx = 1;
    if (x > 270 && x < 310) xx = 2;
    if (xx != 2) {
      for (slope = 0; slope < 31; slope++) {
        if (y > slope * 10 && y < slope * 10 + 12) {
          yy = slope;
          break;
        }
      }
      tft.setCursor(xxOld * 136 + 4, yyOld * 10 + 2);
      tft.print(Sounds[yyOld * 2 + xxOld + bank * 62]);
      tft.setTextColor(TFT_WHITE, TFT_RED);
      tft.setCursor(xx * 136 + 4, yy * 10 + 2);
      Patch = yy * 2 + xx + (bank * 62);
      if(!channel ? soundPatch = Patch : leadPatch = Patch);     
      tft.print(Sounds[Patch]);
      talkMIDI(0xC0+channel, Patch, 0); //Set instrument number. 0xC0 is a 1 data byte command
      tft.setTextColor(TFT_WHITE, TFT_BLUE);
      tft.setCursor(320, 57);
      tft.print(Sounds[Patch]);
      noteOn(channel, 40+channel*12, 60);
      treadWater();
      talkMIDI(0xB0+channel, 0x78, 0); //all notes channel 1 off
      xxOld = xx;
      yyOld = yy;
    }  if(xx==2) {
      for (slope = 1; slope < 13; slope++)
      {
        if ((y > (slope * 19) + 36) && (y < (slope * 19) + 55)) {
          yyy = slope;
          break;
        }
      }
      noteOn(channel, bass[11 - yyy]+channel*12, 60);
      treadWater();
      talkMIDI(0xB0+channel, 0x78, 0); //all notes channel 1 off
    }
  }
  }
}
void drawKeys() {
  tft.fillRect(275, 36, 35, 17, TFT_WHITE);
  tft.fillRect(275, 55, 35, 17, TFT_BLACK);
  tft.fillRect(275, 74, 35, 17, TFT_WHITE);
  tft.fillRect(275, 93, 35, 17, TFT_BLACK);
  tft.fillRect(275, 112, 35, 17, TFT_WHITE);
  tft.fillRect(275, 131, 35, 17, TFT_BLACK);
  tft.fillRect(275, 150, 35, 17, TFT_WHITE);
  tft.fillRect(275, 169, 35, 17, TFT_WHITE);
  tft.fillRect(275, 188, 35, 17, TFT_BLACK);
  tft.fillRect(275, 207, 35, 17, TFT_WHITE);
  tft.fillRect(275, 226, 35, 17, TFT_BLACK);
  tft.fillRect(275, 245, 35, 17, TFT_WHITE);
}
void treadWater() {
  curMillis = millis();
  prevMillis = millis();
  while (curMillis - prevMillis <= interval) {
    curMillis = millis();
  }
}
void calibrate() {
  tft.fillScreen(TFT_BLUE);
  drawCrossHair(10, 10, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[0] = tp.x;
  rY[0] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  drawCrossHair(10, 10, TFT_BLUE);
  treadWater();
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  drawCrossHair(480 / 2, 10, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[1] = tp.x;
  rY[1] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  drawCrossHair(480 / 2, 10, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 10, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[2] = tp.x;
  rY[2] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 10, TFT_BLUE);
  treadWater();
  drawCrossHair(10, 320 / 2, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[3] = tp.x;
  rY[3] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  drawCrossHair(10, 320 / 2, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 320 / 2, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[4] = tp.x;
  rY[4] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 320 / 2, TFT_BLUE);
  treadWater();
  drawCrossHair(10, 320 - 11, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[5] = tp.x;
  rY[5] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  drawCrossHair(10, 320 - 11, TFT_BLUE);
  treadWater();
  drawCrossHair(480 / 2, 320 - 11, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  rX[6] = tp.x;
  rY[6] = tp.y;
  drawCrossHair(480 / 2, 320 - 11, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 320 - 11, TFT_WHITE);
  tft.setCursor(200, 160);
  tft.print("touch crosshair");
  WaitTouch;
  GetTouchPoints;
  rX[7] = tp.x;
  rY[7] = tp.y;
  tft.fillRect(200, 160, 112 , 8, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 320 - 11, TFT_BLUE);
  treadWater();
/*  char buffer[30];
  sprintf(buffer, "%d %d %d %d %d %d %d %d ", rX[0], rX[1], rX[2], rX[3], rX[4], rX[5], rX[6],rX[7]);
  Serial.println(buffer);
  sprintf(buffer, "%d %d %d %d %d %d %d %d ", rY[0], rY[1], rY[2], rY[3], rY[4], rY[5], rY[6],rY[7]);
  Serial.println(buffer);*/  
  clx = (rX[0] + rX[3] + rX[5]) / 3;
  crx = (rX[2] + rX[4] + rX[7]) / 3;
  cty = (rY[0] + rY[1] + rY[2]) / 3;
  cby = (rY[5] + rY[6] + rY[7]) / 3;
  py = float(cby - cty) / (320 - 20);
  px = float(crx - clx) / (480 - 20);
  clx -= px * 10;
  crx += px * 10;
  cty -= py * 10;
  cby += py * 10;
  TS_MINX = (crx);
  TS_MAXX = (clx);
  TS_MINY = (cby);
  TS_MAXY = (cty);
  Calibrate.draw(tft);
  WaitTouch;
  GetTouchPoints;
  if(Calibrate.contains(x,y)) {
  file = SD.open("calibrate.txt", FILE_WRITE);
  file.rewind();
  file.println(TS_MINX);
  file.println(TS_MAXX);
  file.println(TS_MINY);
  file.println(TS_MAXY);
  file.close();
  }else calibrate();  
}
void drawCrossHair(int x, int y, int color)
{
  tft.drawRect(x - 10, y - 10, 20, 20, color);
  tft.drawLine(x - 5, y, x + 5, y, color);
  tft.drawLine(x, y - 5, x, y + 5, color);
}
