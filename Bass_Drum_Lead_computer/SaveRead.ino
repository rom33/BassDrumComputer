void readPat()
{
for(slope=0;slope<3;slope++){
  String str = String(slope) + "-" + String(pat) + "-pat.txt";
  file = SD.open(str);
  if (file) {
    for (slope2 = 0; slope2 < 13; slope2++) {
      String test = file.readStringUntil('\n');
      instrument[slope][slope2][pat] = test.toInt();
    }
  }
}
  file.close();
}
void savePat() {
  String str = String(instSelect) + "-" + String(pat) + "-pat.txt";
  file = SD.open(str, FILE_WRITE);
  file.rewind();
  for (slope = 0; slope < 13; slope++) {
    file.println(instrument[instSelect][slope][pat]);
  }
  file.close();
}
void readCalibrate() {
  String test;
  file = SD.open("calibrate.txt");
  if (file) {
    test = file.readStringUntil('\n');
    TS_MINX = test.toInt();
    test = file.readStringUntil('\n');
    TS_MAXX = test.toInt();
    test = file.readStringUntil('\n');
    TS_MINY = test.toInt();
    test = file.readStringUntil('\n');
    TS_MAXY = test.toInt();
    file.close();
  }else calibrate();
}
