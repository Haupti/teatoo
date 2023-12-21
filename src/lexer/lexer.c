#include "token.h"
#include <string.h>
#include "lexerr.h"
#include <stdlib.h>
#include <stdio.h>
#include "token_vector.h"

typedef enum {
    SYMBOL,
    WORD,
    COMMENT,
    MULTI_LINE_COMMENT,
} ReadMode;

int is_word_start(char c);
int is_word_character(char c);

TokenVector create_tokens(char * prog, int len){
    Token tokens[len];
    int tokens_index = 0;

    char word[50];
    int word_index = 0;

    ReadMode mode = SYMBOL;


    int i;
    for(i = 0; i <= len; i++){
        char c = prog[i];

        // read section
        if(mode == COMMENT && c=='\n'){
            mode = SYMBOL;
            continue;
        }
        else if(mode == MULTI_LINE_COMMENT && c == ']'){
            if((i+1 < len) && prog[i+1] == '-') {
                mode = SYMBOL;
                continue;
            }
        }

        else if(mode == WORD){
            if(is_word_character(c)){
                word[word_index] = c;
                word_index += 1;
            }
            else { // word is therefor over and we can check what it is
                if(strcmp(word, "IF") == 0){
                    tokens[tokens_index] = new_token(IF);
                    tokens_index += 1;
                }
                else if(strcmp(word, "POW") == 0){
                    tokens[tokens_index] = new_token(POW);
                    tokens_index += 1;
                }
                else if(strcmp(word, "EMPTY?") == 0){
                    tokens[tokens_index] = new_token(IS_EMPTY);
                    tokens_index += 1;
                }
                else if(strcmp(word, "NULL?") == 0){
                    tokens[tokens_index] = new_token(IS_NULL);
                    tokens_index += 1;
                }
                else if(strcmp(word, "EQ") == 0){
                    tokens[tokens_index] = new_token(EQ);
                    tokens_index += 1;
                }
                else if(strcmp(word, "NEQ") == 0){
                    tokens[tokens_index] = new_token(NEQ);
                    tokens_index += 1;
                }
                else if(strcmp(word, "TAKE") == 0){
                    tokens[tokens_index] = new_token(TAKE);
                    tokens_index += 1;
                }
                else if(strcmp(word, "PUT") == 0){
                    tokens[tokens_index] = new_token(PUT);
                    tokens_index += 1;
                }
                else if(strcmp(word, "PEEK") == 0){
                    tokens[tokens_index] = new_token(PEEK);
                    tokens_index += 1;
                }
                else if(strcmp(word, "XOR") == 0){
                    tokens[tokens_index] = new_token(XOR);
                    tokens_index += 1;
                }
                else if(strcmp(word, "RETURN") == 0){
                    tokens[tokens_index] = new_token(RETURN);
                    tokens_index += 1;
                }
                else if(strcmp(word, "EXEC") == 0){
                    tokens[tokens_index] = new_token(EXEC);
                    tokens_index += 1;
                }
                else if(strcmp(word, "OUTNUM") == 0){
                    tokens[tokens_index] = new_token(OUTNUM);
                    tokens_index += 1;
                }
                else if(strcmp(word, "OUTCHAR") == 0){
                    tokens[tokens_index] = new_token(OUTCHAR);
                    tokens_index += 1;
                }
                else if(strcmp(word, "OUT") == 0){
                    tokens[tokens_index] = new_token(OUT);
                    tokens_index += 1;
                }
                else { // must be an identifier then
                    char current_word[word_index+2];
                    memcpy(current_word, word, word_index+1);
                    current_word[word_index+1] = '\0';
                    tokens[tokens_index] = new_identifier_token(current_word);
                    tokens_index += 1;
                }
                memset(word, '\0', sizeof(word));
                word_index = 0;
                mode = SYMBOL;
            }
        }

        if(c == '\0'){
            break;
        }

        if(mode == SYMBOL){
            switch(c){
                case '{':{
                    tokens[tokens_index] = new_token(SCOPE_OPEN);
                    tokens_index += 1;
                    break;
                }
                case '}':{
                    tokens[tokens_index] = new_token(SCOPE_CLOSE);
                    tokens_index += 1;
                    break;
                }
                case '(':{
                    tokens[tokens_index] = new_token(GRP_OPEN);
                    tokens_index += 1;
                    break;
                }
                case ')':{
                    tokens[tokens_index] = new_token(GRP_CLOSE);
                    tokens_index += 1;
                    break;
                }
                case ':':{
                    tokens[tokens_index] = new_token(DEFINE);
                    tokens_index += 1;
                    break;
                }
                case '\n':{
                    tokens[tokens_index] = new_token(TERM_NL);
                    tokens_index += 1;
                    break;
                }
                case ';':{
                    tokens[tokens_index] = new_token(TERM_SEM);
                    tokens_index += 1;
                    break;
                }
                case '[':{
                    tokens[tokens_index] = new_token(BYTE_START);
                    tokens_index += 1;
                    break;
                }
                case ']':{
                    tokens[tokens_index] = new_token(BYTE_END);
                    tokens_index += 1;
                    break;
                }
                case '$':{
                    tokens[tokens_index] = new_token(COPY);
                    tokens_index += 1;
                    break;
                }
                case '1':{
                    tokens[tokens_index] = new_token(BIT_ON);
                    tokens_index += 1;
                    break;
                }
                case '0':{
                    tokens[tokens_index] = new_token(BIT_OFF);
                    tokens_index += 1;
                    break;
                }
                case '!':{
                    tokens[tokens_index] = new_token(NOT);
                    tokens_index += 1;
                    break;
                }
                case '&':{
                    tokens[tokens_index] = new_token(AND);
                    tokens_index += 1;
                    break;
                }
                case '|':{
                    tokens[tokens_index] = new_token(OR);
                    tokens_index += 1;
                    break;
                }
                case ' ':
                case 9: // tab
                    break;
                default:{
                    if(is_word_start(c)){
                        mode = WORD;
                        word[0] = c;
                        word_index = 1;
                        break;
                    }
                    else if(c == '-' && i+1 < len){
                        if(prog[i+1] == '-'){
                            mode = COMMENT;
                            break;
                        }
                        if(prog[i+1] == '['){
                            mode = MULTI_LINE_COMMENT;
                            break;
                        }
                    }
                    lex_err("while parsing: unexpected character", c, i);
                }
            }
        }
    }

    int tokens_size = sizeof(Token) * (tokens_index);
    Token * heap_tokens = malloc(tokens_size);
    memcpy(heap_tokens, tokens, tokens_size);
    TokenVector vec = {heap_tokens, tokens_index};
    return vec;
}


int is_word_start(char c){
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122); // a-Z
}

int is_word_character(char c){
    return is_word_start(c) || (c >= 48 && c <= 57) || c == 95 || c == 63; // a-Z and 0-9 and _ and ?
}




























