#include <iostream>
#include <cstdio>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>  
#include <pwd.h> 
#include <grp.h>
#include <map>

using namespace std;

typedef map <long long int , string> Mint ;		
string content ;
long long int size_s;
Mint Grade;
Mint :: iterator iter; 
void sortsize(char *);
void shortsize(char *);
void sort_iterator();    


void ls(char *);	
void longformat(struct stat ,char *);
void shortformat(struct stat ,char *);

int tokenizer();
int check(int );

struct passwd *pw;   //uid
struct group *grp;   //gid
struct tm *tm;

char buff[256],datestring[256];//,*strlong,*strshort;
char *c=NULL, *tokens[50],*temp=NULL,*path=NULL; 
int no_of_tokens,flag=0,l=0,R=0,d=0,t=0,S=0,a=0;
long long int size=0;	

int main(int argc , char *argv[])
{
char cwd[1024];
while(strcmp(buff,"exit")!=0)
{	
	flag=0;l=0;R=0;d=0;t=0;S=0;a=0;
	
	  cout<<"command: ";
	scanf("%[^\n]",buff);	getchar();
	no_of_tokens = tokenizer();	

	if(strcmp(buff,"exit")==0)
	{	return 0;}
		
	if(check(no_of_tokens) == 1)
	{	cout<<"invalid command"<<endl;			}
	else	//do further process
	{		
		if(d==1)
		{
			if(no_of_tokens==2)		//pass default '.'
			{	cout<<"."<<endl;		}
			else
			{
				for(int i=2;i < no_of_tokens;i++)
				{	cout<<tokens[i]<<" ";		}
				cout<<endl;
			}
		}
		else
		{
			if(path[0]!='/') //no path is there supply "current working directory"	
			{	 
				if (getcwd(cwd, sizeof(cwd)) != NULL)
				{		ls(cwd);	
						if(S==1){sort_iterator();	}
				}
				else
				{	perror("getcwd() error");	}
			}
			else
			{	ls(path);
				if(S==1){sort_iterator();	}
			}			
		}
	}	
}

return 0;
}	//MAIN OVER


void ls(char *path)   
{
struct dirent *dptr;
DIR *dirp;  //dir file discreptor
size_t path_len;
path_len = strlen(path);  

	if((dirp=opendir(path))==NULL)
	{	cout<<"cant open Error"<<endl;	}
	else
	{	
		while(dptr=readdir(dirp))
		{				
			struct stat st;					
			char full_name[_POSIX_PATH_MAX + 1];				
			
			strcpy(full_name, path);
			if (full_name[path_len - 1] != '/')
			{	strcat(full_name, "/");		}
			
			strcat(full_name, dptr->d_name);
			stat(full_name, &st);
			temp=(char *)malloc(strlen(dptr->d_name));
			strcpy(temp,dptr->d_name);
			
			if( (strcmp(temp,".")==0 || strcmp(temp,"..")==0) && a==1)		
			{	if(l!=1)
				{shortformat(st,temp);continue ;		}//cout<<temp<<endl; continue ;		}
				else
				{	longformat(st,temp);		}
			}
								
			if( temp[0]!='.' || a==1 )		//-a all files 
			{
				if(R!=1)		//not recursive
				{	if(l!=1)
					{	shortformat(st,temp);}//cout<<temp<<endl;	}//call short format to insert into map 
					else 	//long format
					{	longformat(st,temp);		}
				}
				else  //Recursive==1
				{		
					if (S_ISDIR(st.st_mode))
					{	if(l!=1)
						{	shortformat(st,temp);}//cout<<temp<<endl;     }//call short format to insert into map 
						else
						{	longformat(st,temp);			}
						ls(full_name);
					}		
					else
					{	if(l!=1)
						{	shortformat(st,temp);}//cout<<temp<<endl;		}   //call short format  to insert into map 
						else
						{	longformat(st,temp);			}
					}																					
				}						
			}
		}
		closedir(dirp);
	}
}	//ls() OVER
	
	
void sortsize(char *strlong)
{		
	size_s=size; 
    content=strlong ;    
    //cout<<"size_s:"<<size_s<<"  content : "<<content<<" \nSTRING->"<<strlong<<"\n";
    Grade [size_s] = content ;     
 }	
void sort_iterator()
{	
    for( iter = Grade.begin(); iter != Grade.end(); iter++)
    {
		   cout<<(*iter).second<<endl;
	}
}	
void shortsize(char *strshort)
{		
	size_s=size; 
    content=strshort ;    
    //cout<<"size_s:"<<size_s<<"  content : "<<content<<" \nSTRING->"<<strlong<<"\n";
    Grade [size_s] = content ;     
 }
void shortformat(struct stat st,char *dname)
{
	char *strshort;
	strshort=(char *)malloc(200);
	char siz[10];
	int n;
	strcat(strshort,dname) ; 			
	
	n = st.st_size;	
	size=(long long int)n;  //size is global variable	
	if(S!=1){	printf("%s\n",strshort);}//	cout<<strlong<<endl;	}
	else //s==1
	{	shortsize(strshort);				}
	
}

	
void longformat(struct stat st,char *dname)
{		
	char *strlong;
	strlong=(char *)malloc(500);
	int n;
	char link[3],siz[10];
	
	if( (S_ISDIR(st.st_mode))  ? strcat(strlong,"d") : strcat(strlong,"-"));
    if( (st.st_mode & S_IRUSR) ? strcat(strlong,"r") : strcat(strlong,"-"));
    if( (st.st_mode & S_IWUSR) ? strcat(strlong,"w") : strcat(strlong,"-"));
    if( (st.st_mode & S_IXUSR) ? strcat(strlong,"x") : strcat(strlong,"-"));
    if( (st.st_mode & S_IRGRP) ? strcat(strlong,"r") : strcat(strlong,"-"));
    if( (st.st_mode & S_IWGRP) ? strcat(strlong,"w") : strcat(strlong,"-"));
    if( (st.st_mode & S_IXGRP) ? strcat(strlong,"x") : strcat(strlong,"-"));
    if( (st.st_mode & S_IROTH) ? strcat(strlong,"r") : strcat(strlong,"-"));
    if( (st.st_mode & S_IWOTH) ? strcat(strlong,"w") : strcat(strlong,"-"));
    if( (st.st_mode & S_IXOTH) ? strcat(strlong,"x") : strcat(strlong,"-"));
				
	n = st.st_nlink;	
	
	sprintf(link, "%d", n);	
	strcat(strlong," ") ; strcat(strlong,(char *)link); strcat(strlong," ");					
	
	grp=getgrgid(st.st_gid);  strcat(strlong,(char *)grp->gr_name) ; 			
	pw =getpwuid(st.st_uid);  strcat(strlong," ") ; strcat(strlong,(char *)pw->pw_name) ; 			
	
	n = st.st_size;	
	size=(long long int)n;  //size is global variable
	sprintf(siz, "%d", n);
	strcat(strlong," ") ; strcat(strlong,(char *)siz); strcat(strlong," ");					
	
	tm = localtime(&st.st_mtime);	
	
	//cout<<"	time-> "<<(long long int)tm<<endl;//tim=(long long int)tm;
	
	strftime(datestring, sizeof(datestring), "%Y-%m-%d %T", tm);
	strcat(strlong," ") ; strcat(strlong,datestring); strcat(strlong," ");				
	strcat(strlong,dname) ; 
	
	
	if(S!=1){	printf("%s\n",strlong);}//	cout<<strlong<<endl;	}
	else //s==1
	{	sortsize(strlong);				}
	//printf("%lld %lld",size,tim);
	
}


int check(int nots)  //RETURN 1 FOR INVALID COMMAND
{
int len,i;

if((no_of_tokens<1 || no_of_tokens>3) || ((nots==1 && strcmp(tokens[0],"ls") != 0 )))
{	return 1;				}
else if(strcmp(tokens[0],"ls") != 0 || no_of_tokens > 3 )
{	return 1; 	}
else
{
	if(nots!=1)
	{
	len = strlen(tokens[1]);
	c=(char *)malloc(len);		
	strcpy(c,tokens[1]);

	if(c[0]=='-')
	{	for(i=1 ; i<len ; i++)
		{	
		if (c[i]=='l' || c[i]=='R' || c[i]=='a' || c[i]=='d' || c[i]=='S' || c[i]=='t' )
		{	
			if(c[i]=='l')
			{	l=1;	flag=1;}
			else if(c[i]=='R')
			{	R=1;	flag=1;}
			else if(c[i]=='a')
			{	a=1;	flag=1;}
			else if(c[i]=='d')
			{	d=1;	flag=1;}
			else if(c[i]=='S')
			{	S=1;	flag=1;}
			else if(c[i]=='t')
			{	t=1;	flag=1;}
			else
			{	flag=0;		   }
		}
		else 
		{	return 1;		}
		}
	}			
	
	}
}
	i=no_of_tokens-1;
	len = strlen(tokens[i]);
	path=(char *)malloc(len);
	strcpy(path,tokens[i]);
	
return 0;
}


int tokenizer()
{
	char *pch;
	int i=0;

	pch = strtok (buff," ");		
		
	while (pch != NULL)
	{
		tokens[i]=(char *)malloc(strlen(pch)+1);
		strcpy(tokens[i],pch);
		i++;
		pch = strtok (NULL," ");
	}

return i;
}

