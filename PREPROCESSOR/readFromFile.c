#include"myheader.h"
void *readFromFile(char *filename)
{
        FILE *fp=NULL;
        char *p=NULL;
        fp=fopen(filename,"r");
        if(fp==NULL)
                printf("error");
        else
        {
                fseek(fp,0,2);
                long size=ftell(fp);
                rewind(fp);
                p=realloc(p,size+1);
                fread(p,size,1,fp);
                p[size]=0;
        }
        fclose(fp);
        return p;
}
