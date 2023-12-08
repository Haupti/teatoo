#ifndef TOKEN_HEADER
#define TOKEN_HEADER

#define TKN_STR(x) \
    x.type == IF ? "IF" : ( \
    x.type == EQ ? "EQ" :( \
    x.type == NEQ ? "NEQ" :( \
    x.type == TAKE ? "TAKE" :( \
    x.type == PUT ? "PUT" :( \
    x.type == PEEK ? "PEEK" :( \
    x.type == NOT ? "NOT" :( \
    x.type == AND ? "AND" :( \
    x.type == OR ? "OR" :( \
    x.type == XOR ? "XOR" :( \
    x.type == GRP_OPEN ? "GRP_OPEN" :( \
    x.type == GRP_CLOSE ? "GRP_CLOSE" :( \
    x.type == BYTE_START ? "BYTE_START" :( \
    x.type == BYTE_END ? "BYTE_END" :( \
    x.type == BIT_ON ? "BIT_ON" :( \
    x.type == BIT_OFF ? "BIT_OFF" :( \
    x.type == IDENTIFIER ? "IDENTIFIER" :( \
    x.type == DEFINE ? "DEFINE" :( \
    x.type == COPY ? "COPY" :( \
    x.type == TERM_SEM ? "TERM_SEM" :( \
    x.type == TERM_NL ? "TERM_NL" :( \
    x.type == SCOPE_OPEN ? "SCOPE_OPEN" :( \
    x.type == SCOPE_CLOSE ? "SCOPE_CLOSE" :( \
    x.type == RETURN ? "RETURN" :( \
    x.type == EXEC ? "EXEC" :( \
    x.type == OUTNUM ? "OUTNUM" :( \
    x.type == OUTCHAR ? "OUTCHAR" :( \
    x.type == OUT ? "OUT" :(\
    "" \
))))))))))))))))))))))))))))

typedef enum {
   IF,
   EQ,
   NEQ,
   TAKE,
   PUT,
   PEEK,
   NOT,
   AND,
   OR,
   XOR,
   GRP_OPEN,
   GRP_CLOSE,
   BYTE_START,
   BYTE_END,
   BIT_ON,
   BIT_OFF,
   IDENTIFIER,
   DEFINE,
   COPY,
   TERM_SEM,
   TERM_NL,
   SCOPE_OPEN,
   SCOPE_CLOSE,
   RETURN,
   EXEC,
   OUTNUM,
   OUTCHAR,
   OUT
} TokenType;

typedef struct {
    TokenType type;
    char * name;
} Token;

Token new_token(TokenType type);
Token new_identifier_token(char * name);
void destroy_token(Token token);

typedef struct {
    Token * arr;
    int len;
} TokenVec;

#endif
