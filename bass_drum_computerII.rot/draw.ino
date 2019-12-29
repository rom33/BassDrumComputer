void drawPattern()
{
   if(pat>7){
      holeNote.draw(tft);
      halfNote.draw(tft);
      quaterNote.draw(tft);
      eighthNote.draw(tft);
      sixteenthNote.draw(tft);
      tft.setCursor(90, 290);
      tft.print("Sixteen Note");
      noteLen=5;     
   }else{
      tft.fillRect(440, 160, 40, 150, TFT_BLUE);
      tft.setCursor(90, 290);
      tft.print("            ");
        }
  for (x = 0; x < 16; x++) {
    for (slope2 = 0; slope2 < 13; slope2++) {
      tool = !((instrument[slope2][pat]) & (1 <<  x));
      if(pat>7&&slope2!=12){
        if((instrument[slope2][pat] >> x) & (1))
        noteLen = bassNoteLen[x][pat];
              switch(noteLen){
                  case 1:
                  col = TFT_MAGENTA;
                  break;
                  case 2:
                  col = TFT_YELLOW;
                  break;
                  case 3:
                  col = TFT_CYAN;
                  break;
                  case 4:
                  col = TFT_GREEN;
                  break;
                  case 5:
                  col = TFT_RED;
                  break;
                }
        }else col=TFT_RED;
      xx = (x * 16) + 14;
      yy = 245 - (slope2 * 19);
      DRAW(col);
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
      DRAW(TFT_RED);
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
  tft.setCursor(360, 150 );
  tft.print(tempo);
  tft.setCursor(385, 150 );
  tft.print("bpm");
  if (!mode) {
    CopyButton.draw(tft);
    PasteButton.draw(tft);
    SongButton.draw(tft);
    drawKeys();
    tft.setCursor(275, 23 );
    tft.print("Acc");
  } else {
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
void DrawValuePan(int value,int height){
  tft.drawFastVLine(320+value, height, 6, TFT_WHITE);
  tft.drawFastVLine(321+value, height, 6, TFT_RED);
  tft.drawFastVLine(322+value, height, 6, TFT_WHITE);
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
