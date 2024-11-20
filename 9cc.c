#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
// #include <Kernel/stdarg.h>

typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

Token *token;

void error(char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;
  token = token->next;
  return true;
}

void expect(char op){
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("'%c' It is not", op);
  token = token->next;
}

// If the next token is a number, read one token and return that number.
// In other cases, report an error.
int expect_number(){
  if (token->kind != TK_NUM)
    error("It's not a number");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof(){
  return token->kind == TK_EOF;
}

// Create a new token and connect it to cur

Token *new_token(TokenKind kind, Token *cur, char *str){
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// Tokenize the input string p and return it

Token *tokenize(char *p){
  Token head;
  head.next = NULL;
  Token *cur  = &head;

  while(*p) {
    // Skip blank characters
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-'){
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)){
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;    
    }
    error("I can't talk");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}


int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments\n");
    return 1;
  }

  // Tokenize
  token = tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".global _main\n");
  printf("_main:\n");

  // The beginning of the formula must be a number, so check it out
  // Output the first mov command
  printf("  mov rax, %d\n", expect_number());
  
  while(!at_eof()) {
    if(consume('+')){
      printf(" add rax, %d\n", expect_number());
      continue;
    }
    expect('-');
    printf(" sub rax, %d\n", expect_number());

  }

  printf("  ret\n");
  return 0;
}
