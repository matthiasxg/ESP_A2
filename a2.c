//-----------------------------------------------------------------------------
// a2.c
//
// Playfair-Chiffre encryption and decryption
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
} PlayfairRules;

// forward declarations
int getOption();
void encryptMessage(char *square);
void decryptMessage(char *square);
void fillEncrypted(char *encrypted, char *bigrams, char *square);
void fillDecrypted(char *decrypted, char *ciphertext, char *square);
void getBigrams(char *plaintext, char *bigrams);
void getCiphertext(char *ciphertext);
int fillBigrams(char *plaintext, char *bigrams);
void encryptBigram(char *encrypted_Index, int *indices, char *square, PlayfairRules type);
void decryptBigram(char *decrypted_Index, int *indices, char *square, PlayfairRules type);
void getIndices(char *bigram_Index, char *square, int *indices);
void cleanArray(char *array, int max_Length);
void printArray(char *array);

//-----------------------------------------------------------------------------
///
/// The main program:
/// Encrypts / decrypts the user input. Loops until the user
/// exits with zero.
/// Command inputs:
/// 1: encrypt
/// 2: decrypt
///
/// @return always zero
//
int main()
{
  char square[SQUARE_SIDE * SQUARE_SIDE] = {'\0'};
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

//------------------------------------------------------------------------------
/// Encrypts the message from the users input.
/// Generates Bigrams of the user input and afterwards
/// calculates the encrypted Bigrams.
///
/// @param char *playfair square array for encryption
//
void encryptMessage(char *square)
{
  char plaintext[MAX_PLAINTEXT_LENGTH] = {'\0'};
  char bigrams[MAX_PLAINTEXT_LENGTH] = {'\0'};
  char encrypted[MAX_PLAINTEXT_LENGTH] = {'\0'};

  getBigrams(plaintext, bigrams);

  printf("Vorbereiteter Klartext: ");
  printArray(bigrams);

  fillEncrypted(encrypted, bigrams, square);

  printf(CIPHERTEXT_MESSAGE);
  printArray(encrypted);

  // cleaning the arrays for next time
  cleanArray(bigrams, MAX_PLAINTEXT_LENGTH);
  cleanArray(encrypted, MAX_PLAINTEXT_LENGTH);
  cleanArray(plaintext, MAX_PLAINTEXT_LENGTH);
}

//------------------------------------------------------------------------------
/// Decrypts the ciphertext from the users input.
/// Splits the ciphertext in bigrams and afterwards calculates the
/// encrypted text.
///
/// @param char *playfair square array for decryption
//
void decryptMessage(char *square)
{
  char ciphertext[MAX_CIPHERTEXT_LENGTH] = {'\0'} ;
  char decrypted[MAX_CIPHERTEXT_LENGTH] = {'\0'};

  getCiphertext(ciphertext);

  printf("Vorbereiteter Ciphertext: ");
  printArray(ciphertext);

  fillDecrypted(decrypted, ciphertext, square);

  printf(PLAINTEXT_MESSAGE);
  printArray(decrypted);

  //cleaning the arrays for next time
  cleanArray(ciphertext, MAX_CIPHERTEXT_LENGTH);
  cleanArray(decrypted, MAX_CIPHERTEXT_LENGTH);
}

//------------------------------------------------------------------------------
/// Fills the enrypthed char array with the encrypted plain text.
/// Iterates through bigram array, for each individually calculates the new
/// bigram. After this function encryption is finished.
///
/// @param char *encrypted, empty array to fill in the result
/// @param char *bigrams, generates bigrams from plaintext
/// @param char *playfair square array for encryption
//
void fillEncrypted(char *encrypted, char *bigrams, char *square)
{
  char *bigram_Index = NULL;
  char *encrypted_Index = encrypted;
  int indices[4] = {0};

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
    else // different
    {
      encryptBigram(encrypted_Index, indices, square, DIFFERENT);
    }
    encrypted_Index += 2;
  }
}

//------------------------------------------------------------------------------
/// Generates the encrypted Bigram from the given one depending on the given
/// playfair square. Also takes care of the given playfair rule.
///
/// @param char *encrypted_Index, points to the current index of the encrypted array
/// @param int *indices, array with four indices of the given bigram (encrypted index)
/// @param char *playfair square array for encryption
/// @param PlayFairRules rule, contains the correct rule for the given bigram
//
void encryptBigram(char *encrypted_Index, int *indices, char *square, PlayfairRules rule)
{
  switch (rule)
  {
  case SAME_ROW: // bigram in the same row
    if (*(indices + 1) == SQUARE_SIDE - 1) // if first letter is on the right
    {
      *encrypted_Index = *(square + (*indices * SQUARE_SIDE)); // left letter in this row
    }
    else
    {
      *encrypted_Index = *(square + (*indices * SQUARE_SIDE) + *(indices + 1) + 1); // next to the right
    }

    if (*(indices + 3) == SQUARE_SIDE - 1) // if second letter is on the right
    {
      *(encrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE)); // left letter in this row
    }
    else
    {
      *(encrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE) + *(indices + 3) + 1); // next to the right
    }
    break;

  case SAME_COLUMN: // bigram is in the same column
    if (*indices == SQUARE_SIDE - 1) // if first letter is in the last row
    {
      *encrypted_Index = *(square + *(indices + 1)); // first row + column
    }
    else
    {
      *encrypted_Index = *(square + (SQUARE_SIDE * (*(indices) + 1)) + *(indices + 1)); // next row + column
    }

    if (*(indices + 2) == SQUARE_SIDE - 1) // if second letter is in the last row
    {
      *(encrypted_Index + 1) = *(square + *(indices + 3)); // first row + column
    }
    else
    {
      *(encrypted_Index + 1) = *(square + (SQUARE_SIDE * (*(indices + 2) + 1)) + *(indices + 3)); // next row + column
    }
    break;

  case DIFFERENT: // not in the same row or collum
    *encrypted_Index = *(square + (SQUARE_SIDE * *(indices)) + *(indices + 3)); // first letter -> same row, second letter column
    *(encrypted_Index + 1) = *(square + (SQUARE_SIDE * *(indices + 2)) + *(indices + 1)); // second letter -> same row, first letter column
    break;

  }
}

//------------------------------------------------------------------------------
/// Generates the decrypted Bigram from the given one depending on the given
/// playfair square. Also takes care of the given playfair rule.
///
/// @param char *decrypted_Index, points to the current index of the decrypted array
/// @param int *indices, array with four indices of the given bigram (decrypted index)
/// @param char *playfair square array for encryption
/// @param PlayFairRules rule, contains the correct rule for the given bigram
//
void decryptBigram(char *decrypted_Index, int *indices, char *square, PlayfairRules type)
{
  switch (type)
  {
  case SAME_ROW: // bigram in the same row
    if (*(indices + 1) == 0) // if first letter is on the left side
    {
      *decrypted_Index = *(square + (*indices * SQUARE_SIDE) + (SQUARE_SIDE - 1)); // right side, same row
    }
    else
    {
      *decrypted_Index = *(square + (*indices * SQUARE_SIDE) + *(indices + 1) - 1); // one right, same row
    }

    if (*(indices + 3) == 0) // if second letter is on the left side
    {
      *(decrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE) + (SQUARE_SIDE - 1)); // right side, same row
    }
    else
    {
      *(decrypted_Index + 1) = *(square + (*(indices + 2) * SQUARE_SIDE) + *(indices + 3) - 1); // one right, same row
    }
    break;

  case SAME_COLUMN: // bigram in the same column
    if (*indices == 0) // if first letter is in the first row
    {
      *decrypted_Index = *(square + *(indices + 1) + ((SQUARE_SIDE - 1) * SQUARE_SIDE)); // last row, same column
    }
    else
    {
      *decrypted_Index = *(square + (SQUARE_SIDE * (*(indices)-1)) + *(indices + 1)); // next row, same column
    }

    if (*(indices + 2) == 0) // if second letter is in the first row
    {
      *(decrypted_Index + 1) = *(square + *(indices + 3) + ((SQUARE_SIDE - 1) * SQUARE_SIDE)); // last row, same column
    }
    else
    {
      *(decrypted_Index + 1) = *(square + (SQUARE_SIDE * (*(indices + 2) - 1)) + *(indices + 3)); // newxt row, same column
    }
    break;

  case DIFFERENT: // not in the same row and column
    *decrypted_Index = *(square + (SQUARE_SIDE * *(indices)) + *(indices + 3)); // same row, second letters column
    *(decrypted_Index + 1) = *(square + (SQUARE_SIDE * *(indices + 2)) + *(indices + 1)); // same row, first letters column
    break;
  }
}

//------------------------------------------------------------------------------
/// Searchs for the bigram indices in the playfair square. Saves them in
/// the indices array given.
///
/// @param char *index, pointer of the first element of a bigram
/// @param char *playfair square array for encryption
/// @param int *indices, array to save the found indices
//
void getIndices(char *index, char *square, int *indices)
{
  char *square_Index = square;
  int counter = 0;

  for (int i = 0; i < SQUARE_SIDE * SQUARE_SIDE; i++)
  {
    if (*square_Index == *index)
    {
      *indices = counter / SQUARE_SIDE; // first row
      *(indices + 1) = counter % SQUARE_SIDE; // first column
    }
    else if (*square_Index == *(index + 1))
    {
      *(indices + 2) = counter / SQUARE_SIDE; // second row
      *(indices + 3) = counter % SQUARE_SIDE; // second column
    }
    square_Index++;
    counter++;
  }
}

//------------------------------------------------------------------------------
/// Reads the plaintext from the users input. Afterwards creates the bigram
/// from it. 
///
/// @param char *plaintext, user input to encrypt
/// @param char *bigrams, array to save generated bigrams
//
void getBigrams(char *plaintext, char *bigrams)
{
  int bigrams_Size = 0;
  do
  {
    printf("%s", PLAINTEXT_MESSAGE);
    cleanArray(bigrams, MAX_PLAINTEXT_LENGTH); // for next time

    fgets(plaintext, MAX_PLAINTEXT_LENGTH, stdin);
    cleanString(plaintext, IS_ENCRYPTING);

    bigrams_Size = fillBigrams(plaintext, bigrams);
  } while (!checkStringValidity(plaintext, MAX_PLAINTEXT_LENGTH, IS_ENCRYPTING) &&
           bigrams_Size * 2 <= MAX_PLAINTEXT_LENGTH);
}

//------------------------------------------------------------------------------
/// Iterates through the plaintext and coninuesly add bigrams
/// to the given bigram array 
///
/// @param char *plaintext, user input to encrypt
/// @param char *bigrams, array to save generated bigrams
///
/// @return int bigrams_size, amount of bigrams in the array
//
int fillBigrams(char *plaintext, char *bigrams)
{
  char *character = NULL;
  int bigrams_size = 0;
  for (character = plaintext; *character != '\n'; character++)
  {
    if ((bigrams_size % 2 == 1) && *(bigrams - 1) == *character)
    {
      *bigrams = 'X';
      bigrams++;
      *bigrams = *character;
      bigrams++;
      bigrams_size += 2;
    }
    else
    {
      *bigrams = *character;
      bigrams++;
      bigrams_size++;
    }
  }
  if (bigrams_size % 2 == 1) // add X if odd
  {
    *bigrams = 'X';
    bigrams_size++;
  }

  return bigrams_size;
}

///-----------------------------------------------------------------------------
/// Reads the ciphertext from the users input, cleans and checks it
/// 
/// @param char *ciphertext, array to save users ciphertext
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

///-----------------------------------------------------------------------------
/// Iterates throught the ciphertext given and calculates the decrypted
/// bigrams. Afterwards saves it to the decrypted array.
/// 
/// @param char *decrypted, array to save decrypted bigrams
/// @param char *ciphertext, given users input
/// @param char *playfair square array for decryption
//
void fillDecrypted(char *decrypted, char *ciphertext, char *square)
{
  char *decrypted_index = decrypted;
  char *ciphertext_index = NULL;
  int indices[4] = {0};

  for (ciphertext_index = ciphertext; *ciphertext_index != '\0'; ciphertext_index += 2)
  {
    getIndices(decrypted_index, square, indices);

    if (*indices == *(indices + 2)) //same row
    {
      decryptBigram(decrypted_index, indices, square, SAME_ROW);
    }
    else if (*(indices + 1) == *(indices + 3)) // same column
    {
      decryptBigram(decrypted_index, indices, square, SAME_COLUMN);
    }
    else
    {
      decryptBigram(decrypted_index, indices, square, DIFFERENT);
    }
    decrypted_index += 2;
  }
}

///-----------------------------------------------------------------------------
/// Asks the user what he wand to do. Either to encrypt a message, decrypt
/// a ciphertext or to quit the program.
///
/// @return int option, users choice for encryption, decyription or quit
//
int getOption()
{
  int option = 0;
  do
  {
    printf("\nWählen Sie bitte eine Option:\n%s\n", OPTIONS);
    scanf("%i", &option);
    getchar(); // clear enter
  } while (option != ENCRYPT && option != DECRYPT && option != QUIT);
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
  char *index = NULL;
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
  char *index = NULL;
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

//------------------------------------------------------------------------------
/// Iterates through the whole array and sets every cell to zero
///
/// @param char *array, to clean
/// @param int max_length of the array
//
void cleanArray(char *array, int max_length)
{
  for (int index = 0; index < max_length; index++)
  {
    *(array + index) = '\0';
  }
}

//------------------------------------------------------------------------------
/// Iterates through the array and prints the array as bigrams
///
/// @param char *array, to print as bigram
//
void printArray(char *array)
{
  for (char *index = array; *index != '\0'; index += 2)
  {
    if (*(index + 2) != '\0')
    {
      printf("%c%c ", *index, *(index + 1));
    }
    else
    {
      printf("%c%c", *index, *(index + 1)); // if last bigram
    }
  }
  printf("\n");
}

