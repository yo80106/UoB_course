#include "extension.h"

int main(int argc, char** argv)
{
  int i;
  FILE *fp;
  program prog;
  SDL_Simplewin sdl;
  graph grp;
  
  Test();
  
  if(argc != 2){
    fprintf(stderr, "No file found.\n");
    exit(2);
  }

  prog.cw = 0;
  for(i = 0; i < MAXNUMTOKENS; i++){
    prog.wds[i][0] = '\0';
  }
  if(!(fp = fopen(argv[1], "r"))){
    fprintf(stderr, "Cannot open the file: %s\n", argv[1]);
    exit(2);
  }
  i = 0;
  while(fscanf(fp, "%s", prog.wds[i++]) == 1 && i < MAXNUMTOKENS);
  fclose(fp);
  
  /* Initialise window and set drawing colour as white */
  Neill_SDL_Init(&sdl);
  Neill_SDL_SetDrawColour(&sdl, 255, 255, 255);
  
  /* Initialise graphing information. */
  grp.x_new = X_CENTER;
  grp.y_new = Y_CENTER;
  grp.angle = 0;
  
  SDL_Delay(1000);
  Prog(&prog, &sdl, &grp);
  SDL_Delay(2000);
  
  return 0;
}

void Prog(program *p, SDL_Simplewin *sdl, graph *grp)
{
  if(!strsame(p->wds[p->cw], "{")){
    ERROR("No { at the beginning?");
  }
  p->cw += 1;
  Instruclst(p, sdl, grp);
}

void Instruclst(program *p, SDL_Simplewin *sdl, graph *grp)
{
  if(strsame(p->wds[p->cw], "}")){
    return;
  }
  if(Instruction(p, sdl, grp) == false){
    ERROR("Expecting a number or an upper letter in <VARNUM>.");
  }
  p->cw += 1;
  Instruclst(p, sdl, grp);
}

bool Instruction(program *p, SDL_Simplewin *sdl, graph *grp)
{
  if(strsame(p->wds[p->cw], "FD")){
    return FD(p, sdl, grp);
  }else if(strsame(p->wds[p->cw], "LT")){
    return LT(p, grp);
  }else if(strsame(p->wds[p->cw], "RT")){
    return RT(p, grp);
  }else if(strsame(p->wds[p->cw], "DO")){
    return Doloop(p, sdl, grp);
  }else if(strsame(p->wds[p->cw], "SET")){
    return Set(p, grp);
  }else if(strsame(p->wds[p->cw], "}")){
    return true;
  }else{
    return false;
  }
}

bool FD(program *p, SDL_Simplewin *sdl, graph *grp)
{
  float f;
  float x_angle, y_angle;
  
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

bool RT(program *p, graph *grp)
{
  float f;
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

bool LT(program *p, graph *grp)
{
  float f;
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

float VarNum(char *varnum, graph *grp)
{
  char c = *varnum;
  int ascii = c - 'A';
  float f;
  
  if(Var(varnum)){
    f = grp->var[ascii];
    if(f > MAXNUMBER-1){
      if(grp->ptr_arr[ascii] != NULL){
        ERROR("Variable cannot be operated by <INSTRUCTION>")
      }else{
        ERROR("Variable is not defined.")
      }
    }
  }
  /* VarNum can also be a number (including float). */
  if(sscanf(varnum, "%f", &f) == -1){
    ERROR("<VARNUM> shall be single letter or a number.")
  }
  return f;
}

bool Var(char *str)
{
  char c = *str;
  
  /* Var shall be an upper [A-Z]. */
  if(strlen(str) == 1 && isupper(str[0])){
    if((c >= 'A') & (c <= 'Z')){
      return true;
    }
  }
  return false;
}

bool Doloop(program *p, SDL_Simplewin *sdl, graph *grp)
{
  int n;
  do_info *do_loop = (do_info*)malloc(sizeof(do_info));
  /* Variable used in do loop. */
  char c;
  int ascii;
  
  if(!strsame(p->wds[p->cw], "DO")){
    return false;
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
  free(do_loop);
  return true;  
}

bool Set(program *p, graph *grp)
{
  /* Variable used in SET statement. */
  char c;
  int ascii;
  stack *st;
  float a[ARRAYSIZE];
  
  if(!strsame(p->wds[p->cw], "SET")){
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
  if(List(p, grp, ascii, a) == true){
    /* Store the list to the variable. */
    grp->ptr_arr[ascii] = a;
  }else if(List_func(p, grp) == true){
    p->cw += 1;
    /* Store the variable for list function output. */
    grp->var[ascii] = List_func_inter(p, grp, a);
  }else{
    if(Polish(p, grp, ascii, st) == false){
      ERROR("Incorrect POLISH format.");
    }
  }
  stack_free(st);
  return true;
}

bool Polish(program *p, graph *grp, int ascii, stack *st)
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

bool Oper(char *str)
{
  if (strsame(str, "+")) {
    return true;
  }
  if (strsame(str, "-")) {
    return true;
  }
  if (strsame(str, "*")) {
    return true;
  }
  if (strsame(str, "/")) {
    return true;
  }
  return false;
}

bool List(program *p, graph *grp, int ascii, float a[ARRAYSIZE])
{
  float f;
  int i = 0;
  
  if(!strsame(p->wds[p->cw],"[")){
    return false;
  }
  p->cw += 1;
  
  /* Store variables or numbers inside of LIST,
  e.g. [ 10 20 30 A ]*/
  while(!strsame(p->wds[p->cw],"]")){
    f = VarNum(p->wds[p->cw], grp);
    a[i] = f;
    i++;
    p->cw += 1;
  }
  /* Keep the length of LIST here */
  /* Store in variable if possible. */
  grp->list_sz[ascii] = i;
  /* Use LIST directly. */
  grp->list_sz_tmp = i;
  return true;
}

bool List_func(program *p, graph *grp)
{ 
  if(strsame(p->wds[p->cw], "SUM")){
    grp->list_func = 1;
    return true;
  }else if(strsame(p->wds[p->cw], "MEAN")){
    grp->list_func = 2;
    return true;
  }else if(strsame(p->wds[p->cw], "LEN")){
    grp->list_func = 3;
    return true;
  }
  return false;
}

float List_func_inter(program *p, graph *grp, float a[ARRAYSIZE])
{
  float *flt_arr;
  float output = 0;
  int i, var;
  int size = 0;
  int func = grp->list_func;
  char c;
  
  c = *p->wds[p->cw];
  var = c - 'A';
  
  /* List function == SUM */
  if(func == 1){
    if(List(p, grp, var, a) == false){
      flt_arr = grp->ptr_arr[var];
      size = grp->list_sz[var];
      for(i = 0; i < size; i++){
        output += flt_arr[i];
      }
    }else{
      size = grp->list_sz_tmp;
      for(i = 0; i < size; i++){
        output += a[i];
      }
    }
  }else if(func == 2){
    if(List(p, grp, var, a) == false){
      flt_arr = grp->ptr_arr[var];
      size = grp->list_sz[var];
      for(i = 0; i < size; i++){
        output += flt_arr[i];
      }
    }else{
      size = grp->list_sz_tmp;
      for(i = 0; i < size; i++){
        output += a[i];
      }
    }
    output /= size;
  }else if(func == 3){
    if(List(p, grp, var, a) == false){
      size = grp->list_sz[var];
    }else{
      size = grp->list_sz_tmp;
    }
    output = size;
  }else{
    ERROR("Invalid LIST function")
  }
  return output;
}

stack *stack_init(void)
{
  stack *st;
  st = (stack*)malloc(sizeof(stack));
  st->top = -1;
  return st;
}

void stack_push(stack *st, float f)
{
  if(st->top == ARRAYSIZE-1){
    ERROR("No space in stack.")
  }else{
    st->top++;
    st->items[st->top] = f;
  }
}

float stack_pop(stack *st)
{
  float f;
  
  if(st->top == -1){
    ERROR("Stack is empty");
  }else{
    f = st->items[st->top];
    st->top--;
  }
  return f;
}

bool stack_free(stack *st)
{
  if(st == NULL){
     return true;
  }
  free(st);
  return true;
}