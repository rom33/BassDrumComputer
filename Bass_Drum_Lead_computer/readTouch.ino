void readTouch() {
  readRot();
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    GetTouchPoints;
    if (toggle > 0 && play) {
      tool = !tool;
      switch (toggle) {
        case 1:
          ButtPat[patRow1].draw(tft, buttonColor[!tool]);
          break;
        case 2:
          ButtPat[patRow2 + 4].draw(tft, buttonColor[!tool]);
          break;
        case 3:
          ButtInstPlay[playtrack - 1].draw(tft, buttonColor[!tool]);
          break;
      }
    }
    buttonReverse();
    // *** get touch point
    if (tp.z > MINPRESSURE) {
      GetTouchPoints;
      xx = map(tp.x, TS_MINX, TS_MAXX, 480, 0);
      yy = map(tp.y, TS_MINY, TS_MAXY, 320, 0);
      // *** setup touched?
      /*    if (Setup.contains(x,y)){
            goSetup();
            patternMode();
            return;
          }*/
      // *** save touched?
      if (SaveButton.contains(xx, yy)) {
        savePat();
        SaveButton.draw(tft, buttonColor[0]);
        touched = 6;
        return;
      }
      // *** loop touched?
      if (LoopButton.contains(xx, yy)) {
        loopMode = !loopMode;
        LoopButton.draw(tft, buttonColor[!loopMode]);
        return;
      }
      // *** loop len touched?
      if (LoopLen[0].contains(xx, yy)) {
        loopLen += 1;
        if (loopLen > 3)loopLen = 1;
        LoopLen[loopLen - 1].draw(tft, buttonColor[0]);
        touched = 16;
        return;
      }
      // *** clear touched?
      if (ClearButton.contains(xx, yy)) {
        for (slope = 0; slope < 13; slope++) {
          instrument[instSelect][slope][16] = instrument[instSelect][slope][pat];
          instrument[instSelect][slope][pat] = 0;
          for (slope2 = 0; slope2 < 16; slope2++) {
            instNoteLen[instSelect][slope][pat][slope2] = 0;
          }
        }
        nextPat = pat;
        pat = 16;
        drawPattern();
        ClearButton.draw(tft, buttonColor[0]);
        for (slope = 0; slope < 13; slope++) {
          instrument[instSelect][slope][16] = 0;
        }
        touched = 12;
        return;
      }
      // *** copy touched?
      if (CopyButton.contains(xx, yy)) {
        CopyButton.draw(tft, buttonColor[0]);
        copyPat = pat;
        return;
      }
      // *** paste touched?
      if (PasteButton.contains(xx, yy)) {
        for (slope = 0; slope < 13; slope++) {
          instrument[instSelect][slope][nextPat] = instrument[instSelect][slope][copyPat];
          for (slope2 = 0; slope2 < 16; slope2++) {
            instNoteLen[instSelect][slope][nextPat][slope2] = instNoteLen[instSelect][slope][copyPat][slope2];
          }
        }
        drawPattern();
        PasteButton.draw(tft, buttonColor[0]);
        touched = 13;
        pat = 16;
        return;
      }
      // *** tempo minus touched?
      if (TempMinusButton.contains(xx, yy)) {
        tempo -= 1;
        tft.setCursor(400, 195 );
        Format(tempo);
        tft.print(tempo);
        TempMinusButton.draw(tft, buttonColor[0]);
        touched = 10;
        return;
      }
      // *** tempo plus touched?
      if (TempPlusButton.contains(xx, yy)) {
        tempo += 1;
        tft.setCursor(400, 195 );
        Format(tempo);
        tft.print(tempo);
        TempPlusButton.draw(tft, buttonColor[0]);
        touched = 11;
        return;
      }
      // *** raster touch?
      if ( xx > 6 && xx < 261 && yy > 6 && yy < 257) {
        stp = abs((xx - 5) / 16);
        note = 12 - (abs((yy - 5) / 19));
        xDraw = stp * 16 + 12;
        yDraw = (12 - note) * 19 + 12;
        tool = ((instrument[instSelect][note][pat] >> stp) & (1));
        instrument[instSelect][note][pat] = ((1 << stp) ^ (instrument[instSelect][note][pat]));
        if (instNoteLen[instSelect][note][pat][stp] > 0) {
          switchNoteLen(instNoteLen[instSelect][note][pat][stp], note, pat);
        }
        instNoteLen[instSelect][note][pat][stp] = noteLen;
        switchNoteLen(noteLen, note, pat);
        DRAW(buttonColor[noteLen]);
        return;
      }
      // *** note key touch?
      for (slope = 0; slope < 12; slope++) {
        if (keys[slope].contains (xx, yy)) {
          noteOn(9 - instSelect, instSet[instSelect][slope], 60);
          return;
        }
      }
      // *** start stop touched?
      if (StartStopButton.contains(xx, yy)) {
        play = !play;
        StartStopButton.draw(tft, buttonColor[!play]);
        return;
      }
      // **** rewind button?
      if (Rewind.contains(xx, yy)) {
        Rewind.draw(tft, buttonColor[0]);
        touched = 9;
        if (tick > 0) {
          stp = tick - 1;
          tick = 0;
          drawRec();
        }
        return;
      }
      // *** pattern select
      if (toggle == 0) {
        if (!loopMode) {
          for (slope = 0; slope < 4; slope++) {
            if (ButtPat[slope].contains (xx, yy)) {
              patRow1Old = patRow1;
              patRow1 = slope;
              if (patRow1 != patRow1Old) {
                nextPat = patRow1 + patRow2 * 4;
                ButtPat[patRow1].draw(tft, buttonColor[0]);
                toggle = 1;
              }
              return;
            }
          }
        }
        for (slope = 0; slope < 4; slope++) {
          if (ButtPat[slope + 4].contains (xx, yy)) {
            patRow2Old = patRow2;
            patRow2 = slope;
            if (patRow2 != patRow2Old) {
              pat = nextPat;
              nextPat = patRow1 + patRow2 * 4;
              ButtPat[patRow2 + 4].draw(tft, buttonColor[0]);
              toggle = 2;
            }
            return;
          }
        }
        // *** track play touched?
        for (slope = 0; slope < 3; slope++) {
          if (ButtInstPlay[slope].contains (xx, yy)) {
            ButtInstPlay[slope].draw(tft, buttonColor[0]);
            playtrack = slope + 1;
            toggle = 3;
            return;
          }
        }
      }
      // *** instrument select touched?
      for (slope = 0; slope < 3; slope++) {
        if (ButtInst[slope].contains (xx, yy)) {
          instSelectOld = instSelect;
          instSelect = slope;
          if (instSelect == 0) {
            NoteLen[noteLen].draw(tft, buttonColor[1]);
            NoteLen[1].draw(tft, buttonColor[0]);
          }
          if (instSelect != instSelectOld) {
            ButtInst[instSelect].draw(tft, buttonColor[0]);
            ButtInst[instSelectOld].draw(tft, buttonColor[1]);
            for (slope = 0; slope < 12; slope++) {
              if (instSelect == 0 ? printInst = drumSounds[instSet[0][slope] - 27] : printInst =  (instSet[instSelect][1] / 12 + octave[instSelect] / 12) + noteName[slope]);
              keys[slope].drawKey(tft, printInst);
            }
            if (instSelect == 0) {
              tft.fillRect(388, 170, 66, 8, TFT_BLUE);
            }else {
              tft.setCursor(388, 170);
              tft.print(Sounds[Sound[instSelect]]);
            }
            if (instSelect == 2) {
              tft.setCursor(388, 170);
              tft.print(Sounds[Sound[2]]);
            }
            DrawOrNot();
          }
          return;
        }
      }
      if (instSelect > 0) {
        // *** note length touched?
        for (slope = 1; slope < 6; slope++) {
          if (NoteLen[slope].contains(xx, yy)) {
            NoteLen[noteLen].draw(tft, buttonColor[noteLen]);
            NoteLen[slope].draw(tft, buttonColor[0]);
            noteLen = slope;
            return;
          }
        }
        if (OctaveUp.contains (xx, yy)) {
          octave[instSelect] += 12;
          if (octave[instSelect] > 120) octave[instSelect] = 120;
          OctaveUp.draw(tft, buttonColor[0]);
          for (slope = 0; slope < 12; slope++) {
            printInst =  (instSet[instSelect][1] / 12 + octave[instSelect] / 12) + noteName[slope];
            keys[slope].drawKey(tft, printInst);
          }
          touched = 14;
          return;
        }
        if (OctaveDown.contains (xx, yy)) {
          octave[instSelect] -= 12;
          if (octave[instSelect] < 0) octave[instSelect] = 0;
          OctaveDown.draw(tft, buttonColor[0]);
          for (slope = 0; slope < 12; slope++) {
            printInst =  (instSet[instSelect][1] / 12 + octave[instSelect] / 12) + noteName[slope];
            keys[slope].drawKey(tft, printInst);
          }
          touched = 15;
          return;
        }
      }
    if (InstPlus.contains (xx, yy)) {
      touched = 1;
      InstPlus.draw(tft, buttonColor[0]);
      Sound[instSelect] +=1;
      if(Sound[instSelect] > 127) Sound[instSelect] = 127;
      tft.setCursor(388, 170);
      tft.print(Sounds[Sound[instSelect]]); 
      talkMIDI(0xC9 - instSelect, Sound[instSelect], 0);    
    }
    if (InstMinus.contains (xx, yy)) {
      touched = 2;
      InstMinus.draw(tft, buttonColor[0]);
      Sound[instSelect] -=1;
      if(Sound[instSelect] < 0) Sound[instSelect] = 0;
      tft.setCursor(388, 170);
      tft.print(Sounds[Sound[instSelect]]);
      talkMIDI(0xC9 - instSelect, Sound[instSelect], 0);
    }
    }// end of touch
  }
  yield();
}
void buttonReverse() {
  if (touched > 0)
  {
    switch (touched) {
      case 1:
        InstPlus.draw(tft, buttonColor[1]);
        break;
      case 2:
        InstMinus.draw(tft, buttonColor[1]);
        break;
      case 6:
        SaveButton.draw(tft, buttonColor[1]);
        break;
      case 9:
        Rewind.draw(tft, buttonColor[1]);
        break;
      case 10:
        TempMinusButton.draw(tft, buttonColor[1]);
        break;
      case 11:
        TempPlusButton.draw(tft, buttonColor[1]);
        break;
      case 12:
        ClearButton.draw(tft, buttonColor[1]);
        break;
      case 13:
        CopyButton.draw(tft, buttonColor[1]);
        PasteButton.draw(tft, buttonColor[1]);
        break;
      case 14:
        OctaveUp.draw(tft, buttonColor[1]);
        break;
      case 15:
        OctaveDown.draw(tft, buttonColor[1]);
        break;
      case 16:
        LoopLen[loopLen - 1].draw(tft, buttonColor[1]);
        break;
    }
    touched = 0;
  }
}
void readRot() {
  for (slope = 0; slope < 3; slope++) {
    currentPosition[slope] = digitalRead(CLK[slope]);
    currTime = millis();
    if (currTime - prevTime >= rotInterval) pressed = false;
    if (!digitalRead(rotButton[slope]) && !pressed)
    {
      rotMode[slope] += 1;
      if (rotMode[slope] > 2)rotMode[slope] = 0;
      if (rotMode[slope] == 0) {
        tft.drawRect(320, 117 - (slope * 40), 129, 8, TFT_RED);
        tft.drawRect(320, 127 - (slope * 40), 129, 8, TFT_WHITE);
        tft.drawRect(320, 137 - (slope * 40), 129, 8, TFT_WHITE);
      }
      if (rotMode[slope] == 1) {
        tft.drawRect(320, 117 - (slope * 40), 129, 8, TFT_WHITE);
        tft.drawRect(320, 127 - (slope * 40), 129, 8, TFT_RED);
        tft.drawRect(320, 137 - (slope * 40), 129, 8, TFT_WHITE);
      }
      if (rotMode[slope] == 2) {
        tft.drawRect(320, 117 - (slope * 40), 129, 8, TFT_WHITE);
        tft.drawRect(320, 127 - (slope * 40), 129, 8, TFT_WHITE);
        tft.drawRect(320, 137 - (slope * 40), 129, 8, TFT_RED);
      }
      prevTime = currTime;
      pressed = true;
    }
    if (currentPosition[slope] != lastPosition[slope]) {
      (digitalRead(DT[slope]) != currentPosition[slope]) ? val++ : val--;
      if (rotMode[slope] == 0) {
        Vol[slope] = Value(Vol[slope] + val);
        talkMIDI(0xB7 + slope, 0x07, Vol[slope]);
        tft.setCursor(455, 118 - (slope * 40));
        Format(Vol[slope]);
        tft.print(Vol[slope]);
        DrawValue(Vol[slope], 118 - (slope * 40));
      }
      if (rotMode[slope] == 1) {
        reverb[slope] = Value(reverb[slope] + val);
        talkMIDI(0xB7 + slope, 0x5b, reverb[slope]);
        tft.setCursor(455, 128 - (slope * 40));
        Format(reverb[slope]);
        tft.print(reverb[slope]);
        DrawValue(reverb[slope], 128 - (slope * 40));
      }
      if (rotMode[slope] == 2) {
        pan[slope] = Value(pan[slope] + val);
        talkMIDI(0xB7 + slope, 0x0A, pan[slope]);
        tft.setCursor(455, 138 - (slope * 40));
        Format(pan[slope] - 64);
        tft.print(pan[slope] - 64);
        DrawValuePan(pan[slope], 138 - (slope * 40));
      }
      lastPosition[slope] = currentPosition[slope];
      val = 0;
    }
  }
}
int Value(int value) {
  if (value > 127) value = 127;
  if (value < 0 ) value = 0;
  return value;
}
void DrawValuePan(int value, int height) {
  tft.drawFastVLine(320 + value, height, 6, TFT_WHITE);
  tft.drawFastVLine(321 + value, height, 6, TFT_RED);
  tft.drawFastVLine(322 + value, height, 6, TFT_WHITE);
}
void switchNoteLen(int LEN, int NOTE, int PAT) {
  switch (LEN) {
    case 5:
      instNoteOff[instSelect][NOTE][PAT] = ((1 << 16 + stp) ^ (instNoteOff[instSelect][NOTE][PAT]));
      break;
    case 4:
      instNoteOff[instSelect][NOTE][PAT] = ((1 << 8 + stp) ^ (instNoteOff[instSelect][NOTE][PAT]));
      break;
    case 3:
      instNoteOff[instSelect][NOTE][PAT] = ((1 << 4 + stp) ^ (instNoteOff[instSelect][NOTE][PAT]));
      break;
    case 2:
      instNoteOff[instSelect][NOTE][PAT] = ((1 << 2 + stp) ^ (instNoteOff[instSelect][NOTE][PAT]));
      break;
    case 1:
      instNoteOff[instSelect][NOTE][PAT] = ((1 << 1 + stp) ^ (instNoteOff[instSelect][NOTE][PAT]));
      break;
  }
}
