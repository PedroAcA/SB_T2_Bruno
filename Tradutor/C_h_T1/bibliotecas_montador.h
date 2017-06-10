/* container de todas as bibliotecas usadas pelo montador
   para, por exemplo, fazer #include"bibliotecas_montador.h" em
   todos os .c em vez de 2 ou mais includes (stdio, stdlib e string, etc)*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"tabelas.h"
#include"scanner.h"
#include"arquivos.h"
#include"pre_processador.h"
#include"parser.h"
#include "parser2.h"
/*a variavel rotulos_linha indica quantos rotulos existem por linha
(possibilita detectar o erro de mais de um rótulo por linha)
*/
FILE *obj;
char *nome_pre,*nome_asm,*nome_obj;
short int *indice_realocacao;
int contador_linha,rotulos_linha,tamanho_realoc;
short int contador_posicao;
int passagem,total_erros,fechou_begin_end,endereco_dados,tem_begin,tem_stop;
int qte_text,qte_data;
TabelaDeInstrucoes* Instrucoes;
TabelaDeSimbolos* TS;
TabelaDeDiretivas* Tab_Dir;
TabelaDeUso* TU;
TabelaDeDefinicoes* TD;
