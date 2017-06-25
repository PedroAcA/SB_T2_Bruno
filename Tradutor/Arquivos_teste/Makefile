#Makefile para compilar, ligar e/ou executar codigos em assembly (assembly intel inicialmente) 
#Para facilitar a utilizacao deste makefile para varios codigos asm, foi colocado o nome do arquivo como variavel TARGET
#Opcoes disponiveis nesse makefile: 
#	->Compilar (32 bits),ligar e executar um asm para 64 bits ->comando padrao executado com make ou make padrao 

#TARGET = nome (sem extensao)  do arquivo a ser compilado, ligado e/ou executado. Mudar o nome para realizar umas das opcoes mencionadas acima
# em um arquivo assembly, codigo maquina ou executavel do Ubuntu
# Caso o arquivo em TARGET nao  estja na mesma pasta do makefile, colocar o diretorio ate o arquivo

# para enviar o nome do arquivo por linha de comando digite make nome_regra arq=nome_arquivo
TARGET = 
#Primeira linha da regra: nome para chamar com "make nome" seguido dos pre-requisitos.
#Comando padrao eh compilar para 32 bits e ligar e executar para 64
padrao: 
#Comando a ser executado (colocar o @ antes do comando faz com que ele nao apareca no terminal -> make so vai executar o comando, e nao fazer um
# "echo" dele). 1 Tab eh obrigatorio. 
	@nasm -f elf -o IA_32/obj/$(TARGET).o $(TARGET).s && ld -m elf_i386 -o IA_32/$(TARGET) IA_32/obj/$(TARGET).o && ./IA_32/$(TARGET)

