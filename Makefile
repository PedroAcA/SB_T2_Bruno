#SOURCES_IA_32 = arquivos.c montador.c parser.c parser2.c pre_processador.c scanner.c tabelas.c
SOURCES_TRADUTOR = scanner.c tradutor.c arquivos.c
projeto: 
	@make traduzir --no-print-directory #&& make IA_32 --no-print-directory

#$(addprefix Tradutor/, $(SOURCES_TRADUTOR)) concatena  Tradutor/ e distribui esse diretorio para todos .c presentes em SOURCES_TRADUTOR
traduzir: $(addprefix Tradutor/,$(SOURCES_TRADUTOR))
	@cd Tradutor && echo "\nAbrindo pasta Tradutor\n" && gcc -Wall -o tradut $(SOURCES_TRADUTOR) -I. && gcc -Wall -o ../tradut $(SOURCES_TRADUTOR) -I.

#$(addprefix /, $(SOURCES_IA_32)) concatena o diretorio / e distribui esse diretorio para todos .c presentes em SOURCES_IA_32
#IA_32: $(addprefix /,$(SOURCES_IA_32)) 
#	@cd  && echo "\nAbrindo pasta \n" && gcc -Wall -o nome $(SOURCES_IA_32) -I. && gcc -Wall -o ../nome $(SOURCES_IA_32) -I.	
