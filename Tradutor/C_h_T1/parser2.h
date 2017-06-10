//arquivo criado para processar algumas etapas da segunda passagem
void converte_em_enderecos(char *,TabelaDeInstrucoes*,short int*,short int*);
int extrai_endereco(char*,short int*);
void processa_argumentos_copy(char*,short int*,short int*);
void erros_num_args_copy(short int);
int enderecos_sem_erros(char*,short int*,short int*);
int eh_constante(TabelaDeDiretivas*);
int memoria_alocada(TabelaDeDiretivas*,int);
int eh_pulo(char*);
int acessa_memoria(char*);
int endereco_acesso_memoria_valido(TabelaDeDiretivas*,short int*,int);
int endereco_alocado(TabelaDeDiretivas*,short int*,int);
int argumentos_copy_corretos(TabelaDeDiretivas*,short int*,short int*);
int endereco_stop_correto(int);
int divide_zero(TabelaDeDiretivas*);
int pulo_valido(int);
int existe_argumento_externo();
int endereco_nao_externo_sem_erro(char*,short int *,short int*);
