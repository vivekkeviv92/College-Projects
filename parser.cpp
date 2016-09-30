//Recursive Descent Parser
//Assumption - After each looping statement, the statements inside the loop (0,1 or more) should be enclosed in {}

#include<string.h>
#include<fstream.h>
#include<stdio.h>
#include<iostream.h>
#include<ctype.h>
#include<process.h>
#include<conio.h>

//using namespace std;

char loop[2][6]={"for","while"};
char sym[18][2]={";",",","=","+","<",">","!",".","(",")","{","}","-","*","/"};
char types[6][30]={"Keywords","Loops","Decision Statements","Symbols","Identifiers","Arrays"};
char kw[4][7]={"int","char","void","main"};
char dec1[3][5]={"if","else"};
char numb[10]={'0','1','2','3','4','5','6','7','8','9'};

int tcount=0,n1=0,err=0,term1=1,term2=1,line=0,iserror=0;
char token[200][20],toks[20];

int statement();
int dprime();
int statementlist();
int identifierlist();
int declaration();

struct node
{
    char info[10];
    struct node *link;
};

node * htbl[6];

int chkkw(char a[])
{
    for(int i=0;i<4;i++)
    {
	    if(strcmp(a,kw[i])==0)
		return 1;
    }
    return 0;
}

int chkloop(char a[])
{
    for(int i=0;i<2;i++)
    {
	if(strcmp(a,loop[i])==0)
	    return 1;
    }
    return 0;
}

int chkdec(char a[])
{
    for(int i=0;i<2;i++)
    {
	if(strcmp(a,dec1[i])==0)
	    return 1;
    }
    return 0;
}

int chksym(char a[])
{
    for(int i=0;i<15;i++)
    {
	if(strcmp(a,sym[i])==0)
	    return 1;
    }
    return 0;
}

int chknum(char c)
{
    for(int i=0;i<10;i++)
    {
	if(numb[i]==c)
	    return 1;
    }
    return 0;
}

void initial()
{
    tcount=0;
    int i,j;
    for(i=0;i<6;i++)
	htbl[i]=NULL;
    for(i=0;i<200;i++)
    {
	for(j=0;j<20;j++)
	    token[i][j]=NULL;
    }
}

void display()
{
    for(int j=0;j<tcount;j++)
	cout<<token[j]<<"\t";
}

void scanner()
{
    FILE *fp;
    //fp=fopen("D:\\3rd Year CSE\\Language Processors Lab\\compiler_code.txt","r");
    fp=fopen("compiler.txt","r");
    char buffer[20];
    initial();
    char c=' ';
    c=getc(fp);
    int alph_check=0,op_check=0,i=0,j=0,k=0;
    while(c!=EOF)
    {
	k=0;i=0;
	alph_check=0;
	for(i=0;i<20;i++)
	    buffer[i]='\0';
	if(c==' ')
	{
	    c=getc(fp);
	    continue;
	}
	if(isalpha(c))
	{
	    alph_check=1;
	    while(isalnum(c))
	    {
		buffer[k++]=c;
		c=getc(fp);
	    }
	    buffer[k]=NULL;
	}
	else
	{
	    buffer[k]=c;
	    for(i=0;i<18;i++)
	    {
		if(strcmp(buffer,sym[i])==0)
		{
		    op_check=1;
		    break;
		}
	    }

	    if(op_check==0)
	    {
		while(chknum(c))
		{
		    buffer[k++]=c;
		    c=getc(fp);
		}
	    }
	}
	if(alph_check==0)
	    c=getc(fp);
	if(strcmp("",buffer)==0)
	    break;
	buffer[strlen(buffer)]='\0';
	strcpy(token[tcount],buffer);
	tcount++;
	for(i=0;i<20;i++)
	    buffer[i]=NULL;
    }
    cout<<"\nDisplaying tokens : \n";
    display();
    fclose(fp);
}

void gettoken(int n)
{
    int i=0;
    if(token[n][0]==' ')
    {
	n++;
	n1++;
    }
    else if(token[n][0]=='\n')
    {
	n++;
	n1++;
	line++;
    }
    for(i=0;i<20;i++)
	toks[i]=token[n][i];
}

void error()
{
    cout<<"\n\nError code : "<<err<<"\tToken : "<<toks<<"\n" ;
    cout<<"Line Number : "<<line;
    cout<<"\n\nProgram REJECTED!!";
    iserror=1;
    //getch();
    cout<<"\ntoks = "<<toks;
    // exit(0);
    getch();
}

int match(char *exptoken)
{
    int i=0;
    if(strcmp(toks,exptoken)==0)
    {
	for(i=0;i<20;i++)
	    toks[i]='\0';
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	return 1;
    }
    else
    {
	cout<<"\nERROR : Expected token not matched!";
	cout<<"\nToken got is "<<toks;
	cout<<"\nThe expected token is "<<exptoken;
	cout<<"\nLine Number = "<<line;
	error();
	//getch();
  //	exit(0);
    }
}

int datatype()
{
    err=2;
    //int term1=1;
    if(toks[0]=='i')
	term1=match("int");
    else if(toks[0]=='c')
	term1=match("char");
    return term1;
}

int id()
{
    err=3;
    if(isalpha(toks[0]))
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
    }
    return 1;
}

int id1()
{
    if(isalnum(toks[0]))
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
    }
    return 1;
}

int addop()
{
    err=19;
    //int term1=1;
    if(strcmp(toks,"+")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,";")==0)
	{
	    error();
	    exit(1);
	}
	return 1;
    }
    else if(strcmp(toks,"-")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,";")==0)
	{
	    error();
	    exit(1);
	}
	return 1;
    }
    return 0;
}

int mulop()
{
    err=20;
    //int term1=1;
    if(strcmp(toks,"*")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,";")==0)
	{
	    error();
	    exit(1);
	}
	return 1;
    }
    else if(strcmp(toks,"/")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,";")==0)
	{
	    error();
	    exit(1);
	}
	return 1;
    }
    else if(strcmp(toks,"%")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,";")==0)
	{
	    error();
	    exit(1);
	}
	return 1;
    }
    return 0;
}

int relop()
{
    err=21;
    //int term1=1;
    if(strcmp(toks,"=")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,"=")==0)
	{
	    gettoken(n1++);
	    toks[strlen(toks)]='\0';
	    return 1;
	}
    }
    else if(strcmp(toks,"!")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,"=")==0)
	{
	    gettoken(n1++);
	    toks[strlen(toks)]='\0';
	    return 1;
	}
    }
    else if(strcmp(toks,"<")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,"=")==0)
	{
	    gettoken(n1++);
	    toks[strlen(toks)]='\0';
	    return 1;
	}
    }
    else if(strcmp(toks,">")==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(strcmp(toks,"=")==0)
	{
	    gettoken(n1++);
	    toks[strlen(toks)]='\0';
	    return 1;
	}
    }
    else
	return 0;
}

int factor()
{
    err=14;
    //int term1=1;
    term1*=id1();
    if(isalnum(toks[0]))
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
    }
    else
	term1=0;
    return term1;
}

int tprime()
{
    err=13;
    int term1=1,term2=1;
    while(term1)
    {
	term1*=mulop();
	term1*=factor();
	if(term1)
	{
	    //term1*=factor();
	    term1*=tprime();
	}
    }
    return term1;
}

int term()
{
    err=12;
    //int term1=1;
    term1*=factor();
    term1*=tprime();
    return term1;
}

int seprime()
{
    err=11;
    int term1=1,term2=1;
    while(term1)
    {
	term1*=addop();
	term1*=term();
	if(term1)
	    //term1*=term();
	    term1*=seprime();
    }
    return term1;
}

int simpleexpression()
{
    err=10;
    //int term1=1;
    term1*=term();
    term1*=seprime();
    return term1;
}

int eprime()
{
	err=9;
	//int term1=1,term2=1;
	term1*=relop();
	term1*=simpleexpression();
	return term1;
}

int expn()
{
    err=8;
    //int term1=1;
    term1*=simpleexpression();
    term1*=eprime();
    return term1;
}

int assignstatement()
{
    err=7;
    if(!isalpha(toks[0]))
	return term1;
    //int term1=1;
    term1*=id();
    term1*=match("=");
    term1*=expn();
    term1*=match(";");
    return term1;
}

int loopassignstatement()
{
    err=7;
    if(strcmp(toks,";")==0 || strcmp(toks,")")==0)
	return term1;
    else if(!isalpha(toks[0]))
	return term1;
    //int term1=1;
    term1*=id();
    term1*=match("=");
    term1*=expn();
    return term1;
}
int loopingstatement()
{
    err=17;
    //int term1=1;
    if((strcmp(toks,"while"))==0)
    {
	term1*=match("while");
	term1*=match("(");
	term1*=identifierlist();
	term1*=relop();
	term1*=identifierlist();
	term1*=match(")");
	term1*=match("{");
	term1*=statementlist();
	term1*=match("}");
    }
    else if((strcmp(toks,"for"))==0)
    {
	term1*=match("for");
	term1*=match("(");
	term1*=loopassignstatement();
	term1*=match(";");
	term1*=identifierlist();
	term1*=relop();
	term1*=identifierlist();
	term1*=match(";");
	term1*=loopassignstatement();
	term1*=match(")");
	term1*=match("{");
	term1*=statementlist();
	term1*=match("}");
    }
    else
	return 0;
    return term1;
}

int decisionstatement()
{
    err=15;
    //int term1=1;
    if(strcmp(toks,"if")==0)
    {
	term1*=match("if");
	term1*=match("(");
	term1*=identifierlist();
	term1*=relop();
	term1*=identifierlist();
	term1*=match(")");
	term1*=match("{");
	term1*=statementlist();
	term1*=match("}");
	term1*=dprime();
	return term1;
    }
    return 0;
}

int statement()
{
    err=6;
     if(strcmp(toks,"")==0)
	exit(0);
    //int term1=1;
    else if((strcmp(toks,"while")==0) || (strcmp(toks,"for")==0))
	loopingstatement();
    else if((strcmp(toks,"if"))==0)
	decisionstatement();
    else if((strcmp(toks,"int")==0) || (strcmp(toks,"char")==0))
	declaration();
    else if((strcmp(toks,"}"))==0)
	return term1;
    else if((strcmp(toks,"else"))==0)
	error();
    else
	assignstatement();
    return term1;
}

int dprime()
{
    err=16;
    //int term1=1;
    if(strcmp(toks,"else")==0)
    {
	term1*=match("else");
	term1*=match("{");
	term1*=statementlist();
	term1*=match("}");
    }
    return term1;
}

int statementlist()
{
    err=5;
    //int term1=1,term2=1;
    if(strcmp(toks,"")==0)
	exit(0);
    while(1)
    {
	term1=statement();
	if((strcmp(toks,"}"))==0)
	    break;
    }
    return term1;
}

int identifierlist()
{
    err=4;
    //int term=1,n3=0;
    int n3=0;
    term1*=id();
    //gettoken(n1++);
    if(isalnum(toks[0]))
    {
	term1*=1;
	gettoken(n1++);
	toks[strlen(toks)]='\0';
    }
    else if((strcmp(toks,"["))==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	if(isdigit(toks[0]))
	{
	    while(isdigit(toks[0]))
	    {
		term1*=1;
		gettoken(n1++);
		toks[strlen(toks)]='\0';
	    }
	    if(strcmp(toks,"]")!=0)
	    {
		error();
		exit(0);
	    }
	    else
	    {
		term1*=1;
		gettoken(n1++);
		toks[strlen(toks)]='\0';
		if((strcmp(toks,",")!=0) && (strcmp(toks,";")!=0))
		{
		    error();
		    exit(0);
		}
		term1*=identifierlist();
	    }
	}
	else if(strcmp(toks,"]")==0)
	{
	    term1*=1;
	    gettoken(n1++);
	    toks[strlen(toks)]='\0';
	    if((strcmp(toks,",")!=0) && (strcmp(toks,";")!=0))
	    {
		error();
		exit(0);
	    }
	    identifierlist();
	}
    }
    else if((strcmp(toks,","))==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	term1*=identifierlist();
    }
    else if((strcmp(toks,"="))==0)
    {
	gettoken(n1++);
	toks[strlen(toks)]='\0';
	term1*=expn();
	term1*=identifierlist();
    }
    else
	return 1;
    return term1;
}

int declaration()
{
    err=1;
    //int term=1,term1=1;
    term1*=datatype();
    term1=identifierlist();
    term1=match(";");

    return term1;
}

int main()
{
    clrscr();
    scanner();
    char * gtoken;
    gettoken(n1++);
    if(strcmp("main",toks)!=0)
    {
	cout<<"ERROR : main expected!";
	exit(0);
    }
    gettoken(n1++);
    if(strcmp("(",toks)!=0)
    {
	cout<<"ERROR : ( expected!";
	exit(0);
    }
    gettoken(n1++);
    if(strcmp(")",toks)!=0)
    {
	cout<<"ERROR : ) expected!";
	exit(0);
    }
    gettoken(n1++);
    if(strcmp("{",toks)!=0)
    {
	cout<<"ERROR : { expected!";
	exit(0);
    }
    gettoken(n1++);
    toks[strlen(toks)]='\0';
    int res1=statementlist();
    if(iserror==0)
	cout<<"\n\nNo errors!! Program ACCEPTED!!";
    getch();
    return 0;
    getch();
}
