#include<string.h>
#include<stdio.h>
#include<stdlib.h>
char header[100]="#include";
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
void *macroReplace(char (*p)[100])
{
	int i,j;
	char s1[100],s2[100];

}
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
//	int loop=0;
	while(q=strstr(q,header))
	{
		s[0]='\0';
		r=q;
		flag=0;
	//	memmove(r,r+l,strlen(r+l)+1);
	//	r=realloc(r,strlen(r));
		for(i=l;((r[i]==' '));i++);
		if(r[i]=='<')
			flag=1;
		else
			flag=0;
		memmove(q,q+i+1,strlen(q+i+1)+1);
		p=realloc(p,strlen(p));
	//	printf("realloc%s\n",p);
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
	//	memmove(p,p+i+1,strlen(p+i+1)+1);
	//	p=realloc(p,strlen(p));
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
	//	q=q+strlen(buff);
	//	printf("p==%s\n",p);
	//	loop++;
	//	if(loop==2)
	//	break;
	}	
	return p;
}
void writeToFile(char *p,char *filename)
{
	FILE *fp;
	fp=fopen(filename,"w");
	fwrite(p,strlen(p),1,fp);
	fclose(fp);

}
int main(int argc,char *argv[])
{
	if(argc!=3)
	{
		printf("invalid input");
		exit(0);
	}
	char *buf=NULL;
	if(strcmp(argv[1],"-E")==0)
	{
		buf=readFromFile(argv[2]);
		buf=removeComments(buf);
		buf=fileInclusion(buf);
		writeToFile(buf,argv[2]);
	}
	else
	{
		printf("incorrect command");
		exit(0);
	}
}
