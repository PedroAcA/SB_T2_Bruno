global _start
section .text
_start:
mov eax, dword[n2]
imul dword[n1]
mov dword[r], eax
pusha
mov ecx,[r]
call EscreverInteiro
popa
mov eax, dword[n1]
imul dword[n1]
mov dword[r], eax
pusha
mov ecx,[r]
call EscreverInteiro
popa
mov eax, dword[n2]
imul dword[n2]
mov dword[r], eax
pusha
mov ecx,[r]
call EscreverInteiro
popa
mov eax, dword[n2]
cdq
idiv dword[n1]
mov dword[r], eax
pusha
mov ecx,[r]
call EscreverInteiro
popa
mov eax, dword[n2]
cdq
idiv dword[n2]
mov dword[r], eax
pusha
mov ecx,[r]
call EscreverInteiro
popa
mov eax, dword[n1]
cdq
idiv dword[n1]
mov dword[r], eax
pusha
mov ecx,[r]
call EscreverInteiro
popa
mov eax, 1
mov ebx, 0
int 0x80
section .data
n1   dd   4
n2   dd   -4 
section .bss
r   resb 4



;Funcoes implementadas:
section .data
msg     db      'Digitos invalidos', 0AH
len     equ     $-msg
resultado dd 0
flag db 0
quociente db '0000'
string db '000000000000'

LerChar:
mov eax, 3 ; ler
mov ebx, 1 ; teclado
mov edx, 1
int 0x80   ; ler(teclado, digitado, 1byte);

mov eax,ecx
ret
EscreverChar:
mov eax, 4 ; escrever
mov ebx, 1 ; monitor
mov edx, 1
int 0x80   ; escrever(monitor, digitado, 1byte);

ret

EscreverHexa:
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
push edx ; empilha o resultado
mov dword  ecx,eax ;coloca o quociente de volta em ecx

cmp ecx,0 ;ve se ja converteu o numero todo
je fim1

jmp inicio1

fim1:
pop eax ;desempilha o numero
cmp eax,0 ; ve se ainda não acabou
je retorno ; se ja, acaba
mov dword [resultado], eax ;se não, coloca pra ser impresso

mov eax, 4 ; escrever
mov ebx, 1 ; monitor
mov ecx, resultado
mov edx, 1
int 0x80   ; escrever(monitor, digitado, 1byte);

jmp fim1

retorno:
ret

LerHexa:
call LerString
mov ecx, string ;aponta para o vetor digitado
mov dword [resultado],0
mov edx, resultado ; resultado
inicio2:
mov eax,0
mov al, [ecx] ;pega um digito
cmp al, 0ah ;se for 
je fim2 ; vai pro fim
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
;multiplica o resultado por 16
shl dword [edx], 4  
add dword[edx], ebx ;soma o resultado parcial com o digito

inc ecx ;muda o poneiro pro porximo digito
jmp inicio2

fim2:
mov edx, [edx] ;retorna o resultado em eax
ret

EscreverInteiro:
mov dword [flag],0
push byte 0 ;empilha 0 para indicar o final da string
push byte 0ah
cmp ecx, 0 ;ve se o numero é maior que 0
jge inicio3 ; se for, nao faz nada
;se nao for, empilha o '-' e converte pra positivo
mov dword [flag],1
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
push edx ; empilha o resultado
mov dword  ecx,eax ;coloca o quociente de volta em ecx

cmp ecx,0 ;ve se ja converteu o numero todo
je fim3

jmp inicio3



fim3:
cmp dword [flag],0
je impressao
push byte 45

impressao:
pop eax ;desempilha o numero
cmp eax,0 ; ve se ainda não acabou
je retorno2 ; se ja, acaba
mov dword [resultado], eax ;se não, coloca pra ser impresso

mov eax, 4 ; escrever
mov ebx, 1 ; monitor
mov ecx, resultado
mov edx, 1
int 0x80   ; escrever(monitor, digitado, 1byte);

jmp impressao

retorno2:
ret



LerInteiro:
call LerString
mov ecx, string ;aponta para o vetor digitado
;inicia corretamente as variaveis
mov dword [flag], 0
mov dword [resultado],0
mov edx, resultado ; resultado
mov eax,0

mov al, [ecx] ;pega o primeiro digito
cmp al, 45 ;se for '-'
jnz inicio4
mov dword [flag], 1
inc ecx
inicio4:
mov eax,0
mov al, [ecx] ;pega um digito
cmp al, 0ah ;se for \nje fim4 ; vai pro fim
sub al, 0x30 ; se não, converte em int
;rotina que define se o digito é valido ou não
cmp al,0 
jl erro_fim
cmp al,10
jg erro_fim

mov ebx, eax ; salva em ebx
;multiplica o resultado por 10
mov dword eax, [edx]
shl dword eax,1
shl dword [edx], 3
add dword [edx], eax
adc dword[edx], ebx ;soma o resultado parcial com o digito

inc ecx ;muda o poneiro pro porximo digito
jmp inicio4

fim4:
mov eax, [flag]
cmp eax, 0
je retorna
neg dword [edx]

retorna:
mov edx, [edx] ;retorna o resultado em eax
ret

LerString:
mov eax, 3 ; ler
mov ebx, 1 ; teclado
int 0x80   ; ler(teclado, string, size bytes);

ret

EscreverString:
mov eax, 4 ; escrever
mov ebx, 1 ; monitor
int 0x80   ; escrever(monitor, string, size bytes);

ret

erro_fim:
mov     edx, len
mov     ecx, msg
mov     ebx, 1
mov     eax, 4
int     80h

mov     ebx, 0
mov     eax, 1
int     80h
