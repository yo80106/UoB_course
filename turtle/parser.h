#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include<ctype.h>

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 7
#define strsame(A,B) (strcmp(A,B) == 0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s \
occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); \
exit(2);}

struct prog{
  char wds[MAXNUMTOKENS][MAXTOKENSIZE];
  int cw;
};
typedef struct prog program;

void Prog(program *p);
void Instruclst(program *p);
bool Instruction(program *p);
bool FD(program *p);
bool RT(program *p);
bool LT(program *p);
bool VarNum(char *varnum);
bool Var(char *str);
bool Doloop(program *p);
bool Set(program *p);
bool Polish(program *p);
bool Oper(char *str);
void Test(void);