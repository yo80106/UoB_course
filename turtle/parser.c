#include "parser.h"

int main(int argc, char** argv)
{
  int i;
  FILE *fp;
  program prog;
  
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
  Prog(&prog);
  printf("Parsed OK\n");
  return 0;
}

void Prog(program *p)
{
  if(!strsame(p->wds[p->cw], "{")){
    ERROR("No { at the beginning?");
  }
  p->cw += 1;
  Instruclst(p);
}

void Instruclst(program *p)
{
  if(strsame(p->wds[p->cw], "}")){
    return;
  }
  if(Instruction(p) == false){
    ERROR("Expecting a number or an upper letter in <VARNUM>.");
  }
  p->cw += 1;
  Instruclst(p);
}

bool Instruction(program *p)
{
  if(strsame(p->wds[p->cw], "FD")){
    return FD(p);
  }else if(strsame(p->wds[p->cw], "LT")){
    return LT(p);
  }else if(strsame(p->wds[p->cw], "RT")){
    return RT(p);
  }else if(strsame(p->wds[p->cw], "DO")){
    return Doloop(p);
  }else if(strsame(p->wds[p->cw], "SET")){
    return Set(p);
  }else if(strsame(p->wds[p->cw], "}")){
    return true;
  }else{
    return false;
  }
}

bool FD(program *p)
{
  if(!strsame(p->wds[p->cw], "FD")){
    return false;
  }else{
    p->cw += 1;
    if(VarNum(p->wds[p->cw])){
      return true;
    }
    return false;
  }
}

bool RT(program *p)
{
  if(!strsame(p->wds[p->cw], "RT")){
    return false;
  }else{
    p->cw += 1;
    if(VarNum(p->wds[p->cw])){
      return true;
    }
    return false;
  }
}

bool LT(program *p)
{
  if(!strsame(p->wds[p->cw], "LT")){
    return false;
  }else{
    p->cw += 1;
    if(VarNum(p->wds[p->cw])){
      return true;
    }
    return false;
  }
}

bool VarNum(char *varnum)
{
  float n;
  
  if(Var(varnum)){
    return true;
  }
  /* VarNum can also be a number (including float). */
  if(sscanf(varnum, "%f", &n) == 1){
    return true;
  }
  return false;
}

bool Var(char *str)
{
  /* Var shall be an upper [A-Z]. */
  if(strlen(str) == 1 && isupper(str[0])){
    return true;
  }
  return false;
}

bool Doloop(program *p)
{
  if(!strsame(p->wds[p->cw], "DO")){
    return false;
  }
  p->cw += 1;
  /* Check <VAR> */
  if(!Var(p->wds[p->cw])){
    ERROR("Incorrect <VAR> in DO statement.");
  }
  p->cw += 1;
  if(strsame(p->wds[p->cw],"FROM")){
    p->cw += 1;
    if(VarNum(p->wds[p->cw])){
      p->cw += 1;
    }
    if(strsame(p->wds[p->cw],"TO")){
      p->cw += 1;
      if(VarNum(p->wds[p->cw])){
        p->cw += 1;
      }
      if(strsame(p->wds[p->cw],"{")){
        return true;
      }else{
        ERROR("Missing { in DO statement.")
      }
    }else{
      ERROR("Missing TO in DO statement.")
    }
  }else{
    ERROR("Missing FROM in DO statement.")
  }
}

bool Set(program *p)
{
  if(!strsame(p->wds[p->cw], "SET")){
    return false;
  }
  p->cw += 1;
  /* Check <VAR> */
  if(!Var(p->wds[p->cw])){
    ERROR("Incorrect <VAR> in SET statement.");
  }
  p->cw += 1;
  if(strsame(p->wds[p->cw],":=")){
    p->cw += 1;
    if(Polish(p) == false){
      ERROR("Incorrect POLISH format.");
    }
    return true;
  }
  ERROR("Expecting := symbol in SET statement.");
}

bool Polish(program *p)
{
  if(strsame(p->wds[p->cw],";")){
    return true;
  }
  if(Oper(p->wds[p->cw])){
    p->cw += 1;
    return Polish(p);
  }
  if(VarNum(p->wds[p->cw])){
    p->cw += 1;
    return Polish(p);
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