void printCopyTo()
{
  tft.setCursor(10, 305 );
  tft.print("To      :");
  tft.setCursor(70, 305);
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
