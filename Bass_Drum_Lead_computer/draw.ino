void welcomeScreen() {

}
void patternScreen() {
  // *** draw raster
  for (int x = 5; x < 262; x += 16) {
    tft.drawFastVLine(x, 5, 268, TFT_WHITE);
    tft.drawFastVLine(x + 1, 5, 268, TFT_WHITE);
  }
  for (int y = 5; y < 277; y += 19) {
    tft.drawFastHLine(5, y, 256, TFT_WHITE);
    tft.drawFastHLine(5, y + 1, 256, TFT_WHITE);
  }
  tft.fillRect(7, 254, 14, 17, TFT_BLACK);
  tft.drawRect(6, 6, 16, 266, TFT_RED);
  for (slope = 0; slope < 16; slope++) {
    tft.setCursor(11 + (slope * 16), 260);
    tft.print(slope, HEX);
    tft.setCursor(265, 12 );
    tft.print("Acc");
  }
  // *** draw keys
  for (slope = 0; slope < 12; slope++) {
    keys[slope].drawKey(tft);
    if (slope < 8) {
      if (slope == 0 || slope == 4 ? color = 0 : color = 1);
      ButtPat[slope].draw(tft, buttonColor[color]);
    }
    if (slope > 1 && slope < 6) {
      NoteLen[slope].draw(tft, buttonColor[slope]);
    }
  }
  NoteLen[1].draw(tft, buttonColor[0]);
  // *** draw buttons
  Setup.draw(tft, buttonColor[1]);
  StartStopButton.draw(tft, buttonColor[1]);
  Rewind.draw(tft, buttonColor[1]);
  TempMinusButton.draw(tft, buttonColor[1]);
  TempPlusButton.draw(tft, buttonColor[1]);
  CopyButton.draw(tft, buttonColor[1]);
  ClearButton.draw(tft, buttonColor[1]);
  SaveButton.draw(tft, buttonColor[1]);
  PasteButton.draw(tft, buttonColor[1]);
  LoopButton.draw(tft, buttonColor[1]);
  LoopLen[0].draw(tft, buttonColor[1]);
  ButtInst[0].draw(tft, buttonColor[0]);
  ButtInst[1].draw(tft, buttonColor[1]);
  ButtInst[2].draw(tft, buttonColor[1]);
  ButtInstPlay[0].draw(tft, buttonColor[0]);
  ButtInstPlay[1].draw(tft, buttonColor[1]);
  ButtInstPlay[2].draw(tft, buttonColor[1]);

  tft.setCursor(320, 27);
  tft.print("Drums");
  tft.setCursor(320, 67);
  tft.print("Bass");
  tft.setCursor(320, 107);
  tft.print("Melody");
  tft.drawRect(320, 37, 129, 8, TFT_RED);
  tft.drawRect(320, 47, 129, 8, TFT_WHITE);
  tft.fillRect(320, 57, 129, 8, TFT_WHITE);
  tft.drawRect(320, 77, 129, 8, TFT_RED);
  tft.drawRect(320, 87, 129, 8, TFT_WHITE);
  tft.fillRect(320, 97, 129, 8, TFT_WHITE);
  tft.drawRect(320, 117, 129, 8, TFT_RED);
  tft.drawRect(320, 127, 129, 8, TFT_WHITE);
  tft.fillRect(320, 137, 129, 8, TFT_WHITE);

  tft.setCursor(455, 38);
  Format(Vol[2]);
  tft.print(Vol[2]);
  DrawValue(Vol[2], 38);
  tft.setCursor(455, 78);
  Format(Vol[1]);
  tft.print(Vol[1]);
  DrawValue(Vol[1], 78);
  tft.setCursor(455, 118);
  Format(Vol[0]);
  tft.print(Vol[0]);
  DrawValue(Vol[0], 118);

  tft.setCursor(455, 48);
  Format(reverb[2]);
  tft.print(reverb[2]);
  DrawValue(reverb[2], 48);
  tft.setCursor(455, 88);
  Format(reverb[1]);
  tft.print(reverb[1]);
  DrawValue(reverb[1], 88);
  tft.setCursor(455, 128);
  Format(reverb[0]);
  tft.print(reverb[0]);
  DrawValue(reverb[0], 128);

  tft.setCursor(455, 58);
  Format(pan[2] - 64);
  tft.print(pan[2] - 64);
  DrawValuePan(pan[2], 58);
  tft.setCursor(455, 98);
  Format(pan[1] - 64);
  tft.print(pan[1] - 64);
  DrawValuePan(pan[1], 98);
  tft.setCursor(455, 138);
  Format(pan[0] - 64);
  tft.print(pan[0] - 64);
  DrawValuePan(pan[0], 138);

  tft.setCursor(425, 195 );
  tft.print("bpm");
  tft.setCursor(400, 195 );
  tft.print(tempo);
}
void drawRec() {
  tft.fillRect((stp) * 16 + 7, 254, 14, 17, TFT_BLUE);
  tft.drawRect((stp) * 16 + 6, 6, 16, 266, TFT_WHITE);
  tft.setCursor(11 + (stp * 16), 260);
  tft.print(stp, HEX);
  tft.fillRect((tick) * 16 + 7, 254, 14, 17, TFT_BLACK);
  tft.drawRect((tick) * 16 + 6, 6, 16, 266, TFT_RED);
  tft.setCursor(11 + (tick * 16), 260);
  tft.print(tick, HEX);
}
void drawPattern() {
  for (slope = 0; slope < 16; slope++) {
    for (slope2 = 0; slope2 < 13; slope2++) {
      if ((instrument[instSelectOld][slope2][pat] & (1 << slope)) != (instrument[instSelect][slope2][nextPat] & (1 << slope))) {
        tool = !(instrument[instSelect][slope2][nextPat] & (1 << slope));
        xDraw = slope * 16 + 12;
        yDraw = (12 - slope2) * 19 + 12;
        DRAW(buttonColor[instNoteLen[instSelect][slope2][nextPat][slope]]);
        if (instNoteLen[instSelect][slope2][nextPat][slope] > 0) {
          switchNoteLen(instNoteLen[instSelect][slope2][nextPat][slope], slope2, nextPat);
        }
      }
    }
  }
}
void DrawOrNot() {
  if (pat != nextPat) {
    drawPattern();
    if (patRow1Old != patRow1) {
      ButtPat[patRow1Old].draw(tft, buttonColor[1]);
      ButtPat[patRow1].draw(tft, buttonColor[0]);
      patRow1Old = patRow1;
    } else {
      ButtPat[patRow2Old + 4].draw(tft, buttonColor[1]);
      ButtPat[patRow2 + 4].draw(tft, buttonColor[0]);
      patRow2Old = patRow2;
    }
    pat = nextPat;
    toggle = 0;
  }
  if (instSelect != instSelectOld) {
    drawPattern();
    ButtInst[instSelectOld].draw(tft, buttonColor[1]);
    ButtInst[instSelect].draw(tft, buttonColor[0]);
    instSelectOld = instSelect;
  }
  if (playtrack > 0) {
    switch (playtrack) {
      case 1:
        drumPlay = !drumPlay;
        ButtInstPlay[0].draw(tft, buttonColor[!drumPlay]);
        break;
      case 2:
        bassPlay = !bassPlay;
        ButtInstPlay[1].draw(tft, buttonColor[!bassPlay]);
        break;
      case 3:
        leadPlay = !leadPlay;
        ButtInstPlay[2].draw(tft, buttonColor[!leadPlay]);
        break;
    }
    playtrack = toggle = 0;
  }
}
