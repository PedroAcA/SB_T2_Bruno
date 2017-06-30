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
int if_aberto;
int linha_if;// indica quantas linhas do if faltam traduzir. -1 significa que nao ha mais linhas para traduzir
char *op1,*op2;// usados para instrucoes de copy, store e load
char tipo_in_out;/*contem a a informacao se eh o input ou output
eh decimal ('d'),caracter ('c'), string ('s'), hexa ('h') ou nenhuma das
anteriores ('\0')*/
int ultima_sessao;
