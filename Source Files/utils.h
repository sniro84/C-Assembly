/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
utils.h
----------------------------------------
*/

#ifndef UTILS_H
#define UTILS_H

#include "globals.h"
#include "memoryWord.h"
#include "symbolTable.h"
#include "assembler.h"

/*
 * Function:  isSymbolDefinition
 * -----------------------------
 *  description:      this function checks if a field is a symbol definition.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :         TRUE if a field is a symbol definition
 *                    FALSE otherwise 
 */ 
boolean isSymbolDefinition(char* field);


/*
 * Function:  isEmptyLine
 * -----------------------------
 *  description:      this function checks if line from source file is an empty line.
 *                                      		                          
 *  line:             a line of code from source file.
 * 
 *  returns :         TRUE if a line has nothing but spaces.
 *                    FALSE otherwise 
 */
boolean isEmptyLine(char* line);


/*
 * Function:  isCommentLine
 * -----------------------------
 *  description:      this function checks if line from source file is a comment line.
 *                                      		                          
 *  line:             a line of code from source file.
 * 
 *  returns :         TRUE if a line is a comment line.
 *                    FALSE otherwise 
 */
boolean isCommentLine(char* line);


/*
 * Function:  isStringDirective
 * -----------------------------
 *  description:     this function checks if a field is a ".string" directive.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a ".string" directive
 *                   FALSE otherwise 
 */ 
boolean isStringDirective(char* field);


/*
 * Function:  isDataDirective
 * -----------------------------
 *  description:     this function checks if a field is a ".data" directive.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a ".data" directive
 *                   FALSE otherwise 
 */
boolean isDataDirective(char* field);

/*
 * Function:  isExternDirective
 * -----------------------------
 *  description:     this function checks if a field is a ".extern" directive.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a ".extern" directive
 *                   FALSE otherwise 
 */
boolean isExternDirective(char* field);


/*
 * Function:  isEntryDirective
 * -----------------------------
 *  description:     this function checks if a field is a ".entry" directive.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a ".entry" directive
 *                   FALSE otherwise 
 */
boolean isEntryDirective(char* field);



/*
 * Function:  isValidRegister
 * -----------------------------
 *  description:     this function checks if a field is a valid register.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a valid register
 *                   FALSE otherwise 
 */
boolean isValidRegister(char* field);



/*
 * Function:  isValidNumber
 * -----------------------------
 *  description:     this function checks if a field is a valid number.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a valid number
 *                   FALSE otherwise 
 */
boolean isValidNumber(char* field);



/*
 * Function:  isValidSymbol
 * -----------------------------
 *  description:     this function checks if a field is a valid symbol label.
 *                                      		                          
 *  field:           a string representation of field (code segment from source file)
 * 
 *  returns :        TRUE if a field is a valid symbol label
 *                   FALSE otherwise 
 */
boolean isValidSymbol(char* field);




/*
 * Function:  isLegalOperand
 * -----------------------------
 *  description:     this function checks if a field is a legal opernad.
 *                                      		                          
 *  operand:         a field that contains an operand.
 * 
 *  returns :        TRUE if a operand field is a register, number or symbol
 *                   FALSE otherwise 
 */
boolean isLegalOperand(char* operand);




/*
 * Function:  operandTypeMismatch
 * -----------------------------
 *  description:     this function checks if a field is a legal opernad.
 *                                      		                          
 *  operand:         a field that contains an operand.
 * 
 *  returns :        TRUE if a command and operands are incompatible
 *                   FALSE otherwise 
 */
boolean operandTypeMismatch(char* command, char* srcOper, char* destOper);



/*
 * Function:  getAddressMethod
 * -----------------------------
 *  description:     this function gets an operand and returns its address method.
 *                                      		                          
 *  operand:         a field that contains an operand.
 * 
 *  returns :        address method of operand.
 *                   
 */
addressMethod getAddressMethod(char* operand);



/*
 * Function:  codeLength
 * ---------------------
 *  description:     this function gets a number of operands in a command and source/dest operands and returns 
 *                   the number of memory words required to code.
 *
 *  operands:        number of operands (0, 1 or 2).                             		                          
 *  srcOper:         a field that contains a source operand.
 *  destOper:        a field that contains a destination operand.
 * 
 *  returns :        the number of memory words required to code (instruction + additional words).
 *                   
 */
int codeLength(int operands, char* srcOper, char* destOper);



/*
 * Function:  freeMemory
 * ---------------------
 *  description:     this function gets a pointer to memory location  
 *                   and frees the memory that the pointer points to.
 *
 *  ptr:             general pointer to memory location.
 *                   
 */
void freeMemory(void* ptr);




/*
 * Function:  getRegID
 * ---------------------
 *  description:     this function gets a register string and returns the register ID number.
 *                   
 *  reg:             register string.                             		                          
 * 
 *  returns :        register ID.
 *                   
 */
int getRegID(char* reg);




/*
 * Function:  twosComplement
 * -------------------------
 *  description:     this function gets an integer and a number of bits and returns the integer's binary
 *                   representation in that number of bits according to two's complement method.
 *                   
 *  number:          the integer.
 *  bits:            number of bits that will be used to store the number.                             		                          
 *  
 *  returns :        string binary representation of the number.
 *                   
 */
char* twosComplement(int number, int bits);




/*
 * Function:  codeSegmentInBase64
 * -------------------------
 *  description:     this function gets a binary string and returns it's base64 form.
 *                                       
 *  binaryCode:      a string representation of binary machine code.                            		                          
 *  
 *  returns :        string representation of code in base64 form.
 *                   
 */
char* codeSegmentInBase64(char* binaryCode);



#endif
