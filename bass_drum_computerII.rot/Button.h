/* __________________________________________________________________________________________________________________
  |  Button.h is the header file for the Button class - which is used to draw buttons on the touch screen and        |
  |  to check if these buttons have been touched.                                                                    |
  |  It declares data members: caption, color, and the coordinates and dimensions used for drawing the octave        |
  |  buttons.                                                                                                        |
  |  It also declares methods (also known as member functions): draw() and contains().                               |
  |  (The implementation of these methods is defined in Button.cpp.)                                                 |
  |__________________________________________________________________________________________________________________| 
   */
   

#ifndef Button_h
#define Button_h

#include"Arduino.h"
#include <TFT_HX8357_Due.h>

class Button // defines data type and names it 'Button'
{
public:  
  String m_caption; // captions for octave change buttons - either '+' or '-' 
                    // member variables are designated with 'm_' prefix
  int m_color; 
  int m_x; // x and y coordinates are start point for drawing octave buttons
  int m_y;  
  int m_width; // width of octave button
  int m_height; // height of octave button

  Button(int xcoord, int ycoord, int width, int height, String buttonCaption); // prototype for constructor of Button object

  boolean contains(int tx, int ty);
  // This is a method (or "member function") declaration.
  // Its implementation (in Button.cpp) will check to see if one of the octave buttons has been pressed.

  void draw(TFT_HX8357_Due tft);  
  // A second method declaration, it passes the parameter "TFT_HX8357_Due" 
  // to draw() and gives it the local name "tft". 
};

#endif
