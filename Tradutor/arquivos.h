void analisa_args_linha_comando(int,char **);
int arg_contem_extensao(char*,char*);
int arg_saida_sem_extensao(char*);
void salva_nomes_entrada_saida(char**,char**,char*);

int existe_arquivo(FILE*);
void retira_extensao(char*,char*);
void cria_arq_obj();
void escreve_rotulo(char *);
void escreve_op_aritmetica(char*);
void escreve_pulo_incondicional(char*);
void escrve_cmp();
void escreve_pulo_p(char*);
void escreve_pulo_n(char*);
void escreve_pulo_z(char*);
void escreve_memoria(char*,char*);
void escreve_stop();

void exclui_arq_obj();
void libera_nome_arquivos();
void fecha_arq_obj();
