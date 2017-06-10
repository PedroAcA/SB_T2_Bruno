void analisa_args_linha_comando(int,char **);
int arg_contem_extensao(char*,char*);
int arg_saida_sem_extensao(char*);
void salva_nomes_entrada_saida(char**,char**,char*);

int existe_arquivo(FILE*);
FILE *abre_arq_pre();
void cria_arq_obj();
void exclui_arq_obj();
void escreve_instrucao(TabelaDeInstrucoes* ,short int*);
void escreve_tabelas();
void escreve_tabela_uso();
void escreve_tabela_definicao();
void escreve_tabela_realoc();
void escreve_diretiva(char,short int);
void libera_nome_arquivos();
void fecha_arq_obj();
