/*  
Systems Programming Lab Course (20465)
Maman 14 - Final Project - Assembler.
Author : Snir Holland
Id : 039998943
Date : 20/02/2019
----------------------------------------
commands.h
----------------------------------------
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include "globals.h"
 
/*
 * Function:  generateCommands
 * --------------------
 * description:       this function builds a matrix of commands and their names. 
 *                     
 *  pCmdMat:          address of container for command matrix.  
 *           
 */
void generateCommands(char*** pCmdMat);



/*
 * Function:  requiredOperands
 * --------------------
 * description:       this function returns the number of operands that
 *                    must be added to command name, in order for it to work. 
 *                     
 *  command:          command name.
 *  cmdMat:           command matrix.  
 *  
 * returns:           number of operands the command requires,
 *                    -1 if command is illegal.          
 */
int requiredOperands(char* command, char** cmdMat);




/*
 * Function:  isLegalCommand
 * --------------------
 * description:       this function scans for command in the command table 
 *                    and returns TRUE/FALSE if command is legal/illegal respectively. 
 *                     
 *  command:          command name.
 *  cmdMat:           command matrix.  
 *  
 * returns:           TRUE if command is legal,
 *                    FALSE otherwise.          
 */
boolean isLegalCommand(char* command, char** cmdMat);



/*
 * Function:  getOpCode
 * --------------------
 * description:       this function gets a command string and returns its operation
 *                    code (integer in the range [0,15])
 *
 *  command:          command name.
 *  cmdMat:           command matrix.  
 *  
 * returns:           an integer that represents operation code of command.
 *                           
 */
int getOpCode(char* command, char** cmdMat);


#endif
