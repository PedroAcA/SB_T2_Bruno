char *proxima_linha(FILE*);/* le o arquivo texto e pega a proxima linha
                             (ate achar espaco)
                            */
char *prox_token();
int tam_string(char *);
int tem_char(char *,char);
char* divide_tokens(char *);
char* le_linha(FILE *);
int eh_rotulo(char *);
char* elimina_caracter(char* str, char* c);
int eh_aritmetico(char*);
int classifica_pulo(char*);
int acessa_memoria(char*);
int section_data (char*);
int eh_vetor(char*);
void escreve(char*);
void escreve_data(char*);
void escreve_bss(char*);