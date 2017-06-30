#include"bibliotecas_tradutor.h"
char **argumentos_cmd;
char *nome_asm, *nome_obj;
FILE* obj;
void analisa_args_linha_comando(int n_arg,char **args ){
    if(n_arg==2){// Nome programa + arquivo de entrada
        if(arg_contem_extensao(args[1],".asm") ){
            nome_asm = args[1];
            nome_obj = (char*)calloc(tam_string(args[1])+1,sizeof(char));
            strcpy(nome_obj,nome_asm);
            retira_extensao(nome_obj,".asm");
            strcat(nome_obj,".s");
        }else{
            printf("Arquivo de entrada tem que ter a extensao .asm .\nPor favor, tente novamente.");
            exit(1);
        }
    }else if(n_arg==3){// Nome programa + arquivo de entrada + arquivo de saida
        argumentos_cmd = args;
        if(arg_contem_extensao(args[1],".asm") ){
            salva_nomes_entrada_saida(&nome_asm,&nome_obj,".s");
        }else{
            printf("Arquivo de entrada tem que ter a extensao .asm .\nPor favor, tente novamente.");
            exit(1);
        }
    }else{
        printf("\nNumero de argumentos diferente do esperado.\nPor favor, tente novamente inserindo nome do programa + 3 argumentos\n");
        exit(1);
    }
}
void retira_extensao(char*nome, char* extensao){ // serve para seprar o nome do arquivo asm da sua extensao, possibilitando nomear a saida de acordo com  a entrada
    char *pos_ponto = strstr (nome,".");
    while(*pos_ponto != '\0'){
        *pos_ponto = '\0';
        pos_ponto = pos_ponto+1;
    }
}
int arg_contem_extensao(char* arg,char* extensao){
    return (strstr(arg,extensao)!=NULL);
}
int arg_saida_sem_extensao(char *arg){
    return (strrchr (arg,'.')==NULL );
}
void salva_nomes_entrada_saida(char** entrada,char** saida,char* ext_saida){
    *entrada= argumentos_cmd[2];
    if(arg_saida_sem_extensao(argumentos_cmd[2])){
        *saida = (char*)calloc( (tam_string(argumentos_cmd[2]) + tam_string(ext_saida) + 1),sizeof(char));
        strcpy(*saida,argumentos_cmd[2]);
        strcat(*saida,ext_saida);
    }else{
        printf("\nArquivo de saida nao pode ter extensao\n Por favor, tente novamente com um arquivo de saida sem extensao\n");
        exit(-5);
    }
}

int existe_arquivo(FILE* arq){
    return arq!=NULL;
}
FILE* abre_arq_entrada(){// abre para o modo leitura
    return fopen(nome_asm,"r");
}
void cria_arq_obj(){
    obj = fopen(nome_obj,"w+");
}
void escreve_rotulo(char *token){
    fprintf(obj,"%s ",token);
}
void escreve_if(char *tok){
    fprintf(obj,"%%ifn %s=0\n",prox_token());
}
void escreve_fecha_if(){
    fprintf(obj,"%%endif\n");
}

// funcao escreve_op_aritmetica traduz as funcoes de assembly inventado que sao aritmeticas para IA-32
void escreve_op_aritmetica(char*tok){
    if(strcmp(tok,"div")!=strings_iguais && strcmp(tok,"mult")!=strings_iguais)// mul e div podem ser feitos somente como imul m32 e idiv m32
        fprintf(obj,"%s eax, dword[%s]\n",tok,prox_token());
    else if(strcmp(tok,"div")!=strings_iguais)// para divisao, eh necessario fazer a extensao de sinal para o edx
        fprintf(obj,"imul dword[%s]\n",prox_token());
    else{
        fprintf(obj,"cdq\n");
        fprintf(obj,"idiv dword[%s]\n",prox_token());
    }
}
void escreve_pulo_incondicional(char* token){
    fprintf(obj,"jmp %s\n",token);
}
void escrve_cmp(){
    fprintf(obj,"cmp eax,0\n");
}
void escreve_pulo_p(char* token){
    fprintf(obj,"jg %s\n",token);
}
void escreve_pulo_n(char* token){
    fprintf(obj,"jl %s\n",token);
}
void escreve_pulo_z(char* token){
    fprintf(obj,"je %s\n",token);
}
void escreve_memoria(char* src,char* dest){
    if( strcmp(src,"eax") == strings_iguais ){
        fprintf(obj,"mov dword[%s], eax\n",dest);
    }else if(strcmp(dest,"eax") == strings_iguais ){
        fprintf(obj,"mov eax, dword[%s]\n",src);
    }else{// operacao memoria memoria
        fprintf(obj,"push eax\n");
        fprintf(obj,"mov eax, dword[%s]\n",src);
        fprintf(obj,"mov dword[%s], eax\n",dest);
        fprintf(obj,"pop eax\n");
    }
}
void escreve_stop(){
    fprintf(obj,"mov eax, 1\n");
    fprintf(obj,"mov ebx, 0\n");
    fprintf(obj,"int 0x80\n");
}

void escreve_data(char* linha_lida){
    fprintf(obj,"section .data\n%s\n",linha_lida);
}

void escreve_bss(char* linha_lida){
    fprintf(obj,"section .bss\n%s\n",linha_lida);
}

void escreve(char* linha_lida){
    fprintf(obj,"%s\n",linha_lida);
}
void escreve_inicio(){
    fprintf(obj,"global _start\nsection .text\n_start:\n");
}
void escreve_input(char* arg1,char* arg2,char tipo){
    if (tipo == 's'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,%s\n",arg1);
        fprintf(obj,"mov edx,%s\n",arg2);
        fprintf(obj,"call LerString\n");
        fprintf(obj,"popa\n");
    }
    if (tipo == 'd'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,string\n");
        fprintf(obj,"mov edx,12\n");
        fprintf(obj,"call LerInteiro\n");
        fprintf(obj,"mov dword [%s],edx\n",arg1);
        fprintf(obj,"popa\n");
    }
    if (tipo == 'h'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,string\n");
        fprintf(obj,"mov edx,9\n");
        fprintf(obj,"call LerHexa\n");
        fprintf(obj,"mov dword [%s],edx\n",arg1);
        fprintf(obj,"popa\n");
    }
    if (tipo == 'c'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,%s\n",arg1);
        fprintf(obj,"call LerChar\n");
        fprintf(obj,"popa\n");
    }
}
void escreve_output(char* arg1,char* arg2,char tipo){
    if (tipo == 's'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,%s\n",arg1);
        fprintf(obj,"mov edx,%s\n",arg2);
        fprintf(obj,"call EscreverString\n");
        fprintf(obj,"popa\n");
    }
    if (tipo == 'd'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,[%s]\n",arg1);
        fprintf(obj,"call EscreverInteiro\n");
        fprintf(obj,"popa\n");
    }
    if (tipo == 'h'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,[%s]\n",arg1);
        fprintf(obj,"call EscreverHexa\n");
        fprintf(obj,"popa\n");
    }
    if (tipo == 'c'){
        fprintf(obj,"pusha\n");
        fprintf(obj,"mov ecx,%s\n",arg1);
        fprintf(obj,"call EscreverChar\n");
        fprintf(obj,"popa\n");
    }
}

void imprime_funcoes(){
    fprintf(obj, "\n\n\n;Funcoes implementadas:\n");
    fprintf(obj, "section .data\n");
    fprintf(obj, "msg     db      'Digitos invalidos', 0AH\n");
    fprintf(obj, "len     equ     $-msg\n");
    fprintf(obj, "resultado dd 0\n");
    fprintf(obj, "flag db 0\n");
    fprintf(obj, "quociente db '0000'\n");
    fprintf(obj, "string db '000000000000'\n");
    fprintf(obj, "  \n");
    fprintf(obj, "LerChar:\n");
    fprintf(obj, "  mov eax, 3 ; ler\n");
    fprintf(obj, "  mov ebx, 1 ; teclado\n");
    fprintf(obj, "  mov edx, 1\n");
    fprintf(obj, "  int 0x80   ; ler(teclado, digitado, 1byte);\n");
    fprintf(obj, "\n");
    fprintf(obj, "  mov eax,ecx\n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "EscreverChar:\n");
    fprintf(obj, "  mov eax, 4 ; escrever\n");
    fprintf(obj, "  mov ebx, 1 ; monitor\n");
    fprintf(obj, "  mov edx, 1\n");
    fprintf(obj, "  int 0x80   ; escrever(monitor, digitado, 1byte);\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "  \n");
    fprintf(obj, "EscreverHexa:\n");
    fprintf(obj, "  push byte 0\n");
    fprintf(obj, "  push byte 0ah\n");
    fprintf(obj, "  inicio1:\n");
    fprintf(obj, "  mov eax, ecx\n");
    fprintf(obj, "  shr eax,4\n");
    fprintf(obj, "  mov dword edx, 0x0000000F\n");
    fprintf(obj, "  and edx, ecx    \n");
    fprintf(obj, "  \n");
    fprintf(obj, "  cmp edx,09h; verifica qual é o digito\n");
    fprintf(obj, "  jbe numero0a9\n");
    fprintf(obj, "  add edx, 0x37;converte o resto em ascii\n");
    fprintf(obj, "  jmp continua\n");
    fprintf(obj, "  \n");
    fprintf(obj, "numero0a9:\n");
    fprintf(obj, "  add edx, 0x30 ;converte o resto em ascii\n");
    fprintf(obj, "  \n");
    fprintf(obj, "continua:\n");
    fprintf(obj, "  push edx ; empilha o resultado\n");
    fprintf(obj, "  mov dword  ecx,eax ;coloca o quociente de volta em ecx\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  cmp ecx,0 ;ve se ja converteu o numero todo\n");
    fprintf(obj, "  je fim1\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  jmp inicio1\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  fim1:\n");
    fprintf(obj, "  pop eax ;desempilha o numero\n");
    fprintf(obj, "  cmp eax,0 ; ve se ainda não acabou\n");
    fprintf(obj, "  je retorno ; se ja, acaba\n");
    fprintf(obj, "  mov dword [resultado], eax ;se não, coloca pra ser impresso\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  mov eax, 4 ; escrever\n");
    fprintf(obj, "  mov ebx, 1 ; monitor\n");
    fprintf(obj, "  mov ecx, resultado\n");
    fprintf(obj, "  mov edx, 1\n");
    fprintf(obj, "  int 0x80   ; escrever(monitor, digitado, 1byte);\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  jmp fim1\n");
    fprintf(obj, "\n");
    fprintf(obj, "  retorno:\n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "\n");
    fprintf(obj, "LerHexa:\n");
    fprintf(obj, "  call LerString\n");
    fprintf(obj, "  mov ecx, string ;aponta para o vetor digitado\n");
    fprintf(obj, "  mov dword [resultado],0\n");
    fprintf(obj, "  mov edx, resultado ; resultado\n");
    fprintf(obj, "inicio2:\n");
    fprintf(obj, "  mov eax,0\n");
    fprintf(obj, "  mov al, [ecx] ;pega um digito\n");
    fprintf(obj, "  cmp al, 0ah ;se for \n\n");
    fprintf(obj, "  je fim2 ; vai pro fim\n");
    fprintf(obj, "  ;se não, verifica se eh um digito valido\n");
    fprintf(obj, "  cmp al, 2fh\n");
    fprintf(obj, "  jbe erro_fim\n");
    fprintf(obj, "  cmp al,3ah; verifica qual é o digito\n");
    fprintf(obj, "  jbe numero0a92\n");
    fprintf(obj, "  cmp al,40h\n");
    fprintf(obj, "  jbe erro_fim\n");
    fprintf(obj, "  cmp al, 46h\n");
    fprintf(obj, "  jbe eh_letra\n");
    fprintf(obj, "  jmp erro_fim\n");
    fprintf(obj, "eh_letra:\n");
    fprintf(obj, "  sub al, 0x37;se for letra, converte em int\n");
    fprintf(obj, "  jmp continua2\n");
    fprintf(obj, "numero0a92:\n");
    fprintf(obj, "  sub al, 0x30 ; se for numero, converte em int\n");
    fprintf(obj, "continua2:\n");
    fprintf(obj, "  mov ebx, eax ; salva em ebx\n");
    fprintf(obj, "  ;multiplica o resultado por 16\n");
    fprintf(obj, "  shl dword [edx], 4  \n");
    fprintf(obj, "  add dword[edx], ebx ;soma o resultado parcial com o digito\n");
    fprintf(obj, "\n");
    fprintf(obj, "  inc ecx ;muda o poneiro pro porximo digito\n");
    fprintf(obj, "  jmp inicio2\n");
    fprintf(obj, "\n");
    fprintf(obj, "fim2:\n");
    fprintf(obj, "  mov edx, [edx] ;retorna o resultado em eax\n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "\n");
    fprintf(obj, "EscreverInteiro:\n");
    fprintf(obj, "  mov dword [flag],0\n");
    fprintf(obj, "  push byte 0 ;empilha 0 para indicar o final da string\n");
    fprintf(obj, "  push byte 0ah\n");
    fprintf(obj, "  cmp ecx, 0 ;ve se o numero é maior que 0\n");
    fprintf(obj, "  jge inicio3 ; se for, nao faz nada\n");
    fprintf(obj, "  ;se nao for, empilha o '-' e converte pra positivo\n");
    fprintf(obj, "  mov dword [flag],1\n");
    fprintf(obj, "  neg ecx\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  inicio3:\n");
    fprintf(obj, "  ;coloca a parte mais siginificativa do numero em dx\n");
    fprintf(obj, "  mov dword edx, 0x00000000\n");
    fprintf(obj, "  and edx, ecx\n");
    fprintf(obj, "  rol edx, 16\n");
    fprintf(obj, "  ;coloca a parte menos significativa em ax\n");
    fprintf(obj, "  mov dword eax, ecx\n");
    fprintf(obj, "  ;divide o numero por 10 \n");
    fprintf(obj, "  mov dword ebx, 10\n");
    fprintf(obj, "  div ebx\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  add edx, 0x30 ;converte o resto em ascii\n");
    fprintf(obj, "  push edx ; empilha o resultado\n");
    fprintf(obj, "  mov dword  ecx,eax ;coloca o quociente de volta em ecx\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  cmp ecx,0 ;ve se ja converteu o numero todo\n");
    fprintf(obj, "  je fim3\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  jmp inicio3\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  \n");
    fprintf(obj, "  \n");
    fprintf(obj, "fim3:\n");
    fprintf(obj, "  cmp dword [flag],0\n");
    fprintf(obj, "  je impressao\n");
    fprintf(obj, "  push byte 45\n");
    fprintf(obj, "  \n");
    fprintf(obj, "impressao:\n");
    fprintf(obj, "  pop eax ;desempilha o numero\n");
    fprintf(obj, "  cmp eax,0 ; ve se ainda não acabou\n");
    fprintf(obj, "  je retorno2 ; se ja, acaba\n");
    fprintf(obj, "  mov dword [resultado], eax ;se não, coloca pra ser impresso\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  mov eax, 4 ; escrever\n");
    fprintf(obj, "  mov ebx, 1 ; monitor\n");
    fprintf(obj, "  mov ecx, resultado\n");
    fprintf(obj, "  mov edx, 1\n");
    fprintf(obj, "  int 0x80   ; escrever(monitor, digitado, 1byte);\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  jmp impressao\n");
    fprintf(obj, "\n");
    fprintf(obj, "  retorno2:\n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "\n");
    fprintf(obj, "\n");
    fprintf(obj, "\n");
    fprintf(obj, "LerInteiro:\n");
    fprintf(obj, "  call LerString\n");
    fprintf(obj, "  mov ecx, string ;aponta para o vetor digitado\n");
    fprintf(obj, "  ;inicia corretamente as variaveis\n");
    fprintf(obj, "  mov dword [flag], 0\n");
    fprintf(obj, "  mov dword [resultado],0\n");
    fprintf(obj, "  mov edx, resultado ; resultado\n");
    fprintf(obj, "  mov eax,0\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  mov al, [ecx] ;pega o primeiro digito\n");
    fprintf(obj, "  cmp al, 45 ;se for '-'\n");
    fprintf(obj, "  jnz inicio4\n");
    fprintf(obj, "  mov dword [flag], 1\n");
    fprintf(obj, "  inc ecx\n");
    fprintf(obj, "inicio4:\n");
    fprintf(obj, "  mov eax,0\n");
    fprintf(obj, "  mov al, [ecx] ;pega um digito\n");
    fprintf(obj, "  cmp al, 0ah ;se for \n\n");
    fprintf(obj, "  je fim4 ; vai pro fim\n");
    fprintf(obj, "  sub al, 0x30 ; se não, converte em int\n");
    fprintf(obj, "  ;rotina que define se o digito é valido ou não\n");
    fprintf(obj, "  cmp al,0\n");
    fprintf(obj, "  jl erro_fim\n");
    fprintf(obj, "  cmp al,10\n");
    fprintf(obj, "  jg erro_fim\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  mov ebx, eax ; salva em ebx\n");
    fprintf(obj, "  ;multiplica o resultado por 10\n");
    fprintf(obj, "  mov dword eax, [edx]\n");
    fprintf(obj, "  shl dword eax,1\n");
    fprintf(obj, "  shl dword [edx], 3\n");
    fprintf(obj, "  add dword [edx], eax\n");
    fprintf(obj, "  adc dword[edx], ebx ;soma o resultado parcial com o digito\n");
    fprintf(obj, "\n");
    fprintf(obj, "  inc ecx ;muda o poneiro pro porximo digito\n");
    fprintf(obj, "  jmp inicio4\n");
    fprintf(obj, "\n");
    fprintf(obj, "fim4:\n");
    fprintf(obj, "  mov eax, [flag]\n");
    fprintf(obj, "  cmp eax, 0\n");
    fprintf(obj, "  je retorna\n");
    fprintf(obj, "  neg dword [edx]\n");
    fprintf(obj, "  \n");
    fprintf(obj, "retorna:\n");
    fprintf(obj, "  mov edx, [edx] ;retorna o resultado em eax\n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "  \n");
    fprintf(obj, "LerString:\n");
    fprintf(obj, "  mov eax, 3 ; ler\n");
    fprintf(obj, "  mov ebx, 1 ; teclado\n");
    fprintf(obj, "  int 0x80   ; ler(teclado, string, size bytes);\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "  \n");
    fprintf(obj, "EscreverString:\n");
    fprintf(obj, "  mov eax, 4 ; escrever\n");
    fprintf(obj, "  mov ebx, 1 ; monitor\n");
    fprintf(obj, "  int 0x80   ; escrever(monitor, string, size bytes);\n");
    fprintf(obj, "  \n");
    fprintf(obj, "  ret\n");
    fprintf(obj, "  \n");
    fprintf(obj, "erro_fim:\n");
    fprintf(obj, "  mov     edx, len\n");
    fprintf(obj, "        mov     ecx, msg\n");
    fprintf(obj, "        mov     ebx, 1\n");
    fprintf(obj, "        mov     eax, 4\n");
    fprintf(obj, "        int     80h\n");
    fprintf(obj, " \n");
    fprintf(obj, "        mov     ebx, 0\n");
    fprintf(obj, "        mov     eax, 1\n");
    fprintf(obj, "        int     80h\n");
}

void exclui_arq_obj(){
    if(obj!=NULL){
        remove(nome_obj);
    }
}
void libera_nome_arquivos(){
    free(nome_obj);
}
void fecha_arq_obj(){
    if(obj!=NULL)
        fclose(obj);
}
