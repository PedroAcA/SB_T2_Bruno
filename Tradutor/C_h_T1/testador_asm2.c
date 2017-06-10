/*Sites acessados em 17/05/2017 as 15:07:
    http://stackoverflow.com/questions/11736060/how-to-read-all-files-in-a-folder-using-c
    https://linux.die.net/man/3/opendir
    http://stackoverflow.com/questions/30305619/passing-variables-to-system-function-in-c
    http://www.cplusplus.com/reference/cstdio/sprintf/
    http://www.cplusplus.com/reference/cstdlib/system/?kw=system
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
int main (){
    DIR* FD;
    struct dirent* in_file;
    FILE    *common_file;
    FILE    *entry_file;
    char    diretorio_completo[10000],comando_completo[10000],diretorio_destino[10000];
    FD = opendir ("arquivos_teste_moodle");
    if(FD!=NULL){
         while ((in_file = readdir(FD)))
    {
        /* On linux/Unix we don't want current and parent directories
         * On windows machine too, thanks Greg Hewgill
         */
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))
            continue;
        if (!strcmp (in_file->d_name, "objetos"))
            continue;
        /* Open directory entry file for common operation */
        /* TODO : change permissions to meet your need! */
        strcpy(diretorio_completo,"arquivos_teste_moodle");
        strcat(diretorio_completo,"/");
        strcat(diretorio_completo,in_file->d_name);
        strcpy(comando_completo,"./montador -o ");
        strcat(comando_completo,diretorio_completo);
        //strcat(comando_completo," teste");
        strcpy(diretorio_destino," arquivos_teste_moodle/objetos/");
        in_file->d_name[strlen(in_file->d_name)-4] = '\0';
        strcat(diretorio_destino,in_file->d_name);;
        strcat(comando_completo,diretorio_destino);
        //printf("\nArquivo encontrado (incluindo diretorio): %s\n",diretorio_completo);
        printf("\nComando executado:%s\n",comando_completo);
        system(comando_completo);
        getchar();
        getchar();
     //   entry_file = fopen(in_file->d_name, "rw");

    }

    }else{
        printf("\nNao achei diretorio\n");
    }
}