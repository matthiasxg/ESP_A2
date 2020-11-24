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

//constants
#define ASCII_GAP -32 // a -> A
#define MAX_PLAINTEXT_LENGTH 50
#define MAX_CIPHERTEXT_LENGTH 50
#define ENCRYPT 1
#define DECRYPT 2
#define QUIT 0

//strings
#define OPTIONS "1 - Verschlüsseln\n2 - Entschlüsseln\n0 - Programm beenden\n"
#define PLAINTEXT_MESSAGE "Klartext: "
#define CIPHERTEXT_MESSAGE "Ciphertext: "

// forward declarations
int getOption();
void encryptMessage(char *square);
void getBigram(char *plaintext, char *bigrams);
int fillBigram(char *plaintext, char *bigrams);
void cleanBigram(char *bigrams);
void decryptMessage(char *square);

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

  int option;
  while ((option = getOption()) != QUIT)
  {
    switch (option)
    {
    case ENCRYPT:
      encryptMessage(*square);
      break;

    case DECRYPT:
      decryptMessage(*square);
      break;
    }
  }

  return 0;
}

//-----------------------------------------------------------------------------
// TODO
//
//
void encryptMessage(char *square)
{
  char plaintext[MAX_PLAINTEXT_LENGTH];
  char bigrams[MAX_PLAINTEXT_LENGTH];
  //char encrypted[MAX_PLAINTEXT_LENGTH];

  getBigram(plaintext, bigrams);

  printf("%s\n", bigrams);

  printf("\n\n%s", square);
  cleanBigram(bigrams);
}
//-----------------------------------------------------------------------------
// TODO
//
//
void getBigram(char *plaintext, char *bigrams)
{
  int bigrams_Size = 0;
  do
  {

    printf("%s", PLAINTEXT_MESSAGE);
    fgets(plaintext, MAX_PLAINTEXT_LENGTH, stdin);
    cleanString(plaintext, IS_ENCRYPTING);
    cleanBigram(bigrams);
    bigrams_Size = fillBigram(plaintext, bigrams);
  } while (!checkStringValidity(plaintext, MAX_PLAINTEXT_LENGTH, IS_ENCRYPTING) &&
           bigrams_Size * 2 <= MAX_PLAINTEXT_LENGTH);
}

//-----------------------------------------------------------------------------
// TODO
//
//
int fillBigram(char *plaintext, char *bigrams)
{
  char *character;
  int bigrams_Size = 0;
  for (character = plaintext; *character != '\n'; character++)
  {
    if ((bigrams_Size % 2 == 1) && *(bigrams - 1) == *character)
    {
      *bigrams = 'X';
      bigrams++;
      *bigrams = *character;
      bigrams++;
      bigrams_Size += 2;
    }
    else
    {
      *bigrams = *character;
      bigrams++;
      bigrams_Size++;
    }
  }
  if (bigrams_Size % 2 == 1)
  {
    *bigrams = 'X';
    bigrams_Size++;
  }

  return bigrams_Size;
}

//-----------------------------------------------------------------------------
// TODO
//
//
void cleanBigram(char *bigrams)
{
  for (int i = 0; i < MAX_PLAINTEXT_LENGTH; i++)
  {
    *(bigrams + i) = '\0';
  }
}

//-----------------------------------------------------------------------------
// TODO
//
//
void decryptMessage(char *square)
{
  char ciphertext[MAX_CIPHERTEXT_LENGTH];
  do
  {
    printf("%s", CIPHERTEXT_MESSAGE);
    fgets(ciphertext, MAX_CIPHERTEXT_LENGTH, stdin);
  } while (!checkStringValidity(ciphertext, MAX_PLAINTEXT_LENGTH, IS_DECRYPTING));

  printf("%s", square);
}

//-----------------------------------------------------------------------------
// TODO
//
// @return TODO
//
int getOption()
{
  int option;
  do
  {
    printf("\nWählen Sie bitte eine Option:\n%s\n", OPTIONS);
    scanf("%i", &option);
  } while (option != ENCRYPT && option != DECRYPT && option != QUIT);
  getchar(); // Clear input buffer
  return option;
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
    if ('a' <= *index && *index <= 'z')
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
