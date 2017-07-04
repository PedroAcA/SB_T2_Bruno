SOURCES_TRADUTOR = scanner.c tradutor.c arquivos.c
#ASM eh uma variavel usada para testar os arquivos asm. Parametro passado por linha de comando
ASM=default  

projeto: 
	@make traduzir --no-print-directory 

#$(addprefix Tradutor/, $(SOURCES_TRADUTOR)) concatena  Tradutor/ e distribui esse diretorio para todos .c presentes em SOURCES_TRADUTOR
traduzir: $(addprefix Tradutor/,$(SOURCES_TRADUTOR))
	@cd Tradutor && echo "\nAbrindo pasta Tradutor\n" && gcc -Wall -o tradutor $(SOURCES_TRADUTOR) -I. && gcc -Wall -o ../tradutor $(SOURCES_TRADUTOR) -I.
testar:
	@echo "\nExecutando arquivo $(ASM) que se encontra na pasta Tradutor/Arquivos_teste\n" && cd Tradutor/Arquivos_teste && $(MAKE) TARGET=$(ASM) --no-print-directory #se digitar make teste ASM=Nome_arquivo_
