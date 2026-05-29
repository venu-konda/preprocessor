#include "myheader.h"
void writeToFile(char *p,char *filename)
{
        FILE *fp;
        fp=fopen(filename,"w");
        fwrite(p,strlen(p),1,fp);
        fclose(fp);

}
