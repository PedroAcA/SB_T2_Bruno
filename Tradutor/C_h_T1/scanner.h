char *proxima_linha(FILE*);/* le o arquivo texto e pega a proxima linha
                             (ate achar espaco)
                            */
char *prox_token();
int tam_string(char *);
int tem_char(char *,char);
char* divide_tokens(char *);
char* le_linha(FILE *);
char* elimina_caracter(char* str, char* c);
short int converte_em_num(char*,short int);
int tem_aritmetica(char*);
int converte_exp_aritmetica(char*,short int*,char*);
int existe_erro_conversao(long int numero, char*,char*,int);
int variavel_bem_formada(char *);
