/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
externalRefs.h
----------------------------------------
*/
 
#ifndef EXTERNAL_REFS_H
#define EXTERNAL_REFS_H

#include "globals.h"


/*
 * Structure:  ExternalRef
 * ----------------------------------------------
 * description:       this structure contains a list of references to labels 
 *                    (symbols) that are not in the source file (external).   
 *                     
 *  label:            name of the label.
 *  address:          address of the label.
 *  next:             a pointer to the next reference on the list.
 *         
 */
typedef struct ExternalRef
{
	char* label;
	int address;
	struct ExternalRef* next;
} ExternalRef;

/*
 * Function:  insertExternalReference
 * ----------------------------------
 * description:      this function inserts a reference to external symbols into
 *                   a container of references.		      
 *                     
 *                     
 *  pHead:           a pointer to a pointer to the head of the list of references.
 *  label:           name of the label.
 *  address:         address of the label. 
 *          
 */
void insertExternalReference(ExternalRef** pHead, char* label, int address);


/*
 * Function:  isEmpty
 * ------------------
 * description:     this function checks if there are any references to external
 *                  symbols in the container.                      		                          
 *                     
 * head:            a pointer to the head of the list of references.
 *   
 * returns:         TRUE if the container of references is empty.
 *                  FALSE otherwise.          
 */
boolean isEmpty(ExternalRef* head);

#endif
