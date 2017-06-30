I1: EQU 1
I2: EQU 1
SECTION TEXT
	IF I1
		OUTPUT um
	IF I2
		OUTPUT dois
	STOP
SECTION DATA
	um: const 1
	esp1: space; testando section.data misturado com section .bss
	dois: const 2
	esp2: space 3
