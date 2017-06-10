#include"bibliotecas_montador.h"
char **argumentos_cmd;
     /*nome_asm = args[2];
                if(arg_saida_sem_extensao(args[3])){
                    nome_pre = args[3];
                    strcat(nome_pre,".pre");
                    nome_obj= NULL;
                }else{
                    printf("\nArquivo de saida nao pode ter extensao\n Por favor, tente novamente com um arquivo de saida sem extensao\n");
                    exit(-5);
                }*/
void analisa_args_linha_comando(int n_arg,char **args ){
    if(n_arg==4){//3 arguemntos + nome do programa
        argumentos_cmd = args;
        if(strcmp(args[1],"-p")==0){
            if(arg_contem_extensao(args[2],".asm") ){
                salva_nomes_entrada_saida(&nome_asm,&nome_pre,".pre");
                nome_obj= NULL;
            }else{
                printf("\nArquivo de entrada para pre-processamento sem extensao que assembly (.asm)\nPor favor, tente novamente com um arquivo de entrada com extensao .asm\n");
                exit(-6);
            }
        }else if(strcmp(args[1],"-o")==0){
            if(arg_contem_extensao(args[2],".asm") ){
                salva_nomes_entrada_saida(&nome_asm,&nome_obj,".o");
                nome_pre = (char*)calloc( (tam_string(args[3]) + tam_string(".pre") + 1 ), sizeof(char) );
                strcpy(nome_pre,args[3]);
                strcat(nome_pre,".pre");
            }else if(arg_contem_extensao(args[2],".pre") ){
                salva_nomes_entrada_saida(&nome_pre,&nome_obj,".o");
                nome_asm =  NULL;
            }else{
                printf("\nAs extensoes de arquivos de entrada reconhecidas sao somente .asm ou .pre\n Por favor, tente novamente com uma dessas extensoes");
                exit(-7);
            }
        }else{
            printf("\nOperacoes aceitas sao somente -p e -o\nPor favor, tente novamente com uma opcao valida\n");
            exit(-2);
        }
    }else{
        printf("\nNumero de argumentos diferente do esperado.\nPor favor, tente novamente inserindo nome do programa + 3 argumentos\n");
        exit(-3);
    }
}
FILE *abre_arq_pre(){
    if(nome_pre!=NULL)
        return fopen(nome_pre,"r");
    else
        return NULL;
}
int arg_contem_extensao(char* arg,char* extensao){
    return (strstr(arg,extensao)!=NULL);
}
int arg_saida_sem_extensao(char *arg){
    return (strrchr (arg,'.')==NULL );
}
void salva_nomes_entrada_saida(char** entrada,char** saida,char* ext_saida){
    *entrada= argumentos_cmd[2];
    if(arg_saida_sem_extensao(argumentos_cmd[3])){
        *saida = (char*)calloc( (tam_string(argumentos_cmd[3]) + tam_string(ext_saida) + 1),sizeof(char));
        strcpy(*saida,argumentos_cmd[3]);
        strcat(*saida,ext_saida);
    }else{
        printf("\nArquivo de saida nao pode ter extensao\n Por favor, tente novamente com um arquivo de saida sem extensao\n");
        exit(-5);
    }
}

int existe_arquivo(FILE* arq){
    return arq!=NULL;
}
void cria_arq_obj(){
    obj = fopen(nome_obj,"w+");
    if(obj!=NULL && tem_begin)
        fprintf(obj,"CODE\n");
}
void exclui_arq_obj(){
    if(obj!=NULL){
        remove(nome_obj);
    }
}
void escreve_instrucao(TabelaDeInstrucoes *instrucao_atual,short int* args){
    if(obj!=NULL){
        if(strcmp(instrucao_atual->mnemonico,"copy")==0 ){
            fprintf(obj," %d %d %d",(int)instrucao_atual->opcode,(int)args[0],(int)args[1]);
        }else if(strcmp(instrucao_atual->mnemonico,"stop")==0){
            fprintf(obj," %d",(int)instrucao_atual->opcode);
        }else{
            fprintf(obj," %d %d",(int)instrucao_atual->opcode,(int)args[0]);
        }
    }
}
void escreve_diretiva(char tipo, short int valor){
    short int i;
    if(obj!=NULL){
        if(tipo == 'E'){
            for(i=0;i<valor;i++)
                fprintf(obj," %d",0);
        }else if(tipo== 'C'){
            fprintf(obj," %d",valor);
        }
    }
}
void escreve_tabelas(){
    escreve_tabela_uso();
    escreve_tabela_definicao();
    escreve_tabela_realoc();
}
void escreve_tabela_uso(){
    TabelaDeUso* aux = TU;
    if(obj!=NULL){
        fprintf(obj,"\n\nTABLE USE\n");
        while(aux!=NULL){
            fprintf(obj,"%s %d\n",aux->nome,(int)aux->valor);
            aux = aux->prox;
        }
        fprintf(obj,"\n");
    }
}
void escreve_tabela_definicao(){
    TabelaDeDefinicoes* aux = TD;
    if(obj!=NULL){
        fprintf(obj,"TABLE DEFINITION\n");
        while(aux!=NULL){
            fprintf(obj,"%s %d\n",aux->nome,(int)aux->valor);
            aux = aux->prox;
        }
        fprintf(obj,"\n");
    }
}
void escreve_tabela_realoc(){
    int i;
    if(obj!=NULL){
        fprintf(obj,"TABLE REALOCATION\n");
        for(i=0;i<(tamanho_realoc-1);i++)
            fprintf(obj,"%d ",(int)indice_realocacao[i]);

        fprintf(obj,"\n");
    }
}
void libera_nome_arquivos(){
    if(nome_obj!=NULL)
        free(nome_obj);
    if( (strcmp(argumentos_cmd[1],"-o") == 0) && arg_contem_extensao(argumentos_cmd[2],".asm"))
        free(nome_pre);
}
void fecha_arq_obj(){
    if(obj!=NULL)
        fclose(obj);
}
