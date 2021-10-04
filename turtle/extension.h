#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<assert.h>
#include<math.h>
#include "neillsdl2.h"

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 7
#define strsame(A,B) (strcmp(A,B) == 0)
#define ERROR(PHRASE) {fprintf(stderr, "Fatal Error %s \
occured in %s, line %d\n", PHRASE, __FILE__, __LINE__); \
exit(2);}
/* Center is defined as halves of window's width and height. */
#define X_CENTER WWIDTH/2
#define Y_CENTER WHEIGHT/2
#define ARRAYSIZE 200
#define NUMLETTERS 26
#define PI 3.14
#define DEGREES 180
#define STACKSIZE 100
#define MAXNUMBER 99999

struct graph_info{
  float x_old;
  float y_old;
  float x_new;
  float y_new;
  float angle;
  float var[NUMLETTERS];
  float cal_val[ARRAYSIZE];
  float *ptr_arr[NUMLETTERS];
  int list_sz[NUMLETTERS];
  int list_sz_tmp;
  int list_func;
};
typedef struct graph_info graph;

struct do_info{
  int from;
  int to;
  int var;
  int start;
};
typedef struct do_info do_info;

struct stack{
  float items[STACKSIZE];
  int top;
};
typedef struct stack stack;

struct prog{
  char wds[MAXNUMTOKENS][MAXTOKENSIZE];
  int cw;
};
typedef struct prog program;

/* Turtule functions */
void Prog(program *p, SDL_Simplewin *sdl, graph *grp);
void Instruclst(program *p, SDL_Simplewin *sdl, graph *grp);
bool Instruction(program *p, SDL_Simplewin *sdl, graph *grp);
bool FD(program *p, SDL_Simplewin *sdl, graph *grp);
bool RT(program *p, graph *grp);
bool LT(program *p, graph *grp);
float VarNum(char *varnum, graph *grp);
bool Var(char *str);
bool Doloop(program *p, SDL_Simplewin *sdl, graph *grp);
bool Set(program *p, graph *grp);
bool Polish(program *p, graph *grp, int ascii, stack *st);
bool Oper(char *str);
void Test(void);
bool List(program *p, graph *grp, int ascii, float a[ARRAYSIZE]);
bool List_func(program *p, graph *grp);
float List_func_inter(program *p, graph *grp, float a[ARRAYSIZE]);

/* Stack functions */
stack *stack_init(void);
void stack_push(stack *st, float f);
float stack_pop(stack *st);
bool stack_free(stack *st);