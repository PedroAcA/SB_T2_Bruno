#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"arquivos.h"
#include"scanner.h"
#define FALSE 0
#define TRUE !(FALSE)
#define strings_iguais 0
#define JMP 2
#define JMPP 3
#define JMPN 4
#define JMPZ 5
#define INDEFINIDO 0
#define DATA 1
#define BSS 2
#define TEXT 0
int tipo_pulo;
char *op1,*op2;// usados para instrucoes de copy, store e load
int ultima_sessao;
