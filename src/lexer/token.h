#ifndef TOKEN_HEADER
#define TOKEN_HEADER

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
   TERM,
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
