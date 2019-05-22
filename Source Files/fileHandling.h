/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
fileHandling.h
---------------------------------------- 
*/
 
#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "memoryWord.h"

/*
 * Function:  openFile
 * ----------------------------------
 * description:      this function opens a file from command-line arguments vector
 *                   and adds ".as" extension to it.
 *                   		                          
 *  pfp:             a pointer to a pointer to the file.
 *  argv:            a pointer to the command-line arguments vector.
 *  i:               index of the command-line argument in which the file pointer is at. 
 *          
 */
void openFile(FILE** pfp, char** argv, int i);


/*
 * Function:  closeFile
 * ----------------------------------
 * description:      this function closes a file.
 *                   		                          
 *  fp:              a pointer to a file. 
 *         
 */
void closeFile(FILE* fp);


/*
 * Function:  createObjectFile
 * ----------------------------------
 * description:      this function creates an object file with machine code translated to base64.
 *                                      		                          
 *  pfp:             a pointer to a pointer to the file.
 *  argv:            a pointer to the command-line arguments vector.
 *  i:               index of the command-line argument in which the file pointer is at.
 *  pData:           a pointer to the container of data.
 *  pInstructions:   a pointer to the container of instructions.
 *  counters:        a pointer to the counters of data and instructions (number of elements).
 *  
 */
void createObjectFile(FILE** pfp, char** argv,int i, MemoryWord** pData, MemoryWord** pInstructions, int* counters);


/*
 * Function:  createEntryFile
 * ----------------------------------
 * description:      this function creates an entry file contains labels with ENTRY mark and their addresses (if exist)
 *                                      		                          
 *  pfp:             a pointer to a pointer to the file.
 *  argv:            a pointer to the command-line arguments vector.
 *  i:               index of the command-line argument in which the file pointer is at.
 *  SymbHead	     a pointer to the head symbol in the symbol table. 
 *  
 */
void createEntryFile(FILE** pfp, char** argv,int i, Symbol* SymbHead);


/*
 * Function:  createExternFile
 * ----------------------------------
 * description:      this function creates an extern file contains references to external labels and their addresses (if exist)
 *                                      		                          
 *  pfp:             a pointer to a pointer to the file.
 *  argv:            a pointer to the command-line arguments vector.
 *  i:               index of the command-line argument in which the file pointer is at.
 *  extRefHead	     a pointer to the head reference in the external references container. 
 *  
 */

void createExternFile(FILE** pfp, char** argv,int i, ExternalRef* extRefHead);

#endif
