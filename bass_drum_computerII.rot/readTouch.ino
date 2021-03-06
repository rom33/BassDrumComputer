void readTouch() {
  readRot();
  currentMillis = millis();
  GetTouchPoints;
if(currentMillis - previousMillis >= interval){
  if(playNote){
    playNote=0;
    if(pat <12) {
        noteOff(0, bass[pressedKey], 60);
      }else{
        noteOff(1, bass[pressedKey]+12, 60);        
      }
  }
  if(touched>0)
  {
    switch(touched){
    case 1:
    holeNote.draw(tft,TFT_MAGENTA);
    break;
    case 2:
    halfNote.draw(tft,TFT_YELLOW);
    break;
    case 3:
    quaterNote.draw(tft,TFT_CYAN);
    break;
    case 4:
    eighthNote.draw(tft,TFT_GREEN);
    break;
    case 5:
    sixteenthNote.draw(tft,buttonColor);
    break;
    case 6:
    SaveButton.draw(tft,buttonColor);
    break; 
    case 7:
    Rewind.draw(tft,buttonColor);
    break;
    case 8:
    StartStopButton.draw(tft,buttonColor);  
    break;
    case 9:
    TempMinusButton.draw(tft,buttonColor);                 
    break;
    case 10:
    TempPlusButton.draw(tft,buttonColor);
    break;
    case 11:
    ClearButton.draw(tft,buttonColor);
    break;
    case 12:
    CopyButton.draw(tft,buttonColor);
    break;
    case 13:
    ScrollUp.draw(tft,buttonColor);
    break;
    case 14:
    PasteButton.draw(tft,buttonColor);
    break;
    case 15:
    ScrollDown.draw(tft,buttonColor);
    break;
    }
  touched=0;
  }
  if (tp.z > MINPRESSURE) {
    GetTouchPoints;
    previousMillis = currentMillis;
if(pat>7){
  if(holeNote.contains(x,y)){
      tft.setCursor(90, 290);
      tft.print("Hole Note   ");
      noteLen=16;
      touched=1;
      holeNote.draw(tft,buttonTouched);
      return;
  }
  if(halfNote.contains(x,y)){
      tft.setCursor(90, 290);
      tft.print("Halv Note   ");
      noteLen=8;
      touched=2;
      halfNote.draw(tft,buttonTouched);
      return;    
  }
  if(quaterNote.contains(x,y)){
      tft.setCursor(90, 290);
      tft.print("Quater Note ");
      noteLen=4;
      touched=3;
      quaterNote.draw(tft,buttonTouched);
      return;          
  }    
  if(eighthNote.contains(x,y)){
      tft.setCursor(90, 290);
      tft.print("Eighth Note ");
      noteLen=2;
      touched=4;
      eighthNote.draw(tft,buttonTouched);
      return;    
  }
  if(sixteenthNote.contains(x,y)){
      tft.setCursor(90, 290);
      tft.print("Sixteen Note");
      noteLen=1;
      touched=5;
      sixteenthNote.draw(tft,buttonTouched);
      return;          
  }
}
    if (Setup.contains(x,y)){
      goSetup();
      patternMode();
      return;  
    }
    if (Rewind.contains(x, y)) 
    {
      if (!mode&&tick!=-1) {
        tft.fillRect((tick) * 16 + 12, 263, 14, 17, TFT_BLUE);
        tft.drawRect((tick) * 16 + 11, 15, 16, 266, TFT_WHITE);
        tft.setCursor(16 + (tick * 16), 269);
        tft.print(tick, HEX);
        tick = -1;
        tft.fillRect(12, 263, 14, 17, TFT_BLACK);
        tft.drawRect(11, 15, 16, 266, TFT_RED);
        tft.setCursor(16, 269);
        tft.print(0, HEX);
      }
      else if(mode)
      {
        tick = -1;
        pos = 1;
        tft.fillRect(280,20,30,260,TFT_BLUE);
        drawSong();
        printSongPart();
      }
    Rewind.draw(tft,buttonTouched);
    touched=7;
    return;
    }
    if (StartStopButton.contains(x, y)) {
      play = !play;  //Start/Stop button
      tft.fillRect(455, 5, 20, 20 ,TFT_BLUE);
      StartStopButton.draw(tft,buttonTouched);
      touched=8;
      return;
    }
    if (TempMinusButton.contains(x, y)) { //tempo buttons
        tempo -= 1;
        tft.setCursor(361, 150 );
        Format(tempo);
        tft.print(tempo);
        TempMinusButton.draw(tft,buttonTouched);
        touched=9;
        return;
    }
    if (TempPlusButton.contains(x, y)) { //tempo buttons
        tempo += 1;
        tft.setCursor(361, 150 );
        Format(tempo);
        tft.print(tempo);
        TempPlusButton.draw(tft,buttonTouched);
        touched=10;
        return;
    }    
    if (ClearButton.contains(x, y)) {
      if (!mode) // clear pattern
      { for (slope = 0; slope < 13; slope++) {
          instrument[slope][pat] = 0;
        }
        for (xx = 14; xx < 270; xx += 16)
        {
          for (yy = 17; yy < 247; yy += 19)
          {
            tool = true;
            DRAW(TFT_RED);
          }
          if(pat>7){
            for(slope=0;slope<16;slope++){
            for(slope2=0;slope2<12;slope2++){
              bassNoteOff[slope2][slope][pat]=0; 
            }
            }
          talkMIDI(0xB0, 0x7b, 127); //all notes channel 1 off
          }
        }
      } else //clear song
      { for (slope = 0; slope < 300; slope++) {
                  pattern[slope] = 0;
          }
        for (xx = 14; xx < 271; xx += 16)
        {
          for (yy = 17; yy < 266; yy += 19)
          {
            tool = true;
            DRAW(TFT_RED);
          }
        }
      }
      ClearButton.draw(tft,buttonTouched);
      touched=11;
      return;
    }
    if (SaveButton.contains(x, y)) // save pattern / song
    {
      if (!mode) {
        savePat();
       } else {
        saveSong();
      } 
      SaveButton.draw(tft,buttonTouched);
      touched=6;
      return;
    }

    if (CopyButton.contains(x, y)) //copy pattern
    {
     if(!mode){
      copyPat = true;
      CopyButton.draw(tft,buttonTouched);
      touched=12;
      return;
    }
else{ //or up in song mode
      {
      pos+=1;
      if(pos==300)pos=1;
        drawSong();
        printSongPart();
        ScrollUp.draw(tft,buttonTouched);
        touched=13;  
        return;    
    }    
}
    }
    if (PasteButton.contains(x, y)) //paste pattern
    {
      if(!mode){
      patCopy();
      pat = nextPat = copyTo;
      drawPattern();
      tft.setCursor(10, 305);
      tft.print("Next Pat:");
      PasteButton.draw(tft,buttonTouched);
      touched=14;
      return;
    }
    else // or down in song mode
    {
        pos-=1;
        if(pos==0)pos=1;   
        drawSong();
        tft.fillRect(280,20,30,260,TFT_BLUE);
        printSongPart();
        ScrollDown.draw(tft,buttonTouched);
        touched=15;
        return;     
    }
    }
    if (SongButton.contains(x, y)) //song mode
    {
      play = false;
      mode = !mode;
      if (!mode) {
        patternMode();
      }
      else {
        songMode();
        }
      
      return;
    }
    if ((y > 260) & (y < 282)) {
      for (slope = 0; slope < 16; slope++) {
        if ((x > (slope * 16 + 10) & (x < ((slope * 16) + 26)))) //read pattern
        { if (!mode) {
            if (copyPat) {
              copyTo = slope;
              printCopyTo();
            }
            if ((!play) && (!copyPat)) {
              pat = slope;
              drawPattern();
              playNotes();
            }
            nextPat = slope;
            return;
          } else {
            shift = slope;
            xx = (slope * 16) + 14;
            yy=264;
            tool = ((pattern[pos] >> shift) & (1));
            pattern[pos] = ((1 << shift) ^ (pattern[pos]));
            DRAW(TFT_RED);
            return;
          }
        }
      }
    }
   for (int i = 0; i < 12; i++)//play note
    {
     if (keys[i].contains (x,y))
      {
      playNote=1;  
      pressedKey=i;
      if (pat < 8) {
        noteOn(9, patch[i], 60);
        } else if(pat >7 && pat <12) {
        noteOn(0, bass[i], 60);
        }else{
        noteOn(1,bass[i]+12, 60);
        }
      return;
        }
     }
    if ( x > 10 && x < 266 && y > 17 && y < 280 ) {
        for (slope = 0; slope < 13; slope++) {
          if ((y > (245 - (slope * 19))) && (y < (265 - (slope * 19)))) {
            inst = slope;
            posy = slope;
            yy = 245 - (slope * 19);
            break;
          }
        }
        for (slope = 0; slope < 16; slope++)
          if ((x > (slope * 16) + 10) && (x < (slope * 16) + 26)) {
            shift = slope;
            xx = (slope * 16) + 14;
            if(!mode) { //plot in pattern mode
              if(pat>7){
                if(inst!=12){
                switch(noteLen){
                  case 16:
                  col = TFT_MAGENTA;
                  bassNoteLen[inst][shift][pat]=16;
                  offBeat(16,pat,inst,shift);
                  break;
                  case 8:
                  col = TFT_YELLOW;
                  bassNoteLen[inst][shift][pat]=8;
                  offBeat(8,pat,inst,shift);                  
                  break;
                  case 4:
                  col = TFT_CYAN;
                  bassNoteLen[inst][shift][pat]=4;
                  offBeat(4,pat,inst,shift);
                  break;
                  case 2:
                  col = TFT_GREEN;
                  bassNoteLen[inst][shift][pat]=2;
                  offBeat(2,pat,inst,shift);
                  break;
                  case 1:
                  col = TFT_RED;
                  bassNoteLen[inst][shift][pat]=1;
                  offBeat(1,pat,inst,shift);                  
                  break;
                }
                }else col = TFT_RED;
               }else col = TFT_RED;
              tool = ((instrument[inst][pat] >> shift) & (1));
              instrument[inst][pat] = ((1 << shift) ^ (instrument[inst][pat]));
              if(tool){
                // del Note
                offBeat(bassNoteLen[inst][shift][pat],pat,0,shift);
                noteOff(0, bass[inst], 60);
              }
              DRAW(col);
            }
            else {  //plot in song mode
                tool = ((pattern[posy+pos+1] >> shift) & (1));
                pattern[posy+pos+1] = ((1 << shift) ^ (pattern[posy+pos+1]));
                DRAW(TFT_RED);
            }
          }
        return;
    }
  }
}
  yield();
}
void offBeat(int OffStep, int Pattern,int Note, int Beat){
      OffStep=Beat+OffStep; 
      if(OffStep>15) OffStep-=16;
      bassNoteOff[Note][OffStep][Pattern]=bass[Note];
}
void readRot(){
   currentPosition1 = digitalRead(CLK1);
   currentPosition2 = digitalRead(CLK2);
   currentPosition3 = digitalRead(CLK3);
   currTime=millis();
   if (currTime - prevTime >= interval*3) pressed = false;
   if(!digitalRead(rotButton1)&&!pressed)
     {
       rotMode1 += 1;
       if(rotMode1>2)rotMode1=0;
if(rotMode1 == 0){
  tft.drawRect(320,117,129,8,TFT_RED);
  tft.drawRect(320,127,129,8,TFT_WHITE);
  tft.drawRect(320,137,129,8,TFT_WHITE);  
}
if(rotMode1 == 1){
  tft.drawRect(320,117,129,8,TFT_WHITE);  
  tft.drawRect(320,127,129,8,TFT_RED);
  tft.drawRect(320,137,129,8,TFT_WHITE);  
}
if(rotMode1 == 2){
  tft.drawRect(320,117,129,8,TFT_WHITE);  
  tft.drawRect(320,127,129,8,TFT_WHITE);
  tft.drawRect(320,137,129,8,TFT_RED);    
}
       prevTime = currTime;
       pressed = true; 
     }
   if(!digitalRead(rotButton2)&&!pressed)
     {
       rotMode2 += 1;
       if(rotMode2>2)rotMode2=0;       
if(rotMode2 == 0){
  tft.drawRect(320,77,129,8,TFT_RED);
  tft.drawRect(320,87,129,8,TFT_WHITE);
  tft.drawRect(320,97,129,8,TFT_WHITE); 
}
if(rotMode2 == 1){
  tft.drawRect(320,77,129,8,TFT_WHITE);
  tft.drawRect(320,87,129,8,TFT_RED);  
  tft.drawRect(320,97,129,8,TFT_WHITE);   
}
if(rotMode2 == 2){
  tft.drawRect(320,77,129,8,TFT_WHITE);
  tft.drawRect(320,87,129,8,TFT_WHITE);  
  tft.drawRect(320,97,129,8,TFT_RED);   
}       
       prevTime = currTime;
       pressed = true; 
     }
   if(!digitalRead(rotButton3)&&!pressed)
     {
       rotMode3 += 1;
       if(rotMode3>2)rotMode3=0;       
if(rotMode3== 0){
  tft.drawRect(320,37,129,8,TFT_RED);
  tft.drawRect(320,47,129,8,TFT_WHITE);
  tft.drawRect(320,57,129,8,TFT_WHITE);  
}
if(rotMode3 == 1){
  tft.drawRect(320,37,129,8,TFT_WHITE);
  tft.drawRect(320,47,129,8,TFT_RED);
  tft.drawRect(320,57,129,8,TFT_WHITE);  
}
if(rotMode3 == 2){
  tft.drawRect(320,37,129,8,TFT_WHITE);
  tft.drawRect(320,47,129,8,TFT_WHITE);
  tft.drawRect(320,57,129,8,TFT_RED);  
}    
       prevTime = currTime;
       pressed = true; 
     }          
   if (currentPosition1 != lastPosition1){
     (digitalRead(DT1) != currentPosition1)?val++:val--;
        if(rotMode1 == 0){
        Vol1 = Value(Vol1+val);
        talkMIDI(0xB1, 0x07, Vol1);
        tft.setCursor(455,118);
        Format(Vol1);
        tft.print(Vol1);
        DrawValue(Vol1,118);
   }
        if(rotMode1 == 1){
        reverb1 = Value(reverb1+val);
        talkMIDI(0xB1, 0x5b, reverb1);
//        talkMIDI(0xB1, 0x0c, reverb1);
        tft.setCursor(455,128);
        Format(reverb1);
        tft.print(reverb1);
        DrawValue(reverb1,128);
   }
   if(rotMode1 == 2){
        pan1 = Value(pan1+val);
        talkMIDI(0xB1, 0x0A, pan1);
        tft.setCursor(455,138);
        Format(pan1-64);
        tft.print(pan1-64);
        DrawValuePan(pan1,138);
   }
           lastPosition1 = currentPosition1; 
           val = 0;
   }
   if (currentPosition2 != lastPosition2){
     (digitalRead(DT2) != currentPosition2)?val++:val--;
        if(rotMode2 == 0){
        Vol2 = Value(Vol2+val);
        talkMIDI(0xB0, 0x07, Vol2);
        tft.setCursor(455,78);
        Format(Vol2);
        tft.print(Vol2);
        DrawValue(Vol2,78);
   }
        if(rotMode2 ==  1){
        reverb2 = Value(reverb2+val);
//        talkMIDI(0xE0, reverb1, reverb2);
        talkMIDI(0xB0, 0x5b, reverb2);        
//        talkMIDI(0xB0, 0x0c, reverb2);
        tft.setCursor(455,88);
        Format(reverb2);
        tft.print(reverb2);
        DrawValue(reverb2,88);
   }
        if(rotMode2 == 2){
        pan2 = Value(pan2+val);
        talkMIDI(0xB0, 0x0A, pan2);
        tft.setCursor(455,98);
        Format(pan2-64);
        tft.print(pan2-64);
        DrawValuePan(pan2,98);     
   }
        lastPosition2 = currentPosition2; 
        val = 0;   
   }
   if (currentPosition3 != lastPosition3){
     (digitalRead(DT3) != currentPosition3)?val++:val--;
        if(rotMode3 == 0){
        Vol3 = Value(Vol3+val);
        talkMIDI(0xB9, 0x07, Vol3);
        tft.setCursor(455,38);
        Format(Vol3);
        tft.print(Vol3);
        DrawValue(Vol3,38);
   }
        if(rotMode3 == 1){
        reverb3 = Value(reverb3+val);
        talkMIDI(0xB9, 0x5b, reverb3);
//        talkMIDI(0xB9, 0x0c, reverb3);
        tft.setCursor(455,48);
        Format(reverb3);
        tft.print(reverb3);
        DrawValue(reverb3,48);
   }
        if(rotMode3 == 2){
        pan3 = Value(pan3+val);
        talkMIDI(0xB9, 0x0A, pan3);
        tft.setCursor(455,58);
        Format(pan3-64);
        tft.print(pan3-64);
        DrawValuePan(pan3,58);     
   }
        lastPosition3 = currentPosition3; 
        val = 0;   
   }
} 
int Value(int value){
        if (value > 127) value = 127;
        if (value < 0 ) value = 0;
        return value;
}
