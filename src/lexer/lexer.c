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
    memset(word, '\0', sizeof(word));
    int word_index = 0;

    ReadMode mode = SYMBOL;

    int line_counter = 0;

    int i;
    for(i = 0; i <= len; i++){
        char c = prog[i];

        if(c == '-' && i+1 < len){
            if(prog[i+1] == '-'){
                mode = COMMENT;
                continue;
            }
            if(prog[i+1] == '['){
                mode = MULTI_LINE_COMMENT;
                continue;
            }
        }

        // read section
        if(mode == COMMENT && c=='\n'){
            mode = SYMBOL;
            tokens[tokens_index] = new_token(TERM_NL, line_counter);
            tokens_index += 1;
            line_counter += 1;
            continue;
        }
        else if(mode == MULTI_LINE_COMMENT){
            if(c == '\n'){
                tokens[tokens_index] = new_token(TERM_NL, line_counter);
                tokens_index += 1;
                line_counter += 1;

            }
            else if(c == ']' && (i+1 < len) && prog[i+1] == '-') {
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
                    tokens[tokens_index] = new_token(IF, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "POW") == 0){
                    tokens[tokens_index] = new_token(POW, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "EMPTY?") == 0){
                    tokens[tokens_index] = new_token(IS_EMPTY, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "NULL?") == 0){
                    tokens[tokens_index] = new_token(IS_NULL, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "EQ") == 0){
                    tokens[tokens_index] = new_token(EQ, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "NEQ") == 0){
                    tokens[tokens_index] = new_token(NEQ, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "TAKE") == 0){
                    tokens[tokens_index] = new_token(TAKE, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "PUT") == 0){
                    tokens[tokens_index] = new_token(PUT, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "PEEK") == 0){
                    tokens[tokens_index] = new_token(PEEK, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "XOR") == 0){
                    tokens[tokens_index] = new_token(XOR, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "RETURN") == 0){
                    tokens[tokens_index] = new_token(RETURN, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "EXEC") == 0){
                    tokens[tokens_index] = new_token(EXEC, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "OUTNUM") == 0){
                    tokens[tokens_index] = new_token(OUTNUM, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "OUTCHAR") == 0){
                    tokens[tokens_index] = new_token(OUTCHAR, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "OUT") == 0){
                    tokens[tokens_index] = new_token(OUT, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "STACK") == 0){
                    tokens[tokens_index] = new_token(STACK, line_counter);
                    tokens_index += 1;
                }
                else if(strcmp(word, "APPLY") == 0){
                    tokens[tokens_index] = new_token(APPLY, line_counter);
                    tokens_index += 1;
                }
                else { // must be an identifier then
                    char current_word[word_index+2];
                    memcpy(current_word, word, word_index+1);
                    current_word[word_index+1] = '\0';
                    tokens[tokens_index] = new_identifier_token(current_word, line_counter);
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
                    tokens[tokens_index] = new_token(SCOPE_OPEN, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '}':{
                    tokens[tokens_index] = new_token(SCOPE_CLOSE, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '(':{
                    tokens[tokens_index] = new_token(GRP_OPEN, line_counter);
                    tokens_index += 1;
                    break;
                }
                case ')':{
                    tokens[tokens_index] = new_token(GRP_CLOSE, line_counter);
                    tokens_index += 1;
                    break;
                }
                case ':':{
                    tokens[tokens_index] = new_token(DEFINE, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '\n':{
                    tokens[tokens_index] = new_token(TERM_NL, line_counter);
                    tokens_index += 1;
                    line_counter += 1;
                    break;
                }
                case ';':{
                    tokens[tokens_index] = new_token(TERM_SEM, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '[':{
                    tokens[tokens_index] = new_token(BYTE_START, line_counter);
                    tokens_index += 1;
                    break;
                }
                case ']':{
                    tokens[tokens_index] = new_token(BYTE_END, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '$':{
                    tokens[tokens_index] = new_token(COPY, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '1':{
                    tokens[tokens_index] = new_token(BIT_ON, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '0':{
                    tokens[tokens_index] = new_token(BIT_OFF, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '!':{
                    tokens[tokens_index] = new_token(NOT, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '&':{
                    tokens[tokens_index] = new_token(AND, line_counter);
                    tokens_index += 1;
                    break;
                }
                case '|':{
                    tokens[tokens_index] = new_token(OR, line_counter);
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
                    lex_err("while parsing: unexpected character", c, i, line_counter);
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




























