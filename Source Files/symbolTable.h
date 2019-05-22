/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
symbolTable.h
----------------------------------------
*/ 

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "globals.h"

/* Symbol mark */
typedef enum {DATA,EXTERN,CODE,ENTRY} Mark;


/*
 * Structure:  	      Symbol
 * -------------------------
 * description:       this structure contains a label name, a memory address
 *                    and a mark (type of symbol) 
 *                     
 * label:             string representation of symbol name. 
 * address:           address of the symbol.
 * mark:              symbol mark (external or entry symbols are defined outside of the source file).    
 *                 
 */
typedef struct Symbol
{
	char* label;
	int address;
	Mark mark;
	struct Symbol* next;
} Symbol;


/*
 * Function:  insertSymbol
 * -----------------------
 * description:      this function inserts a symbol to table.
 *                                      		                          
 *  pHead:           a pointer to a pointer to the head of the symbol table.
 *  label :          string representation of symbol name.  
 *  address:         address of the symbol.
 *  mark:            symbol mark.
 *  
 */ 
void insertSymbol(Symbol** pHead,char* label, int address, Mark mark);


/*
 * Function:  symbolExists
 * ------------------
 * description:     this function checks whether a symbol already exists in the symbol table. 
 *                                      		                                               
 * head:            pointer to head of the symbol table.
 * label:           string representation of symbol name.
 *   
 * returns:         TRUE if it already exists in the symbol table.
 *                  FALSE otherwise.
 *                            
 */
boolean symbolExists(Symbol* head , char* label); 


/*
 * Function:  hasEntrySymbols
 * ------------------
 * description:     this function checks if symbol table contains entry symbols. 
 *                                      		                                               
 * head:            pointer to head of the symbol table.
 *   
 * returns:         TRUE if symbol table contains entry symbols.
 *                  FALSE otherwise.
 *                            
 */
boolean hasEntrySymbols(Symbol* head);


/*
 * Function:  getLabelMark
 * -----------------------
 * description:     this function returns the mark of a symbol with specific label name.
 *                                      		                                               
 * head:            pointer to head of the symbol table.
 * label:           name of the label.
 *   
 * returns:         mark of a symbol.
 *                                              
 */
Mark getLabelMark(Symbol* head , char* label);


/*  */

/*
 * Function:  getLabelAddress
 * --------------------------
 * description:     this function returns the address of the label.
 *                                      		                                               
 * head:            pointer to head of the symbol table.
 * label:           name of the label.
 *   
 * returns:         address of the label.
 *                                              
 */
int getLabelAddress(Symbol* head , char* label);



#endif

