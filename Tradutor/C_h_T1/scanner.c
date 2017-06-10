#include"bibliotecas_montador.h"
#include <errno.h>//usada para identificar erros de conversao erros ao usar a funcao strtol
#include <limits.h>//usada para identificar erros de conversao erros ao usar a funcao strtol

char * proxima_linha(FILE * arq){//assume que o arquivo ja esta aberto
    char *palavra;
    palavra = le_linha(arq);
    return palavra;
}
/*funcao divide_tokens espera que o arquivo ja tenha sido pre-processado
e que os comentarios tenham sido retirados*/
char* divide_tokens(char *linha){
    return strtok(linha," ");//subvide a string em tokens

}
// funcao prox_token espera que a palavra ja tenha sido dividida em tokens!
char* prox_token(){
    return strtok(NULL," ");
}
/*tam_string retorna o numero de elementos em uma string*/
int tam_string(char* str){
    return strlen(str);
}
int tem_char(char *palavra,char caract){
    return (strchr(palavra,caract)!=NULL);
}
char* le_linha(FILE * arq){
    int i;
    char c = fgetc(arq);
    char* linha = (char*)calloc(1,sizeof(char));
    /*laco percorre cada posicao da string linha (de i =0  incrementando i++)
    e a cada iteracao le o proximo caracter do arquivo. Se esse carater for
    o fim do arquivo ou '\n' entao encerra o laco. A cada itercao, salva o
    caracter na string e aloca espaco para receber o proximo caracter*/
    for(i=0;(c!=EOF && c!='\n');i++,c=fgetc(arq) ){
        linha[i] = c;
        linha = (char*) realloc(linha,(i+2)*sizeof(char));
    }
        linha[i] = '\0';
        return linha;
}
char* elimina_caracter(char* str, char* c){//eleimina somente a primeira ocorrencia do caracter na string
     char* elimina;//usado para substituir o caracter , para um espaco quando ha copy
     elimina= strstr (str,c);//aponta para o endereco de tok que contem ,
        if(elimina!=NULL){
            strncpy(elimina," ",1);
        //    elimina = strstr (str,c);
        }
    return str;
}
short int converte_em_num(char* copia_tok,short int numero){
    long int str_p_num;
    char *aux_erro;
    errno=0;
    str_p_num = strtol(copia_tok,&aux_erro,0);
    if(!existe_erro_conversao(str_p_num,copia_tok,aux_erro,errno)){
        numero+= (short int)str_p_num;
    }else{
        printf("\nErro sintatico: Erro na contante numerica na linha %d\n",contador_linha);
        numero=-1;
        total_erros++;
    }
    return numero;
}
int existe_erro_conversao(long int numero, char*tok,char*aux_erro,int erno){
    return (aux_erro == tok || *aux_erro!= '\0' ||
            ((numero == LONG_MIN || numero == LONG_MAX) && erno == ERANGE));
}
int tem_aritmetica(char* tok){
    return(strchr(tok,'+')!=NULL);
}
//funcao converte_exp_artimetica usa strtok_r para nao perder a referencia
    // do proximo token com relacao ao token original da linha lida
int converte_exp_aritmetica(char* tok,short int* indice_vetor,char *tem_extern){
    int numero=0;
    char *copia_tok = (char*) malloc((tam_string(tok)+1)*sizeof(char));
    char* end_tok,*inicio_desaloc;//o inicio da string copia tem q se salvo pois a funcao strtok_r muda o endereco de copia_tok a cada passagem
    TabelaDeSimbolos* buscador;
    strcpy(copia_tok,tok);
    inicio_desaloc = copia_tok;
    copia_tok = elimina_caracter(copia_tok,"+");
    copia_tok =strtok_r(copia_tok," ",&end_tok);// como houve experessoes como v+1, tem q retokenizar a string
    if(existe_token(copia_tok)){//inicio da avaliacao do rotulo
            buscador = busca_simbolo(TS,copia_tok);
        if(buscador!=NULL){
            variavel_bem_formada(copia_tok);
            if(buscador->externo == 's'){
                *tem_extern='T';
                TU = Insere_var_externa(TU,buscador->nome);
            }
            numero= buscador->valor;
            copia_tok = strtok_r(NULL," ",&end_tok);
            if(existe_token(copia_tok)){// inicio da avalicao do numero
                numero = converte_em_num(copia_tok,numero);
                *indice_vetor = numero-buscador->valor;
            }else{
                printf("\nErro sintatico: Erro na formacao do endereco do vetor %s na linha %d \n",tok,contador_linha);
                total_erros++;
            }
        }else{
            printf("\nErro semantico: Simbolo %s nao definido na linha %d\n",copia_tok,contador_linha);
            numero = -1;
            total_erros++;
        }
    }else{
        printf("\nErro sintatico: Expressao %s invalida na linha %d \n",tok,contador_linha);
        total_erros++;
    }
    free(inicio_desaloc);
    return numero;
}
int variavel_bem_formada(char *var){
    int ret = FALSE;
    if(tam_string(var)>51){// 50 caracteres de nome de variavel + 1 para ':'
        printf("\nErro lexico: Variavel %s com mais de 50 caracteres na linha %d\n",var,contador_linha);
        total_erros++;
    }else if(strpbrk(var,"';!@#$%&*()-+={[}]?/°>.<,|\\  */")!=NULL){//ha um caracter invaliido
        printf("\nErro lexico: Caracteres invalidos na variavel %s presente na linha %d\n",var,contador_linha);
        total_erros++;
    }else if(eh_numero(var[0])){
            printf("\nErro lexico: Variavel %s mal formada na linha %d (Variavel nao pode comecar com numero)\n",var,contador_linha);
            total_erros++;
    }else{
        ret = TRUE;
    }
    return ret;
}
