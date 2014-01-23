#include <cstdio>
#include <iostream>
#include "lex.h"
#include "code_gen.h"

using namespace std;

void statement()
{
    /*  statements -> expression SEMI  |  expression SEMI statements  */

    char *tempvar;
    if(match( ID )){
        advance();
        if(match( EQUALS )){
            advance();
            tempvar = expression();
        } else {
            fprintf( stderr, "%d: := Expected\n", yylineno );
        }
    }
    else if( match( IF )){
        advance();
        tempvar = expression_prime();
        if(match (THEN) ){
            advance();
            statement();
        }else{
            fprintf( stderr, "%d: then Expected\n", yylineno );
        }
    }
    else if( match( WHILE )){
        advance();
        tempvar = expression_prime();
        if(match (DO) ){
            advance();
            statement();
        } else{
            fprintf( stderr, "%d: do Expected\n", yylineno );
        }
    }
    else if( match( BEGIN )){
        advance();
        opt_statements();
        if(match( END )){
            advance();
           // statement();
        }else{
            fprintf( stderr, "%d: end Expected\n", yylineno );
        }
    }
    else
        return;
}

char    *expression()
{
    /* expression -> term expression'
     * expression' -> PLUS term expression' |  epsilon
     */

    char  *tempvar, *tempvar2;

    tempvar = term();
    while( match( PLUS ) )
    {
        advance();
        tempvar2 = term();
        printf("    %s += %s\n", tempvar, tempvar2 );
        freename( tempvar2 );
    }

    return tempvar;
}

char    *term()
{
    char  *tempvar, *tempvar2 ;

    tempvar = factor();
    while( match( TIMES ) )
    {
        advance();
        tempvar2 = factor();
        printf("    %s *= %s\n", tempvar, tempvar2 );
        freename( tempvar2 );
    }

    return tempvar;
}

char    *factor()
{
    char *tempvar;

    if( match(NUM) )
    {
        /* Print the assignment instruction. The %0.*s conversion is a form of
         * %X.Ys, where X is the field width and Y is the maximum number of
         * characters that will be printed (even if the string is longer). I'm
         * using the %0.*s to print the string because it's not \0 terminated.
         * The field has a default width of 0, but it will grow the size needed
         * to print the string. The ".*" tells printf() to take the maximum-
         * number-of-characters count from the next argument (yyleng).
         */

        printf("    %s = %0.*s\n", tempvar = newname(), yyleng, yytext );
        advance();
    }
    else if( match(LP) )
    {
        advance();
        tempvar = expression();
        if( match(RP) )
            advance();
        else
            fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno );
    }
    else
        fprintf( stderr, "%d: Number or identifier expected\n", yylineno );

    return tempvar;
}

char    *expression_prime ( void ){
    char *tempvar = expression();
    if(match(GREATER)){
        advance();
        tempvar = expression();
    }else if(match(LESS)){
        advance();
        tempvar = expression();
    }else if(match(REQUALS)){
        advance();
        tempvar = expression();
    }else if(match(GTOET)){
        advance();
        tempvar = expression();
    }else if(match(LTOET)){
        advance();
        tempvar = expression();
    }
}

void opt_statements ( void ){
    while(!match(END)){
        statement();
        if(match(SEMI)){
            advance();
        }else{
            break;
        }
    }
}
