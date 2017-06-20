mov dword[zero], dword[older]
mov dword[one], dword[old]
front: mov eax, dword[older]
add eax, dword[old]
mov dword[new], eax
sub eax, dword[limit]
cmp eax,0
jg final
mov dword[old], dword[older]
mov dword[new], dword[old]
jmp front
final: add eax, dword[zero]
sub eax, dword[one]
mov eax, 1
mov ebx, 0
int 0x80
zero: one: older: old: new: limit: 