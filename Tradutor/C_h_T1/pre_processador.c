#include"bibliotecas_montador.h"

void pre_processa(char nome_arq[]){

    FILE* arq;
    TabelaEQU * tabela = NULL;

    //REMOVE OS COMENTARIOS E PASSA TODO O TEXTO PARA CAIXA BAIXA
    arq = fopen(nome_arq,"r");
    if(existe_arquivo(arq)){
        remove_desnecessarios(arq);
        fclose(arq);
    }else{
        printf("Arquivo %s nao encontrado!\n",nome_arq);
        exit(1);
    }

    //ANALISA A DIRETIVA EQU, GERA UMA LISTA DE EQUIVALENCIAS E SUBSTITUI AS EQUIVALENCIAS
    arq = fopen("temp1.pre","r");
    if(existe_arquivo(arq)){
        tabela = cria_lista(arq,tabela); //GERA UMA LISTA COM TODAS AS EQUIVALENCIAS
        rewind(arq);
        analisa_equivalencias(arq,tabela); //SUBSTITUI TODOS OS NOMES POR SEUS EQUIVALENTES
        fclose(arq);
    }else{
        printf("Arquivo temp1.pre nao encontrado!");
    }

    //REMOVE OS IF'S E EQU'S DO CODIGO
    arq = fopen("temp2.pre","r");
    if(existe_arquivo(arq)){
        remove_diretivas(arq);
        fclose(arq);
    }else{
        printf("Arquivo temp2.pre naco encontrado!");
    }

    remove(nome_pre);
    rename("temp3.pre",nome_pre);
    remove("temp1.pre");
    remove("temp2.pre");

}

void remove_desnecessarios (FILE * arq){
    FILE *pre = fopen("temp1.pre","w") ;
    char c, anterior = 'a';

    while((c = fgetc(arq)) != EOF){//ENQUANTO NAO ATINGE O FINAL DO ARQUIVO, LE CARACTERE POR CARACTERE
        if (c!= ';'){ //SE NAO FOR COMENTARIO
            if(c == '\t'){ //REMOVE AS TABULAÇÕES
                c = ' ';
            }
            if(c != ' ' || anterior != ' '){ //SE NAO FOR ESPACOS DUPLICADOS
                c = tolower(c); //PASSA O CARACTERE PARA MINUSCULO
                fprintf(pre,"%c",c);
            }
        }
        else{ //SE FOR COMENTARIO, IGNORA
            while (fgetc(arq) != '\n' );
            fprintf(pre,"\n");
        }
        anterior = c;
    }
    fclose(pre);
}

TabelaEQU* cria_lista(FILE* arq, TabelaEQU* tabela){
    char linha[150], token[50], token2[50];
    char* a = NULL;
    while(!feof(arq)){ //ENQUANTO NAO ATINGE O FINAL DO ARQUIVO
        a = fgets(linha,100,arq); //LE LINHA POR LINHA
//        printf("linha lida: %s",linha);

        a++;

        if(strcmp(linha,"section text\n") ==0) //ATE ENCONTRAR SECTION TEXT
            break;

        if (EhEQU(linha, token, token2) == 1){ //SE FOR DIRETIVA EQU
            tabela = InsereEquivalencia(tabela,token, token2); //ADICIONA EQUIVALENCIA NUMA LISTA;
        }
        limpa(linha,100);
        limpa(token,50);
        limpa(token2,50);
    }

    //if(feof(arq)) //SE NAO ENCONTROU SECTION TEXT
      //  printf("SECTION TEXT FALTANTE\n");

    /*TabelaEQU* p;

    printf("lista: \n");
    for (p=tabela;p!=NULL;p=p->prox){
        printf("Nome: %s\nEquivalencia: %s\n", p->nome, p->equivalencia);
    }*/
    return tabela;
}

void limpa(char* linha, int n){ //PREENCHE UMA STRING COM '\0'
    int i;
    for (i=0;i<n;i++){
        linha[i] = '\0';
    }
}

int EhEQU (char *linha, char token[], char token2[]){
    int i = 0,j;

    while (linha[i] != ' ' && i<50){ //SEPARA O PRIMEIRO TOKEN
        token[i] = linha[i];
        i++;
    }

    if (linha[i-1] == ':' && linha[i+1] == 'e' && linha[i+2] == 'q' && linha[i+3] == 'u'){// SE FOR EQU
        token[i-1] = '\0';

    for(j = i+5; linha[j]!='\n'; j++){ //SEPARA O SEGUNDO TOKEN
        token2[j-(i+5)] = linha[j];
    }
    token2[j-(i+5)] = '\0';

        return 1;
    }
    else
        return 0;
}

TabelaEQU* InsereEquivalencia (TabelaEQU* tabela, char nome[], char equivalencia[] ){ //INSERE UMA EQUIVALENCIA NUMA LISTA
    TabelaEQU * novo = (TabelaEQU*) malloc(sizeof(TabelaEQU));
    strcpy(novo->nome, nome);
    strcpy(novo->equivalencia, equivalencia);
    novo->prox = tabela;
    tabela = novo;
    return tabela;
}

void analisa_equivalencias(FILE* arq, TabelaEQU* tabela){

     TabelaEQU* p;
     char token[50],c;
     FILE *arq2 = fopen("temp2.pre", "w");

     while(!feof(arq)){ //ENQUANTO NAO ALCANCAR O FINAL DO ARQUIVO
        if(( c=fgetc(arq) ) == '\n'){ //SE A LINHA FOR VAZIA, COPIA ELA NO ARQUIVO
            fprintf(arq2,"\n");
        }
        else if(c!=EOF && c!= 13){// SE A LINHA TIVER CODIGO
            fseek(arq,ftell(arq)-sizeof(char),SEEK_SET);
            fscanf(arq,"%s",token); //PEGA UMA PALAVRA
            c= fgetc(arq);// PEGA O ESPAÇO OU \N SUBSEQUENTE

            p=tabela;
            while( (p!= NULL) && (strcmp(p->nome,token) != 0)){
                p= p->prox;
            }

            if(p==NULL){ //SE A PALAVRA NAO ESTA NA LISTA DE QUIVALENCIAS
                fprintf(arq2,"%s",token); //IMPRIME A PALAVRA
            }else{ // SE A PALAVRA ESTA NA LISTA DE EQUIVALENCIAS
                fprintf(arq2,"%s",p->equivalencia); //IMPRIME A EQUIVALENCIA
            }
            if(c!=EOF)
                fprintf(arq2,"%c",c); //IMPRIME O ESPAÇO OU \N SUSEQUENTE
        }
    }
    fclose(arq2);
}

void remove_diretivas(FILE* arq){
    FILE* arq2 = fopen("temp3.pre","w");
    char linha[100],token[50], token2[50];
    int numero_linha = 0;

    while(!feof(arq)){ //ENQUANTO NAO ALCANCAR O FIM DO ARQUIVO
        fgets(linha,100,arq); //PEGA LINHA POR LINHA
        numero_linha++;
        //printf("linha lida: %s",linha);

        if (EhEQU(linha,token, token2)){ //SE A LINHA FOR DE EQU, APAGA
            fprintf(arq2,"\n");
        }
        else if(strstr(linha,"equ") != NULL){ //SE O EQU ESTIVER MAU ESCRITO, ERRO
            fprintf(arq2,"\n");
            printf("ERRO SINTATICO: DIRETIVA EQU INCORRETA NA LINHA: %d\n",numero_linha);
        }
        else if (linha[0] == 'i' && linha [1] == 'f'){ //SE FOR IF
            if(linha[3]=='1'){ //CASO VERDADEIRO, MANTEM A LINHA
                fprintf(arq2,"\n");
                fgets(linha,100,arq);
                numero_linha++;
                fprintf(arq2,"%s",linha);
            }
            else if(linha[3]=='0'){// CASO FALSO, APAGA A LINHA
                fprintf(arq2,"\n");
                fgets(linha,100,arq);
                numero_linha++;
                fprintf(arq2,"\n");
            }
            else{ //CASO CONTRARIO, ERRO
                printf("ERRO SINTATICO: ARGUMENTO DA DIRETIVA IF INVALIDO NA LINHA: %d\n",numero_linha);
                fprintf(arq2,"\n");
                fgets(linha,100,arq);
                numero_linha++;
                fprintf(arq2,"\n");
            }
        }
        else // CASO NAO SEJA DIRETIVA, MANTEM A LINHA
            fprintf(arq2,"%s",linha);

        limpa(linha,100);
        limpa(token,50);
        limpa(token2,50);

    }
    fclose(arq2);
}
