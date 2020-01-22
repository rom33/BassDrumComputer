void readTouch() {
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    buttonReverse();
    // *** get touch point
    tp = myTouch.getPoint();
    if (tp.z > MINPRESSURE) {
      previousMillis = currentMillis;
      tp = myTouch.getPoint();
      xx = map(tp.x, TS_MINX, TS_MAXX, 480, 0);
      yy = map(tp.y, TS_MINY, TS_MAXY, 320, 0);
      // *** raster touch?
      if ( xx > 6 && xx < 261 && yy > 6 && yy < 257) {
        stp = abs((xx - 5) / 16);
        note = 12 - (abs((yy - 5) / 19));
        xDraw = stp * 16 + 12;
        yDraw = (12 - note) * 19 + 12;
        if (instSelect == 0) {
          tool = ((drum[note][pat] >> stp) & (1));
          drum[note][pat] = ((1 << stp) ^ (drum[note][pat]));
          DRAW(buttonColor[0]);
        }
        if (instSelect == 1) {
          tool = ((bass[note][pat] >> stp) & (1));
          bass[note][pat] = ((1 << stp) ^ (bass[note][pat]));
          DRAW(buttonColor[0]);
        }
        if (instSelect == 2) {
          tool = ((lead[note][pat] >> stp) & (1));
          lead[note][pat] = ((1 << stp) ^ (lead[note][pat]));
          DRAW(buttonColor[0]);
        }        
        return;
      }
      // *** note key touch?
      for (slope = 0; slope < 12; slope++)//play note
      {
        if (keys[slope].contains (xx, yy)) {
          if (instSelect == 0) noteOn(9, drumSet[slope], 60);
          if (instSelect == 1) noteOn(0, bassSet[slope], 60);
          if (instSelect == 2) noteOn(1, leadSet[slope], 60);
          return;
        }
      }
      // *** start stop touched?
      if (StartStopButton.contains(xx, yy)) {
        play = !play;
        StartStopButton.draw(tft, buttonColor[1]);
        touched = 10;
        return;
      }
      // **** rewind button?
      if (Rewind.contains(xx, yy))
      {
        Rewind.draw(tft, buttonColor[1] && tick != 0);
        stp = tick - 1;// + (!play * 1);
        tick = 0;
        touched = 9;
        drawRec();
        return;
      }
      // *** pattern select
      for (slope = 0; slope < 4; slope++) {
        if (ButtPat[slope].contains (xx, yy)) {
          patRow1Old = patRow1;
          patRow1 = slope;
          nextPat = patRow1 + patRow2 * 4;
          ButtPat[patRow1Old].draw(tft, buttonColor[0]);
          ButtPat[patRow1].draw(tft, buttonColor[1]);
          return;
        }
      }
      for (slope = 0; slope < 4; slope++) {
        if (ButtPat[slope + 4].contains (xx, yy)) {
          patRow2Old = patRow2;
          patRow2 = slope;
          nextPat = patRow1 + patRow2 * 4;
          ButtPat[patRow2Old + 4].draw(tft, buttonColor[0]);
          ButtPat[patRow2 + 4].draw(tft, buttonColor[1]);
          return;
        }
      }
      for (slope = 0; slope < 4; slope++) {
        if (ButtInst[slope].contains (xx, yy)) {
          instSelectOld = instSelect;
          instSelect = slope;
          ButtInst[instSelectOld].draw(tft, buttonColor[0]);
          ButtInst[instSelect].draw(tft, buttonColor[1]);
          return;
        }
      }
    }// end of touch
  }
  yield();
}
void patternSelect() {

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
        StartStopButton.draw(tft, buttonColor[0]);
        break;
      case 17:
        TempMinusButton.draw(tft, buttonColor[0]);
        break;
      case 16:
        TempPlusButton.draw(tft, buttonColor[0]);
        break;
      case 11:
        ClearButton.draw(tft, buttonColor[0]);
        break;
      case 12:
        CopyButton.draw(tft, buttonColor[0]);
        break;
      case 13:
        ScrollUp.draw(tft, buttonColor[0]);
        break;
      case 14:
        PasteButton.draw(tft, buttonColor[0]);
        break;
      case 15:
        ScrollDown.draw(tft, buttonColor[0]);
        break;
    }
    touched = 0;
  }
}
