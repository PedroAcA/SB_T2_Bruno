//arquivo criado para processar algumas etapas da segunda passagem
#include"bibliotecas_montador.h"
#include <errno.h>//usada para identificar erros de conversao erros ao usar a funcao strtol
#include <limits.h>//usada para identificar erros de conversao erros ao usar a funcao strtol
// assembly inventado so aceita label+nuemro e nao label-numero
// a funcao converte_em_enderecos retira a virgula se for encontado a funcao
// copy e susbtui ela pro espaco. Espera que o ultimo token seja o primeiro argumento.
char ha_externo[2];
int indice_externo;
void converte_em_enderecos(char *tok,TabelaDeInstrucoes* instrucao_atual,short int* indice_vetor,short int*args){
    int tam=0;
    indice_externo=0;
    ha_externo[0] = 'F';
    ha_externo[1] = 'F';
    if(strcmp(instrucao_atual->mnemonico,"copy")==0){
        if(existe_token(tok))
            processa_argumentos_copy(tok,indice_vetor,args);
        else{
            printf("\nErro sintatico: Instrucao copy sem argumentos na linha %d\n",contador_linha);
            args[0] = -1;
            args[0] = -1;
            indice_vetor[0] = 0;
            indice_vetor[1] = 0;
            total_erros++;
        }
    }else if(existe_token(tok)){
        indice_realocacao[tamanho_realoc-1] = contador_posicao-1;
        tamanho_realoc++;
        indice_realocacao = (short int*) realloc(indice_realocacao,tamanho_realoc*sizeof(short int));
        tam++;
        args[0] = extrai_endereco(tok,indice_vetor);
        if(ha_externo[0]=='T')
            Insere_pos_uso(TU,(contador_posicao-1) );
    }else if(strcmp(instrucao_atual->mnemonico,"stop")==0){//nao ha argumentos e a instrucao eh stop
          args[0] = -2;// entao colocamos -2 na posicao 0 de args para indicar stop com argumentos corretos
          indice_vetor[0] = 0;
    }else{
        printf("\nErro Sintatico: Instrucao %s  sem argumentos na linha %d\n",instrucao_atual->mnemonico,contador_linha);
        total_erros++;
    }
}
int extrai_endereco(char* tok,short int* indice_vetor){// vai pegar uma label ou label+ numero e retornar o endereco resultante
    TabelaDeSimbolos* buscador;
    int endereco;
    if(tem_aritmetica(tok)){
        endereco = converte_exp_aritmetica(tok,indice_vetor,(ha_externo+indice_externo));
    }else{
        buscador = busca_simbolo(TS,tok);
        if(buscador!=NULL){
            variavel_bem_formada(tok);
            endereco = buscador->valor;
            if(buscador->externo == 's'){
                ha_externo[indice_externo]='T';
                TU = Insere_var_externa(TU,buscador->nome);
            }
        }else{
            endereco = -1;
            printf("\nErro semantico: Simbolo %s nao definido na linha %d\n",tok,contador_linha);
            total_erros++;
        }
        *indice_vetor = 0;
    }
    return endereco;
}

void processa_argumentos_copy(char* tok,short int* indice_vetor,short int* args){
    short int tam=0;
    char *copia_tok,*end_tok,*inicio_desaloc;
    copia_tok = (char*) malloc((tam_string(tok)+1)*sizeof(char));
    strcpy(copia_tok,tok);
    inicio_desaloc =  copia_tok;
    if(strchr(copia_tok,',')!=NULL){
        copia_tok = elimina_caracter(copia_tok,",");// testar para ve se strok funciona apos essa sepacacao
        copia_tok = strtok_r(copia_tok," ",&end_tok);
        while(existe_token(copia_tok) && (strchr(copia_tok,',')==NULL) ){// pra copy ser valido so pode ter uma virgula no espaco dos argumentos
            tam++;
            // para o primeiro argumento, o contador de posicao eh contador_posicao-2. Como tam vai de 1 a 2, a relacao contador_posicao-2+(tam-1) da a posicao do codigo objeto de cada argumento de copy
            indice_realocacao[tamanho_realoc-1] = contador_posicao-2+(tam-1);
            tamanho_realoc++;
            indice_realocacao = (short int*) realloc(indice_realocacao,tamanho_realoc*sizeof(short int));
            indice_externo = tam-1;
            args[tam-1]=extrai_endereco(copia_tok,(indice_vetor+tam-1));
            if(ha_externo[(tam-1)]=='T')
                Insere_pos_uso(TU,(contador_posicao-2+(tam-1)) );
            copia_tok = strtok_r(NULL," ",&end_tok);
        }

        while(existe_token(copia_tok) && strchr(copia_tok,',')!=NULL){
            copia_tok = elimina_caracter(copia_tok,",");
            tam+=2;// += a 2 eh pq se ele entrara neste laco, significa q ele avaliou somente o primeiro argumento. Se entrar neste laco, tem mais argumentos q o que a copy requer (numero de argumentos a a mais n eh inportante)
        }
        erros_num_args_copy(tam);
    }else{
        printf("\nErro sintatico: Formato dos argumentos de copy incorreto na linha %d\n",contador_linha);
        total_erros++;
    }
    free(inicio_desaloc);
}
void erros_num_args_copy(short int n_args){
    if(n_args!=2){
        printf("\nErro sintatico: Numero incorreto de argumentos do tipo (A,B) para copy na linha %d\n",contador_linha);
        total_erros++;
    }
}
int enderecos_sem_erros(char *nome_instr,short int* enderecos,short int*indices){
    short int end_base[2],i;
    TabelaDeDiretivas* buscador;
    for(i=0;i<2;i++){
        end_base[i] = enderecos[i] - indices[i];
    }
     buscador =  busca_end_incial(Tab_Dir,end_base[0]);
    if(strcmp(nome_instr,"copy")==0 && buscador!=NULL){
        return argumentos_copy_corretos(buscador,indices,end_base);
    }else if(eh_pulo(nome_instr)){
        return pulo_valido(enderecos[0]);
    }else if (strcmp(nome_instr,"stop")==0){
        tem_stop=TRUE;
        return endereco_stop_correto(enderecos[0]);
    }else{
        if(buscador!=NULL){
            if(acessa_memoria(nome_instr)){// todas as instrucoes que acessam a memoria, a execessao do copy
                return endereco_acesso_memoria_valido(buscador,indices,0);
            }else if(strcmp(nome_instr,"div")==0){//n se pode dividir por 0
                return (!divide_zero(buscador));
            }else{
                return endereco_alocado(buscador,indices,0);
            }
        }else if(busca_simbolo_por_posicao_memoria(TS,end_base[0])!=NULL){ //o rotulo pode ter sido definido, mas nao como espaco ou constante
            printf("\nErro semantico: Endereco do rotulo nao reservado como constante ou espaco na linha %d\n",contador_linha);
            total_erros++;
        }
    }
    return FALSE;
}
int eh_constante(TabelaDeDiretivas* diretiva_atual){
    return (diretiva_atual->tipo == 'C');
}
int memoria_alocada(TabelaDeDiretivas* diretiva_atual,int indice){
    if(diretiva_atual->tipo=='C')// se foi declarado como constante, endereco esta alocado
        return (indice==0);
    else
        return indice < diretiva_atual->valor;
}
int eh_pulo(char *instrucao){
    return (strcmp(instrucao,"jmp")==0 ||strcmp(instrucao,"jmpn")==0
            || strcmp(instrucao,"jmpp")==0 || strcmp(instrucao,"jmpz")==0);
}
int acessa_memoria(char *instrucao){
    return (strcmp(instrucao,"store")==0 ||strcmp(instrucao,"input")==0);
}
int endereco_alocado(TabelaDeDiretivas* buscador,short int* indices,int ind_arg){
    if(memoria_alocada(buscador,indices[ind_arg])){
            return TRUE;
    }else{
            printf("Erro semantico: Endereco nao alocado para vetor na linha %d\n",contador_linha);
            total_erros++;
    }
    return FALSE;
}
int endereco_acesso_memoria_valido(TabelaDeDiretivas* buscador,short int* indices,int ind_arg){
    if(!eh_constante(buscador)){
        return endereco_alocado(buscador,indices,ind_arg);
    }else{
        printf("Erro semantico: Tentiva de modificar um valor constante na linha %d\n",contador_linha);
        total_erros++;
    }
    return FALSE;
}

int argumentos_copy_corretos(TabelaDeDiretivas* buscador, short int* indices, short int* end_base){
    if(endereco_alocado(buscador,indices,0)){
        buscador = busca_end_incial(Tab_Dir,end_base[1]);
        if(buscador!=NULL)// existe o segundo argumento
            return endereco_acesso_memoria_valido(buscador,indices,1);
        else
            return FALSE;
    }else{
        return FALSE;
    }
}
int endereco_stop_correto(int endereco){
    if(endereco==-2){//flag para indicar que nao foi dado argumento para stop
        return TRUE;
    }else{
        printf("Erro sintatico: Instrucao stop com argumentos na linha %d\n",contador_linha);
        total_erros++;
        return FALSE;
    }
}
int divide_zero(TabelaDeDiretivas* buscador){
    if(buscador->tipo=='C'){
        if(buscador->valor==0){
            printf("\nErro semantico: Tentaiva de divisao por 0 na linha %d\n",contador_linha);
            total_erros++;
            return TRUE;
        }
    }
    return FALSE;
}
int pulo_valido(int endereco_total){
    if(endereco_dados!=-1){
        if(endereco_total >= endereco_dados){
            printf("\nErro semantico: Pulo da linha %d esta acessando area de dados\n",contador_linha);
            total_erros++;
            return FALSE;
        }
        return (endereco_total < endereco_dados);
    }else{
        return TRUE;// se nao ha secao de dados, entao todo codigo eh secao texto
    }
}
int existe_argumento_externo(){
    return ( (ha_externo[0] == 'T') || (ha_externo[1] == 'T') );
}
int endereco_nao_externo_sem_erro(char* nome_instrucao,short int *enderecos,short int* indices){
    TabelaDeDiretivas* buscador;
    if(strcmp(nome_instrucao,"copy")==0 ){
        if(ha_externo[0]=='T' && ha_externo[1]=='F'){// segundo argumento nao eh externo
            buscador = busca_end_incial(Tab_Dir,(enderecos[1] - indices[1]) );
            return (buscador!=NULL && endereco_acesso_memoria_valido(buscador,indices,1) );
        }else if(ha_externo[0]=='F' && ha_externo[1]=='T'){// primeiro argumento eh externo
            buscador = busca_end_incial(Tab_Dir,(enderecos[0] - indices[0]) );
            return (buscador!=NULL && endereco_alocado(buscador,indices,0) );
        }
    }else if(strcmp(nome_instrucao,"stop")==0 ){
        return endereco_stop_correto(enderecos[0]);
    }
    return TRUE; // todas as outras funcoes so tem um argumento. Como ele eh externo, entao ligador eh quem avalia eles
}
