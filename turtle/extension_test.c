#include "extension.h"

void reset_prog(program *p);

void Test(void)
{
  program prog;
  graph grp;
  char c;
  float a[ARRAYSIZE];
  int ascii;
  
  grp.x_new = X_CENTER;
  grp.y_new = Y_CENTER;
  grp.angle = 0;
  
  /*Check <LIST> statement*/
  c = 'A';
  ascii = c-'A';
  strcpy(prog.wds[0], "[");
  strcpy(prog.wds[1], "10");
  strcpy(prog.wds[2], "20");
  strcpy(prog.wds[3], "30");
  strcpy(prog.wds[4], "40");
  strcpy(prog.wds[5], "50");
  strcpy(prog.wds[6], "]");
  assert(List(&prog, &grp, ascii, a) == true);
  
  /*Check <LIST_FUNC> keywords*/
  reset_prog(&prog);
  strcpy(prog.wds[0], "SUM");
  assert(List_func(&prog, &grp) == true);
  assert((grp.list_func == 1) == true);
  prog.cw += 1;
  strcpy(prog.wds[1], "MEAN");
  assert(List_func(&prog, &grp) == true);
  assert((grp.list_func == 2) == true);
  prog.cw += 1;
  strcpy(prog.wds[2], "LEN");
  assert(List_func(&prog, &grp) == true);
  assert((grp.list_func == 3) == true);
  prog.cw += 1;
  
  /*Interpret <LIST_FUNC>*/
  /*Test SUM without including a variable inside of LIST.*/
  reset_prog(&prog);
  strcpy(prog.wds[0], "SUM");
  strcpy(prog.wds[1], "[");
  strcpy(prog.wds[2], "10");
  strcpy(prog.wds[3], "20");
  strcpy(prog.wds[4], "30");
  strcpy(prog.wds[5], "40");
  strcpy(prog.wds[6], "50");
  strcpy(prog.wds[7], "]");
  /*List_func shall use with List function.*/
  List_func(&prog, &grp);
  prog.cw += 1;
  assert((((int)List_func_inter(&prog, &grp, a)) == 150) == true);
  
  /*Test SUM without including a variable inside of LIST.*/
  reset_prog(&prog);
  strcpy(prog.wds[0], "MEAN");
  strcpy(prog.wds[1], "[");
  strcpy(prog.wds[2], "10");
  strcpy(prog.wds[3], "20");
  strcpy(prog.wds[4], "30");
  strcpy(prog.wds[5], "40");
  strcpy(prog.wds[6], "50");
  strcpy(prog.wds[7], "]");
  /*List_func shall use with List function.*/
  List_func(&prog, &grp);
  prog.cw += 1;
  assert((((int)List_func_inter(&prog, &grp, a)) == 30) == true);
  
  /*Test LEN without including a variable inside of LIST.*/
  reset_prog(&prog);
  strcpy(prog.wds[0], "LEN");
  strcpy(prog.wds[1], "[");
  strcpy(prog.wds[2], "10");
  strcpy(prog.wds[3], "20");
  strcpy(prog.wds[4], "30");
  strcpy(prog.wds[5], "40");
  strcpy(prog.wds[6], "50");
  strcpy(prog.wds[7], "]");
  /*List_func shall use with List function.*/
  List_func(&prog, &grp);
  prog.cw += 1;
  assert((((int)List_func_inter(&prog, &grp, a)) == 5) == true);
  
  /*Use SET to keep a LIST in a variable*/
  reset_prog(&prog);
  strcpy(prog.wds[0], "SET");
  strcpy(prog.wds[1], "X");
  strcpy(prog.wds[2], ":=");
  strcpy(prog.wds[3], "50");
  strcpy(prog.wds[4], ";");
  Set(&prog, &grp);
  
  reset_prog(&prog);
  strcpy(prog.wds[0], "SET");
  strcpy(prog.wds[1], "Y");
  strcpy(prog.wds[2], ":=");
  strcpy(prog.wds[3], "SUM");
  strcpy(prog.wds[4], "[");
  strcpy(prog.wds[5], "10");
  strcpy(prog.wds[6], "20");
  strcpy(prog.wds[7], "30");
  strcpy(prog.wds[8], "40");
  strcpy(prog.wds[9], "X");
  strcpy(prog.wds[10], "]");
  
  Set(&prog, &grp);
  /*Variable Y shall keep the value of SUM of the LIST (150)*/
  c = 'Y';
  ascii = c-'A';
  assert(((int)grp.var[ascii] == 150) == true);
}

void reset_prog(program *p)
{
  int i;
  
  p->cw = 0;
  for(i = 0; i < MAXNUMTOKENS; i++){
    strcpy(p->wds[i], "'\0'");
  }
}