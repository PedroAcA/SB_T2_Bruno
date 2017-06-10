typedef struct simbolos{
    char nome[50];
    short int valor;
    char externo;
    struct simbolos *prox;
} TabelaDeSimbolos;

typedef struct instrucoes{
    char mnemonico[7];//6 caracteres de output + '\0'
    short int opcode;
    short int tamanho;//numero total de blocos de 16 bits que os operandos + instrucao ocupa ocupam
                     // como o contador de posicao comeca em zero, somar o tamnho total gera o contador para a proxima instrucao
    short int operandos;// eh realmente necessario se ja tem tamanho?
    struct instrucoes* prox;
} TabelaDeInstrucoes;

typedef struct definicoes{
    char nome[50];
    short int valor;
    struct definicoes* prox;
}TabelaDeDefinicoes;

typedef struct uso{
    char nome[50];
    short int valor;
    struct uso* prox;
}TabelaDeUso;

/*
typedef struct obj{
    char tipo;// valores posssiveis sao 'C' (constante), 'E'(espaco) ou 'I'(instrucao)
    short int op1;
    short int op2;
    short int valor;// vai indicar valor da constante (para o caso , numero de espacos alocados ou opcode)
    struct obj *prox;
} TabelaDeObjetos;//salva todas as informacoes necessarias para fazer o codigo objeto
*/
// a TabeladeDiretivas vai facilitar a valiacao de divisoes por 0, modificacoes
// de constantes e acesso de espacos nao reservados de memoria
typedef struct diretivas{
    short int posicao_memoria;//contador_posicao no momento em que const ou space foram chamadas
    char tipo;// valores posssiveis sao 'C' (constante), 'E'(espaco)
    short int valor;//para a diretiva space sao os espcaos reservado e para a const eh o numero reservado (a fim de saber se ha divisao por 0);
    struct diretivas *prox;
} TabelaDeDiretivas;

void* CriaTabela (void);
TabelaDeSimbolos* InsereSimbolo (TabelaDeSimbolos*, char*,short int);
void InsereSimbolo_Externo (TabelaDeSimbolos*);
TabelaDeSimbolos* busca_simbolo(TabelaDeSimbolos*,char*);
TabelaDeSimbolos* busca_simbolo_por_posicao_memoria(TabelaDeSimbolos*,short int);

TabelaDeInstrucoes* inicializa_instrucoes(void);
TabelaDeInstrucoes* InsereInstrucao(TabelaDeInstrucoes*,char*,short int,short int);
TabelaDeInstrucoes* busca_instrucao(TabelaDeInstrucoes*,char*);
TabelaDeInstrucoes* busca_incrementa_posicao(char*);

TabelaDeDiretivas* insereDiretiva(TabelaDeDiretivas*,int,char,int);
TabelaDeDiretivas* busca_end_incial(TabelaDeDiretivas*,int);

TabelaDeUso* Insere_var_externa(TabelaDeUso*,char*);
void Insere_pos_uso(TabelaDeUso*,short int);

TabelaDeDefinicoes* Insere_Simbolo_publico(TabelaDeDefinicoes*,char*);
void transfere_da_TS_para_TabelaDefinicoes(TabelaDeSimbolos*,TabelaDeDefinicoes*);

void libera_tabela_definicoes(TabelaDeDefinicoes*);
void libera_tabela_uso(TabelaDeUso*);
void libera_tabela_instrucoes(TabelaDeInstrucoes*);
void libera_tabela_simbolos(TabelaDeSimbolos*);
void libera_tabela_diretivas(TabelaDeDiretivas*);

