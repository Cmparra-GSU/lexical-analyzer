/* Cody Parra
** this program follows the language rules:
** <program>->void main'('')'<block>
**
** <block>->'{'{statement} {return}'}' ---- 
**
** <statement>-><ifStmnt>||<switchStmnt>||<forStmnt>||
   <forEach>||<while>||<do>||<variableExpr>||<mathExpr>
   <emptyStatement>||
**
** <variableExpr> -> <var> -> <assignmentOp>-> <var>||<int_lit>
                   ||<var> -> <assignmentOp>->{<mathExpr>}
                   ||<var> -> <inc_op>||<dec_op>
**
** <var> -> [a-zA-Z]+[a-zA-Z0-9]*
**
** <int_lit> -> [0-9]+
**
** <mathExpr>-> <var> {<add_op>||<sub_op>||<mul_op>||<div_op>||<mod_op>
**
** <ifStmnt>-> if'('<expr>')'['{']<statement>['}'] [<else>]
**
** <expr>-> <var> { <operator> } {<var> | <int_lit>}
**
** <operator> -> '<' || '<=' || '>' || '>=' || '||' ||
                '!=' || '==' || '&&'
**
** <else> -> '{' <statement> '}'
**
** <switchStmnt>-> switch'('<expr>')''{' {<case>} [default] '}'
** <case>->case <char>||<int_lit> ':' {<statement>}
** <char>-> '''[a-zA-Z]'''
** <default>-> default ':' <statement>
**
** <forStmnt>->for'('<expression>')''{'{<statement>}'}'
**
** <do>->do '{'<statement>'}' while'('<expression>')'';"
**
** <while>->while'('<expression>')''{'{statement}'}'
**
** <forEach>->foreach'(' [ref] [readonly] <var> || <var> || <char> || <string>
                      <in> <var> ')' '{' {<statement>} '}'
**
*/



#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
int whileCheck;
FILE *in_fp;
fpos_t position;
char c;
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
int quoteLex(void);
void error();
#define LETTER 0
#define NUMBER 1
#define SYMBOL 99

#define INT_LIT 10
#define VAR 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MUL_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define MOD_OP 27
#define LESS 28
#define LESS_EQ 29
#define GREAT 30
#define GREAT_EQ 31
#define EQ 32
#define NE 33
#define DO_CODE 34
#define OR 35
#define AND 36
#define INC_OP 37
#define DEC_OP 38
#define IF_CODE 40
#define ELSE_CODE 41
#define LEFT_BRACKET 42
#define RIGHT_BRACKET 43
#define SEMICOLON 44
#define QUOTE 45
#define QUOTES 46
#define WHILE_CODE 47
#define SWITCH_CODE 48
#define CASE_CODE 49
#define DECIMAL 50
#define COLON 51
#define BREAK 52
#define DEFAULT 53
#define FOR_CODE 54
#define FOREACH_CODE 55
#define INT 56
#define STRING 57
#define CHAR 58
#define REF 59
#define IN 60
#define READONLY 61
#define WORD 62
#define VOID 63
#define MAIN 64
#define RETURN 65

/*
makes sure it follows correct format of void main(){<block><return>}
*/
int main(void) {
    if ((in_fp = fopen ("file.txt", "r")) == NULL)
        printf("file not found");
    else{
            fgetpos(in_fp, &position);
            getChar();
            quoteLex();
            if(strcmp(lexeme, "void") != 0)
            {
                error();
            }
            else{
                fsetpos(in_fp, &position);
                getChar();
                lex();
            }
    }

	 return 0;
}

void getChar(void){
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = NUMBER;
        else
            charClass = SYMBOL;
    }else
        charClass = EOF;
}

void addChar(void){
    if (lexLen <=98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else
    printf("error\n");
}

void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}

void error(){
    printf("error : exiting\n");
    exit(EXIT_FAILURE);
}

int symbolTable(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 nextToken = ADD_OP;
			 fgetpos(in_fp, &position);
             if (getc(in_fp) == '+'){
                nextChar = '+';
                addChar();
                nextToken = INC_OP;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
			 break;
		 case '-':
			 addChar();
			 nextToken = SUB_OP;
			 fgetpos(in_fp, &position);
             if (getc(in_fp) == '-'){
                nextChar = '-';
                addChar();
                nextToken = DEC_OP;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
			 break;
		 case '*':
			 addChar();
			 nextToken = MUL_OP;
			 break;
		 case '/':
			 addChar();
			 nextToken = DIV_OP;
			 break;
         case '%':
			 addChar();
			 nextToken = MOD_OP;
			 break;
         case '<':
             addChar();
             fgetpos(in_fp, &position);
             if (getc(in_fp) == '='){
                nextChar = '=';
                addChar();
                nextToken = LESS_EQ;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
             nextToken = LESS;
             break;

         case '>':
             addChar();
             fgetpos(in_fp, &position);
             if (getc(in_fp) == '='){
                nextChar = '=';
                addChar();
                nextToken = GREAT_EQ;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
             nextToken = GREAT;
             break;

         case '=':
             addChar();
             nextToken = ASSIGN_OP;
             fgetpos(in_fp, &position);
             if (getc(in_fp) == '='){
                nextChar = '=';
                addChar();
                nextToken = EQ;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
             break;

          case '!':
             addChar();
             fgetpos(in_fp, &position);
             if (getc(in_fp) == '='){
                nextChar = '=';
                addChar();
                nextToken = NE;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
             break;

          case '&':
             addChar();
             fgetpos(in_fp, &position);
             if (getc(in_fp) == '&'){
                nextChar = '&';
                addChar();
                nextToken = AND;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }
             break;

          case '|':
             addChar();
             fgetpos(in_fp, &position);
             if (getc(in_fp) == '|'){
                nextChar = '|';
                addChar();
                nextToken = OR;
                break;
             }
             else{
                fsetpos(in_fp, &position);
             }

          case '{':
            addChar();
            nextToken = LEFT_BRACKET;
            break;

          case '}':
            addChar();
            nextToken = RIGHT_BRACKET;
            break;
          case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
          case ':':
            addChar();
            nextToken = COLON;
            break;
          case '\'' :
            addChar();
            nextToken = QUOTE;
            break;

          case '"':
            addChar();
            nextToken = QUOTES;
            break;
         case '.':
            addChar();
            nextToken = DECIMAL;
            break;

		 default:
			 addChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/*
a special symbol table for the quote lexer, which is described below.
*/
int quoteSymTable(char ch){
    switch(ch){
    case '"':
        addChar();
        nextToken = QUOTES;
    default:
        addChar();
        symbolTable(nextChar);
    }
}

void expr(){
    printf("beginning expression check\n");
    if (nextToken != VAR)
        error();
    else{
        lex();
        if (nextToken != GREAT && nextToken != GREAT_EQ && nextToken != LESS && nextToken != OR &&
            nextToken != LESS_EQ && nextToken != NE && nextToken!= EQ && nextToken != AND){
                printf("missing operator\n");
                error();
                }
        else{
            lex();
            if (nextToken != VAR && nextToken != INT_LIT){
                printf("invalid format \n");
                error();
                }
            else{
                lex();
                if(nextToken != GREAT && nextToken != GREAT_EQ && nextToken != LESS && nextToken != OR &&
                    nextToken != LESS_EQ && nextToken != NE && nextToken!= EQ && nextToken != AND){
                    printf("this is a valid expression\n");
                }
                else{
                    while (nextToken != RIGHT_PAREN){
                        if (nextToken!= RIGHT_PAREN){
                            lex();
                            expr();
                        }
                    }
                }

            }
        }
    }
}

void mathExpr(){
    printf("beginning math op\n");
    if(nextToken != ADD_OP && nextToken != SUB_OP &&
    nextToken != MUL_OP && nextToken != DIV_OP &&
    nextToken != MOD_OP){
        printf("math error\n");
        error();
    }
    else{
        lex();
        if(nextToken!=VAR && nextToken!=INT_LIT){
            printf("math error\n");
            error();
        }
        else{
            lex();
            while(nextToken!=SEMICOLON){
                if(nextToken!=SEMICOLON){
                    if(nextToken != ADD_OP && nextToken != SUB_OP &&
                    nextToken != MUL_OP && nextToken != DIV_OP &&
                    nextToken != MOD_OP){
                        printf("math error1\n");
                    }
                    else{
                        lex();
                        if(nextToken!=SEMICOLON){
                            if(nextToken!= VAR && nextToken!= INT_LIT)
                            {
                                printf("math error2\n");
                                error();
                            }
                            else{
                                lex();
                            }
                        }

                    }
                }
            }
            if(nextToken!=SEMICOLON){
                printf("invalid math expression\n");
                error();
            }
            else{
                printf("valid math expression\n");
            }
        }
    }
}
void varExpr(){
    printf("beginning var exp\n");
        if(nextToken == INC_OP || nextToken == DEC_OP){
            lex();
            if(nextToken == SEMICOLON){
                printf("valid variable expression \n");
            }
        }

        if (nextToken == ASSIGN_OP){
            lex();
            if (nextToken!=VAR && nextToken!=INT_LIT){
                printf("invalid assignment\n");
                error();
            }
            else{
                lex();
                if (nextToken!=SEMICOLON){
                    if(nextToken != ADD_OP && nextToken != SUB_OP &&
                     nextToken != MUL_OP && nextToken != DIV_OP &&
                     nextToken != MOD_OP){
                        printf("assignment error\n");
                        error();
                     }
                     else{
                        mathExpr();
                     }
                }
                else{
                    printf("valid assignment\n");
                }
            }
        }
        else{
            if(nextToken == SEMICOLON){
                printf("valid variable expression\n");
            }
            else{
                printf("invalid variable expression\n");
                error();
            }
        }
}

/*
** case statements are a little weird so i made this part separate
** to account for the various types of statements
*/
void caseCheck(){
    printf("beginning case check\n");
    if(nextToken != INT_LIT && nextToken!= QUOTE){
        printf("");
        printf("invalid case format\n");
        error();
    }
    else{
        if (nextToken == INT_LIT)
        {
            lex();
            if (nextToken == DECIMAL){
                printf("decimal error \n");
                error();
            }
                else{
                    if(nextToken != ADD_OP && nextToken != SUB_OP && nextToken != MUL_OP &&
                        nextToken != DIV_OP && nextToken != MOD_OP){
                        if(nextToken!= COLON){
                            printf("missing colon\n");
                            error();
                        }
                        else{
                                printf("22r2\n");
                            printf("valid case\n");
                        }
                    }
                    else{
                        lex();
                        if(nextToken!=INT_LIT){
                            if(nextToken!=QUOTE){
                                printf("invalid case statement\n");
                                error();
                            }
                            else{
                                lex();
                                if(nextToken!=VAR){
                                    printf("invalid case statement\n");
                                    error();
                                }
                                else{
                                    if(lexLen > 1){
                                        printf("invalid case statement\n");
                                        error();
                                    }
                                    else{
                                        lex();
                                        if(nextToken!=QUOTE){
                                            printf("invalid case statement\n");
                                            error();
                                        }
                                        else{
                                            lex();
                                            if(nextToken!=COLON){
                                                printf("missing colon\n");
                                                error();
                                            }
                                            else{
                                                    printf("2282\n");
                                                printf("valid case\n");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else{
                            lex();
                            if(nextToken!=COLON){
                                printf("missing colon\n");
                                error();
                            }
                            else{
                                    printf("2262\n");
                                printf("valid case\n");
                            }
                        }
                    }
                }
        }
        else{
            if(nextToken!=QUOTE){
                printf("invalid case format\n");
                error();
            }
            else{
                lex();
                if(nextToken!=VAR){
                    printf("invalid case format\n");
                    error();
                }
                else{
                    if(lexLen !=1){
                        printf("invalid case format\n");
                        error();
                    }
                    else{
                        lex();
                        if(nextToken!=QUOTE){
                            printf("invalid case format\n");
                            error();
                        }
                        else{
                            lex();
                            if(nextToken != ADD_OP && nextToken != SUB_OP && nextToken != MUL_OP &&
                                nextToken != DIV_OP && nextToken != MOD_OP){
                                if(nextToken!= COLON){
                                    printf("missing colon\n");
                                    error();
                                }
                                else{
                                        printf("2522\n");
                                    printf("valid case\n");
                                }
                            }
                            else{
                                lex();
                                if(nextToken!=INT_LIT){
                                    if(nextToken!=VAR){
                                        printf("invalid case format\n");
                                        error();
                                    }
                                    else{
                                        if(lexLen > 1){
                                            printf("invalid case format\n");
                                            error();
                                        }
                                        else{
                                            lex();
                                            if(nextToken!=QUOTE){
                                                printf("invalid case format\n");
                                                error();
                                            }
                                            else{
                                                lex();
                                                if(nextToken!=COLON){
                                                    printf("missing colon\n");
                                                    error();
                                                }
                                                else{
                                                        printf("2223\n");
                                                    printf("valid case\n");
                                                }
                                            }
                                        }
                                    }
                                }
                                else{
                                    lex();
                                    if(nextToken!=COLON){
                                        printf("missing colon\n");
                                        error();
                                    }
                                    else{
                                        printf("222\n");
                                        printf("valid case\n");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
for expressions are also a little picky
    the formats can be
    for(;;), for(;condition;modification), for(;condition;) or the normal
    for(initialization;condition;modification)
*/

void forExpr(){
    printf("beginning for expr\n");
    if(nextToken!= VAR){
        if (nextToken!= SEMICOLON){
            printf("invalid for expression\n");
            error();
        }
        else{
            lex();
            if (nextToken != VAR){
                lex();
                if(nextToken!=SEMICOLON){
                    printf("missing condition\n");
                    error();
                }
                else{
                    lex();
                    if(nextToken!=VAR){
                        if(nextToken!=RIGHT_PAREN){
                            printf("invalid format\n");
                            error();
                        }
                        else{
                            printf("valid for expression\n");
                        }
                    }
                    else{
                        lex();
                        if(nextToken!=INC_OP&&nextToken!=DEC_OP){
                            printf("failed modification\n");
                            error();
                        }
                        else{
                            lex();
                            if(nextToken!=RIGHT_PAREN){
                                printf("missing right parenthesis\n");
                                error();
                            }
                            else{
                                printf("valid for expression\n");
                            }
                        }
                    }
                }
            }
            else{
                lex();
                if(nextToken != GREAT && nextToken != GREAT_EQ && nextToken != LESS &&
                   nextToken != LESS_EQ && nextToken != EQ && nextToken != NE){
                    printf("failed condition\n");
                    error();
                }
                else{
                    lex();
                    if(nextToken!= VAR && INT_LIT){
                        printf("failed condition\n");
                        error();
                    }
                    else{
                        lex();
                        if(nextToken!=SEMICOLON){
                            printf("missing semicolon\n");
                        }
                        else{
                            lex();
                            if(nextToken!=VAR){
                                if(nextToken!=SEMICOLON){
                                        printf("missing modification\n");
                                        error();
                                }
                                else{
                                    lex();
                                    if(nextToken!=RIGHT_PAREN){
                                        printf("invalid format\n");
                                        error();
                                    }
                                    else{
                                        printf("valid for expression\n");
                                    }
                                }
                            }
                            else{
                                lex();
                                if(nextToken!=INC_OP&&nextToken!=DEC_OP){
                                    printf("failed modification\n");
                                    error();
                                }
                                else{
                                    lex();
                                    if(nextToken!=RIGHT_PAREN){
                                        printf("missing right parenthesis\n");
                                        error();
                                    }
                                    else{
                                        printf("valid for expression\n");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        lex();
        if(nextToken != ASSIGN_OP){
            printf("missing initialization\n");
            error();
        }
        else{
            lex();
            if(nextToken!=INT_LIT){
                printf("failed initialization\n");
                error();
            }
            else{
                lex();
                if(nextToken!=SEMICOLON){
                    printf("missing semicolon\n");
                }
                else{
                    lex();
                    if(nextToken!=VAR){
                        printf("failed condition\n");
                        error();
                    }
                    else{
                        lex();
                        if(nextToken != GREAT && nextToken != GREAT_EQ && nextToken != LESS &&
                            nextToken != LESS_EQ && nextToken != EQ && nextToken != NE){
                            printf("failed condition\n");
                            error();
                        }
                        else{
                            lex();
                            if(nextToken!= VAR && nextToken!= INT_LIT){
                                printf("failed condition\n");
                                error();
                            }
                            else{
                                lex();
                                if(nextToken!=SEMICOLON){
                                    printf("missing semicolon\n");
                                }
                                else{
                                    lex();
                                    if(nextToken!=VAR){
                                        printf("invalid modification\n");
                                        error();
                                    }
                                    else{
                                        lex();
                                        if(nextToken!=INC_OP&&nextToken!=DEC_OP){
                                            printf("failed modification\n");
                                            error();
                                        }
                                        else{
                                            lex();
                                            if(nextToken!=RIGHT_PAREN){
                                                printf("missing right parenthesis\n");
                                                error();
                                            }
                                            else{
                                                printf("valid for expression\n");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void stmnt(){
    printf("beginning statement\n");
    if (nextToken != SEMICOLON){
      if (nextToken != VAR){
        printf("statement error\n");
        error();
      }
        else{
           lex();
           if (nextToken == LEFT_PAREN){
              lex();
                if(nextToken != QUOTE ){
                    if (nextToken != QUOTES){
                        if (nextToken != VAR){
                            printf("statement error\n");
                            error();
                        }
                        else{
                            lex();
                            if(nextToken != RIGHT_PAREN){
                                printf("missing right parenthesis \n");
                                error();}
                             else{
                                lex();
                                if(nextToken != SEMICOLON){
                                    printf("missing semicolon\n");
                                    error();}
                                else{
                                    printf("valid statement\n");
                                }
                             }
                        }
                    }
                    else{
                        quoteLex();
                        if(nextToken!=QUOTES){
                            while(nextToken!=QUOTES){
                                quoteLex();
                            }
                        }
                            if(nextToken != QUOTES){
                                printf("quotes missing\n");
                                error();}
                            else{
                                lex();
                                if (nextToken != RIGHT_PAREN){
                                    printf("no closing parenthesis\n");
                                    error();}
                                else{
                                    lex();
                                    if (nextToken != SEMICOLON){
                                        printf("no semicolon\n");
                                        error();}
                                    else{
                                        printf("valid statement with double quotes\n");
                                    }
                                }
                            }
                    }
                }
                else{
                    lex();
                    if (nextToken != VAR){
                        printf("invalid statement");
                        error();
                        }
                    else{
                        if(lexLen>1){
                            printf("single quote variable too long\n");
                            error();
                        }
                        lex();
                        if(nextToken != QUOTE){
                            printf("quote missing\n");
                            error();}
                        else{
                            lex();
                            if (nextToken != RIGHT_PAREN){
                                printf("no closing parenthesis\n");
                                error();
                            }
                            else{
                                lex();
                                if (nextToken != SEMICOLON){
                                    printf("no semicolon\n");
                                    error();}
                                else{
                                    printf("valid statement with single quotes\n");
                                }
                            }
                        }
                    }
                }
           }
           else{
            varExpr();
            printf("valid expression statement\n");
            }
           }
        }
   else{
      printf("this is a valid empty statement\n");
      }
}

void ifStmnt(){
    if (nextToken != IF_CODE)
        error();
        else{
            printf("beginning if statement\n");
            lex();
            if (nextToken !=LEFT_PAREN){
                printf("missing opening parenthesis\n");
                error();
            }
            else{
                lex();
                expr();
                if (nextToken != RIGHT_PAREN){
                    printf("missing closing parenthesis\n");
                    error();}
                else{
                    lex();
                    if (nextToken!= LEFT_BRACKET && nextToken != ELSE_CODE){
                        stmnt();
                        lex();
                        if(nextToken!= ELSE_CODE){
                            printf("valid if statement with no else\n");
                        }
                        else{
                                lex();
                                if (nextToken != LEFT_BRACKET){
                                    printf("missing open bracket\n");
                                    error();
                                }
                                else{
                                    lex();
                                    if(nextToken!= RIGHT_BRACKET){
                                        stmnt();
                                        lex();
                                        if(nextToken!=RIGHT_BRACKET){
                                            printf("else error\n");
                                            error();
                                        }
                                        else{
                                            printf("valid if else statement\n");
                                        }
                                    }
                                }
                        }
                    }
                    else{
                        printf("beginning bracket if statement\n");
                        lex();
                        while (nextToken!= RIGHT_BRACKET){
                            if(nextToken != RIGHT_BRACKET){
                            stmnt();
                            lex();
                            }
                        }
                        if (nextToken == RIGHT_BRACKET){
                            lex();
                            if (nextToken == EOF){
                                printf("valid if statement\n");
                            }
                            else{
                                lex();
                                if (nextToken != ELSE_CODE)
                                    printf("this is a valid if statement\n");
                                else{
                                     lex();
                                     if (nextToken != LEFT_BRACKET){
                                            printf("invalid else statement\n");
                                            error();
                                      }
                                      else{
                                        lex();
                                        if(nextToken!=RIGHT_BRACKET){
                                            stmnt();
                                            lex();
                                            if(nextToken !=RIGHT_BRACKET){
                                                printf("Missing right bracket\n");
                                                error();
                                            }
                                            else{
                                                printf("this is a valid if else statement\n");
                                            }
                                        }
                                        else{
                                            printf("this is a valid if else statement\n");
                                        }
                                      }
                                }
                            }
                    }
                }
            }
        }
    }
}

void whileStmnt(){
    if (nextToken != WHILE_CODE)
        error();
    else{
        printf("beginning while statement\n");
        lex();
        if (nextToken != LEFT_PAREN){
            printf("missing open parenthesis\n");
            error();
        }
        else{
            lex();
            expr();
            if (nextToken != RIGHT_PAREN){
                printf("missing closing parenthesis\n");
                error();
            }
            else{
                lex();
                if (nextToken == SEMICOLON){
                    whileCheck = 1;
                }
                else{
                    if (nextToken!= LEFT_BRACKET){
                        printf("missing open bracket\n");
                        error();
                    }
                    else{
                        lex();
                        if (nextToken == RIGHT_BRACKET)
                        {
                            printf("This is a valid while statement\n");
                        }
                        else{
                            while(nextToken!= RIGHT_BRACKET){
                                if(nextToken!= RIGHT_BRACKET){
                                    stmnt();
                                    lex();
                                }
                            }
                            if (nextToken != RIGHT_BRACKET){
                                printf("missing right bracket \n");
                                error();}
                            else{
                                printf("valid while statement\n");
                            }
                        }
                    }
                }
            }
        }
    }
}

void doStmnt(){
    if (nextToken != DO_CODE)
        error();
    else{
        printf("beginning do while statement\n");
        lex();
        if(nextToken!= LEFT_BRACKET)
        {
            printf("missing open bracket\n");
            error();
        }
        else{
            lex();
            while(nextToken != RIGHT_BRACKET){
                if(nextToken!= RIGHT_BRACKET){
                    stmnt();
                    lex();
                }
            }
            if (nextToken != RIGHT_BRACKET)
            {
                printf("Missing closing bracket");
                error();
            }
            else{
                    lex();
                    if(whileCheck != 1){
                        printf("While statement failed\n");
                        error();
                    }
                    else{
                        printf("valid do while statement \n");
                    }
                }
        }
    }

}

void switchStmnt(){
    printf("beginning switch statement\n");
    if(nextToken!= SWITCH_CODE)
        error();
    else{
        lex();
        if(nextToken!=LEFT_PAREN){
            printf("Missing open parenthesis \n");
            error();
        }
        else{
            lex();
            if (nextToken!=VAR && nextToken!= INT_LIT){
                printf("invalid switch expression");
                error();
            }
            else{
                lex();
                if(nextToken != RIGHT_PAREN){
                    if(nextToken!=ADD_OP && nextToken!=SUB_OP && nextToken!=MUL_OP)
                    {
                        printf("Invalid switch expression\n");
                        error();
                    }
                    else{
                        while (nextToken!= RIGHT_PAREN){
                            if(nextToken!=RIGHT_PAREN){
                                if(nextToken!=ADD_OP && nextToken!=SUB_OP && nextToken!=MUL_OP)
                                {
                                    printf("invalid switch expression\n");
                                    error();
                                }
                                else{
                                    lex();
                                    if(nextToken != VAR && nextToken != INT_LIT)
                                    {
                                        printf("Invalid switch expression\n");
                                        error();
                                    }
                                    else{
                                        lex();
                                    }
                                }
                            }
                        }
                        lex();
                        if (nextToken!= LEFT_BRACKET){
                            printf("missing open bracket\n");
                            error();
                        }
                        else{
                            lex();
                            if (nextToken!= CASE_CODE){
                                printf("missing case\n");
                                error();
                            }
                            else{
                                while (nextToken!= RIGHT_BRACKET && nextToken==CASE_CODE){
                                    lex();
                                    caseCheck();
                                    lex();
                                    if(nextToken!=RIGHT_BRACKET && nextToken!=CASE_CODE){
                                        stmnt();
                                        lex();
                                        if(nextToken==BREAK){
                                            lex();
                                            if(nextToken!=SEMICOLON){
                                                printf("missing semicolon\n");
                                                error();
                                            }
                                            else{
                                                lex();
                                            }
                                        }
                                    }
                                }
                                if(nextToken!=DEFAULT){
                                    if(nextToken!=RIGHT_BRACKET){
                                        printf("missing closing bracket\n");
                                        error();
                                    }
                                    else{
                                        printf("valid switch statement\n");
                                    }

                                }
                                else{
                                    printf("beginning default\n");
                                    lex();
                                    if(nextToken!=COLON){
                                        printf("missing colon\n");
                                        error();
                                    }
                                    else{
                                        lex();
                                        stmnt();
                                        lex();
                                        if(nextToken!=RIGHT_BRACKET){
                                            printf("missing closing bracket\n");
                                            error();
                                        }
                                        else{
                                            printf("valid switch statement with default\n");
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
                else{
                    if(nextToken!= RIGHT_PAREN){
                        printf("invalid switch statement\n");
                        error();
                    }
                    else{
                        lex();
                        if (nextToken!= LEFT_BRACKET){
                            printf("missing open bracket\n");
                            error();
                        }
                        else{
                            lex();
                            if (nextToken!= CASE_CODE){
                                printf("missing case\n");
                                error();
                            }
                            else{
                                while (nextToken!= RIGHT_BRACKET && nextToken==CASE_CODE){
                                    lex();
                                    caseCheck();
                                    lex();
                                    if(nextToken!=RIGHT_BRACKET&&nextToken!=CASE_CODE){
                                        stmnt();
                                        lex();
                                        if(nextToken==BREAK){
                                            lex();
                                            if(nextToken!=SEMICOLON){
                                                printf("missing semicolon\n");
                                                error();
                                            }
                                            else{
                                                lex();
                                            }
                                        }
                                    }
                                }
                                if(nextToken!=DEFAULT){
                                    if(nextToken!=RIGHT_BRACKET){
                                        printf("missing closing bracket\n");
                                        error();
                                    }
                                    else{
                                        printf("valid switch statement\n");
                                    }

                                }
                                else{
                                    printf("beginning default\n");
                                    lex();
                                    if(nextToken!= COLON){
                                        printf("missing colon\n");
                                        error();
                                    }
                                    else{
                                        lex();
                                        stmnt();
                                        lex();
                                        if(nextToken!=RIGHT_BRACKET){
                                            printf("missing closing bracket\n");
                                            error();
                                        }
                                        else{
                                            printf("valid switch statement with default\n");
                                        }
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void forStmnt(){
    printf("beginning for statement\n");
    if(nextToken!=FOR_CODE)
        error();
    else{
        lex();
        if(nextToken!=LEFT_PAREN){
            printf("missing open parenthesis \n");
            error();
        }
        else{
            lex();
            forExpr();
            lex();
            if(nextToken!=LEFT_BRACKET){
                printf("missing open bracket\n");
                error();
            }
            else{
                lex();
                while(nextToken!=RIGHT_BRACKET){
                    stmnt();
                    if(nextToken!=RIGHT_BRACKET){
                        lex();
                    }
                }
                if(nextToken!=RIGHT_BRACKET){
                    printf("missing closing bracket\n");
                    error();
                }
                else{
                    printf("valid for statement\n");
                }
            }
        }
    }
}

void forEach(){
    printf("beginning for each\n");
    if(nextToken!=FOREACH_CODE)
        error();
    else{
        lex();
        if(nextToken!=LEFT_PAREN){
            printf("missing open parenthesis\n");
            error();
        }
        else{
            lex();
            if(nextToken!=INT && nextToken!=CHAR && nextToken!= STRING && nextToken!= REF){
                printf("missing data type\n");
                error();
            }
            else{
                if(nextToken == REF){
                    lex();
                    if(nextToken!=READONLY &&nextToken!=INT){
                        printf("invalid reference \n");
                        error();
                    }
                    if(nextToken==READONLY){
                        lex();
                        if(nextToken!=INT){
                            printf("invalid reference \n");
                            error();
                        }
                        else{
                            lex();
                        }
                    }
                    else{
                        if(nextToken!=INT){
                            printf("invalid reference \n");
                            error();
                        }
                        else{
                            lex();
                        }
                    }
                }
                else{
                    lex();
                }
                if(nextToken!=VAR){
                    printf("missing variable \n");
                    error();
                }
                else{
                    lex();
                    if(nextToken!=IN){
                        printf("missing 'in' keyword\n");
                        error();
                    }
                    else{
                        lex();
                        if(nextToken!=VAR){
                            printf("missing variable \n");
                            error();
                        }
                        else{
                            lex();
                            if(nextToken!=RIGHT_PAREN){
                                printf("missing closing parenthesis\n");
                                error();
                            }
                            else{
                                lex();
                                if(nextToken!=LEFT_BRACKET){
                                    printf("missing left bracket\n");
                                    error();
                                }
                                else{
                                    lex();
                                    while(nextToken!=RIGHT_BRACKET){
                                        stmnt();
                                        if(nextToken!=RIGHT_BRACKET){
                                            lex();
                                        }
                                    }
                                    if(nextToken!=RIGHT_BRACKET){
                                        printf("missing closing bracket\n");
                                        error();
                                    }
                                    else{
                                        printf("valid for each statement\n");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void main1(){
    if(nextToken!=VOID){
        printf("void expected\n");
        error();
    }
    else{
        lex();
        if(nextToken!=MAIN){
            printf("main expected\n");
            error();
        }
        else{
            lex();
            if (nextToken!= LEFT_PAREN){
                printf("no left parenthesis\n");
                error();
            }
            else{
                lex();
                if(nextToken!=RIGHT_PAREN){
                    printf("no right parenthesis\n");
                    error();
                }
                else{
                    lex();
                    if(nextToken!=LEFT_BRACKET){
                        printf("no left bracket\n");
                        error();
                    }
                    else{
                        lex();
                        if(nextToken== SEMICOLON){
                            printf("valid empty statement\n");
                        }
                        while(nextToken!=RETURN){
                            if(nextToken!=RETURN){
                                lex();
                            }
                        }
                        if(nextToken!=RETURN){
                                printf("return expected\n");
                                error();
                        }
                        else{
                            lex();
                            if(nextToken!=SEMICOLON){
                                printf("semicolon missing\n");
                                error();
                            }
                            else{
                               lex();
                               if(nextToken!=RIGHT_BRACKET){
                                printf("missing right bracket\n");
                                error();
                               }
                               else{
                                printf("check complete\n");
                               }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*
** the regular lexer
    based on front.c with modifications to handle statements
*/
int lex(void){
    lexLen = 0;
    getNonBlank();
    switch(charClass){
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == NUMBER) {
            addChar();
            getChar();
        }
        if ((strcmp(lexeme, "if")) == 0)
        {
            nextToken = IF_CODE;
            ifStmnt();
            break;
        }
        if ((strcmp(lexeme, "else")) == 0)
        {
            nextToken = ELSE_CODE;
            break;
        }

        if ((strcmp(lexeme, "while")) == 0)
        {
            nextToken = WHILE_CODE;
            whileStmnt();
            break;
        }

        if ((strcmp(lexeme, "do")) == 0)
        {
            nextToken = DO_CODE;
            doStmnt();
            break;
        }

        if ((strcmp(lexeme, "switch")) == 0)
        {
            nextToken = SWITCH_CODE;
            switchStmnt();
            break;
        }
        if ((strcmp(lexeme, "case")) == 0)
        {
            nextToken = CASE_CODE;
            break;
        }
        if ((strcmp(lexeme, "break")) == 0)
        {
            nextToken = BREAK;
            break;
        }

        if ((strcmp(lexeme, "default")) == 0)
        {
            nextToken = DEFAULT;
            break;
        }

        if ((strcmp(lexeme, "for")) == 0)
        {
            nextToken = FOR_CODE;
            forStmnt();
            break;
        }

        if ((strcmp(lexeme, "int")) == 0)
        {
            nextToken = INT;
            break;
        }
        if ((strcmp(lexeme, "char")) == 0)
        {
            nextToken = CHAR;
            break;
        }
        if ((strcmp(lexeme, "string")) == 0)
        {
            nextToken = STRING;
            break;
        }
        if ((strcmp(lexeme, "ref")) == 0)
        {
            nextToken = REF;
            break;
        }
        if ((strcmp(lexeme, "readonly")) == 0)
        {
            nextToken = READONLY;
            break;
        }
        if ((strcmp(lexeme, "in")) == 0)
        {
            nextToken = IN;
            break;
        }
        if ((strcmp(lexeme, "foreach")) == 0)
        {
            nextToken = FOREACH_CODE;
            forEach();
            break;
        }
        if ((strcmp(lexeme, "void")) == 0)
        {
            nextToken = VOID;
            main1();
            break;
        }
        if ((strcmp(lexeme, "main")) == 0)
        {
            nextToken = MAIN;
            break;
        }
        if ((strcmp(lexeme, "return")) == 0)
        {
            nextToken = RETURN;
            break;
        }


        else{
            nextToken = VAR;
            break;
        }
    case NUMBER:
        addChar();
        getChar();
        while (charClass == NUMBER) {
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;
    case SYMBOL:
        symbolTable(nextChar);
        getChar();
        break;
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
        break;
    }
    printf("next Token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}

/*
** a special lexer for strings in quotes
** this way keywords inside of quotes will be ignored
*/
int quoteLex(void){
    lexLen = 0;
    getNonBlank();
    switch(charClass){
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == NUMBER) {
                addChar();
                getChar();
            }
            nextToken = WORD;
            break;
        case SYMBOL:
            quoteSymTable(nextChar);
            getChar();
            break;
    }
    printf("next Token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
