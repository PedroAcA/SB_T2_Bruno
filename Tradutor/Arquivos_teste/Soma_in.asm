;Programa soma dois numeros (inteiros com sinal) fornecidos pelo usuario
SECTION TEXT
INPUT N1
INPUT N2
LOAD N2
ADD N1
STORE R
OUTPUT R
OUTPUT N1
OUTPUT N2
STOP
 
SECTION DATA
N1: SPACE 4; 4 bytes que equivalem a 1 inteiro
N2: SPACE 4; 4 bytes que equivalem a 1 inteiro 
R: SPACE 4;  4 bytes que equivalem a 1 inteiro
