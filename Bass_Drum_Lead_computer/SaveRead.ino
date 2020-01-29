void readPat()
{
  String str = String(pat) + "-pat.txt";
  file = SD.open(str);
  if (file) {
    for (slope = 0; slope < 13; slope++) {
      String test = file.readStringUntil('\n');
      instrument[instSelect][slope][pat] = test.toInt();
    }
  }
  file.close();
}
void savePat() {
  String str = String(pat) + "-pat.txt";
  file = SD.open(str, FILE_WRITE);
  file.rewind();
  for (slope = 0; slope < 13; slope++) {
    file.println(instrument[instSelect][slope][pat]);
  }
  file.close();
}
