// Key.cpp
/*   ___________________________________________________________________________________________
    |   The class 'Key' has a constructor for the keyboard keys, and two methods (also called   |
    |   'member functions'): draw and contains.                                                 |
    |   Key.cpp defines the implementation of these methods.                                    |
    |___________________________________________________________________________________________|  */
 
#include "Key.h"
#include <TFT_HX8357_Due.h>

// constructor for keyboard keys
Key::Key(int color, int keyx, int keyy) 
{
  m_color  = color;
  m_y = keyy;
  m_x = keyx;
  m_width = 35 ;
  m_height = 17 ;
}


// The 'contains' method checks to see if a given point that has been 
// touched on the screen is inside the key - i.e. if the key has been pressed.
boolean Key::contains(int tx, int ty)
{
  if ((tx > m_x) && (tx < m_x + m_width))
  {
    if ((ty > m_y) && (ty < m_y + m_height))
    {
      return true;  // Test point lies inside the key
    }
  }
  return false;  // Test point lies outside the key
}


// the 'draw' method checks the key colour and draws the key 
void Key::draw(TFT_HX8357_Due tft) 
{
    tft.fillRect(m_x, m_y, m_width, m_height, m_color);
}
