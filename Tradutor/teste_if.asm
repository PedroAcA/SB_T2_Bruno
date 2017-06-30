; programa em assembly IA32 para testar como as diretivas .if, .else e .endif funcionam
; referencias usadas (acessadas no dia 29/06/2017):
; https://www.tortall.net/projects/yasm/manual/html/nasm-macro-conditional.html
;http://www.nasm.us/doc/nasmdoc4.html
section .data
	msg_if db "Condicao != 0",0dh,0ah
	tam_if EQU $-msg_if; colocar logo apos a mensagem, senao ele vai ver o tamanho de todas as strings acima ate chegar a msg_igual
	msg_not_if db "Condicao = 0",0dh,0ah
	tam_not_if EQU $-msg_not_if
	
	cond EQU 0

section .text
	global _start
	_start:
	
	%ifn cond=0; 0 eh falso e td diferente de 0 eh verdadeiro (considerar assim ou so considerar 1 como verdadeiro?)
		mov eax,4;sys write
		mov ebx,1;stdout
		mov ecx,msg_if;char *
		mov edx,tam_if
		int 080h;chamada ao sistema
	%else
		mov eax,4;sys write
		mov ebx,1;stdout
		mov ecx,msg_not_if;char *
		mov edx,tam_not_if
		int 080h;chamada ao sistema
	%endif
		
	mov eax,1
	mov ebx,0
	int 080h
	