#include"bibliotecas_tradutor.h"
char **argumentos_cmd;
char *nome_asm, *nome_obj;
FILE* obj;
void analisa_args_linha_comando(int n_arg,char **args ){
    if(n_arg==2){// Nome programa + arquivo de entrada
        if(arg_contem_extensao(args[1],".asm") ){
            nome_asm = args[1];
            nome_obj = (char*)calloc(tam_string(args[1])+1,sizeof(char));
            strcpy(nome_obj,nome_asm);
            retira_extensao(nome_obj,".asm");
            strcat(nome_obj,".s");
        }else{
            printf("Arquivo de entrada tem que ter a extensao .asm .\nPor favor, tente novamente.");
            exit(1);
        }
    }else if(n_arg==3){// Nome programa + arquivo de entrada + arquivo de saida
        argumentos_cmd = args;
        if(arg_contem_extensao(args[1],".asm") ){
            salva_nomes_entrada_saida(&nome_asm,&nome_obj,".s");
        }else{
            printf("Arquivo de entrada tem que ter a extensao .asm .\nPor favor, tente novamente.");
            exit(1);
        }
    }else{
        printf("\nNumero de argumentos diferente do esperado.\nPor favor, tente novamente inserindo nome do programa + 3 argumentos\n");
        exit(1);
    }
}
void retira_extensao(char*nome, char* extensao){ // serve para seprar o nome do arquivo asm da sua extensao, possibilitando nomear a saida de acordo com  a entrada
    char *pos_ponto = strstr (nome,".");
    while(*pos_ponto != '\0'){
        *pos_ponto = '\0';
        pos_ponto = pos_ponto+1;
    }
}
int arg_contem_extensao(char* arg,char* extensao){
    return (strstr(arg,extensao)!=NULL);
}
int arg_saida_sem_extensao(char *arg){
    return (strrchr (arg,'.')==NULL );
}
void salva_nomes_entrada_saida(char** entrada,char** saida,char* ext_saida){
    *entrada= argumentos_cmd[2];
    if(arg_saida_sem_extensao(argumentos_cmd[2])){
        *saida = (char*)calloc( (tam_string(argumentos_cmd[2]) + tam_string(ext_saida) + 1),sizeof(char));
        strcpy(*saida,argumentos_cmd[2]);
        strcat(*saida,ext_saida);
    }else{
        printf("\nArquivo de saida nao pode ter extensao\n Por favor, tente novamente com um arquivo de saida sem extensao\n");
        exit(-5);
    }
}

int existe_arquivo(FILE* arq){
    return arq!=NULL;
}
FILE* abre_arq_entrada(){// abre para o modo leitura
    return fopen(nome_asm,"r");
}
void cria_arq_obj(){
    obj = fopen(nome_obj,"w+");
}
// traduz as funcoes de assembly inventado que tem 1 operando para IA-32
void escreve_relacao_direta(char*tok){
    fprintf(obj,"%s EAX, %s\n",tok,prox_token());
}
void exclui_arq_obj(){
    if(obj!=NULL){
        remove(nome_obj);
    }
}
void libera_nome_arquivos(){
    free(nome_obj);
}
void fecha_arq_obj(){
    if(obj!=NULL)
        fclose(obj);
}
