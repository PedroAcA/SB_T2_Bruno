;Programa soma dois numeros (inteiros com sinal) fornecidos pelo usuario
SECTION TEXT
LOAD N2
MULT N1; resultado tem que ser -16
STORE R
OUTPUT R

LOAD N1
MULT N1; tem que ser 16
STORE R
OUTPUT R

LOAD N2
MULT N2; tem que ser 16
STORE R
OUTPUT R

LOAD N2
DIV N1; tem que ser -1
STORE R
OUTPUT R

LOAD N2
DIV N2; tem que ser 1
STORE R
OUTPUT R

LOAD N1
DIV N1; tem que ser 1
STORE R
OUTPUT R

STOP
 
SECTION DATA
N1: CONST 4
N2: CONST -4 
R: SPACE 4;  4 bytes que equivalem a 1 inteiro
