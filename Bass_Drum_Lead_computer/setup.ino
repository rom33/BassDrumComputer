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
  drawCrossHair(10, 10, TFT_BLUE);
  treadWater();
  drawCrossHair(480 / 2, 10, TFT_WHITE);
  WaitTouch;
  GetTouchPoints;
  rX[1] = tp.x;
  rY[1] = tp.y;
  treadWater();
  drawCrossHair(480 / 2, 10, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 10, TFT_WHITE);
  WaitTouch;
  GetTouchPoints;
  rX[2] = tp.x;
  rY[2] = tp.y;
  treadWater();
  drawCrossHair(480 - 11, 10, TFT_BLUE);
  treadWater();
  drawCrossHair(10, 320 / 2, TFT_WHITE);
  WaitTouch;
  GetTouchPoints;
  rX[3] = tp.x;
  rY[3] = tp.y;
  treadWater();
  drawCrossHair(10, 320 / 2, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 320 / 2, TFT_WHITE);
  WaitTouch;
  GetTouchPoints;
  rX[4] = tp.x;
  rY[4] = tp.y;
  treadWater();
  drawCrossHair(480 - 11, 320 / 2, TFT_BLUE);
  treadWater();
  drawCrossHair(10, 320 - 11, TFT_WHITE);
  WaitTouch;
  GetTouchPoints;
  rX[5] = tp.x;
  rY[5] = tp.y;
  treadWater();
  drawCrossHair(10, 320 - 11, TFT_BLUE);
  treadWater();
  drawCrossHair(480 / 2, 320 - 11, TFT_WHITE);
  WaitTouch;
  GetTouchPoints;
  treadWater();
  rX[6] = tp.x;
  rY[6] = tp.y;
  drawCrossHair(480 / 2, 320 - 11, TFT_BLUE);
  treadWater();
  drawCrossHair(480 - 11, 320 - 11, TFT_WHITE);
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
  Calibrate.draw(tft,buttonColor[0]);
  tft.setCursor(210, 200);
  tft.print("touch button");
  WaitTouch;
  GetTouchPoints;
  if(Calibrate.contains(xx,yy)) {
  file = SD.open("calibrate.txt", FILE_WRITE);
  file.rewind();
  file.println(TS_MINX);
  file.println(TS_MAXX);
  file.println(TS_MINY);
  file.println(TS_MAXY);
  file.close();
  tft.fillScreen(TFT_BLUE);
  } else {
  tft.fillScreen(TFT_BLUE);
  tft.setCursor(207, 190);
  tft.print("not working"); 
  tft.setCursor(210, 200);
  tft.print("try again!"); 
  tft.setCursor(205, 210);
  tft.print("touch screen"); 
  treadWater();
  WaitTouch;
  tft.fillScreen(TFT_BLUE);
  calibrate();  
  }
}
void drawCrossHair(int x, int y, int color)
{
  tft.drawRect(x - 10, y - 10, 20, 20, color);
  tft.drawLine(x - 5, y, x + 5, y, color);
  tft.drawLine(x, y - 5, x, y + 5, color);
}
