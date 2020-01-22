void welcomeScreen() {
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
  for (slope = 0; slope < 12; slope++) keys[slope].drawKey(tft);
  // *** draw buttons
  StartStopButton.draw(tft, buttonColor[0]);
  Rewind.draw(tft, buttonColor[0]);
  ButtInst[0].draw(tft, buttonColor[1]);
  ButtInst[1].draw(tft, buttonColor[0]);
  ButtInst[2].draw(tft, buttonColor[0]);     
  for (slope = 0; slope < 8; slope++){
    if(slope==0 || slope==4 ? color=1 : color=0);
    ButtPat[slope].draw(tft, buttonColor[color]);
    }
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
      if ((drum[slope2][pat] & (1 <<  slope)) != (drum[slope2][nextPat] & (1 <<  slope))) {
      tool = drum[slope2][pat] & (1 <<  slope);
        xDraw = slope * 16 + 12;
        yDraw = (12 - slope2) * 19 + 12;
        DRAW(TFT_RED);
      }
    }
  }
}
