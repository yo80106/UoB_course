#include "interpreter.h"

void reset_prog(program *p);
bool test_Var(program *p, char *str);
float test_VarNum(char *varnum, graph *grp);
bool test_FD(program *p, SDL_Simplewin *sdl, graph *grp, 
             char *str1, char *str2);
bool test_RT(program *p, graph *grp, char *str1, char *str2);
bool test_LT(program *p, graph *grp, char *str1, char *str2);
int test_Doloop(program *p, SDL_Simplewin *sdl, graph *grp);
bool test_Oper(program *p, char *str);
bool test_Polish(program *p, graph *grp, int ascii, stack *st);
bool test_Set(program *p, graph *grp);

void Test(void)
{
  program prog;
  SDL_Simplewin sdl;
  graph grp;
  stack *st;
  
  grp.x_new = X_CENTER;
  grp.y_new = Y_CENTER;
  grp.angle = 0;
  
  Neill_SDL_Init(&sdl);
  
  /* Check if Var function could correctly identify
  single upper letter (A-Z). */
  assert(test_Var(&prog, "A") == true);
  assert(test_Var(&prog, "XY") == false);
  assert(test_Var(&prog, "6") == false);
  
  /* Check if VarNum function could turn string varible or 
  number into float. */
  grp.var['X'-'A'] = 13;
  assert(((int)test_VarNum("X", &grp) == 13) == true);
  assert(((int)test_VarNum("14.9", &grp) == 14) == true);
  
  /* Check if FD function can identify keyword and use
  the variable. */
  reset_prog(&prog);
  assert(test_FD(&prog, &sdl, &grp, "FD", "30") == true);
  assert(test_FD(&prog, &sdl, &grp, "FF", "10") == false);
  
  grp.var['X'-'A'] = 0;
  assert(test_FD(&prog, &sdl, &grp, "FD", "X") == true);
  
   /* Check if RT function can identify keyword and use
  the variable. */
  reset_prog(&prog);
  grp.angle = 45;
  assert(test_RT(&prog, &grp, "RT", "45") == true);
  grp.var['Y'-'A'] = 45;
  assert(test_RT(&prog, &grp, "RT", "Y") == true);
  assert(test_RT(&prog, &grp, "TR", "45") == false);
  
  /* Check if LT function can identify keyword and use
  the variable. */
  reset_prog(&prog);
  grp.angle = 90;
  assert(test_LT(&prog, &grp, "LT", "135") == true);
  grp.var['Z'-'A'] = 45;
  assert(test_LT(&prog, &grp, "LT", "Z") == true);
  assert(test_LT(&prog, &grp, "LR", "135") == false);
  
  /*Check if Doloop function could identify correct grammar */
  reset_prog(&prog);
  strcpy(prog.wds[0], "DO");
  strcpy(prog.wds[1], "N");
  strcpy(prog.wds[2], "FROM");
  strcpy(prog.wds[3], "1");
  strcpy(prog.wds[4], "TO");
  strcpy(prog.wds[5], "10");
  strcpy(prog.wds[6], "{");
  strcpy(prog.wds[7], "}");
  assert((test_Doloop(&prog, &sdl, &grp) == 10) == true);

  /*Check if Oper function could identify correct grammar */
  reset_prog(&prog);
  assert(test_Oper(&prog, "+") == true);
  assert(test_Oper(&prog, "-") == true);
  assert(test_Oper(&prog, "*") == true);
  assert(test_Oper(&prog, "/") == true);
  assert(test_Oper(&prog, "3") == false);

  /*Check if Polish function could operate Polish calculation */
  st = stack_init();
  reset_prog(&prog);
  grp.var['X'-'A'] = 10;
  strcpy(prog.wds[0], "X");
  strcpy(prog.wds[1], "20");
  strcpy(prog.wds[2], "+");
  strcpy(prog.wds[3], ";");
  assert(test_Polish(&prog, &grp, 'X'-'A', st) == true);
  assert(((int)grp.var['X'-'A'] == 30) == true);
  stack_free(st);

  /*Check if Set function could identify correct grammar */
  reset_prog(&prog);
  strcpy(prog.wds[0], "SET");
  strcpy(prog.wds[1], "X");
  strcpy(prog.wds[2], ":=");
  strcpy(prog.wds[3], "10");
  strcpy(prog.wds[4], ";");
  assert(test_Set(&prog, &grp) == true);
  assert(((int)grp.var['X'-'A'] == 10) == true);

}

void reset_prog(program *p)
{
  int i;
  
  p->cw = 0;
  for(i = 0; i < MAXNUMTOKENS; i++){
    strcpy(p->wds[i], "'\0'");
  }
}

bool test_Var(program *p, char *str)
{
  p->cw = 0;
  strcpy(p->wds[p->cw], str);
  return Var(p->wds[p->cw]);
}

float test_VarNum(char *varnum, graph *grp)
{
  char c = *varnum;
  int ascii = c - 'A';
  float f;
  
  if(Var(varnum)){
    f = grp->var[ascii];
    if((f < f) == true){
      ERROR("Variable is not defined.")
    }
  }
  /* VarNum can also be a number (including float). */
  if(sscanf(varnum, "%f", &f) == -1){
    ERROR("<VARNUM> shall be single letter or a number.")
  }
  return f;
}

bool test_FD(program *p, SDL_Simplewin *sdl, graph *grp, char *str1, char *str2)
{
  float f;
  int x_angle, y_angle;
  
  /* Copy string for testing. */
  p->cw = 0;
  strcpy(p->wds[p->cw], str1);
  strcpy(p->wds[(p->cw)+1], str2);
  
  if(!strsame(p->wds[p->cw], "FD")){
    return false;
  }else{
    p->cw += 1;
    f = VarNum(p->wds[p->cw], grp);
  }
  grp->x_old = grp->x_new;
  grp->y_old = grp->y_new;
  
  /* Rotate the angle for FD */
  x_angle = f*sin(grp->angle);
  y_angle = f*cos(grp->angle);
  
  grp->x_new = grp->x_old + x_angle;
  grp->y_new = grp->y_old - y_angle;
  
  SDL_RenderDrawLine(sdl->renderer, grp->x_old, grp->y_old, 
                     grp->x_new, grp->y_new);
  Neill_SDL_UpdateScreen(sdl);
  SDL_Delay(100);
  return true;
}

bool test_RT(program *p, graph *grp, char *str1, char *str2)
{
  float f;
  
  /* Copy string for testing. */
  p->cw = 0;
  strcpy(p->wds[p->cw], str1);
  strcpy(p->wds[(p->cw)+1], str2);
  
  if(!strsame(p->wds[p->cw], "RT")){
    return false;
  }else{
    p->cw += 1;
    f = VarNum(p->wds[p->cw], grp);
  }
  /* Calculate new angle (using radius). */
  grp->angle = grp->angle - f * PI/DEGREES;
  return true;
}

bool test_LT(program *p, graph *grp, char *str1, char *str2)
{
  float f;
  
  /* Copy string for testing. */
  p->cw = 0;
  strcpy(p->wds[p->cw], str1);
  strcpy(p->wds[(p->cw)+1], str2);
  
  if(!strsame(p->wds[p->cw], "LT")){
    return false;
  }else{
    p->cw += 1;
    f = VarNum(p->wds[p->cw], grp);
  }
  /* Calculate new angle (using radius). */
  grp->angle = grp->angle + f * PI/DEGREES;
  return true;
}

/* This test function returns number of loop. */
int test_Doloop(program *p, SDL_Simplewin *sdl, graph *grp)
{
  int n, output;
  do_info *do_loop = (do_info*)malloc(sizeof(do_info));
  /* Variable used in do loop. */
  char c;
  int ascii;
  
  if(!strsame(p->wds[p->cw], "DO")){
    return 0;
  }
  p->cw += 1;

  /* Check <VAR> */
  if(!Var(p->wds[p->cw])){
    ERROR("Incorrect <VAR> in DO statement.")
  }else{
    c = *p->wds[p->cw];
    ascii = c - 'A';
    do_loop->var = ascii;
  }
  p->cw += 1;

  /* Check FROM */
  if(strsame(p->wds[p->cw],"FROM")){
    p->cw += 1;
  }else{
    ERROR("Missing FROM in DO statement.")
  }
  
  /* Get FROM's value. */
  n = (int)(VarNum(p->wds[p->cw], grp));

  do_loop->from = n;
  p->cw += 1;

  /* Check TO */
  if(strsame(p->wds[p->cw],"TO")){
    p->cw += 1;
  }else{
    ERROR("Missing TO in DO statement.")
  }
  
  /* Get TO's value. */
  n = (int)(VarNum(p->wds[p->cw], grp));
  do_loop->to = n;
  p->cw += 1;
  
  if(do_loop->to <= do_loop->from){
    ERROR("Start index is larger than end index of DO loop.")
  }
  /* Start point of DO loop. */
  do_loop->start = p->cw;
  
  for(grp->var[do_loop->var] = do_loop->from; 
      grp->var[do_loop->var] <= do_loop->to;
      grp->var[do_loop->var]++){
        p->cw = do_loop->start;
        Prog(p, sdl, grp);
  }
  output = (grp->var[do_loop->var])-1;
  free(do_loop);
  return output;
}

bool test_Oper(program *p, char *str)
{
  p->cw = 0;
  strcpy(p->wds[p->cw], str);
  return Oper(p->wds[p->cw]);
}

bool test_Polish(program *p, graph *grp, int ascii, stack *st)
{
  float f, x = 0, y = 0, z = 0, result;
  
  if(strsame(p->wds[p->cw],";")){
    result = stack_pop(st);
    grp->var[ascii] = result;
    return true;
  }
  if(Oper(p->wds[p->cw])){
    y = stack_pop(st);
    x = stack_pop(st);
    if(strsame(p->wds[p->cw], "+")){
      z = x + y;
    }else if(strsame(p->wds[p->cw], "-")){
      z = x - y;
    }else if(strsame(p->wds[p->cw], "*")){
      z = x * y;
    }else if(strsame(p->wds[p->cw], "/")){
      z = x / y;
    }
    stack_push(st, z);
    p->cw += 1;
    return Polish(p, grp, ascii, st);
  }else{
    f = VarNum(p->wds[p->cw], grp);
    stack_push(st, f);
    p->cw += 1;
    return Polish(p, grp, ascii, st);
  }
  return false;
}

bool test_Set(program *p, graph *grp)
{
  /* Variable used in SET statement. */
  char c;
  int ascii;
  stack *st;
  
  if(!strsame(p->wds[p->cw], "SET")){
    printf("Inside SET: %s\n", p->wds[p->cw]);
    return false;
  }
  p->cw += 1;
  /* Check <VAR> */
  if(!Var(p->wds[p->cw])){
    ERROR("Incorrect <VAR> in SET statement.");
  }else{
    c = *p->wds[p->cw];
    ascii = c - 'A';
  }
  p->cw += 1;
  if(strsame(p->wds[p->cw],":=")){
    p->cw += 1;
  }else{
    ERROR("Expecting := symbol in SET statement.");
  }
  st = stack_init();
  if(Polish(p, grp, ascii, st) == false){
    ERROR("Incorrect POLISH format.");
  }
  stack_free(st);
  return true;
}