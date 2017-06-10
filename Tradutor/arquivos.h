void analisa_args_linha_comando(int,char **);
int arg_contem_extensao(char*,char*);
int arg_saida_sem_extensao(char*);
void salva_nomes_entrada_saida(char**,char**,char*);

int existe_arquivo(FILE*);
void retira_extensao(char*,char*);
void cria_arq_obj();
void escreve_relacao_direta(char*);
void exclui_arq_obj();
void libera_nome_arquivos();
void fecha_arq_obj();
