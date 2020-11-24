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

//Enums
typedef enum
{
  SAME_ROW = 1,
  SAME_COLUMN = 2,
  DIFFERENT = 3,
} encrypt_type;

// forward declarations
int getOption();
void encryptMessage(char *square);
void fillEncrypted(char *encrypted, char *bigrams, char *square);
void encryptBigram(char *encrypted_Index, int *indices, char *square, encrypt_type type);
void decryptBigram(char *decrypted_Index, int *indices, char *square, encrypt_type type);
void getIndices(char *bigram_Index, char *square, int *indices);
void getBigrams(char *plaintext, char *bigrams);
int fillBigrams(char *plaintext, char *bigrams);
void cleanArray(char *array, int max_Length);
void printArray(char *array);
void decryptMessage(char *square);
void getCiphertext(char *ciphertext);
void fillDecrypted(char *decrypted, char *ciphertext, char *square);

//-----------------------------------------------------------------------------
///
/// The main program:
/// TODO
///
/// @return TODO
//
int main()
{
  char square[SQUARE_SIDE * SQUARE_SIDE];
  generatePlayfairSquare(square);

  int option;
  while ((option = getOption()) != QUIT)
  {
    switch (option)
    {
    case ENCRYPT:
      encryptMessage(square);
      break;

    case DECRYPT:
      decryptMessage(square);
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
  char encrypted[MAX_PLAINTEXT_LENGTH];

  getBigrams(plaintext, bigrams);

  printf("Vorbereiteter Klartext: ");
  printArray(bigrams);

  fillEncrypted(encrypted, bigrams, square);

  printf(CIPHERTEXT_MESSAGE);
  printArray(encrypted);
  printf("\n");

  cleanArray(bigrams, MAX_PLAINTEXT_LENGTH);
  cleanArray(encrypted, MAX_PLAINTEXT_LENGTH);
  cleanArray(plaintext, MAX_PLAINTEXT_LENGTH);
}

//-----------------------------------------------------------------------------
// TODO
//
//
void fillEncrypted(char *encrypted, char *bigrams, char *square)
{
  char *bigram_Index;
  char *encrypted_Index = encrypted;
  int indices[4];
  for (bigram_Index = bigrams; *bigram_Index != '\0'; bigram_Index += 2)
  {

    getIndices(bigram_Index, square, indices);
    if (*indices == *(indices + 2)) //same row
    {
      encryptBigram(encrypted_Index, indices, square, SAME_ROW);
    }
    else if (*(indices + 1) == *(indices + 3)) // same column
    {
      encryptBigram(encrypted_Index, indices, square, SAME_COLUMN);
    }
    else
    {
      encryptBigram(encrypted_Index, indices, square, DIFFERENT);
    }
    encrypted_Index += 2;
  }
}

//-----------------------------------------------------------------------------
// TODO
//
//
void encryptBigram(char *encrypted_Index, int *indices, char *square, encrypt_type type)
{
  switch (type)
  {
  case SAME_ROW:
    if (*(indices + 1) == SQUARE_SIDE - 1)
    {
      *encrypted_Index = *(square + (*indices * SQUARE_SIDE));
    }
    else
    {
      *encrypted_Index = *(square + (*indices * SQUARE_SIDE) + *(indices + 1) + 1);
    }
    if (*(indices + 3) == SQUARE_SIDE - 1)
    {
      *(encrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE));
    }
    else
    {
      *(encrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE) + *(indices + 3) + 1);
    }
    break;
  case SAME_COLUMN:
    if (*indices == SQUARE_SIDE - 1)
    {
      *encrypted_Index = *(square + *(indices + 1));
    }
    else
    {
      *encrypted_Index = *(square + (SQUARE_SIDE * (*(indices) + 1)) + *(indices + 1));
    }
    if (*(indices + 2) == SQUARE_SIDE - 1)
    {
      // square + column
      *(encrypted_Index + 1) = *(square + *(indices + 3));
    }
    else
    {
      // square + SQUARE_SIDE mal neu row + column
      *(encrypted_Index + 1) = *(square + (SQUARE_SIDE * (*(indices + 2) + 1)) + *(indices + 3));
    }
    break;
  case DIFFERENT:
    // square + square side mal row
    *encrypted_Index = *(square + (SQUARE_SIDE * *(indices)) + *(indices + 3));
    *(encrypted_Index + 1) = *(square + (SQUARE_SIDE * *(indices + 2)) + *(indices + 1));
    break;
  }
}

//-----------------------------------------------------------------------------
// TODO
//
//
void decryptBigram(char *decrypted_Index, int *indices, char *square, encrypt_type type)
{
  switch (type)
  {
  case SAME_ROW:
    if (*(indices + 1) == 0)
    {
      *decrypted_Index = *(square + (*indices * SQUARE_SIDE) + (SQUARE_SIDE - 1));
    }
    else
    {
      *decrypted_Index = *(square + (*indices * SQUARE_SIDE) + *(indices + 1) - 1);
    }
    if (*(indices + 3) == 0)
    {
      *(decrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE) + (SQUARE_SIDE - 1));
    }
    else
    {
      *(decrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE) + *(indices + 3) - 1);
    }
    break;

  case SAME_COLUMN:
    if (*indices == 0)
    {
      *decrypted_Index = *(square + *(indices + 1) + ((SQUARE_SIDE - 1) * SQUARE_SIDE));
    }
    else
    {
      *decrypted_Index = *(square + (SQUARE_SIDE * (*(indices)-1)) + *(indices + 1));
    }
    if (*(indices + 2) == 0)
    {
      *(decrypted_Index + 1) = *(square + *(indices + 3) + ((SQUARE_SIDE - 1) * SQUARE_SIDE));
    }
    else
    {
      *(decrypted_Index + 1) = *(square + (SQUARE_SIDE * (*(indices + 2) - 1)) + *(indices + 3));
    }
    break;

  case DIFFERENT:
    *decrypted_Index = *(square + (SQUARE_SIDE * *(indices)) + *(indices + 3));
    *(decrypted_Index + 1) = *(square + (SQUARE_SIDE * *(indices + 2)) + *(indices + 1));
    break;
  }
}

//-----------------------------------------------------------------------------
// TODO
//
//
void getIndices(char *index, char *square, int *indices)
{
  char *square_Index;
  int counter = 0;
  for (square_Index = square; *square_Index != '\0'; square_Index++)
  {
    if (*square_Index == *index)
    {
      *indices = counter / SQUARE_SIDE;
      *(indices + 1) = counter % SQUARE_SIDE;
    }
    else if (*square_Index == *(index + 1))
    {
      *(indices + 2) = counter / SQUARE_SIDE;
      *(indices + 3) = counter % SQUARE_SIDE;
    }
    counter++;
  }
}

//-----------------------------------------------------------------------------
// TODO
//
//
void getBigrams(char *plaintext, char *bigrams)
{
  int bigrams_Size = 0;
  do
  {

    printf("%s", PLAINTEXT_MESSAGE);
    fgets(plaintext, MAX_PLAINTEXT_LENGTH, stdin);
    cleanString(plaintext, IS_ENCRYPTING);
    cleanArray(bigrams, MAX_PLAINTEXT_LENGTH);
    bigrams_Size = fillBigrams(plaintext, bigrams);
  } while (!checkStringValidity(plaintext, MAX_PLAINTEXT_LENGTH, IS_ENCRYPTING) &&
           bigrams_Size * 2 <= MAX_PLAINTEXT_LENGTH);
}

//-----------------------------------------------------------------------------
// TODO
//
//
int fillBigrams(char *plaintext, char *bigrams)
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
void cleanArray(char *array, int max_Length)
{
  for (int i = 0; i < max_Length; i++)
  {
    *(array + i) = '\0';
  }
}

//-----------------------------------------------------------------------------
// TODO
//
//
void printArray(char *array)
{
  for (char *index = array; *index != '\0'; index += 2)
  {
    if(*(index + 2) != '\0')
    {
      printf("%c%c ", *index, *(index + 1));
    }
    else
    {
      printf("%c%c", *index, *(index + 1));
    }
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
// TODO
//
//
void decryptMessage(char *square)
{
  char ciphertext[MAX_CIPHERTEXT_LENGTH];
  char decrypted[MAX_CIPHERTEXT_LENGTH];

  getCiphertext(ciphertext);

  printf("Vorbereiteter Ciphertext: ");
  printArray(ciphertext);

  fillDecrypted(decrypted, ciphertext, square);

  printf("Klartext: ");
  printArray(decrypted);
  printf("\n");

  cleanArray(ciphertext, MAX_CIPHERTEXT_LENGTH);
  cleanArray(decrypted, MAX_CIPHERTEXT_LENGTH);
}

//-----------------------------------------------------------------------------
// TODO
//
//
void getCiphertext(char *ciphertext)
{
  do
  {
    printf("%s", CIPHERTEXT_MESSAGE);
    cleanArray(ciphertext, MAX_CIPHERTEXT_LENGTH);
    fgets(ciphertext, MAX_CIPHERTEXT_LENGTH, stdin);
    cleanString(ciphertext, IS_DECRYPTING);
  } while (!checkStringValidity(ciphertext, MAX_CIPHERTEXT_LENGTH, IS_DECRYPTING));
}

//-----------------------------------------------------------------------------
// TODO
//
//
void fillDecrypted(char *decrypted, char *ciphertext, char *square)
{
  char *decrypted_Index = decrypted;
  char *ciphertext_Index;
  int indices[4];
  for (ciphertext_Index = ciphertext; *ciphertext_Index != '\0'; ciphertext_Index += 2)
  {
    getIndices(ciphertext_Index, square, indices);
    if (*indices == *(indices + 2)) //same row
    {
      decryptBigram(decrypted_Index, indices, square, SAME_ROW);
    }
    else if (*(indices + 1) == *(indices + 3)) // same column
    {
      decryptBigram(decrypted_Index, indices, square, SAME_COLUMN);
    }
    else
    {
      decryptBigram(decrypted_Index, indices, square, DIFFERENT);
    }
    decrypted_Index += 2;
  }
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
