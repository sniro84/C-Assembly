/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------------------------
firstScan.c

This file handles the first scan of the input source file. 
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#include "assembler.h" 

/* first scan of input source file */
boolean firstScan (FILE* fp, MemoryWord** pData, MemoryWord** pInstructions,
                Symbol** pSymbHead, char** cmdMat, int* counters)
{	
	int IC,DC;            /* instruction and data counters */
	int lineNumber;       /* line number */
	int L;                /* length of code (number of memory words needed for the code segment) */
	boolean symbDef;      /* boolean flag that turns on if a symbol has been defined during line scan */
	boolean hasError;     /* boolean flag that turns on if errors have been found during the file scan */
	char* field;          /* for storing symbol definitions, commands or directives */
	char* dataStr;        /* for storing integers */
	char* label;          /* for storing symbol labels */
	char* srcOper;        /* for storing source operand */
	char* destOper;       /* for storing destination operand */	
 
	/* stage 1 : initialization */
	IC = DC = 0;
	lineNumber = 0;
	hasError = FALSE;

	/* stage 2 : read file line by line */
	char buf[MAX_LINE_LEN];
	while (!feof(fp))
	{
		fgets(buf,MAX_LINE_LEN,fp);        /* read next line */
		lineNumber++;       
		symbDef = FALSE;    
		L = 0;
		/* field = dataStr = label = srcOper = destOper = NULL; */

		int i,j,k;            /* iterators */
		int lineLen;          /* length of the line (number of characters */
			
		lineLen = strlen(buf);
		field = (char*)malloc(sizeof(char) * lineLen);
		memset(field,'\0',strlen(field));		

		/* if line contains too many characters, this line will be skipped. */
		if (lineLen > MAX_LINE_LEN)
		{
			hasError = TRUE;
			printf("error at line #%d : too many characters in this line.\n" , lineNumber);
			continue;
		}		

		/* if line is empty or a comment line, skip to the next line */
		if (isEmptyLine(buf) || isCommentLine(buf))
			continue;
		
		/* skip spaces until it gets to first field of the line*/	
		for (i=0; isspace(buf[i]); i++); 

		/* get the first field from the line*/	
		for (i,j=i; !(isspace(buf[i])); i++);
		strncpy(field,&(buf[j]),i-j);

		/* stage 3 : check if the first field is a symbol definition */
		if (isSymbolDefinition(field))
		{
			/* stage 4 : turn on symbol definition flag */
			symbDef = TRUE;
  
			/* store label for further handling  */
			label = (char*)malloc(strlen(field));
			memset(label,'\0',strlen(label));
			strncpy(label,field,strlen(field)-1);
			
			/* go to next word */ 
			for (i; i<lineLen && isspace(buf[i]); i++);   /* skip spaces */
			if (i < lineLen)       
			{
				for (i,j=i; !(isspace(buf[i])); i++);
				memset(field,'\0',strlen(field));  /* reset field to empty string */	
				strncpy(field,&(buf[j]),i-j); 	
			}

			else   /* missing parameters */
			{
				hasError = TRUE;
				printf("error at line #%d : missing parameters.\n" , lineNumber);
				continue;
			}
	   
		}
		
		/* stage 5 : check if the field is a ".data" or ".string" directive */
		if (isStringDirective(field) || isDataDirective(field))
		{
			/* stage 6 : if there is a symbol definition, insert it to symbol table. */
			if (symbDef)
			{	
				if (symbolExists(*pSymbHead,label))  /* check if a symbol has already been defined */
				{
					hasError = TRUE;
					printf("error at line #%d : multiple symbol definitions.\n" , lineNumber);
					continue; 
				}
				else	/* insert symbol to table with data mark and DC value */	
				{      
					insertSymbol(pSymbHead,label,HEAD_ADDRESS + DC,DATA);
				}
			}
			
			/* stage 7 : identify data type, code it in memory and update data counter. */			
			if (isDataDirective(field))
			{
				/* error flags : invalid integer and conssecutive commas */
				boolean invalidInteger, consCommas; 
				invalidInteger = consCommas = FALSE;
				
				/* search for illegal comma before first int */
				for (j=i; j<lineLen && isspace(buf[j]); j++);

				/* search for illegal comma after last int */
				for (k=lineLen-1; k>i && isspace(buf[k]); k--);

				if (buf[j] == ',' ||  buf[k] == ',') /* an illegal comma has been found. */
				{
					printf("error at line #%d : illegal comma.\n" , lineNumber);
					continue;
				}	
								
				/* insertion of all integers to data array */
				while (i<lineLen)
				{
					int value;  /* value of integer to be inserted */
					/* skip spaces*/
					if (isspace(buf[i]))   
						for(i; isspace(buf[i]); i++);

					/* checks the validity of first character of an integer */
					else if (buf[i] == '+' || buf[i] == '-' || isdigit(buf[i]))
					{  
						for (j=i ,i+=1; isdigit(buf[i]); i++);
						if (isspace(buf[i]) || buf[i] == ',')  /* integer is valid. */
						{
							dataStr = (char*)malloc(sizeof(char) * (i-j+1));   /* extracting integer */
							memset(dataStr,'\0',strlen(dataStr));					
							strncpy(dataStr,&(buf[j]),i-j);
							value = atoi(dataStr);       /* turn string dataStr content to integer */      
							insertData(pData,DC,INTEGER_WORD,&value);   /* insertion of integer */
							DC++;   /* update data counter */
						}
						else   /* integer is invalid. */
						{
							invalidInteger = TRUE;
							break;
						}	
					} 
		
					/* validates that there are no conssecutive commas */
					else if (buf[i] == ',')
					{
						for(i=i+1; isspace(buf[i]); i++);
						if (buf[i] ==',')
						{
							consCommas = TRUE;
							break;
						}	
					}
					else  /* integer is invalid. */
					{
						invalidInteger = TRUE;
						break;
					}

				}

				/* invalid integer has been found. */
				if (invalidInteger)
				{
					hasError = TRUE;
					printf("error at line #%d : syntax error (invalid integer).\n" , lineNumber);
				}
				/* conssecutive commas have been found. */
				else if (consCommas)
				{
					hasError = TRUE;
					printf("error at line #%d : consecutive commas.\n" , lineNumber);
				}	
			}  /* end of .data handling */

			else if (isStringDirective(field))
			{
				/* valid string must start with quotation marks (") */
				for (j=i; j<lineLen && isspace(buf[j]); j++);

				/* valid string must end with quotation marks (") */
				for (k=lineLen-1; k>i && isspace(buf[k]); k--);

				if (buf[j] != '"' ||  buf[k] != '"')  /* invalid string */
				{
					hasError = TRUE;
					printf("error at line #%d : syntax error (invalid string).\n" , lineNumber);
				}
				else   /* valid string */
				{	
					/* insert ascii characters */
					for(j=j+1; j<k; j++)
					{ 
						insertData(pData,DC,ASCII_WORD,&buf[j]);
						DC++;
					}
					insertData(pData,DC,ASCII_WORD,"\0");  /* insert suffix */
					DC++;
				}
			}  /* end of .string handling */

			/* return to stage 2. */			
		}	
		/* stage 8 : check if the field is a ".extern" or ".entry" directive */
		else if (isExternDirective(field) || isEntryDirective(field))
		{ 
			/* stage 9 : if this is a ".extern" directive, insert symbol to table 
			   with no value and external mark. */
			if (isExternDirective(field))
			{ 
				/* skip spaces */ 
				for(i; isspace(buf[i]); i++);
				
				/* go to next field */
				for (i,j=i; !(isspace(buf[i])); i++);
				field = (char*)malloc(sizeof(char)*(i-j+1));
				memset(field,'\0',strlen(field));
				strncpy(field,&(buf[j]),i-j);


				/* insert external symbol */ 
				insertSymbol(pSymbHead,field,NONE,EXTERN);
			
				/* make sure there is no additional text after external label */
				for(i; isspace(buf[i]); i++);
				if (i < strlen(buf))
				{
					hasError = TRUE;
					printf("error at line #%d : extraneous text after label.\n" , lineNumber);
				}		
			}
		} /* stage 10 : return to stgae 2. */
		
		/* stage 11 : if there is a symbol definition, insert it to symbol table. */
		else
		{
			int operands;
			int L;
			if (symbDef)
			{	
				if (symbolExists(*pSymbHead,label))  /* check if a symbol has already been defined */
				{
					hasError = TRUE;
					printf("error at line #%d : multiple symbol definitions.\n" , lineNumber);
					continue; 
				}
				else	/* insert symbol to table with code mark and IC value */	
				{      
					insertSymbol(pSymbHead,label,HEAD_ADDRESS + IC,CODE);
				}
			}
			
			/* stage 12 : search for command in command table */
			 
			/* if command not found, turn error flag on */
			if (!isLegalCommand(field,cmdMat))
			{
					hasError = TRUE;
					printf("error at line #%d : undefined command name.\n" , lineNumber);
					continue; 
			}
			
			/* stage 13 : analyze structure of operands */
			operands = 0;

			/* error flags : conssecutive commas, too many operands */
			boolean consCommas, tooManyOpers; 
			consCommas = tooManyOpers = FALSE;
				
			/* search for illegal comma before first operand */
			for (j=i; j<lineLen && isspace(buf[j]); j++);

			/* search for illegal comma after last operand */
			for (k=lineLen-1; k>i && isspace(buf[k]); k--);

				if (buf[j] == ',' ||  buf[k] == ',') /* an illegal comma has been found. */
				{
					printf("error at line #%d : illegal comma.\n" , lineNumber);
					continue;
				}

			/* scan for source and destination operands and store them for further handling. */
			while (i<lineLen)
			{
				/* skip spaces*/
				if (isspace(buf[i]))   
					for(i; i<lineLen && isspace(buf[i]); i++);

				/* a comma has been found */
				else if (buf[i] == ',') 
				{  
					for(i=i+1; i<lineLen && isspace(buf[i]); i++);
					if (buf[i] ==',')
					{
						consCommas = TRUE;
						break;
					}
				}
				/* found first character of an operand */
				else
				{
					operands++;
					for (i,j=i; !(isspace(buf[i])) && buf[i] != ','; i++);
					if (requiredOperands(field,cmdMat) == 0)
					{
						tooManyOpers = TRUE;
						break;
					}
					else if (requiredOperands(field,cmdMat) == 1)
					{
						if (operands == 1)
						{
							destOper = (char*)malloc(sizeof(char)*(i-j+1));
							memset(destOper,'\0',strlen(destOper));
							strncpy(destOper,&(buf[j]),i-j);	
						}
						else /* (operands > 1) */
						{
							tooManyOpers = TRUE;
							break;
						}
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
						else /* (operands > 2) */
						{
							tooManyOpers = TRUE;
							break;
						}
					}					
				}
			}
			
			/* consecutive commas error */
			if (consCommas)
			{
				hasError = TRUE;				
				printf("error at line #%d : consecutive commas.\n" , lineNumber);
				continue;
			}

			/* too many operands error */
			if (tooManyOpers)
			{
				hasError = TRUE;				
				printf("error at line #%d : too many operands.\n" , lineNumber);
				continue;
			}
			
			/* missing operands error */				
			if (operands < requiredOperands(field,cmdMat))
			{
				hasError = TRUE;				
				printf("error at line #%d : missing operands.\n" , lineNumber);
				continue;
			}	

			/* illegal operand error */			
			if (!isLegalOperand(srcOper)  ||   !isLegalOperand(destOper))
			{
				hasError = TRUE;				
				printf("error at line #%d : syntax error (illegal operand).\n" , lineNumber);
				continue;
			}

			/* operand type mismatch error */
			if (operandTypeMismatch(field,srcOper,destOper))
			{
				hasError = TRUE;				
				printf("error at line #%d : operand type mismatch.\n" , lineNumber);
				continue;				
			}  
			 	   
			/* compute length of memory words needed for that line (L) */
			L = codeLength(operands,srcOper,destOper);
			
			/* insert first memory word of instruction,
			   additional memory words will be inserted in the second scan. */
			inserInstruction(pInstructions,srcOper,destOper,field,cmdMat,IC,L);

			/* stage 14 : update instruction counter */
			IC = IC + L;			

			/* stage 15 : return to stage 2. */				
		}			 							
	} /* end of big while */

	/* stage 16 : if errors have been found, stop the program. */
	if (hasError)
		return FALSE;
		
	/* stage 17 : update addresses of data symbols and elements by adding IC to it.  
 
	/* update data symbols addresses. */
	Symbol* curr;
	for(curr = *pSymbHead; curr != NULL; curr = curr->next)
		if (curr->mark == DATA)
			curr->address += IC;
			
	/* update data elements addresses. */
	int i;
	for(i=0; i<DC; i++)
		(*pData)[i].address += IC;

	/* storing DC counter final value for further handling */
	*(counters+1) = DC;
	
	/* stage 18 : first scan completed successfully, continue to second scan. */
	return TRUE;
}

