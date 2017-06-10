#include"bibliotecas_tradutor.h"
//Uma das consideracoes para o trabalho eh que todas as isntrucoes do assembly
// inventado vao estar corretas
void traduz_instrucao(char*);
int main(int argc, char** argv){
   analisa_args_linha_comando(argc,argv);
   FILE* arq_asm = fopen(argv[1],"r");
   char *linha_lida,*tok;
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
        printf("Arquivo %s nao encontrado. Por favor tente novamente.",argv[1]);

   }


}
void traduz_instrucao(char* tok){
    //Observacao: se for trabalhar com o arquivo asm em caixa baixa,
    //colcoar o nome das instrcoes em caixa baixa
    if(strcmp(tok,"ADD") == strings_iguais){
        escreve_relacao_direta(tok);
    }else if(strcmp(tok,"SUB") == strings_iguais){
        escreve_relacao_direta(tok);
    }
}

