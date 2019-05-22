/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
------------------------------------------------------------------------------
globals.h

this file contain global constants that the other files in the program can use.
------------------------------------------------------------------------------- 
*/

#ifndef GLOBALS_H
#define GLOBALS_H

/* boolean flag (TRUE or FALSE) */
typedef unsigned int boolean;

/* boolean flag status */
#define TRUE  1
#define FALSE 0

/* data and instruction containers */
#define COUNTERS 2

/* commands */
#define COMMANDS 16
#define CMDLEN 4

/* memory words */
#define WORDSIZE 12

/* lines */
#define MAX_LINE_LEN 80

/* labels */
#define MAX_LABEL_LEN 31

/* head address */
#define HEAD_ADDRESS 100

/* letters and numbers */
#define LETTERS              26
#define SINGLE_DIGIT_NUMBERS 10

/* base 64 */
#define BASE_64_CHARACTERS 2

/* file extensions */
#define SOURCE_FILE    3
#define OBJECT_FILE    3
#define ENTRY_FILE     4
#define EXTERN_FILE    4

/* error status */
#define ERROR_FILE                 19
#define ERROR_MEMORY               29
#define ERROR_MISSING_ARGUMENTS    39
#define ERROR_UNKNOWN_DATA         49

/* external and entry symbols address */
#define NONE 0      

#endif
