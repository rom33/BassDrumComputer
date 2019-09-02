# BassDrumComputer
A simple sequencer for Arduino Due with 3.5" 8-bit tft-display(e.g.MCUfriends) and vs1053.  

Needed libraries:

TFT_HX8357_Due

Scheduler and SDfat libraries can install from the library manager.

Now we must configure some on the TFT libray. Edit the User-Setup.h, and change

#define HX8357B

//#define HX8357C

//#define ILI9481

//#define ILI9481_8BIT

to

//#define HX8357B

//#define HX8357C

//#define ILI9481

#define ILI9481_8BIT

and the SdFat we must change the SdFatConfig.h

#define ENABLE_SOFTWARE_SPI_CLASS 0

to

#define ENABLE_SOFTWARE_SPI_CLASS 1
Wiring

for the vs1053:

    XCS -> 46

    XDCS -> 48

    DREQ -> 50

    XRST -> 52

    MOSI -> A.26

    MISO -> A.25

    SCK -> A.27

    5V -> 3.3V

for the rotary encoders:

    all GNDs to GND

    all + to 3.3 volts

    SW from encoder1 to 23 

    CLK from encoder1 to 22 

    DT from encoder1 to 24 

    SW from encoder2 to 25 

    CLK from encoder2 to 26 

    DT from encoder2 to 28

    SW from encoder3 to 27

    CLK from encoder3 to 30 

    DT from encoder3 to 32 

Operating

Below 0-F to select the pattern

Pattern 0-7: Drum-sounds

Pattern 8-11: Bass-sound

Pattern 12-15 Lead-Sound

CL button clear the current pattern or the compete song, but not the saved on memory card. If you want to clear it, first CL and then SV.

SV save the current pattern or the song. Resetting or power-off will not affect the data.

CP copy the current pattern. Select 0-F and then PA to paste.

After first start you have this drum set(configurable) :

    note C - Bass

    note C# - Snare

    note D - HiHat

    note D# - Closed HiHat

    note E - Hi Tom

    note F - Mid Tom

    note F# - Lo Tom

    note G - Crash

    note G# - Cowbell

    note A - Bongo

    note A# - Lo Bongo

    note B - Bell 

With the rotary encoders you can control the volume of each channel. Pressing the button change to the adjustment for the reverb and another push goes to the panning. 
