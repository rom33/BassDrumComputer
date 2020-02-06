/*

  File:     MP3_Shield_RealtimeMIDI.ino
  Author:   Matthias Neeracher

  This code is in the public domain, with the exception of the contents of sVS1053b_Realtime_MIDI_Plugin.

  The code is based on Nathan Seidle's Sparkfun Electronics example code for the Sparkfun
  MP3 Player and Music Instrument shields and and VS1053 breakout board.

  http://www.sparkfun.com/Code/MIDI_Example.pde
  http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Dev/Arduino/Shields/VS_Shield_Example.zip

       Spark Fun Electronics 2011
       Nathan Seidle

       This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  THEORY OF OPERATIONS

  The VS1053b has two ways of playing MIDI: One method is that you simply send a Standard MIDI level 0 file through
  SPI, and the chip will play it. This works exactly the same way as MP3 mode and will not be discussed further here.
  The other method is that the VS1053b has a "Real Time MIDI mode", in which it will instantly execute MIDI commands
  sent to it through either the UART or SPI.

  Real Time MIDI mode can be enabled with two different methods, controlled by USE_GPIO_INIT
   (1) Setting GPIO1 to HIGH (which is hardwired in the Sparkfun Music Instrument shield, and can be done through
       pin 4 in the MP3 Player Shield)
   (0) Sending a small software patch through SPI.

  MIDI data can be sent with two different methods as well, controlled by USE_SPI_MIDI
   (0) Through a (software) serial connection on pin 3, at 31250 baud
   (1) Through SPI, at an arbitrary data rate. For SPI, each byte of MIDI data needs to be prefixed by a 0 byte
       (The V1053b data sheet erroneously states that the padding should be a 0xFF byte).

  Both initialization methods and both transmission methods can be selected through the #defines below. Out of the box,
  it probably makes most sense to enable real time MIDI through pin 4, and send serial data through pin 3, but if you
  want to cut the traces for pin 3 and 4 and use those pins for another purpose, the alternative methods may come in
  handy.
*/
void sendMIDI(byte data)
{
  delayMicroseconds(10);
  SPI.transfer(0);
  delayMicroseconds(10);  
  SPI.transfer(data);
}
//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  //
  // Wait for chip to be ready (Unlikely to be an issue with real time MIDI)
  //
  while (!digitalRead(VS_DREQ));
  delayMicroseconds(10);
  digitalWrite(VS_XDCS, LOW);
  sendMIDI(cmd);
  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if ( (cmd & 0xF0) <= 0xB0 || (cmd & 0xF0) >= 0xE0) {
    sendMIDI(data1);
    sendMIDI(data2);
  } else {
    sendMIDI(data1);
  }
  digitalWrite(VS_XDCS, HIGH);
  delayMicroseconds(10);
}
//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}
//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}
//Write to VS10xx register
//SCI: Data transfers are always 16bit. When a new SCI operation comes in
//DREQ goes low. We then have to wait for DREQ to go high again.
//XCS should be low for the full duration of operation.
void VSWriteRegister(unsigned char addressbyte, unsigned char highbyte, unsigned char lowbyte) {
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating IC is available
  digitalWrite(VS_XCS, LOW); //Select control

  //SCI consists of instruction byte, address byte, and 16-bit data word.
  SPI.transfer(0x02); //Write instruction
  SPI.transfer(addressbyte);
  SPI.transfer(highbyte);
  SPI.transfer(lowbyte);
  while (!digitalRead(VS_DREQ)) ; //Wait for DREQ to go high indicating command is complete
  digitalWrite(VS_XCS, HIGH); //Deselect Control
}

//
// Plugin to put VS10XX into realtime MIDI mode
// Originally from http://www.vlsi.fi/fileadmin/software/VS10XX/vs1053b-rtmidistart.zip
// Permission to reproduce here granted by VLSI solution.
//
const unsigned short sVS1053b_Realtime_MIDI_Plugin[28] = { /* Compressed plugin */
  0x0007, 0x0001, 0x8050, 0x0006, 0x0014, 0x0030, 0x0715, 0xb080, /*    0 */
  0x3400, 0x0007, 0x9255, 0x3d00, 0x0024, 0x0030, 0x0295, 0x6890, /*    8 */
  0x3400, 0x0030, 0x0495, 0x3d00, 0x0024, 0x2908, 0x4d40, 0x0030, /*   10 */
  0x0200, 0x000a, 0x0001, 0x0050,
};

void VSLoadUserCode(void) {
  int i = 0;

  while (i < sizeof(sVS1053b_Realtime_MIDI_Plugin) / sizeof(sVS1053b_Realtime_MIDI_Plugin[0])) {
    unsigned short addr, n, val;
    addr = sVS1053b_Realtime_MIDI_Plugin[i++];
    n = sVS1053b_Realtime_MIDI_Plugin[i++];
    while (n--) {
      val = sVS1053b_Realtime_MIDI_Plugin[i++];
      VSWriteRegister(addr, val >> 8, val & 0xFF);
    }
  }
}
