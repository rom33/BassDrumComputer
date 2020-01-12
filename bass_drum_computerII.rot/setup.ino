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
  Calibrate.draw(tft,buttonColor);
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
void goSetup() {
  mode = false;
  play = false;
  tft.fillScreen(TFT_BLUE);
  ScreenCalibrate.draw(tft,buttonColor);
  DrumSetup.draw(tft,buttonColor);
  SoundSetup.draw(tft,buttonColor);
  LeadSetup.draw(tft,buttonColor);
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
  BackButton.draw(tft,buttonColor);
  SaveSetup.draw(tft,buttonColor);
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
  BackButton.draw(tft,buttonColor);
  SaveSetup.draw(tft,buttonColor);
  SoundBank1.draw(tft,buttonColor);
  SoundBank2.draw(tft,buttonColor);
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
