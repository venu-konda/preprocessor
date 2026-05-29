a.out : preprocessor.o fileInclusion.o macroReplacement.o removeComments.o readFromFile.o writeToFile.o
	cc fileInclusion.o macroReplacement.o removeComments.o readFromFile.o writeToFile.o preprocessor.o
preprocessor.o :preprocessor.c
	cc -c preprocessor.c
fileInclusion.o :fileInclusion.c
	cc -c fileInclusion.c
macroReplacement.o : macroReplacement.c
	cc -c macroReplacement.c
removeComments.o : removeComments.c
	cc -c removeComments.c
readFromFile.o : readFromFile.c
	cc -c readFromFile.c
writeToFile.o :writeToFile.c
	cc -c writeToFile.c
