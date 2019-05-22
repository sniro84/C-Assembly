/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------------------------
secondScan.c 

This file handles the second scan of the input source file. 
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "assembler.h"

/* second scan of input source file*/
boolean secondScan (FILE* fp, MemoryWord** pData, MemoryWord** pInstructions, 
		 Symbol** pSymbHead, ExternalRef** pExtRefHead, char** cmdMat, int* counters)
{
	int IC;               /* instruction counter */
	int lineNumber;       /* line number */
	int L;                /* length of code (number of memory words needed for the code segment) */
	boolean symbDef;      /* boolean flag that turns on if a symbol has been defined during line scan */
	boolean hasError;     /* boolean flag that turns on if errors have been found during the file scan */
	char* field;          /* for storing symbol definitions, commands or directives */
	char* label;          /* for storing symbol labels */
	char* srcOper;        /* source operand */
	char* destOper;       /* destination operand */

	/* stage 1 : initialization */
	IC = 0;
	lineNumber = 0;
	hasError = FALSE;
	

	/* stage 2 : read file line by line */
	char buf[MAX_LINE_LEN];
	while (!feof(fp))
	{
		fgets(buf,MAX_LINE_LEN,fp);
		lineNumber++;
		symbDef = FALSE;
		L = 0;
		/* field = label = srcOper = destOper = NULL; */

		int i,j,k;            /* iterators */
		int lineLen;          /* length of the line (number of characters */
			
		lineLen = strlen(buf);
		field = (char*)malloc(sizeof(char) * lineLen);
		memset(field,'\0',strlen(field));

		/* if line is empty or a comment line, skip to the next line */
		if (isEmptyLine(buf) || isCommentLine(buf))
			continue;
		
		/* skip spaces until it gets to first field of the line*/	
		for (i=0; isspace(buf[i]); i++); 

		/* get the first field from the line*/	
		for (i,j=i; !(isspace(buf[i])); i++);
		strncpy(field,&(buf[j]),i-j);

		/* stage 3 : if the first field is a symbol definition, skip to the next field. */
		if (isSymbolDefinition(field))
		{
 
			for (i; i<lineLen && isspace(buf[i]); i++);   /* skip spaces */
			if (i < lineLen)       
			{
				for (i,j=i; !(isspace(buf[i])); i++);
				memset(field,'\0',strlen(field));  /* reset field to empty string */	
				strncpy(field,&(buf[j]),i-j);	
			}
		}
		
		/* stage 4 : if the current fieldis a .data , .string or .extern directives, return to stage 2. */  		             
		if (isStringDirective(field) || isDataDirective(field) || isExternDirective(field))
			continue;
	
		/* stage 5 : if the current field is not a .entry directive, go to stage 7. */
		if (isEntryDirective(field))
		{
			/* go to next word */ 
			for (i; i<lineLen && isspace(buf[i]); i++);   /* skip spaces */
			if (i < lineLen)       
			{
				for (i,j=i; !(isspace(buf[i])); i++);
				memset(field,'\0',strlen(field));  /* reset field to empty string */	
				strncpy(field,&(buf[j]),i-j); 	
			}

			/* stage 6 : mark entry symbol in the table. */
			Symbol* curr;
			for(curr = *pSymbHead; curr != NULL; curr = curr->next)
				if (strcmp(curr->label,field) == 0)
					curr->mark = ENTRY;
		}

		/* stage 7 : code additional memory words of instruction. */ 
		else
		{
			int operands;
			operands = 0;
						
			/* scan for source and destination operands and store them for further handling. */
			while (i<lineLen)
			{
				/* skip spaces*/
				if (isspace(buf[i])) 
					for(i; i<lineLen && isspace(buf[i]); i++);

				/* a comma has been found */
				else if (buf[i] == ',') 
					for(i=i+1; i<lineLen && isspace(buf[i]); i++);

				/* found first character of an operand */
				else
				{
					operands++;
					for (i,j=i; !(isspace(buf[i])) && buf[i] != ','; i++);

					if (requiredOperands(field,cmdMat) == 1)
					{
						destOper = (char*)malloc(sizeof(char)*(i-j+1));
						memset(destOper,'\0',strlen(destOper));
						strncpy(destOper,&(buf[j]),i-j);	
					}
						
					else if (requiredOperands(field,cmdMat) == 2)
					{
						if (operands == 1)
						{
							srcOper = (char*)malloc(sizeof(char)*(i-j+1));
							memset(srcOper,'\0',strlen(srcOper));
							strncpy(srcOper,&(buf[j]),i-j);
						}
						else if (operands == 2)
						{
							destOper = (char*)malloc(sizeof(char)*(i-j+1));
							memset(destOper,'\0',strlen(destOper));
							strncpy(destOper,&(buf[j]),i-j);	
						}	
					}					
				}
			}

			/* if one of the operands is a symbol, make sure it exists in the symbol table. */
			if ( (isValidSymbol(destOper) && !symbolExists(*pSymbHead,destOper)) ||
			     (isValidSymbol(srcOper) && !symbolExists(*pSymbHead,srcOper)) )
			{
				hasError = TRUE;
				printf("error at line #%d : undefined symbol.\n" , lineNumber);
				continue;
			}
			   

			/* compute length of memory words needed for that line (L) */
			L = codeLength(operands,srcOper,destOper);

			/* insert L-1 additional memory words. */ 
			insertAddition(pInstructions,*pSymbHead,pExtRefHead,srcOper,destOper,field,cmdMat,IC,L);
				
		} /* end of stage 7 */
		
		/* stage 8 : update instructions counter */
		IC = IC + L;

		/* stage 9 : return to stage 2 */

	} /* end of big while */
	
	/* storing IC counter final value for further handling */
	*counters = IC;

	/* stage 10 : if errors have been found, stop the program. */
	if (hasError)
		return FALSE;

	/* stage 11 : continue and create output files */
	return TRUE;
}


