/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
-----------------------------------------------
commands.c

This is a file that contains command functions. 
-----------------------------------------------
*/

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

#include "commands.h"

/* this function generates commands list */
void generateCommands(char*** pCmdMat)
{
	/* memory allocation */
	*pCmdMat = (char**)malloc(sizeof(char*) * COMMANDS);
	if (*pCmdMat == NULL)
	{
		printf("memory allocation failed\n");
		exit(ERROR_MEMORY);
	}	
	
	int i;
	for(i=0; i<COMMANDS; i++)
		(*pCmdMat)[i] = (char*)malloc(sizeof(char) * (CMDLEN+1));

	/* copy strings of commands to array */
	strcpy((*pCmdMat)[0],"mov");
	strcpy((*pCmdMat)[1],"cmp");
	strcpy((*pCmdMat)[2],"add");
	strcpy((*pCmdMat)[3],"sub");
	strcpy((*pCmdMat)[4],"not");
	strcpy((*pCmdMat)[5],"clr");
	strcpy((*pCmdMat)[6],"lea");
	strcpy((*pCmdMat)[7],"inc");
	strcpy((*pCmdMat)[8],"dec");
	strcpy((*pCmdMat)[9],"jmp");
	strcpy((*pCmdMat)[10],"bne");
	strcpy((*pCmdMat)[11],"red");
	strcpy((*pCmdMat)[12],"prn");
	strcpy((*pCmdMat)[13],"jsr");
	strcpy((*pCmdMat)[14],"rts"); 
	strcpy((*pCmdMat)[15],"stop");
}

/* this function gets a command and returns the number of operands it needs or -1 if command is illegal */
int requiredOperands(char* command, char** cmdMat) 
{
	if (!isLegalCommand(command, cmdMat))
		return -1;
	else if (strcmp(command, "rts") == 0   ||  strcmp(command, "stop") == 0)
		return 0;
	else if (strcmp(command, "mov") == 0   ||  strcmp(command, "cmp") == 0    ||
		 strcmp(command, "add") == 0   ||  strcmp(command, "sub") == 0    ||  						
	 	 strcmp(command, "lea") == 0)
		return 2;
	else
		return 1;
}
	
/* this function gets a command and returns true if the command is legal, false otherwise.  */
boolean isLegalCommand(char* command, char** cmdMat)
{
	int i;
	for(i=0; i<COMMANDS; i++)
		if (strcmp(command, cmdMat[i]) == 0)
			return TRUE;
	return FALSE;
}

/* this function gets a command string and returns its operation code (integer in the range [0,15]) */
int getOpCode(char* command, char** cmdMat)
{
	int i;
	for(i=0; i<COMMANDS; i++)
		if (strcmp(command, cmdMat[i]) == 0)
			return i;
}

