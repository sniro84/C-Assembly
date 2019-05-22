/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
assembler.h
---------------------------------------- 
*/


#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "globals.h"
#include "symbolTable.h"
#include "memoryWord.h"
#include "fileHandling.h"
#include "externalRefs.h"


/*
 * Function:  firstScan 
 * --------------------
 * description:       this function scans the input source file line by line, looks for
 *                    syntax errors and inserts contents into data and instruction containers
 *                    and into symbol table. it also stores final values in data and instructions
 *                    counters. if errors have been found, second scan of source file will not take place.
 *    
 *  fp:               address of source file.
 *  pData:            address of data container. 
 *  pInstruction:     address of instruction container.
 *  pSymbHead:        address of the head symbol in the symbol table. 
 *  cmdMat:           command matrix.
 *  counters:         address of instruction and data counters container.  
 *
 *  returns:          TRUE if the lines of source file doesn't contain errors.
 *                    FALSE otherwise.
 *           
 */
boolean firstScan (FILE* fp, MemoryWord** pData, MemoryWord** pInstructions,
	            Symbol** pSymbHead, char** cmdMat, int* counters);



/*
 * Function:  secondScan 
 * ---------------------
 * description:       this function scans the input source file line by line just like the first scan,
 *                    but here all symbols are already in the symbol table so that operands which are
 *                    symbols can get their correct address from the symbol table (if such symbol doesn't exist, 
 *                    an error will occur.) additional contents of instructions will be added to instruction
 *                    container. this function also inserts references to external symbols (if such exist) to 
 *                    the external references table.     
 *     
 *  fp:               address of source file.
 *  pData:            address of data container. 
 *  pInstruction:     address of instruction container.
 *  pSymbHead:        address of the head symbol in the symbol table.
 *  pExtRefHead:      address of the head reference in the external references table. 
 *  cmdMat:           command matrix.
 *  counters:         address of instruction and data counters container.  
 *
 *  returns:          TRUE if the lines of source file doesn't contain errors.
 *                    FALSE otherwise.
 *           
 */
boolean secondScan (FILE* fp, MemoryWord** pData, MemoryWord** pInstructions, 
		     Symbol** pSymbHead, ExternalRef** pExtRefHead, char** cmdMat, int* counters);

/* instruction table */
extern MemoryWord* instructions;

/* data table */
extern MemoryWord* data;

/* symbol table head */
extern Symbol* symbHead;

/* external references head */
extern ExternalRef* extRefHead;

/* commands matrix */
extern char** cmdMat;

/* instrctions counter (IC) and data counter (DC) */
extern int* counters;


#endif
