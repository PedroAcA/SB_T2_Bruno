#include"bibliotecas_tradutor.h"
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
/*tam_string retorna o numero de elementos em uma string (sem incluir o \0)*/
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
        }
    return str;
}
