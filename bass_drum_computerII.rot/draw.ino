void drawPattern()
{
   if(pat>7){
      holeNote.draw(tft,TFT_MAGENTA);
      halfNote.draw(tft,TFT_YELLOW);
      quaterNote.draw(tft,TFT_CYAN);
      eighthNote.draw(tft,TFT_GREEN);
      sixteenthNote.draw(tft,buttonColor);
      tft.setCursor(90, 290);
      tft.print("Sixteen Note");
   }else{
      tft.fillRect(440, 160, 40, 150, TFT_BLUE);
      tft.setCursor(90, 290);
      tft.print("            ");
        }
  for (x = 0; x < 16; x++) {
    for (slope2 = 0; slope2 < 13; slope2++) {
      tool = !((instrument[slope2][pat]) & (1 <<  x));
      if(pat>7&&slope2!=12){
        if((instrument[slope2][pat] >> x) & (1)){
        noteLen = bassNoteLen[x][pat];
              switch(noteLen){
                  case 16:
                  col = TFT_MAGENTA;
                  offBeat(16,pat,slope2,x);
                  break;
                  case 8:
                  col = TFT_YELLOW;
                  offBeat(8,pat,slope2,x);             
                  break;
                  case 4:
                  col = TFT_CYAN;
                  offBeat(4,pat,slope2,x);
                  break;
                  case 2:
                  col = TFT_GREEN;
                  offBeat(2,pat,slope2,x);
                  break;
                  case 1:
                  col = TFT_RED;
                  offBeat(1,pat,slope2,x);
                  break;
                }
        }else col=TFT_RED;
        noteLen=1;
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
  StartStopButton.draw(tft,buttonColor);
  TempMinusButton.draw(tft,buttonColor);
  TempPlusButton.draw(tft,buttonColor);
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
                 
  ClearButton.draw(tft,buttonColor);
  SaveButton.draw(tft,buttonColor);
  Rewind.draw(tft,buttonColor);
  Setup.draw(tft,buttonColor);
  tft.setCursor(360, 150 );
  tft.print(tempo);
  tft.setCursor(385, 150 );
  tft.print("bpm");
  if (!mode) {
    CopyButton.draw(tft,buttonColor);
    PasteButton.draw(tft,buttonColor);
    SongButton.draw(tft,buttonColor);
    drawKeys();
    tft.setCursor(275, 23 );
    tft.print("Acc");
  } else {
    PatButton.draw(tft,buttonColor);
    ScrollUp.draw(tft,buttonColor);
    ScrollDown.draw(tft,buttonColor);
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
  for (int i = 0; i<12; i++) // draws keys
  {
    keys[i].draw(tft);
  }
}
