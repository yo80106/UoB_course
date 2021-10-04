#include "parser.h"

void reset_prog(program *p);
bool test_Var(program *p, char *str);
bool test_VarNum(program *p, char *str);
bool test_Instruction(program *p, char *str1, char *str2);
bool test_Doloop(program *p);
bool test_Oper(program *p, char *str);
bool test_Polish(program *p);
bool test_Set(program *p);

void Test(void)
{
  program prog;
  
  /* Check if Var function could correctly identify
  single upper letter (A-Z). */
  assert(test_Var(&prog, "A") == true);
  assert(test_Var(&prog, "XY") == false);
  assert(test_Var(&prog, "6") == false); 
  
  /* Check if VarNUM funvtion could correctly identify
  single upper letter (A-Z) or a number. */
  reset_prog(&prog);
  assert(test_VarNum(&prog, "A") == true);
  assert(test_VarNum(&prog, "6") == true);
  assert(test_VarNum(&prog, "1.5") == true); 
  assert(test_VarNum(&prog, "XY") == false);
  
  /*Check if Instruction function could correctly identify
  keyword FD and its following <VARNUM>. */
  reset_prog(&prog);
  assert(test_Instruction(&prog, "FD", "A") == true);
  assert(test_Instruction(&prog, "FD", "5") == true);
  assert(test_Instruction(&prog, "FB", "A") == false);
  
  /*Check if Instruction function could correctly identify
  keyword RT and its following <VARNUM>. */
  reset_prog(&prog);
  assert(test_Instruction(&prog, "RT", "B") == true);
  assert(test_Instruction(&prog, "RT", "10") == true);
  assert(test_Instruction(&prog, "TR", "B") == false);
  
  /*Check if Instruction function could correctly identify
  keyword LT and its following <VARNUM>. */
  reset_prog(&prog);
  assert(test_Instruction(&prog, "LT", "C") == true);
  assert(test_Instruction(&prog, "LT", "3") == true);
  assert(test_Instruction(&prog, "LR", "C") == false);
  
  /*Check if Doloop function could identify correct grammar */
  reset_prog(&prog);
  strcpy(prog.wds[0], "DO");
  strcpy(prog.wds[1], "N");
  strcpy(prog.wds[2], "FROM");
  strcpy(prog.wds[3], "1");
  strcpy(prog.wds[4], "TO");
  strcpy(prog.wds[5], "10");
  strcpy(prog.wds[6], "{");
  assert(test_Doloop(&prog) == true);
  
  /*Check if Oper function could identify correct grammar */
  reset_prog(&prog);
  assert(test_Oper(&prog, "+") == true);
  assert(test_Oper(&prog, "-") == true);
  assert(test_Oper(&prog, "*") == true);
  assert(test_Oper(&prog, "/") == true);
  assert(test_Oper(&prog, "3") == false);
  
  /*Check if Polish function could identify correct grammar */
  reset_prog(&prog);
  strcpy(prog.wds[0], "A");
  strcpy(prog.wds[1], "20");
  strcpy(prog.wds[2], "+");
  strcpy(prog.wds[3], ";");
  assert(test_Polish(&prog) == true);
  
  /*Check if Set function could identify correct grammar */
  reset_prog(&prog);
  strcpy(prog.wds[0], "SET");
  strcpy(prog.wds[1], "X");
  strcpy(prog.wds[2], ":=");
  strcpy(prog.wds[3], "1");
  strcpy(prog.wds[4], ";");
  assert(test_Set(&prog) == true);
}

void reset_prog(program *p)
{
  int i;
   
  for(i = 0; i < MAXNUMTOKENS; i++){
    strcpy(p->wds[p->cw], "'\0'");
  }
}

bool test_Var(program *p, char *str)
{
  p->cw = 0;
  strcpy(p->wds[p->cw], str);
  return Var(p->wds[p->cw]);
}

bool test_VarNum(program *p, char *str)
{
  p->cw = 0;
  strcpy(p->wds[p->cw], str);
  return VarNum(p->wds[p->cw]);
}

bool test_Instruction(program *p, char *str1, char *str2)
{
  p->cw = 0;
  strcpy(p->wds[p->cw], str1);
  strcpy(p->wds[(p->cw)+1], str2);
  
  return Instruction(p);
}

bool test_Doloop(program *p)
{
  p->cw = 0;
  return Doloop(p);
}

bool test_Oper(program *p, char *str)
{
  p->cw = 0;
  strcpy(p->wds[p->cw], str);
  return Oper(p->wds[p->cw]);
}

bool test_Polish(program *p)
{
  p->cw = 0;
  return Polish(p);
}

bool test_Set(program *p)
{
  p->cw = 0;
  return Set(p);
}