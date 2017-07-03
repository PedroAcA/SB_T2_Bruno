global _start
section .text
_start:
pusha
mov ecx,texto
mov edx,5
call LerString
popa
pusha
mov ecx,texto
mov edx,5
call EscreverString
popa
pusha
mov ecx,string_auxiliar_leitura
mov edx,12
call LerInteiro
mov dword [numero],edx
popa
pusha
mov ecx,[numero]
call EscreverInteiro
popa
pusha
mov ecx,char
call LerChar
popa
pusha
mov ecx,char
call EscreverChar
popa
pusha
mov ecx,string_auxiliar_leitura
mov edx,9
call LerHexa
mov dword [hexa],edx
popa
pusha
mov ecx,[hexa]
call EscreverHexa
popa
mov eax, 1
mov ebx, 0
int 0x80
section .bss
texto   resb 10
char resb 1
numero   resb 4
hexa   resb 4



;Funcoes implementadas:
section .data
mensagem_string_erro     db      'Digitos invalidos', 0AH
tamanho_string_erro     equ     $-mensagem_string_erro
resultado_final_auxiliar dd 0
demarcador_de_sinal db 0
resultado_final_auxiliar_divisao_parcial db '0000'
string_auxiliar_leitura db '000000000000'
ncaracteres dd 0  
  
section .text
  
LerChar:
  mov eax, 3 ; ler
  mov ebx, 1 ; teclado
  mov edx, 2
  int 0x80   ; ler(teclado, digitado, 1byte);

  ;chamada ao sistema retorna quantidade de caracteres em eax
  ret
EscreverChar:
  mov eax, 4 ; escrever
  mov ebx, 1 ; monitor
  mov edx, 1
  int 0x80   ; escrever(monitor, digitado, 1byte);
  
  ;chamada ao sistema retorna quantidade de caracteres em eax
  ret
  
EscreverHexa:
  mov dword [ncaracteres],0
  push byte 0
  push byte 0ah
  inicio1:
  mov eax, ecx
  shr eax,4
  mov dword edx, 0x0000000F
  and edx, ecx    
  
  cmp edx,09h; verifica qual é o digito
  jbe numero0a9
  add edx, 0x37;converte o resto em ascii
  jmp continua
  
numero0a9:
  add edx, 0x30 ;converte o resto em ascii
  
continua:
  push edx ; empilha o resultado_final_auxiliar
  mov dword  ecx,eax ;coloca o resultado_final_auxiliar_divisao_parcial de volta em ecx
  
  cmp ecx,0 ;ve se ja converteu o numero todo
  je fim1
  inc dword [ncaracteres]
  
  jmp inicio1
  
  fim1:
  pop eax ;desempilha o numero
  cmp eax,0 ; ve se ainda não acabou
  je retorno ; se ja, acaba
  mov dword [resultado_final_auxiliar], eax ;se não, coloca pra ser impresso
  
  mov eax, 4 ; escrever
  mov ebx, 1 ; monitor
  mov ecx, resultado_final_auxiliar
  mov edx, 1
  int 0x80   ; escrever(monitor, digitado, 1byte);
  
  jmp fim1

  retorno:
  mov eax, [ncaracteres] ;retorna em eax o num de caracteres
  ret

LerHexa:
  mov dword [ncaracteres],0
  call LerString
  mov ecx, string_auxiliar_leitura ;aponta para o vetor digitado
  mov dword [resultado_final_auxiliar],0
  mov edx, resultado_final_auxiliar ; resultado_final_auxiliar
inicio2:
  mov eax,0
  mov al, [ecx] ;pega um digito
  cmp al, 0ah ;se for 

  je fim2 ; vai pro fim
  inc dword [ncaracteres]
  ;se não, verifica se eh um digito valido
  cmp al, 2fh
  jbe erro_fim
  cmp al,3ah; verifica qual é o digito
  jbe numero0a92
  cmp al,40h
  jbe erro_fim
  cmp al, 46h
  jbe eh_letra
  jmp erro_fim
eh_letra:
  sub al, 0x37;se for letra, converte em int
  jmp continua2
numero0a92:
  sub al, 0x30 ; se for numero, converte em int
continua2:
  mov ebx, eax ; salva em ebx
  ;multiplica o resultado_final_auxiliar por 16
  shl dword [edx], 4  
  add dword[edx], ebx ;soma o resultado_final_auxiliar parcial com o digito

  inc ecx ;muda o poneiro pro porximo digito
  jmp inicio2

fim2:
  mov edx, [edx] 
  mov eax, [ncaracteres] ;retorna em eax o num de caracteres
  ret

EscreverInteiro:
  mov dword [ncaracteres],0
  mov dword [demarcador_de_sinal],0
  push byte 0 ;empilha 0 para indicar o final da string_auxiliar_leitura
  push byte 0ah
  cmp ecx, 0 ;ve se o numero é maior que 0
  jge inicio3 ; se for, nao faz nada
  ;se nao for, empilha o '-' e converte pra positivo
  mov dword [demarcador_de_sinal],1
  neg ecx
  
  inicio3:
  ;coloca a parte mais siginificativa do numero em dx
  mov dword edx, 0x00000000
  and edx, ecx
  rol edx, 16
  ;coloca a parte menos significativa em ax
  mov dword eax, ecx
  ;divide o numero por 10 
  mov dword ebx, 10
  div ebx
  
  add edx, 0x30 ;converte o resto em ascii
  push edx ; empilha o resultado_final_auxiliar
  mov dword  ecx,eax ;coloca o resultado_final_auxiliar_divisao_parcial de volta em ecx
  
  cmp ecx,0 ;ve se ja converteu o numero todo
  je fim3
  inc dword [ncaracteres]
  
  jmp inicio3
  
  
  
fim3:
  cmp dword [demarcador_de_sinal],0
  je impressao
  push byte 45
  
impressao:
  pop eax ;desempilha o numero
  cmp eax,0 ; ve se ainda não acabou
  je retorno2 ; se ja, acaba
  mov dword [resultado_final_auxiliar], eax ;se não, coloca pra ser impresso
  
  mov eax, 4 ; escrever
  mov ebx, 1 ; monitor
  mov ecx, resultado_final_auxiliar
  mov edx, 1
  int 0x80   ; escrever(monitor, digitado, 1byte);
  
  jmp impressao

  retorno2:
  mov eax, [ncaracteres] ;retorna em eax o num de caracteres
  ret



LerInteiro:
  mov dword [ncaracteres],0
  call LerString
  mov ecx, string_auxiliar_leitura ;aponta para o vetor digitado
  ;inicia corretamente as variaveis
  mov dword [demarcador_de_sinal], 0
  mov dword [resultado_final_auxiliar],0
  mov edx, resultado_final_auxiliar ; resultado_final_auxiliar
  mov eax,0
  
  mov al, [ecx] ;pega o primeiro digito
  cmp al, 45 ;se for '-'
  jnz inicio4
  mov dword [demarcador_de_sinal], 1
  inc ecx
inicio4:
  mov eax,0
  mov al, [ecx] ;pega um digito
  cmp al, 0ah ;se for 

  je fim4 ; vai pro fim
  inc dword [ncaracteres]
  sub al, 0x30 ; se não, converte em int
  ;rotina que define se o digito é valido ou não
  cmp al,0
  jl erro_fim
  cmp al,10
  jg erro_fim
  
  mov ebx, eax ; salva em ebx
  ;multiplica o resultado_final_auxiliar por 10
  mov dword eax, [edx]
  shl dword eax,1
  shl dword [edx], 3
  add dword [edx], eax
  adc dword[edx], ebx ;soma o resultado_final_auxiliar parcial com o digito

  inc ecx ;muda o poneiro pro porximo digito
  jmp inicio4

fim4:
  mov eax, [demarcador_de_sinal]
  cmp eax, 0
  je retorna
  neg dword [edx]
  
retorna:
  mov edx, [edx] 
  mov eax, [ncaracteres] ;retorna em eax o num de caracteres
  ret
  
LerString:
  mov eax, 3 ; ler
  mov ebx, 1 ; teclado
  int 0x80   ; ler(teclado, string_auxiliar_leitura, size bytes);
  
  ;chamada ao sistema retorna quantidade de caracteres em eax
  ret
  
EscreverString:
  mov eax, 4 ; escrever
  mov ebx, 1 ; monitor
  int 0x80   ; escrever(monitor, string_auxiliar_leitura, size bytes);
  
  ;chamada ao sistema retorna quantidade de caracteres em eax
  ret
  
erro_fim:
  mov     edx, tamanho_string_erro
        mov     ecx, mensagem_string_erro
        mov     ebx, 1
        mov     eax, 4
        int     80h
 
        mov     ebx, 0
        mov     eax, 1
        int     80h
