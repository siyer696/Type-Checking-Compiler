/*

Subject :       Compiler Design
Course Project: Type Checking
Participants:   16 - Prathamesh Dhawale(Gr. No. - 1710838)
                23 - Praduemna Gore(Gr. No. - 1710365)
                25 - Sachin Iyer(Gr. No. - 1710575)
                49 - Ajit Niras(Gr. No. - 1710923)
Problem Statement: To check the type of all the tokens in a given input string

*/

//importing basic C library
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
//Defining size, EOS, none
#define SIZE 128
#define NONE -1
#define EOS '\0'
/*defining tokens for numbers, keywords, ID, Done(completion), Max size*/
#define NUM 257
#define KEYWORD 258
#define ID 259
#define DONE 260
#define MAX 9999
//Initializing lexems, buffer, lastchar, lastentry, tokenval, lineno, lookahed
//lexeme char array
char lexemes[MAX];
//buffer of ip characters
char buffer[SIZE];
// last char
int lastchar=-1;
//last viewed entry to view next entry fast
int lastentry=0;
//token val is set to done as default
int tokenval=DONE;
//current line no to denote in error
int lineno=1;
//next 
int lookahead;
/*Making symbol table and Keyword table*/
struct entry
{
    char *lexptr;           //asd4345   77      int
    int token;              //Identifier number KEYWORD
    }
symtable[200];
struct entry
keywords[]={"if",KEYWORD,"else",KEYWORD,"for",KEYWORD,"int",KEYWORD,"float",KEYWORD,"double",KEYWORD,"char",KEYWORD,"struct",KEYWORD,"return",KEYWORD,0,0};
/*Function to display error message with line no*/
void Error_Message(char *m)
{
    fprintf(stderr,"line %d, %s \n",lineno,m);
    exit(1);
}
/*A lookup function to find entry into symbol table*/
int look_up(char s[ ])
{
    int k;
    for(k=lastentry;k>0;k--)
    if(strcmp(symtable[k].lexptr,s)==0)
        return k;
    return 0;
    }

/* Inserting string into symbol table and lexem table
   Parameters: Inp string and token
*/
int insert(char s[ ],int tok)
{
    int len;
    len=strlen(s);
    //To check if it exceeds limit
    if(lastentry+1>=MAX)
        Error_Message("Symbol table is full");
    if(lastchar+len+1>=MAX)
        Error_Message("Lexemes array is full");

    lastentry=lastentry+1;
    
    symtable[lastentry].token=tok;
    //saving the symbol in table and lexems
    symtable[lastentry].lexptr=&lexemes[lastchar+1];
    lastchar=lastchar+len+1;
    strcpy(symtable[lastentry].lexptr,s);
    return lastentry;
    }

/*
void Initialize()
{
struct entry *ptr;
for(ptr=keywords;ptr->token;ptr+1)
insert(ptr->lexptr,ptr->token);
}*/

/*lexer takes input one by one and returns done if done
*/
int lexer()
    {
    int t;
    int val,i=0;
    while(1)
    {
        //scanning char and ignoring spaces
        t=getchar();
        if(t==' '||t=='\t');
        else
        if(t=='\n')
        lineno=lineno+1;
        else
        if(isdigit(t))
        {
            //checking if it is digit
            ungetc(t,stdin);
            scanf("%d",&tokenval);
            return NUM;

        }
        else
        if(isalpha(t))
        {
            //asddf435454
            //if it is alphanumeric , scanning all and adding it to buffer
            while(isalnum(t))
            {
            buffer[i]=t;
            t=getchar();
            i=i+1;

            if(i>=SIZE)
                Error_Message("Compiler error");
            }
            buffer[i]=EOS;
            //If not eof, return it
            if(t!=EOF)
                ungetc(t,stdin);
            /*If not present add into table and return token*/
            val=look_up(buffer);
            if(val==0)
                val=insert(buffer,ID);
            tokenval=val;
            return symtable[val].token;
            }
        else
        /*If end of filee, return done
          Else make tokenval NONE*/
        if(t==EOF)  
            return DONE;
        else
            {
            tokenval=NONE;
            return t;
        }
    }
}

//Matching the symbol and lookahead
void Match(int t)
{
    if(lookahead==t)
    lookahead=lexer();
    else
    Error_Message("Syntax error");
}

//Displaying the contents, Number, indentifier, operator, tokens with value
void display(int t,int tval)
    {
    if(t=='+'||t=='-'||t=='*'||t=='/')
        printf("\nArithmetic Operator: %c",t);
    else if(t==NUM)
        printf("\n Number: %d",tval);
    else if(t==ID)
        printf("\n Identifier: %s",symtable[tval].lexptr);
    else
        printf("\n Token %d tokenval %d",t,tokenval);
    }

/*Function to print and match them to respective tokens*/    
void F()
{
    void E();
    switch(lookahead)
    {
        //if opening bracket, it will redirect to E and then finnally match closing bracket
        case '(' : Match('(');
        E();
        Match(')');
        break;
        case NUM : display(NUM,tokenval);
        Match(NUM);
        break;
        case ID : display(ID,tokenval);
        Match(ID);
        break;
        default : Error_Message("Syntax error");
    }
}

//For * and / special fun
void T()
    {
    int t;
    F();
    while(1)
    {
        switch(lookahead)
        {
            case '*' : t=lookahead;
            Match(lookahead);
            F();
            display(t,NONE);
            continue;
            case '/' : t=lookahead;
            Match(lookahead);
            display(t,NONE);
            continue;
            default : return;
        }
    }
}

//For addition and subraction 
void E()
    {
    int t;
    T();
    while(1)
    {
        switch(lookahead)
        {
            case '+' : t=lookahead;
            Match(lookahead);
            T();
            display(t,NONE);

            continue;
            case '-' : t=lookahead;
            Match(lookahead);
            T();
            display(t,NONE);
            continue;
            default : return;
        }
    }
}

//Parser function to take input and match with corresponding tokens
void parser()
    {
        //a + b
    lookahead=lexer();
    while(lookahead!=DONE)
    {
        E();
        Match(';');
    }
    }

//Main function
int main()
{
   // char ans[10];
    //Initialize();
    printf("\n Enter the expression ");
    printf("And place ; at the end\n");
    printf("Press Ctrl-Z to terminate\n");
    parser();
    return 0;
}