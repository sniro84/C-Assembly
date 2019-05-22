/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
memoryWord.h
----------------------------------------
*/

#ifndef MEMORY_WORD_H
#define MEMORY_WORD_H

#include "globals.h"
#include "symbolTable.h"
#include "externalRefs.h"
 
/* Memory Word type */
typedef enum 
{
	INTEGER_WORD = 1,ASCII_WORD = 2,INSTRUCTION = 3,
	IMMEDIATE_VALUE = 4,LABEL_ADDRESS = 5,REGISTER_WORD = 6
} MWType;

/* Coding type (A/R/E) */
typedef enum {ABSOLUTE = 0, EXTERNAL = 1 , RELOCATABLE = 2} codetype;

/* Addressing method */
typedef enum {IRRELEVANT = 0, IMMEDIATE = 1, DIRECT = 3, REG_DIRECT = 5} addressMethod;


/*
 * Structure:  	      MemoryWord
 * -----------------------------------------------------------------------------
 * description:       this structure contains 12-bits which represnts  
 *                    different parts of machine code and additional properties.
 *
 *                    bits 0-1  : coding type
 *                    bits 2-4  : addressing method of destination operand
 *                    bits 5-8  : operation code
 *                    bits 9-11 : addressing method of source operand   
 *                     
 * mwtype:            type of memory word.
 * address:           address of memory word.
 * wordBlock:         a block that contains the parts of the word.
 *         
 */
typedef struct
{
	MWType mwtype;
	int address;
	union
	{
		struct 
		{
			signed int intVal: 12;	
		} integerWord;
	
		struct 
		{	
			unsigned int asciiCode: 12;
		} asciiWord;
	
		struct 
		{
			unsigned int codeType: 2;
			unsigned int destOper: 3;
			unsigned int opCode: 4;
			unsigned int sourceOper: 3;		
		} instruction;
	 
		struct 
		{
			unsigned int codeType: 2;
			signed int imVal: 10;
		} immediateValue;

		struct 
		{
			unsigned int codeType: 2;
			unsigned int address: 10;
		} labelAddress;

		struct 
		{
			unsigned int codeType: 2;
			unsigned int destReg: 5;
			unsigned int sourceReg: 5;
		} regWord;	
	} wordBlock;
} MemoryWord;


/*
 * Function:  insertData
 * ----------------------------------
 * description:      this function inserts a memoryWord to data container.
 *                                      		                          
 *  pData:           a pointer to the container of data.
 *  DC :             counter of data elements.   
 *  type:            type of the inserted data.
 *  p:               a general pointer that allows insertion of data at runtime.
 *  
 */
void insertData(MemoryWord** pData ,int DC, MWType type, void* p);


/*
 * Function:  inserInstruction
 * ----------------------------------
 * description:      this function inserts a memoryWord to instructions container.
 *                                      		                          
 *  pInstruction:    a pointer to the container of instructions.
 *  srcOper:         string representation of source operand.
 *  destOper:        string representation of destination operand.
 *  command:         string representation of command.
 *  cmdMat:          a pointer to command matrix.
 *  IC :             counter of data elements.   
 *  L :              additional memory words required for this instruction.
 *  
 */
void inserInstruction(MemoryWord** pInstruction, char* srcOper,
                       char* destOper, char* command ,char** cmdMat ,int IC, int L);



/*
 * Function:  insertAddition
 * ----------------------------------
 * description:      this function inserts an additional memoryWord to instructions container.
 *                                      		                          
 *  pInstruction:    a pointer to the container of instructions.
 *  symbHead:        a pointer to the head symbol in the symbol table.
 *  pExtRefHead	     a pointer to a pointer to the head reference in the external references container.
 *  srcOper:         string representation of source operand.
 *  destOper:        string representation of destination operand.
 *  command:         string representation of command.
 *  cmdMat:          a pointer to command matrix.
 *  IC :             counter of data elements.   
 *  L :              additional memory words that are about to be inserted.
 *  
 */
void insertAddition(MemoryWord** pInstruction, Symbol* symbHead, ExternalRef** pExtRefHead, 
		     char* srcOper,char* destOper, char* command, char** cmdMat, int IC,int L);


/*
 * Function:  binaryMachineCode
 * ------------------
 * description:     this function translates a memory word to a binary machine code string and returns that string.
 *                                      		                                               
 * memWord:         a memory word.
 *   
 * returns:         a string representation of 12-bits machine code.
 *                            
 */
char* binaryMachineCode(MemoryWord memWord);

#endif
