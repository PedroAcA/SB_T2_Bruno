#include"bibliotecas_tradutor.h"
//Uma das consideracoes para o trabalho eh que todas as isntrucoes do assembly
// inventado vao estar corretas
void traduz_instrucao(char*);
void pre_processa (FILE *);

int main(int argc, char** argv){
   analisa_args_linha_comando(argc,argv);
   FILE* arq_asm = fopen(argv[1],"r");
   char *linha_lida,*tok;
   if(existe_arquivo(arq_asm)){
   	pre_processa(arq_asm);
   }else{
        printf("Arquivo %s nao encontrado. Por favor tente novamente.",argv[1]);
   }
   fclose(arq_asm);
   arq_asm = fopen("temp1.pre","r");
   if(existe_arquivo(arq_asm)){
        cria_arq_obj();
        while(!feof(arq_asm)){
            linha_lida = proxima_linha(arq_asm);
            tok = divide_tokens(linha_lida);
            if(tok!=NULL)
                traduz_instrucao(tok);

            free(linha_lida);
        }
        fecha_arq_obj();
        libera_nome_arquivos();
        fclose(arq_asm);
   }else{
        printf("Falha no pre processamento. Por favor tente novamente.");
   }
}
void traduz_instrucao(char* tok){
    //Observacao: adicionar a traducao das funcoes de input e output
    if(eh_rotulo(tok)){
        //tok = elimina_caracter(tok,":");
        escreve_rotulo(tok);
        tok = prox_token();
    }
    if(eh_aritmetico(tok)){
        escreve_op_aritmetica(tok);
    }else if(classifica_pulo(tok)){
        if(tipo_pulo ==  JMP){
            escreve_pulo_incondicional(prox_token());
        }else{
            escrve_cmp();
            switch(tipo_pulo){
                case JMPP:
                    escreve_pulo_p(prox_token());
                    break;
                case JMPN:
                    escreve_pulo_n(prox_token());
                    break;
                case JMPZ:
                    escreve_pulo_z(prox_token());
            }
        }
    }else if(acessa_memoria(tok)){
        escreve_memoria(op1,op2);
    }else if ( strcmp(tok,"stop")==strings_iguais ){//stop
        escreve_stop();
    }
}



void pre_processa (FILE * arq){
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
