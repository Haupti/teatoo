#ifndef PARSER_ERROR_HEADER
#define PARSER_ERROR_HEADER
#include "../../lexer/token.h"

void parse_err_at(char * msg, int pos, int line);
void err_expected_token(Token token_expected, int index, int line);
void err_unexpected_token(Token token_unexpected, int index);
void err_at(char * msg, int index, int line);

#endif
