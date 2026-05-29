#include "myheader.h"
char define[100]="#define";
void *macroReplacement(char *p)
{
	int i,flag,j,k;
	char *buff=NULL;
	char ch;
	char macro_name[100];
	char macro_define[500];
	char *q=NULL,*r=NULL,*s=NULL;
	const long l=strlen(define);
	long Mlen;
	q=p;
	while(q=strstr(q,define))
	{
		j=0;
		k=0;
//		macro_name[0]='\0';
//		macro_define[0]=0;
		r=q;
		for(i=l;((r[i]==' '));i++);

		for(i;(r[i]!=' ');i++)
			macro_name[j++]=r[i];
		macro_name[j]=0;
	//	printf("%s\n",macro_name);
		//Mlen=strlen(macro_name);

		for(i;((r[i]==' '));i++);

		for(i;((r[i]!=' ')&&(r[i]!='\n'));i++)
			macro_define[k++]=r[i];
		macro_define[k]=0;
	//	printf("%s\n",macro_define);

		s=q+i;
		while(s=strstr(s,macro_name))
		{
			long l1,l2,j;
			l1=strlen(macro_name);
			l2=strlen(macro_define);
			//j=s-q;
			if(l2>l1)
			{
				p=realloc(p,strlen(p)+1+l2-l1);
				//	if(str+i!=p)
				//		p=str+i;
				memmove(s+l2,s+l1,strlen(s+l1)+1);
			}
			else if(l2<l1)
			{
				memmove(s+l2,s+l1,strlen(s+l1)+1);
				p=realloc(p,strlen(p)+1);
			}
			strncpy(s,macro_define,l2);
			s=s+l2;
		}
		q=q+i;
	}
	return p;
}
