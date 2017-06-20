push eax
mov eax, dword[n1]
mov dword[r], eax
pop eax
mov eax, dword[n2]
prox: add eax, dword[r]
mov dword[r], eax
cmp eax,0
jl prox
mov eax, 1
mov ebx, 0
int 0x80
section .data
n1   db   1
n2   db   2
section .bss
r resb 1
