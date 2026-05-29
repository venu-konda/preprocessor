#include "myheader.h"
char header[100]="#include";
void *fileInclusion(char *p)
{
        int i,flag;
        char *buff=NULL;
        char ch;
        char s[100];
        char path[100]="/usr/include/";
        char *q=NULL,*r=NULL;
        const long l=strlen(header);
        q=p;
//      int loop=0;
        while(q=strstr(q,header))
        {
                s[0]='\0';
                r=q;
                flag=0;
        //      memmove(r,r+l,strlen(r+l)+1);
        //      r=realloc(r,strlen(r));
                for(i=l;((r[i]==' '));i++);
                if(r[i]=='<')
                        flag=1;
                else
                        flag=0;
                memmove(q,q+i+1,strlen(q+i+1)+1);
                p=realloc(p,strlen(p));
        //      printf("realloc%s\n",p);
                if(flag)
                {
                        for(i=0;((r[i]!='>'));i++)
                                s[i]=r[i];
                }
                else
                {
                        for(i=0;((r[i]!='"'));i++)
                                s[i]=r[i];
                }
                s[i]=0;
                printf("%s\n",s);
        //      memmove(p,p+i+1,strlen(p+i+1)+1);
        //      p=realloc(p,strlen(p));
                if(flag)
                {
                        strcat(path,s);
                        strcpy(s,path);
                }
                memmove(q,q+i+2,strlen(q+i+2)+1);
                p=realloc(p,strlen(p));
                buff=readFromFile(s);
                p=realloc(p,strlen(buff)+strlen(p));
                memmove(p+strlen(buff),p,strlen(p)+1);
                strncpy(p,buff,strlen(buff));
        //      q=q+strlen(buff);
        //      printf("p==%s\n",p);
        //      loop++;
        //      if(loop==2)
        //      break;
        }
        return p;
}
