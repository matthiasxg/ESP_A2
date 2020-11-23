//-----------------------------------------------------------------------------
// a2.c
//
// Playfair-Chiffre
//
// Group: 9
//
// Author: 12007662
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include "framework.h"

#define ASCII_GAP -32 // a -> A

// forward declarations
int stringLength(char *text);
void toUpper(char *text);
void replaceLetters(char *text, char original, char new_char);

//-----------------------------------------------------------------------------
///
/// The main program:
/// TODO
///
/// @return TODO
//
int main()
{
  char square[SQUARE_SIDE][SQUARE_SIDE];
  generatePlayfairSquare(*square);

  return 0;
}

//-----------------------------------------------------------------------------
// Computes and returns the length of a string
//
// @param text string to check
//
// @return length of the string
//
int stringLength(char *text)
{
  char *index;
  int length = 0;
  for (index = text; *index != '\0'; index++)
  {
    length++;
  }
  return length;
}

//-----------------------------------------------------------------------------
// Changes a string to upper case
//
// @param text string to modify
//
void toUpper(char *text)
{
  char *index;
  for (index = text; *index != '\0'; index++)
  {
    if ('a' <= *index || *index <= 'z')
    {
      *index = *index + ASCII_GAP;
    }
  }
}

//-----------------------------------------------------------------------------
// Replace all the occurences of a letter in a string with another letter
//
// @param text string to modify
// @param original letter to replace
// @param new_char letter used for the substitution
//
void replaceLetters(char *text, char original, char new_char)
{
  char *index;
  for (index = text; *index != '\0'; index++)
  {
    if (*index == original)
    {
      *index = new_char;
    }
  }
}
