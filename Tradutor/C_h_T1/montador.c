#include"bibliotecas_montador.h"
 // printf("\nEnderecoo dados: %d\n",endereco_dados);
    //\u00E3 eh ã para unicode. 198 eh ã em ASCII
void inicializa_variaveis();
int main(int argc, char *argv[]){
    FILE *arq;
    analisa_args_linha_comando(argc,argv);
    if(nome_asm!=NULL){
        pre_processa(nome_asm);
    }
    if(strcmp(argv[1],"-o")==0){
        arq = abre_arq_pre();
        if(existe_arquivo(arq)){
            inicializa_variaveis();
            passagem++;
            verifica_linhas(arq);
            rewind(arq);
            transfere_da_TS_para_TabelaDefinicoes(TS,TD);
            passagem++;
            cria_arq_obj();
            verifica_linhas(arq);
            fclose(arq);
            if(!arg_contem_extensao(argv[2],".pre"))
                remove(nome_pre);
            if(qte_text==0){
                printf("\nErro semantico: Falta section text no codigo\n");
                total_erros++;
            }else if(qte_text>1){
                printf("\nErro semantico: Mais de uma section text no codigo\n");
                total_erros++;
            }
            if(qte_data>1){
                printf("\nErro semantico: Mais de uma section data no codigo\n");
                total_erros++;
            }
            if(tem_begin){
                if(!fechou_begin_end){
                    printf("\nErro sintatico: Arquivo eh modulo, mas nao fechou o begin..end\n");
                    total_erros++;
                }
            }else if(!tem_stop){
                printf("\nErro sintatico: Arquivo nao eh modulo, mas nao tem stop\n");
                total_erros++;
            }
            if(total_erros>0){
                exclui_arq_obj();
            }else{
                if(tem_begin)
                    escreve_tabelas();
                fecha_arq_obj();
            }
            printf("\nTotal de erros: %d\n",total_erros);
            libera_tabela_definicoes(TD);
            libera_tabela_uso(TU);
            libera_tabela_instrucoes(Instrucoes);
            libera_tabela_simbolos(TS);
            libera_tabela_diretivas(Tab_Dir);
            free(indice_realocacao);
            libera_nome_arquivos();
        }else{
            printf("Arquivo %s n\u00E3o encontrado!",nome_pre);
        }

    }
    return 0;
}
void inicializa_variaveis(){
    indice_realocacao = (short int*)malloc(sizeof(short int));
    indice_realocacao[0] = -1; //flag inicial para indicar que nao ha nada no inicio
    tamanho_realoc=1;
    TS =NULL;
    TD=NULL;
    TU=NULL;
    endereco_dados =-1;//indica onde comeca a secao de dados, se ela existe. Se nao existir, variavel tem valor -1
    fechou_begin_end = TRUE;
    tem_stop = FALSE;
    tem_begin = FALSE;
    total_erros = 0;
    qte_text=0;
    qte_data=0;
    Instrucoes = inicializa_instrucoes();
    Tab_Dir = NULL;
    passagem = 0;
}
