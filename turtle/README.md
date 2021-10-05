# Turtle Graphics
This is the final project (assignment) for the C programming course at UoB of Computer Science MSc. This project aims to develop a programming language that is simple and educational. Many attempts have been made to create programming languages which are intuitive and easy to learn. One of the best of these was LOGO, which allowed children as young as 3 to learn a computer language. A subset of this language involved a "turtle" which could be driven around the screen using simple instructions. The turtle, when viewed from above, was represented by a triangle.

## An Example
```
{
    FD 30
    LT 45
    FD 30
    LT 45
    FD 30
    LT 45
    FD 30
    LT 45
    FD 30
    LT 45
    FD 30
    LT 45
    FD 30
    LT 45
    FD 30
    LT 45
}
```
![](https://github.com/yo80106/UoB_course/raw/main/turtle/images/example.png)

## Adding Loops
```
{
    DO A FROM 1 TO 8 {
        FD 30
        LT 45
    }
}
```

## Using Variables
```
{
    DO A FROM 1 TO 100 {
        SET C := A 1.5 * ;
        FD C
        RT 62
    }
}
```
![](https://github.com/yo80106/UoB_course/raw/main/turtle/images/variable.png)

## The Formal Grammar
```
<MAIN> ::= "{" <INSTRCTLST>
<INSTRCTLST> ::= <INSTRUCTION><INSTRCTLST> | "}"
<INSTRUCTION> ::= <FD> | <LT> | <RT> | <DO> | <SET>
<FD> ::= "FD" <VARNUM>
<LT> ::= "LT" <VARNUM>
<RT> ::= "RT" <VARNUM>
<DO> ::= "DO" <VAR> "FROM" <VARNUM> "TO" <VARNUM> "{" <INSTRCTLST>
<VAR> ::= [A−Z]
<VARNUM> ::= number | <VAR>
<SET> ::= "SET" <VAR> ":=" <POLISH>
<POLISH> ::= <OP> <POLISH> | <VARNUM> <POLISH> | ";"
<OP> ::= "+" | "−" | "*" | "/"
```

## Instruction
Implement a recursive descent parser - this will report whether or not a given
turtle program follows the formal grammar or not. The input ﬁle is speciﬁed via argv\[1\] - there is no output if the input ﬁle is valid. Elsewise, a non-zero exit is made. Extend the parser, so it becomes an interpreter. The instructions are now 'executed'. Do not write a new program for this, simply extend your existing parser. Output is via SDL. You may ﬁnd the function call SDL_RenderDrawLine useful. Show a testing strategy on the above - you should give details of unit testing, white/black-box testing done on your code. Describe any test-harnesses used. In addition, give examples of the output of many different turtle programs. Convince me that every line of your C code has been tested.
Show an extension to the project in a direction of your choice. It should demonstrate your understanding of some aspect of programming or S/W engineering. If you extend the formal grammar make sure that you show the new, full grammar.

## Results
There are three main programmes, which are the parser, the interpreter, and the extension. The parser and interpreter come with the testing files to verify if the main functions are working correctly. The functions in the testing files operate similarly to those in the main programmes, but with some modifications that could be tested in the "assert" function. I have wrapped all the compiling processes in a Makefile. Consequently, users can easily execute and test the programmes. For more information about the testing, please check the file "testing.txt". As for my extension, I developed a list structure to hold multiple values, where the values could be values or variables. In addition, I developed several functions that deal with the list, such as "SUM", "MEAN", and "LEN", which made the data manipulations easier. For more information about the extension, please check the file "extension.txt".

## Run Turtle Graphics
After the parser and interpreter are compiled, users can start to write their own turtle programmes. Several sample turtle scripts have been prepared (files with .ttl extension). To run turtle programmes, please use the interpreter. For example, 
```
./interpreter spiral1_c.ttl
```
The graphic will look like:

![](https://github.com/yo80106/UoB_course/raw/main/turtle/images/spiral1.gif)
