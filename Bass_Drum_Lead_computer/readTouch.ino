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
          ButtPat[patRow1].draw(tft, buttonColor[tool]);
          break;
        case 2:
          ButtPat[patRow2 + 4].draw(tft, buttonColor[tool]);
          break;
        case 3:
          ButtInstPlay[playtrack - 1].draw(tft, buttonColor[tool]);
          break;
      }
    }
    buttonReverse();
    // *** get touch point
    if (tp.z > MINPRESSURE) {
      GetTouchPoints;
      xx = map(tp.x, TS_MINX, TS_MAXX, 480, 0);
      yy = map(tp.y, TS_MINY, TS_MAXY, 320, 0);
      // *** save touched?
      if (SaveButton.contains(xx, yy))
      {
        savePat();
        SaveButton.draw(tft, buttonColor[1]);
        touched = 6;
        return;
      }
      // *** loop touched?
      if (LoopButton.contains(xx, yy)) {
        loopMode = !loopMode;
        LoopButton.draw(tft, buttonColor[loopMode]);
      }
      // *** loop len touched?
      if (LoopLen[0].contains(xx, yy)) {
        loopLen += 1;
        if (loopLen > 3)loopLen = 1;
        LoopLen[loopLen - 1].draw(tft, buttonColor[1]);
        touched = 16;
      }
      // *** clear touched?
      if (ClearButton.contains(xx, yy)) {
        for (slope = 0; slope < 13; slope++) {
          instrument[instSelect][slope][16] = instrument[instSelect][slope][pat];
          instrument[instSelect][slope][pat] = 0;
        }
        nextPat = pat;
        pat = 16;
        drawPattern();
        ClearButton.draw(tft, buttonColor[1]);
        for (slope = 0; slope < 13; slope++) {
          instrument[instSelect][slope][16] = 0;
        }
        touched = 12;
      }
      // *** copy touched?
      if (CopyButton.contains(xx, yy))
      {
        CopyButton.draw(tft, buttonColor[1]);
        copyPat = pat;
        return;
      }
      // *** paste touched?
      if (PasteButton.contains(xx, yy))
      {
        for (slope = 0; slope < 13; slope++) {
          instrument[instSelect][slope][nextPat] = instrument[instSelect][slope][copyPat];
        }
        drawPattern();
        PasteButton.draw(tft, buttonColor[1]);
        touched = 13;
        pat = 16;
        return;
      }
      // *** tempo minus touched?
      if (TempMinusButton.contains(xx, yy)) {
        tempo -= 1;
        tft.setCursor(400, 150 );
        Format(tempo);
        tft.print(tempo);
        TempMinusButton.draw(tft, buttonColor[1]);
        touched = 10;
        return;
      }
      // *** tempo plus touched?
      if (TempPlusButton.contains(xx, yy)) {
        tempo += 1;
        tft.setCursor(400, 150 );
        Format(tempo);
        tft.print(tempo);
        TempPlusButton.draw(tft, buttonColor[1]);
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
        DRAW(buttonColor[0]);
        return;
      }
      // *** note key touch?
      for (slope = 0; slope < 12; slope++)//play note
      {
        if (keys[slope].contains (xx, yy)) {
          noteOn(9 - instSelect, instSet[instSelect][slope], 60);
          return;
        }
      }
      // *** start stop touched?
      if (StartStopButton.contains(xx, yy)) {
        play = !play;
        StartStopButton.draw(tft, buttonColor[play]);
        return;
      }
      // **** rewind button?
      if (Rewind.contains(xx, yy))
      {
        Rewind.draw(tft, buttonColor[1]);
        touched = 9;
        if (tick > 0) {
          stp = tick - 1;
          tick = 0;
          drawRec();
          return;
        }
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
                ButtPat[patRow1].draw(tft, buttonColor[1]);
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
              ButtPat[patRow2 + 4].draw(tft, buttonColor[1]);
              toggle = 2;
            }
            return;
          }
        }
        // *** track play touched?
        for (slope = 0; slope < 3; slope++) {
          if (ButtInstPlay[slope].contains (xx, yy)) {
            ButtInstPlay[slope].draw(tft, buttonColor[1]);
            playtrack = slope + 1;
            toggle = 3;
          }
        }
      }
      // *** instrument select touched?
      for (slope = 0; slope < 3; slope++) {
        if (ButtInst[slope].contains (xx, yy)) {
          instSelectOld = instSelect;
          instSelect = slope;
          if (instSelect != instSelectOld) {
            ButtInst[instSelect].draw(tft, buttonColor[1]);
            ButtInst[instSelectOld].draw(tft, buttonColor[0]);
            DrawOrNot();
          }
          return;
        }
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
        holeNote.draw(tft, TFT_MAGENTA);
        break;
      case 2:
        halfNote.draw(tft, TFT_YELLOW);
        break;
      case 3:
        quaterNote.draw(tft, TFT_CYAN);
        break;
      case 4:
        eighthNote.draw(tft, TFT_GREEN);
        break;
      case 5:
        sixteenthNote.draw(tft, buttonColor[0]);
        break;
      case 6:
        SaveButton.draw(tft, buttonColor[0]);
        break;
      case 9:
        Rewind.draw(tft, buttonColor[0]);
        break;
      case 10:
        TempMinusButton.draw(tft, buttonColor[0]);
        break;
      case 11:
        TempPlusButton.draw(tft, buttonColor[0]);
        break;
      case 12:
        ClearButton.draw(tft, buttonColor[0]);
        break;
      case 13:
        CopyButton.draw(tft, buttonColor[0]);
        PasteButton.draw(tft, buttonColor[0]);
        break;
      case 14:
        ScrollUp.draw(tft, buttonColor[0]);
        break;
      case 15:
        ScrollDown.draw(tft, buttonColor[0]);
        break;
      case 16:
        LoopLen[loopLen - 1].draw(tft, buttonColor[0]);
        break;
    }
    touched = 0;
  }
}
void readRot() {
  for (slope = 0; slope < 3; slope++) {
    currentPosition[slope] = digitalRead(CLK[slope]);
    currTime = millis();
    if (currTime - prevTime >= interval * 3) pressed = false;
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
