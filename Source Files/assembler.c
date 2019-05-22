/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
-------------------------------------------
assembler.c

This is the main program of the assembler.
------------------------------------------- 
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
 
#include "assembler.h"

int main(int argc, char** argv)
{

	/* main data structures */	
	MemoryWord* data;                     /* data array */
	MemoryWord* instructions;	      /* instructions array */	
	Symbol* symbHead;                     /* symbol table */
	ExternalRef* extRefHead;              /* external references list */

	/* storage for instruction and data counters */ 
	int* counters;

	/* command matrix */
	char** cmdMat;

	/* file pointers */
	FILE* fp;                   /* pointer to input file */
	FILE* objectFp;             /* pointer to object file (output) */
	FILE* externFp;             /* pointer to external references file (output) */
	FILE* entryFp;              /* pointer to entry points list (output) */

	/* creating storage for IC and DC counters */
	counters = (int*)malloc(sizeof(int) * COUNTERS);

	/* handle all of the command line arguments */
	int i;
	for(i=1;  i<argc;  i++)
	{
		printf("\n");
		/* check validity of command line arguments */
		if (argc < 2)
		{
			fputs("error: missing files.\n",stderr);
			exit(ERROR_MISSING_ARGUMENTS);
		}

		/* initialization */	
		data = NULL;
		instructions = NULL;
		symbHead = NULL;
		extRefHead = NULL;
		counters[0] = counters[1] = 0;

		/* generating commands matrix */
		generateCommands(&cmdMat);
			
		/* open source file */				
		openFile(&fp,argv,i);

		/* perform first scan (if errors have been found, scan the next file) */
		if (!firstScan(fp,&data,&instructions,&symbHead,cmdMat,counters))
			continue;

		/* rewind */
		fseek(fp,0,SEEK_SET);

		/* perform second scan (if errors have been found, scan the next file) */
		if (!secondScan(fp,&data,&instructions,&symbHead,&extRefHead,cmdMat,counters))
			continue;		

		/* create object file */
		createObjectFile(&objectFp,argv,i,&data,&instructions,counters); 
		closeFile(objectFp);
		
		/* create entry file */
		if (hasEntrySymbols(symbHead))
		{
			createEntryFile(&entryFp,argv,i,symbHead);
			closeFile(entryFp);
		}

		/* create extern file */
		if (!isEmpty(extRefHead))
		{
			createExternFile(&externFp,argv,i,extRefHead);
			closeFile(externFp);
		}
					
	}
	/* free allocated memory */
	freeMemory(data);
	freeMemory(instructions);
	freeMemory(symbHead);
	freeMemory(extRefHead);
	freeMemory(cmdMat);
	freeMemory(counters);
			
}
