void patCopy() {
  for (slope = 0; slope < 13; slope++) {
    instrument[instSelect][slope][pat] = instrument[instSelect][slope][copyPat];
  }
}
