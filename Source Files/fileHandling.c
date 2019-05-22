/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------------------------
fileHandling.c

This is a file that contains functions that handles files. 
---------------------------------------------------------- 
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "fileHandling.h"

/* this function opens a file and adds ".as" extension to it. */
void openFile(FILE** pfp, char** argv, int i) 
{
	char* filename;
	filename = (char*)malloc(sizeof(char) * (strlen(argv[i]) + SOURCE_FILE + 1)); 
	strcpy(filename,argv[i]);
	strcat(filename,".as");            
	*pfp = fopen(filename,"r");     /* pfp --> pointer to file pointer */

	/* check if the file has been successfully opened */
	if (*pfp == NULL)
	{
		fputs("error: can't open file\n",stderr);
		exit(ERROR_FILE);
	}	
	printf("Opening file : %s \n",filename);	
}

/* this function closes a file. */
void closeFile(FILE* fp)
{
	if (fp)
	{
	     fclose(fp);
	}		
}

/* this function creates an object file with machine code translated to base64. */
void createObjectFile(FILE** pfp, char** argv,int i, MemoryWord** pData, MemoryWord** pInstructions, int* counters)
{
	/* add ".ob" extension to file */
	char* filename;
	filename = (char*)malloc(sizeof(char) * (strlen(argv[i]) + OBJECT_FILE + 1));
	strcpy(filename,argv[i]);
	strcat(filename,".ob");

	/* open file in write mode */
	*pfp = fopen(filename,"w");

	/* check if the file has been successfully opened */
	if (*pfp == NULL)
	{
		fputs("error: can't open file\n",stderr);
		exit(ERROR_FILE);
	}

	/* write instruction and data counters to object file */
	fprintf(*pfp,"%d %d\n",*counters,*(counters+1));
	
	/* write instructions binary code in base64 form to object file */
	char* output;
	for (i=0; i<(*counters); i++)
	{
		output = (char*)binaryMachineCode((*pInstructions)[i]);
		fprintf(*pfp,"%s\n",(char*)codeSegmentInBase64(output));		
	}
	
	/* write data binary code in base64 form to object file */
	for (i=0; i< *(counters+1); i++)
	{
		output = (char*)binaryMachineCode((*pData)[i]);
		fprintf(*pfp,"%s\n",(char*)codeSegmentInBase64(output));	
	}
	printf("file : %s has been created successfully.\n", filename);	
}

/* this function creates an entry file contains labels with ENTRY mark and their addresses (if exist). */
void createEntryFile(FILE** pfp, char** argv,int i, Symbol* SymbHead)
{	
	/* add ".ent" extension to file */
	char* filename;
	filename = (char*)malloc(sizeof(char) * (strlen(argv[i]) + ENTRY_FILE + 1));
	strcpy(filename,argv[i]);
	strcat(filename,".ent");

	/* open file in write mode */
	*pfp = fopen(filename,"w");
	
	/* check if the file has been successfully opened */
	if (*pfp == NULL)
	{
		fputs("error: can't open file\n",stderr);
		exit(ERROR_FILE);
	}
	
	/* write data to entry file */
	Symbol* curr;
	for(curr = SymbHead; curr != NULL; curr = curr->next)
		if (curr->mark == ENTRY)
			fprintf(*pfp,"%s\t%d\n",curr->label,curr->address);
	
	printf("file : %s has been created successfully.\n", filename);	
}

/* this function creates an extern file contains references to external labels and their addresses (if exist). */
void createExternFile(FILE** pfp, char** argv,int i, ExternalRef* extRefHead)
{	
	/* add ".ext" extension to file */
	char* filename;
	filename = (char*)malloc(sizeof(char) * (strlen(argv[i]) + EXTERN_FILE + 1));
	strcpy(filename,argv[i]);
	strcat(filename,".ext");

	/* open file in write mode */
	*pfp = fopen(filename,"w");

	/* check if the file has been successfully opened */
	if (*pfp == NULL)
	{
		fputs("error: can't open file\n",stderr);
		exit(ERROR_FILE);
	}
		
	/* write data to entry file */
	ExternalRef* curr;
	for(curr = extRefHead; curr != NULL; curr = curr->next)
		fprintf(*pfp,"%s\t%d\n",curr->label,curr->address);

	printf("file : %s has been created successfully.\n", filename);
}
