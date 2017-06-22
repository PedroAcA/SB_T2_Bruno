#include"bibliotecas_tradutor.h"
char * proxima_linha(FILE * arq){//assume que o arquivo ja esta aberto
    char *palavra;
    palavra = le_linha(arq);
    return palavra;
}
/*funcao divide_tokens espera que o arquivo ja tenha sido pre-processado
e que os comentarios tenham sido retirados*/
char* divide_tokens(char *linha){
    return strtok(linha," ,");//subvide a string em tokens

}
// funcao prox_token espera que a palavra ja tenha sido dividida em tokens!
char* prox_token(){
    return strtok(NULL," ,");
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
int eh_rotulo(char * tok){
    return (strstr (tok,":") != NULL);
}
char* elimina_caracter(char* str, char* c){//eleimina somente a primeira ocorrencia do caracter na string
     char* elimina;//usado para substituir o caracter , para um espaco quando ha copy
     elimina= strstr (str,c);//aponta para o endereco de tok que contem ,
        if(elimina!=NULL){
            strncpy(elimina," ",1);
        }
    return str;
}

int eh_aritmetico(char* token){
    return ( (strcmp(token,"add") == strings_iguais) || ( strcmp(token,"sub")== strings_iguais ) ||
             ( strcmp(token,"mult") ==strings_iguais ) || ( strcmp(token,"div")==strings_iguais )
           );
}
int classifica_pulo(char* token){
    if (strcmp(token,"jmp") == strings_iguais){
        tipo_pulo = JMP;
        return TRUE;
    }else if( strcmp(token,"jmpp")== strings_iguais ){
        tipo_pulo= JMPP;
        return TRUE;
    }else if( strcmp(token,"jmpn") == strings_iguais ){
        tipo_pulo =  JMPN;
        return  TRUE;
    }else if( strcmp(token,"jmpz")== strings_iguais ){
        tipo_pulo = JMPZ;
        return TRUE;
    }else{
        return FALSE;
    }
}
int acessa_memoria(char* token){
   // char * operandos;
    // a ordem dos operandos eh: op1 => src (fonte) e op2 => dest (destino)
    if ( (strcmp(token,"copy") == strings_iguais) ){
       // operandos = prox_token();// pega o formato op1,op2;
        op1 = prox_token();// redivide a trsing dos operandos em tokens a partir da ','
        op2 = prox_token();// pega proximo operando apos a divisao em tokens de ','
        return TRUE;
    }else if( ( strcmp(token,"load")== strings_iguais ) ){
        op1 = prox_token();
        op2 = "eax";
        return TRUE;
    }else if( ( strcmp(token,"store")==strings_iguais ) ){
        op1 = "eax";
        op2 = prox_token();
        return TRUE;
    }else{
        return FALSE;
    }
}
int section (char* linha_lida){
    char* pch;

    
    if ((pch = strstr(linha_lida,"section text")) != NULL){
        escreve_inicio();
    }
    else if ((pch = strstr(linha_lida,": equ ")) != NULL){
        strncpy(pch,"  equ ",6);
        if (ultima_sessao == DATA){
            escreve(linha_lida);
        }
        else{
            escreve_data(linha_lida);
        }
        ultima_sessao = DATA;
        return TRUE;
    }
    else if( (pch = strstr(linha_lida,": const ")) != NULL){
        strncpy(pch,"   db   ",8);
        if (ultima_sessao == DATA){
            escreve(linha_lida);
        }
        else{
            escreve_data(linha_lida);
        }
        ultima_sessao = DATA;
        return TRUE;
    }
    else if((pch = strstr(linha_lida,": space") )!= NULL){
        if(!eh_vetor(linha_lida))
            strncpy(pch," resb 1",7);
        else
            strncpy(pch,"   resb",7);

        if (ultima_sessao == BSS){
            escreve(linha_lida);
        }
        else{
            escreve_bss(linha_lida);
        }
        ultima_sessao = BSS;
        return TRUE;
    }
        return FALSE;
}

int eh_vetor(char* linha){
    //printf("Na linha %s o valor da ultimo foi: %d e do penultimo foi: %d\n",linha, );

    if( ((linha[strlen(linha)-1] > 47) && (linha[strlen(linha)-1] < 58)) ||
        ((linha[strlen(linha)-2] > 47) && (linha[strlen(linha)-2] < 58)) )
        return 1;
    else
        return 0;
}
