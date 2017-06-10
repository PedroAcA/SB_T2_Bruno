#include"bibliotecas_montador.h"
//#include <errno.h>//usada para identificar erros de conversao erros ao usar a funcao strtol
//#include <limits.h>//usada para identificar erros de conversao erros ao usar a funcao strtol
void verifica_linhas(FILE* arq){
    int continua_busca_token;
    char *linha,*token;
    rotulos_linha = 0;
    contador_posicao=0;
    contador_linha=1;
   // printf("\nPASSAGEM: %d\n",passagem);
    while(!feof(arq)){
        linha = proxima_linha(arq);
        linha = elimina_caracter(linha,"\r");// caso o formato de arquivo termine com \r\n, substitui o \r por espaco
      //  printf("\nLinha lida %s\n",linha);
        //printf("\nContador linha: %d\n",contador_linha);
        token = divide_tokens(linha);
       // printf("\nToken lido %s\n",token);
        continua_busca_token = classifica(token);
       // printf("\nContador posicao: %d\n",contador_posicao);
        while(token!=NULL && continua_busca_token){
            token = prox_token();
           // printf("\n\tToken lido %s\n",token);
            continua_busca_token =classifica(token);
         //   printf("\nContador posicao: %d\n",contador_posicao);
        }
        rotulos_linha = 0;
        contador_linha++;
        free(linha);
    }
}
/*classifica retorna se achou rotulos (TRUE) ou diretivas/instrucoes (FALSE)
Essas informacoes vao definir se eh necessario continuar avaliando os tokens
da linha ou se ja chegou no campo de operandos*/
int classifica(char* tok){
    if(existe_token(tok)){
        if(existe_rotulo(tok)){
            if(passagem==1)
                busca_primeira_passagem(tok);
            //printf("%s eh rotulo!\n",tok);
            return TRUE;
        }else if(!existe_instrucao(tok)){
            if(!existe_diretiva(tok)){
                if(passagem==1){//para nao repetir a msm msg de erro com relacao a primeira passagem
                    printf("\nErro sintatico: Operacao %s nao identificada na linha %d\n",tok,contador_linha);
                    total_erros++;
                }
            }
        }
    }
    return FALSE;
}
int existe_token(char* tok){
    return tok != NULL;
}
int existe_rotulo(char * tok){
    int ultimo_caract = tam_string(tok)-1;// -1 pois os indices vao de 0 a tamanho-1
   // printf("Ultimo caracter de %s %c, cujo tamanho eh: %d",tok,tok[ultimo_caract],sizeof(tok));
    if(tok[ultimo_caract] == ':'){// ultimo caracter valido do token eh : e primeiro nao eh numero sao os requisitos
        if(passagem==1){// so precisa avaliar o rotulo em uma passagem so
            variavel_bem_formada(tok);
        }
        return TRUE;
    }
    return FALSE;
}
int eh_numero(char c){
    return (c>='0' && c<='9');
}
void busca_primeira_passagem(char* tok){

    rotulos_linha++;// o montador so precisa verificar se ha mais de um simbolo
                    // por linha na primeira passagem. Se houver, um erro
                    // eh registrado e a segunda passagem nao ocorre.
    if(rotulos_linha<2){
        tok[tam_string(tok)-1] = '\0';// tira os 2 pontos do token que eh rotulo
        if(busca_simbolo(TS,tok)==NULL){//existe_simbolo() vai buscar o endereco do delemnto da tabeladesimbolos que tem o token. Se na ha tal elemento, retona NULL
          //  printf("\nInserindo %s na tabela de simbolos\n",tok);
            TS = InsereSimbolo(TS,tok,contador_posicao);
        }else{
            printf("\nErro semantico: Simbolo %s redefinido na linha %d\n",tok,contador_linha);
            total_erros++;
        }
    }else{
        printf("\nErro semantico: Ha mais de um rotulo na linha %d\n",contador_linha);
        total_erros++;
    }
}
int existe_instrucao(char *tok){
    TabelaDeInstrucoes* instrucao_atual = busca_incrementa_posicao(tok);
    short int endereco_args[2],indice_vetor[2];//endereco_args= lista dos enderecos de memoria dos argumentos presentes na linha analisada
    if(instrucao_atual!=NULL){//OBS: adicionar funcionalidades de segunda passagem depois
        if(passagem==2){
            if(endereco_dados!=-1 && ( (contador_posicao - instrucao_atual->tamanho)>= endereco_dados ) ){// existe secao de dados e ha instrucao na area de dados
                printf("\nErro semantico: Instrucao %s na secao de dados na linha %d\n",instrucao_atual->mnemonico,contador_linha);
                total_erros++;
            }
            tok = prox_token();
           // printf("\nToken:%s\n",tok);
            converte_em_enderecos(tok,instrucao_atual,indice_vetor,endereco_args);
          //  printf("\nToken:%s\n",tok);
            tok = prox_token();
          //  printf("\nProximo Token:%s\n",tok);
            if(!existe_token(tok)){
                if(!existe_argumento_externo()){
                    if(enderecos_sem_erros(instrucao_atual->mnemonico,endereco_args,indice_vetor)){
                        escreve_instrucao(instrucao_atual,endereco_args);
                    }
                }else{
                    if(endereco_nao_externo_sem_erro(instrucao_atual->mnemonico,endereco_args,indice_vetor))
                        escreve_instrucao(instrucao_atual,endereco_args);
                }
            }else{
                    printf("Erro sintatico: Numero argumentos maior que o esperado para a instrucao %s na linha %d\n",instrucao_atual->mnemonico,contador_linha);
                    total_erros++;
            }
        }
        return TRUE;
    }
    return FALSE;
}
int existe_diretiva(char *tok){
    short int numero=0;//guarda informacoes da constante da diretiva CONST ou numero de espacos da diretiva space
    if(strcmp(tok,"section")==0 && passagem==1){//avalia diretivas section somente uma vez (na primeira passagem)
        tok = prox_token();
        avalia_argumentos_section(tok);
        tok = prox_token();
        if(existe_token(tok)){//diretiva section tem mais de um operando. Entao erro
            printf("\nErro sintatico: Diretiva section tem mais de um argumento na linha %d \n",contador_linha);
            total_erros++;
        }
        return TRUE;
    }else if(strcmp(tok,"space")==0){
        tok = prox_token();
         numero = diretiva_space_primeira_passagem(tok,numero);
        if(passagem==1){
            if(contador_posicao>=endereco_dados && endereco_dados!=-1){// endereco_dados ==-1 signifca que nao existe section data
                if(existe_token(prox_token())){
                    printf("\nErro sintatico: Numero incorreto de argumentos para a diretiva space na linha %d\n",contador_linha);
                    total_erros++;
                }
            }else{
                printf("\nErro semantico: Diretiva space foi declarada antes da secao de dados (section data) na linha %d\n",contador_linha);
                total_erros++;
            }
        }else if(passagem==2)
            escreve_diretiva('E',numero);

        return TRUE;
    }else if(strcmp(tok,"const")==0){
        tok = prox_token();
        numero = diretiva_const_primeira_passagem(tok,numero);
        if(passagem==1){
            if(contador_posicao>=endereco_dados && endereco_dados!=-1){// endereco_dados ==-1 signifca que nao existe section data
                if(existe_token(prox_token())){
                    printf("\nErro sintatico: Numero incorreto de argumentos para a diretiva const na linha %d\n",contador_linha);
                    total_erros++;
                }
            }else{
                printf("\nErro semantico: Diretiva const foi declarada antes da secao de dados (section data) na linha %d\n",contador_linha);
                total_erros++;
            }
        }else if(passagem==2)
            escreve_diretiva('C',numero);

        return TRUE;
    }else if(strcmp(tok,"public")==0 && passagem==1){
        tok = prox_token();
        TD = Insere_Simbolo_publico(TD,tok);
        if(existe_token(prox_token())){
            printf("\nErro sintatico: Diretiva public com mais de um rotulo associado na linha %d\n",contador_linha);
            total_erros++;
        }
        return TRUE;
    }else if(strcmp(tok,"extern")==0 && passagem==1){
        if(rotulos_linha<1){//dritiva extern declarada sem rotulo.
            printf("\nErro sintatico: Diretiva extern sem rotulo associado na linha %d\n",contador_linha);
            total_erros++;
        }else{
            InsereSimbolo_Externo(TS);
        }
        if(existe_token(prox_token())){
            printf("\nErro sintatico: Diretiva extern com argumento na linha %d\n",contador_linha);
            total_erros++;
        }
        return TRUE;
    }else if(strcmp(tok,"begin")==0 && passagem==1){
        fechou_begin_end = !fechou_begin_end;
        tem_begin = TRUE;
        if(contador_posicao!=0){
            printf("\nErro sintatico: Diretiva begin declarada em lugar invalido na linha %d\n",contador_linha);
            total_erros++;
        }
        return TRUE;
    }else if(strcmp(tok,"end")==0 && passagem==1){
        tem_begin = TRUE;
        fechou_begin_end = !fechou_begin_end;
        return TRUE;
    }
    if(passagem==1)// se entrou em um dos ifs na primeira passagem retorna TRUE. Se n, FALSE. Como a avaliacao ja foi feita na primeira passagem, nao precisa ser feita na segunda
        return FALSE;
    else
        return TRUE;
}
void avalia_argumentos_section(char* tok){
    tok[(tam_string(tok))]='\0';
    if(strcmp(tok,"data")==0){//secao de dados
            endereco_dados = contador_posicao;// manter o endereco inical da secao de dados possibilita saber se ha pulos
                                              //para secao indevida
        qte_data++;
    }else if(strcmp(tok,"text")==0){//section text declarada no meio do codigo assembly
        if(contador_posicao!=0){
            printf("\nErro sintatico: Diretiva section text declarada em lugar invalido na linha %d\n",contador_linha);
            total_erros++;
        }
        qte_text++;
    }else{// a diretiva eh section, mas os argumentos nao sao data ou text
            printf("\nErro sintatico: Argumentos da diretiva section invalidos na linha %d (diretiva section aceita somente text ou data como argumentos)\n",contador_linha);
            total_erros++;
    }
}
short int diretiva_space_primeira_passagem(char* tok, short int numero){
    if(existe_token(tok)){
        numero= converte_em_num(tok,numero);
        if(numero>=0){
                Tab_Dir=insereDiretiva(Tab_Dir,contador_posicao,'E',numero);
                contador_posicao+=numero;
        }else{
                printf("\nErro sintatico: Total de espacos a alocar menor que 0 ou erro na constante numerica na linha %d\n",contador_linha);
                total_erros++;
        }
    }else{
            numero=1;
            Tab_Dir=insereDiretiva(Tab_Dir,contador_posicao,'E',1);
            contador_posicao++;
    }
    return numero;
}
short int diretiva_const_primeira_passagem(char* tok, short int numero){
    if(existe_token(tok)){
            numero = converte_em_num(tok,numero);
            Tab_Dir=insereDiretiva(Tab_Dir,contador_posicao,'C',numero);
            contador_posicao++;
    }else{
            numero=-1;
            printf("\nErro sintatico: Diretiva const sem constante numerica na linha %d",contador_linha);
            total_erros++;
    }
    return numero;
}
