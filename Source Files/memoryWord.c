/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
memoryWord.c 
----------------------------------------
*/

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "memoryWord.h"

/* this function inserts a memoryWord to data container */
void insertData(MemoryWord** pData ,int DC, MWType type, void* p)
{
	if (DC == 0)      /* data container is empty */
	{
		*pData = (MemoryWord*)malloc(sizeof(MemoryWord));
	}
	else	          /* data container has elements */
	{
		*pData = (MemoryWord*)realloc(*pData,(DC+1)*sizeof(MemoryWord));
	}

	/* check if the file has been successfully opened */
	if (*pData == NULL)
	{
		fputs("error: memory allocation failed\n",stderr);
		exit(ERROR_MEMORY);
	}

	/* assign type and address of data */
	(*pData)[DC].mwtype = type;
	(*pData)[DC].address = HEAD_ADDRESS + DC;

	/* make sure data is being inserted to the correct field */	
	if (type == INTEGER_WORD)
	{
		int number = (*(int*)(p)); /* extract the value of the number. */		
		((*pData)[DC].wordBlock).integerWord.intVal = number;
	}
	else if (type == ASCII_WORD)
	{
		char ch = (*(char*)(p)); /* extract the value of the character. */
		((*pData)[DC].wordBlock).asciiWord.asciiCode = ch;
	}				
}

/* this function inserts a memoryWord to instructions container */
void inserInstruction(MemoryWord** pInstruction, char* srcOper,
                       char* destOper, char* command ,char** cmdMat ,int IC, int L)
{
	if (IC == 0)         /* instructions container is empty */
	{
		*pInstruction = (MemoryWord*)malloc(L * sizeof(MemoryWord));
	}
	else                 /* instructions container has elements */
	{
		*pInstruction = (MemoryWord*)realloc(*pInstruction,(IC+L) * sizeof(MemoryWord));
	}

	/* check if the file has been successfully opened */
	if (*pInstruction == NULL)
	{
		fputs("error: memory allocation failed\n",stderr);
		exit(ERROR_MEMORY);
	}	

	/* assign properties of instruction */
	(*pInstruction)[IC].mwtype = INSTRUCTION;
	(*pInstruction)[IC].address = HEAD_ADDRESS + IC;
	((*pInstruction)[IC].wordBlock).instruction.codeType = ABSOLUTE;  /* always absolute for instruction */
	((*pInstruction)[IC].wordBlock).instruction.destOper = getAddressMethod(destOper);
	((*pInstruction)[IC].wordBlock).instruction.opCode = getOpCode(command,cmdMat);
	((*pInstruction)[IC].wordBlock).instruction.sourceOper = getAddressMethod(srcOper);		
}

/* this function inserts an additional memoryWord to instructions container */
void insertAddition(MemoryWord** pInstruction, Symbol* symbHead, ExternalRef** pExtRefHead, 
		       char* srcOper,char* destOper, char* command, char** cmdMat, int IC,int L)
{
	
	if (L == 1)  /* no additional memory words to insert. */
		return;
	else if (L == 2) /* one additional memory word to insert (destination operand). */
	{
		(*pInstruction)[IC+1].address = HEAD_ADDRESS + IC + 1;
		if (getAddressMethod(destOper) == REG_DIRECT && getAddressMethod(srcOper) == REG_DIRECT) /* both operands are registers */
		{
			(*pInstruction)[IC+1].mwtype = REGISTER_WORD;
			((*pInstruction)[IC+1].wordBlock).regWord.codeType = ABSOLUTE;  
			((*pInstruction)[IC+1].wordBlock).regWord.destReg = getRegID(destOper);
			((*pInstruction)[IC+1].wordBlock).regWord.sourceReg = getRegID(srcOper); 
		}
		else
		{
			if (getAddressMethod(destOper) == IMMEDIATE)    
			{
				(*pInstruction)[IC+1].mwtype = IMMEDIATE_VALUE;
				((*pInstruction)[IC+1].wordBlock).immediateValue.codeType = ABSOLUTE;
				((*pInstruction)[IC+1].wordBlock).immediateValue.imVal = atoi(destOper);
			}
			else if (getAddressMethod(destOper) == DIRECT)  
			{
				(*pInstruction)[IC+1].mwtype = LABEL_ADDRESS;
				if ((getLabelMark(symbHead ,destOper) == EXTERN))
				{
					((*pInstruction)[IC+1].wordBlock).labelAddress.codeType = EXTERNAL;
					insertExternalReference(pExtRefHead,destOper, HEAD_ADDRESS+IC+1); 
				}
				else
				{
					((*pInstruction)[IC+1].wordBlock).labelAddress.codeType = RELOCATABLE;
				}
				((*pInstruction)[IC+1].wordBlock).labelAddress.address = getLabelAddress(symbHead,destOper);
			}
			else if (getAddressMethod(destOper) == REG_DIRECT)
			{
				(*pInstruction)[IC+1].mwtype = REGISTER_WORD;
				((*pInstruction)[IC+1].wordBlock).regWord.codeType = ABSOLUTE;  
				((*pInstruction)[IC+1].wordBlock).regWord.destReg = getRegID(destOper);
			}
		}	
	}
	else /* L =3, two additional memorys word to insert (destination and source operands). */
	{
		/* first memory word */		
		(*pInstruction)[IC+1].address = HEAD_ADDRESS + IC + 1;
		
		if (getAddressMethod(srcOper) == IMMEDIATE)
		{
			(*pInstruction)[IC+1].mwtype = IMMEDIATE_VALUE;
			((*pInstruction)[IC+1].wordBlock).immediateValue.codeType = ABSOLUTE;
			((*pInstruction)[IC+1].wordBlock).immediateValue.imVal = atoi(srcOper);
		}
		else if (getAddressMethod(srcOper) == DIRECT)
		{
			(*pInstruction)[IC+1].mwtype = LABEL_ADDRESS;
			if ((getLabelMark(symbHead ,srcOper) == EXTERN))
			{
				((*pInstruction)[IC+1].wordBlock).labelAddress.codeType = EXTERNAL;
				insertExternalReference(pExtRefHead,srcOper, HEAD_ADDRESS+IC+1);
			}
			else
			{
				((*pInstruction)[IC+1].wordBlock).labelAddress.codeType = RELOCATABLE;
			}
			((*pInstruction)[IC+1].wordBlock).labelAddress.address = getLabelAddress(symbHead,srcOper) ;
		}
		else if (getAddressMethod(srcOper) == REG_DIRECT) 
		{
			(*pInstruction)[IC+1].mwtype = REGISTER_WORD;
			((*pInstruction)[IC+1].wordBlock).regWord.codeType = ABSOLUTE; 
			((*pInstruction)[IC+1].wordBlock).regWord.sourceReg = getRegID(srcOper); 
		}

		/* second memory word */
		(*pInstruction)[IC+2].address = HEAD_ADDRESS + IC + 2;
	
		if (getAddressMethod(destOper) == IMMEDIATE)
		{
			(*pInstruction)[IC+2].mwtype = IMMEDIATE_VALUE;
			((*pInstruction)[IC+2].wordBlock).immediateValue.codeType = ABSOLUTE;
			((*pInstruction)[IC+2].wordBlock).immediateValue.imVal = atoi(destOper);
		}
		else if (getAddressMethod(destOper) == DIRECT)
		{
			(*pInstruction)[IC+2].mwtype = LABEL_ADDRESS;

			if ((getLabelMark(symbHead ,destOper) == EXTERN))
			{				
				((*pInstruction)[IC+2].wordBlock).labelAddress.codeType = EXTERNAL;
				insertExternalReference(pExtRefHead,destOper, HEAD_ADDRESS+IC+2);
			}
			else
			{
				((*pInstruction)[IC+2].wordBlock).labelAddress.codeType = RELOCATABLE;
			}
			((*pInstruction)[IC+2].wordBlock).labelAddress.address = getLabelAddress(symbHead,destOper) ;
		}
		else if  (getAddressMethod(destOper) == REG_DIRECT)
		{
			int destRegID = getRegID(destOper);
			(*pInstruction)[IC+2].mwtype = REGISTER_WORD;
			((*pInstruction)[IC+2].wordBlock).regWord.codeType = ABSOLUTE;  
			((*pInstruction)[IC+2].wordBlock).regWord.destReg = destRegID;
		}
	}	
}

/* this function translates a memory word to a binary machine code string and returns that string */ 
char* binaryMachineCode(MemoryWord memWord)
{
	int fieldSize;    /* number of bit the field requires */
	int segLen;       /* length of field code segment (characters in machine code string) */
	char* code;       /* machine code string of complete memory word */

	code = (char*)malloc(WORDSIZE * sizeof(char) + 1);
	
	/* analyze different parts of the memory word */
	switch (memWord.mwtype)
	{
		case INSTRUCTION:
		{
			char* codeTypeStr;
			char* destOperStr;
			char* opcodeStr;
			char* srcOperStr;		

			/* coding type */
			segLen = strlen("00"); 
			codeTypeStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.instruction.codeType;
			codeTypeStr = (char*)twosComplement(fieldSize,segLen);
			
			/* destination operand */
			segLen = strlen("000");
			destOperStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.instruction.destOper;
			destOperStr = (char*)twosComplement(fieldSize,segLen);
			
			/* opcode */
			segLen = strlen("0000");
			opcodeStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.instruction.opCode;
			opcodeStr = (char*)twosComplement(fieldSize,segLen);
	
			/* source operand */
			segLen = strlen("000");
			srcOperStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.instruction.sourceOper;
			srcOperStr = (char*)twosComplement(fieldSize,segLen);
						
			/* concat all parts of instruction */
			strcat(code,srcOperStr);
			strcat(code,opcodeStr);
			strcat(code,destOperStr);
			strcat(code,codeTypeStr);
			 				
		}				  		
		break;

		case IMMEDIATE_VALUE:
		{
			char* codeTypeStr;
			char* imValStr;
					
			/* coding type */
			segLen = strlen("00"); 
			codeTypeStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.immediateValue.codeType;
			codeTypeStr = (char*)twosComplement(fieldSize,segLen);

			/* immediate value (integer) */
			segLen = strlen("0000000000");
			imValStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.immediateValue.imVal;
			imValStr = (char*)twosComplement(fieldSize,segLen);

			/* concat parts */
			strcat(code,imValStr);
			strcat(code,codeTypeStr);
			
		}				  		
		break; 

		case LABEL_ADDRESS:
		{
			char* codeTypeStr;
			char* addressStr;
					
			/* coding type */
			segLen = strlen("00"); 
			codeTypeStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.labelAddress.codeType;
			codeTypeStr = (char*)twosComplement(fieldSize,segLen);
	
			/* address of internal label */
			segLen = strlen("0000000000");
			addressStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.labelAddress.address;
			addressStr = (char*)twosComplement(fieldSize,segLen);

			/* concat parts */
			strcat(code,addressStr);
			strcat(code,codeTypeStr);


		}				  		
		break;
		
		case REGISTER_WORD:
		{
			char* codeTypeStr;
			char* destRegStr;
			char* sourceRegStr;
					
			/* coding type */
			segLen = strlen("00"); 
			codeTypeStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.regWord.codeType;
			codeTypeStr = (char*)twosComplement(fieldSize,segLen);

			/* destination register */
			segLen = strlen("00000"); 
			destRegStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.regWord.destReg;
			destRegStr = (char*)twosComplement(fieldSize,segLen);			

			/* source register */
			segLen = strlen("00000"); 
			sourceRegStr = (char*)malloc(segLen + 1);
			fieldSize = memWord.wordBlock.regWord.sourceReg;
			sourceRegStr = (char*)twosComplement(fieldSize,segLen);

			/* concat parts */
			strcat(code,sourceRegStr);
			strcat(code,destRegStr);
			strcat(code,codeTypeStr);
						
		}				  		
		break;
		
		case INTEGER_WORD:
		{
			char* integerStr;
					
			/* binary representation of an integer */ 
			integerStr = (char*)malloc(sizeof(char)*WORDSIZE + 1);
			fieldSize = memWord.wordBlock.integerWord.intVal;
			integerStr = (char*)twosComplement(fieldSize,WORDSIZE);

			/* copy integer binary string to code string */
			strcpy(code,integerStr);
									
		}				  		
		break;

		case ASCII_WORD:
		{
			char* AsciiStr;
					
			/* binary representation of a character ascii code */ 
			AsciiStr = (char*)malloc(sizeof(char)*WORDSIZE + 1);
			fieldSize = memWord.wordBlock.asciiWord.asciiCode;
			AsciiStr = (char*)twosComplement(fieldSize,WORDSIZE);

			/* copy Ascii bianry string to code string */
			strcpy(code,AsciiStr);									
		}				  		
		break;

		default:   /* an unknown data has been identified */
		{
			printf("error: unknown data identified.\n");
			exit(ERROR_UNKNOWN_DATA);
		}
	}
} 
