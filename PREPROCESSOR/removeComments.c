#include "myheader.h"
void *removeComments(char *p)
{
        int i,j;
        for(i=0;p[i];i++)
        {
                if((p[i]=='/')&&(p[i+1]=='*'))
                {
                        for(j=i+2;((p[j]!='*')&&(p[j+1]!='/')&&(p[j]));j++);
                        if(p[j]==0)
                        {
                                printf("error\n");
                                break;
                        }
                        memmove(p+i,p+j+2,strlen(p+j+2)+1);
                        p=realloc(p,strlen(p));
                        i--;
                }
                else if((p[i]=='/')&&(p[i+1]=='/'))
                {
                        for(j=i;p[j]!='\n';j++);
                        memmove(p+i,p+j+1,strlen(p+j+1)+1);
                        p=realloc(p,strlen(p));i--;
                }

        }

        return p;
}

