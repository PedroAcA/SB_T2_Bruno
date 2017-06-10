#include"bibliotecas_montador.h"
void * CriaTabela(){
    return NULL;
}
//a funcao InsereSimbolo insere simbolos nao Externos.
// Para inserir simbolos externos a funcao InsereSimbolo_Externo foi criada
TabelaDeSimbolos* InsereSimbolo (TabelaDeSimbolos* tabela, char nome[],short int valor){
    TabelaDeSimbolos * novo = (TabelaDeSimbolos*) calloc(1,sizeof(TabelaDeSimbolos));
    strcpy(novo->nome, nome);
    novo->valor = valor;
    novo->prox = tabela;
    novo->externo = 'n';
    tabela = novo;
    return novo;
}
/*Como simbolo externo sempre deve vir apos rotulo, entao se ha extern
o inicio da tabela de simbolos eh o rotulo atual a ser inserido como extern*/
void InsereSimbolo_Externo (TabelaDeSimbolos* tabela){
    tabela->valor = 0;
    tabela->externo = 's';
}
TabelaDeSimbolos* busca_simbolo(TabelaDeSimbolos* tabela,char* simb){
    while(tabela!=NULL){
        if(strcmp(tabela->nome,simb)==0){//achou simbolo na tabela de simbolos
            return tabela;
        }
        tabela = tabela->prox;
    }
    return NULL;
}
TabelaDeSimbolos* busca_simbolo_por_posicao_memoria(TabelaDeSimbolos* tabela, short int endereco){
    while(tabela!=NULL){
        if(tabela->valor == endereco)
            return tabela;

        tabela = tabela->prox;
    }
    return NULL;
}
TabelaDeInstrucoes* inicializa_instrucoes(){
    TabelaDeInstrucoes* Tabela=InsereInstrucao(NULL,"add",1,2);
    Tabela = InsereInstrucao(Tabela,"sub",2,2);
    Tabela = InsereInstrucao(Tabela,"mult",3,2);
    Tabela = InsereInstrucao(Tabela,"div",4,2);
    Tabela = InsereInstrucao(Tabela,"jmp",5,2);
    Tabela = InsereInstrucao(Tabela,"jmpn",6,2);
    Tabela = InsereInstrucao(Tabela,"jmpp",7,2);
    Tabela = InsereInstrucao(Tabela,"jmpz",8,2);
    Tabela = InsereInstrucao(Tabela,"copy",9,3);
    Tabela = InsereInstrucao(Tabela,"load",10,2);
    Tabela = InsereInstrucao(Tabela,"store",11,2);
    Tabela = InsereInstrucao(Tabela,"input",12,2);
    Tabela = InsereInstrucao(Tabela,"output",13,2);
    Tabela = InsereInstrucao(Tabela,"stop",14,1);
    return Tabela;
}
TabelaDeInstrucoes* InsereInstrucao(TabelaDeInstrucoes* tabela,char*nome,short int opcode,short int tamanho){
    TabelaDeInstrucoes* novo = (TabelaDeInstrucoes*) malloc(sizeof(TabelaDeInstrucoes));
    if(novo==NULL){
        printf("Sistema Operacional nao conseguiu reservar espaco de memoria\n");
        exit(-1);
    }else{
        strcpy(novo->mnemonico, nome);
        novo->opcode = opcode;
        novo->tamanho = tamanho;
        novo->prox = tabela;
    }

    return novo;
}
TabelaDeInstrucoes* busca_instrucao(TabelaDeInstrucoes* tabela_instrucoes,char* tok){
    while(tabela_instrucoes!=NULL){
        if(strcmp(tabela_instrucoes->mnemonico,tok)==0){
            return tabela_instrucoes;
        }
        tabela_instrucoes = tabela_instrucoes->prox;
    }
    return NULL;
}
/*A funcao busca_incrementa_posicao busca se a instrucao eh valida e, se for,
  atualiza o contador_posicao. Retorna TRUE(definido em pre_processador.h)
  se instrucao for valida ou FALSE (definido em pre_processador.h) caso
  contrario*/
TabelaDeInstrucoes* busca_incrementa_posicao(char*tok){
    // a funcao busca_instrucao esta definida em tabelas.c
    // a tabela de instrucoes esta declarada em bibliotecas_montador.h
    TabelaDeInstrucoes* buscador = busca_instrucao(Instrucoes,tok);// endereco de onde esta a instrucao na tabela de instrucoes (se esta existir)
    if(buscador!=NULL){
        contador_posicao+= buscador->tamanho;
    }
    return buscador;
}
TabelaDeDiretivas* insereDiretiva(TabelaDeDiretivas* Dir,int posicao_memoria,char tipo,int valor){
    TabelaDeDiretivas* novo = (TabelaDeDiretivas*)malloc(sizeof(TabelaDeDiretivas));
    if(novo==NULL){
        printf("Sistema Operacional nao conseguiu reservar espaco de memoria\n");
        exit(-1);
    }else{
        novo->posicao_memoria = posicao_memoria;
        novo->tipo = tipo;
        novo->valor= valor;
        novo->prox = Dir;
    }
    return novo;
}
TabelaDeDiretivas* busca_end_incial(TabelaDeDiretivas* Diretivas, int end_base){
    while(Diretivas!=NULL){
        if(Diretivas->posicao_memoria == end_base){
            return Diretivas;
        }
        Diretivas = Diretivas->prox;
    }
    return Diretivas;
}

//Insere somente o nome da variavel externa para depois a funcao
//Insere_pos_uso colocar o contador de posicao referente à variavel
TabelaDeUso* Insere_var_externa(TabelaDeUso* TabUso, char* nome){
    TabelaDeUso* novo = (TabelaDeUso*)malloc(sizeof(TabelaDeUso));
    if(novo==NULL){
        printf("Sistema Operacional nao conseguiu reservar espaco de memoria\n");
        exit(-1);
    }else{
        strcpy(novo->nome,nome);
        novo->prox = TabUso;
    }
    return novo;
}
// Funcao Insere_pos_uso espera ser chamada apos a insercao de um simbolo externo
// na tabela de uso.
void Insere_pos_uso(TabelaDeUso* TabUso,short int pos_uso){
    if(TabUso!=NULL)
        TabUso->valor = pos_uso;
}
TabelaDeDefinicoes* Insere_Simbolo_publico(TabelaDeDefinicoes* TabDef, char* nome){
    TabelaDeDefinicoes* novo = (TabelaDeDefinicoes*)malloc(sizeof(TabelaDeDefinicoes));
    if(novo==NULL){
        printf("Sistema Operacional nao conseguiu reservar espaco de memoria\n");
        exit(-1);
    }else{
        strcpy(novo->nome,nome);
        novo->prox = TabDef;
    }
    return novo;
}
void transfere_da_TS_para_TabelaDefinicoes(TabelaDeSimbolos* TabelaSimbolos, TabelaDeDefinicoes* TabelaDefinicoes){
    TabelaDeSimbolos *Simbolo_correspondente;
    while(TabelaDefinicoes!=NULL){
        Simbolo_correspondente = busca_simbolo(TS,TabelaDefinicoes->nome);
        if(Simbolo_correspondente!=NULL){
            TabelaDefinicoes->valor = Simbolo_correspondente->valor;
        }else{
            printf("\nSimbolo publico %s nao foi definido no codigo\n",TabelaDefinicoes->nome);
            total_erros++;
        }
        TabelaDefinicoes = TabelaDefinicoes->prox;
    }
}
void libera_tabela_definicoes(TabelaDeDefinicoes* def){
    TabelaDeDefinicoes* aux;
    while(def!=NULL){
        aux = def->prox;
        free(def);
        def = aux;
    }
}
void libera_tabela_uso(TabelaDeUso* u){
    TabelaDeUso* aux;
    while(u!=NULL){
        aux = u->prox;
        free(u);
        u = aux;
    }
}
void libera_tabela_instrucoes(TabelaDeInstrucoes* ins){
    TabelaDeInstrucoes* aux;
    while(ins!=NULL){
        aux = ins->prox;
        free(ins);
        ins = aux;
    }
}
void libera_tabela_simbolos(TabelaDeSimbolos* s){
    TabelaDeSimbolos* aux;
    while(s!=NULL){
        aux = s->prox;
        free(s);
        s = aux;
    }
}
void libera_tabela_diretivas(TabelaDeDiretivas* d){
    TabelaDeDiretivas* aux;
    while(d!=NULL){
        aux = d->prox;
        free(d);
        d = aux;
    }
}
