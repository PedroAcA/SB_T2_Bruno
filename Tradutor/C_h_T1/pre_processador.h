#define FALSE 0
#define TRUE !(FALSE)
typedef struct EQU{
    char nome[50];
    char equivalencia[50];
    struct EQU* prox;
} TabelaEQU;

void pre_processa(char[]);
void remove_desnecessarios (FILE *);
TabelaEQU* cria_lista(FILE*,TabelaEQU*);
void limpa(char*, int);
int EhEQU (char*, char[], char[]);
TabelaEQU* InsereEquivalencia (TabelaEQU*, char[], char[] );
void analisa_equivalencias(FILE*, TabelaEQU*);
void remove_diretivas(FILE*);
