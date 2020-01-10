/* ______________________________________________________________________________________________________________________
  |  Key.h is the header file that declares the Key class. This class is used to draw keyboard keys on the touch screen  |
  |  and to check if a key has been touched.                                                                             |
  |  Key.h declares these data members: caption, color, frequency, and the coordinates, and dimensions used for drawing  |
  |  keys and checking for key touch.                                                                                    |
  |  (The implementation of methods draw and contains is defined in Key.cpp.)                                            |
  |______________________________________________________________________________________________________________________| 
   */
   
#ifndef Key_h
#define Key_h

#include "Arduino.h"
#include <TFT_HX8357_Due.h>


class Key // defines data type 'Key'
{
  public:
    int m_color;
    int m_y;
    int m_x; // x and y coordinates set the start point for drawing keys
    int m_width; // width of key
    int m_height; // height of key

  Key(int color, int keyy, int keyx); // prototype of constructor for 'Key' data type
  
  
  boolean contains(int tx, int ty);// method (or "member function") declaration.
                                   // Its implementation (in Key.cpp) will check to see if a key has been pressed.
  
  void draw(TFT_HX8357_Due); // A second method declaration, it passes the parameter "Adafruit_ILI9341" 
                                   // to draw() and gives it the local name "tft". 
};

#endif
