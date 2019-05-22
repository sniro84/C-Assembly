assembler : assembler.o  commands.o  externalRefs.o    fileHandling.o  firstScan.o    memoryWord.o    secondScan.o   symbolTable.o   utils.o 
	gcc -g -ansi assembler.o  commands.o  externalRefs.o    fileHandling.o  firstScan.o    memoryWord.o    secondScan.o   symbolTable.o   utils.o -lm -o assembler

assembler.o : assembler.c
	gcc -c -ansi assembler.c -o assembler.o

commands.o : commands.c
	gcc -c -ansi commands.c -o commands.o

externalRefs.o : externalRefs.c
	gcc -c -ansi externalRefs.c -o externalRefs.o	

fileHandling.o : fileHandling.c
	gcc -c -ansi fileHandling.c -o fileHandling.o

firstScan.o : firstScan.c
	gcc -c -ansi firstScan.c -o firstScan.o

memoryWord.o : memoryWord.c
	gcc -c -ansi memoryWord.c -o memoryWord.o

secondScan.o : secondScan.c
	gcc -c -ansi secondScan.c -o secondScan.o

symbolTable.o : symbolTable.c
	gcc -c -ansi symbolTable.c -o symbolTable.o

utils.o : utils.c
	gcc -c -ansi  utils.c -o  utils.o 



