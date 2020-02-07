void readPat()
{
  for (slope = 0; slope < 3; slope++) {
    String str = String(slope) + "." + String(pat) + "_pat.txt";
    file = SD.open(str);
    if (file) {
      for (slope2 = 0; slope2 < 13; slope2++) {
        String test = file.readStringUntil('\n');
        instrument[slope][slope2][pat] = test.toInt();
        for (slope3 = 0; slope3 < 16; slope3++) {
        test = file.readStringUntil('\n');
        instNoteLen[slope][slope2][pat][slope3] = test.toInt();
        }
      }
    }
  }
  file.close();
}
void savePat() {
  String str = String(instSelect) + "." + String(pat) + "_pat.txt";
  file = SD.open(str, FILE_WRITE);
  file.rewind();
  for (slope = 0; slope < 13; slope++) {
    file.println(instrument[instSelect][slope][pat]);
    for (slope2 = 0; slope2 < 16; slope2++) {
      file.println(instNoteLen[instSelect][slope][pat][slope2]);
    }
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
  } else calibrate();
}
