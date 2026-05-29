#include "myheader.h"
int main(int argc,char *argv[])
{
        if(argc!=3)
        {
                printf("invalid input");
                exit(0);
        }
        char *buf=NULL;
	char s[100];
	strcpy(s,argv[2]);
	long l=strlen(s);
	s[l-1]='i';
//	printf("s==%s\n",s);
        if(strcmp(argv[1],"-E")==0)
        {
                buf=readFromFile(argv[2]);
                buf=removeComments(buf);
                buf=fileInclusion(buf);
                buf=macroReplacement(buf);
                writeToFile(buf,s);
        }
        else
        {
                printf("incorrect command");
                exit(0);
        }
}

