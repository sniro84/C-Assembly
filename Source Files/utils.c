/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
utils.c

This file contains helper methods.
----------------------------------------
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#include "utils.h"

/* this function returns true if a field is a symbol definition, false otherwise */
boolean isSymbolDefinition(char* field)
{
	boolean result;
	result = TRUE;
	int lastCharPos = strlen(field)-1;

	/* if the field is too long, does not start with a letter or has a last character
	   which is different than ':', the field is not a symbol definition.*/
	if (strlen(field) > MAX_LABEL_LEN || !(isalpha(field[0]))  ||  field[lastCharPos] != ':')
		result = FALSE;
	int i;

	/* if any of the other characters besides the first and last are not a digit or a letter,
	   the field is not a symbol definition. */
	for (i=1; i<lastCharPos; i++)
	{
		if (!(isalnum(field[i])))
		{
			result = FALSE;
			break;
		}
	}
	return result;
}

/* this function returns true if a line has nothing but spaces, false otherwise */
boolean isEmptyLine(char* line)
{
	int i;
	for (i=0; i<strlen(line); i++)
		if (!isspace(line[i])) /* first non-space character exists */
			return FALSE;
	return TRUE;
}

/* this function returns true if a line is a comment line, false otherwise */
boolean isCommentLine(char* line)
{
	int i;
	for (i=0; i<strlen(line); i++)
		if (!isspace(line[i]))
			return (line[i] == ';'); /* if first non-space character is ';', this is a comment line.  */
}

/* this function returns true if a field is a ".string" directive, false otherwise */
boolean isStringDirective(char* field)
{
	return (strcmp(field,".string") == 0);
}

/* this function returns true if a field is a ".data" directive, false otherwise */
boolean isDataDirective(char* field)
{
	return (strcmp(field,".data") == 0);
}

/* this function returns true if a field is a ".extern" directive, false otherwise */
boolean isExternDirective(char* field)
{
	return (strcmp(field,".extern") == 0);
}

/* this function returns true if a field is a ".entry" directive, false otherwise */
boolean isEntryDirective(char* field)
{
	return (strcmp(field,".entry") == 0);
}

/* this function returns true if a field is a valid register, false otherwise */
boolean isValidRegister(char* field)
{
	return (strlen(field) == 3   &&   field[0] =='@'  &&
		field[1] == 'r'   &&   field[2] >= '0'  &&  field[2] <= '7');
}

/* this function returns true if a field is a valid number, false otherwise */
boolean isValidNumber(char* field)
{
	if (!field)
		return FALSE;

	int i;
	if (field[0] != '+' && field[0] != '-' &&  !isdigit(field[0]))
		return FALSE;
	for(i=1; i<strlen(field); i++)
		if (!isdigit(field[i]))
			return FALSE;
	return TRUE;
}

/* this function returns true if a field is a valid symbol label, false otherwise */
boolean isValidSymbol(char* field)
{
	int i;
	if (!(isalpha(field[0])))
		return FALSE;
	for(i=1; i<strlen(field); i++)
		if (!isalnum(field[i]))
			return FALSE;
	return TRUE;		
}

/* this function returns true if a field is a legal operand, false otherwise */
boolean isLegalOperand(char* operand)
{
	return (isValidNumber(operand) || isValidRegister(operand) || isValidSymbol(operand));
}

/* this function returns true if a command and operands are incompatible, false otherwise */
boolean operandTypeMismatch(char* command, char* srcOper, char* destOper)
{
	if (strcmp(command,"cmp") != 0  &&  strcmp(command,"prn") != 0  &&
	    strcmp(command,"rts") != 0  &&  strcmp(command,"stop") != 0  &&
	    (destOper) && getAddressMethod(destOper) == IMMEDIATE)
		return TRUE;
	if (strcmp(command,"lea") == 0  &&  (srcOper)  &&  getAddressMethod(srcOper) != DIRECT)
		return TRUE;
	return FALSE;  
} 

/* this function gets an operand and returns its address method */
addressMethod getAddressMethod(char* operand)
{
	if (isValidNumber(operand))
		return IMMEDIATE;
	else if (isValidSymbol(operand)) 
		return DIRECT;
	else if (isValidRegister(operand))
		return REG_DIRECT;
	else /* no operand */
		return IRRELEVANT;
		
}

/* this method gets a number of operands in a command and source/dest operands and returns 
the number of memory words required to code (instruction + additional words)  */
int codeLength(int operands, char* srcOper, char* destOper)
{
	if (operands == 0)
		return 1;
	else if ((operands == 1)  ||  (operands == 2 && isValidRegister(srcOper) && isValidRegister(destOper)))
		return 2;
	else 
		return 3; 
} 

/* this function gets a register string and returns the register ID number  */
int getRegID(char* reg)
{
	char* result;
	result = (char*)malloc(sizeof(char) * 2);
	strncpy(result,reg+2,1); /* copy the third character to result string */
	return atoi(result);
}

/* this function gets an integer and returns its binary representation according to two's complement method */
char* twosComplement(int number, int bits)
{
	int i,k;
	char* result;
	result = (char*)malloc(sizeof(char) * bits + 1);

	/* initialize */
	for(i=0; i<bits; i++)
		*(result+i) = '0';

	if (number < 0)
		number += pow(2,bits);

	for(k=bits; number>0; number>>=1,k--)
			*(result+k-1) = (number%2 == 1) ? '1' : '0';
	return result;
}

/* this function gets a binary string and returns it's base64 form */
char* codeSegmentInBase64(char* binaryCode)
{
	int i,j,k;
	int total;
	char* result;

	result = (char*)malloc(strlen("00")+1);


	/* the right side of the word */
	total = 0;
	for(i=WORDSIZE-1 ,j=0 ; i>=WORDSIZE/2; i--,j++)
	{
		if (binaryCode[i] == '1')
			total += pow(2,j);
	}

	if (total >=0 && total < LETTERS)
		*(result+1) = 'A' + total;
	else if (total >= LETTERS && total < 2 * LETTERS)
		*(result+1) = 'a' - LETTERS + total; 
	else if (total >= 2 * LETTERS && total < 2 * LETTERS + SINGLE_DIGIT_NUMBERS)
		*(result+1) = '0' - (2 * LETTERS) + total; 
	else if (total == 2 * LETTERS + SINGLE_DIGIT_NUMBERS)
		*(result+1) = '+' - (2 * LETTERS + SINGLE_DIGIT_NUMBERS) + total;
	else 
		*(result+1) = '/' - (2 * LETTERS + SINGLE_DIGIT_NUMBERS + 1) + total;

	/* the left side of the word */
	total = 0;
	for(i ,j=0 ; i>=0; i--,j++)
	{
		if (binaryCode[i] == '1')
			total += pow(2,j);
	}
	
	if (total >=0 && total < LETTERS)
		*result = 'A' + total;
	else if (total >= LETTERS && total < 2 * LETTERS)
		*result = 'a' - LETTERS + total; 
	else if (total >= 2 * LETTERS && total < 2 * LETTERS + SINGLE_DIGIT_NUMBERS)
		*result = '0' - (2 * LETTERS) + total; 
	else if (total == 2 * LETTERS + SINGLE_DIGIT_NUMBERS)
		*result = '+' - (2 * LETTERS + SINGLE_DIGIT_NUMBERS) + total;
	else 
		*result = '/' - (2 * LETTERS + SINGLE_DIGIT_NUMBERS + 1) + total;	

	return result;	
}

/* this function gets a pointer to memory location and frees the memory that the pointer points to */
void freeMemory(void* ptr)
{
	if (ptr)
	    free(ptr);
}
 
