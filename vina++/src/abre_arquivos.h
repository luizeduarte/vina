#ifndef ABRE_ARQUIVOS_H
#define ABRE_ARQUIVOS_H
#include <stdio.h>
#include <stdlib.h>

FILE* cria_arquivo(char* arquivo);
/*a funcao abre o arquivo passado como argumento para 
leitura e escrita se ja existir, ou para escrita caso contrario*/

FILE* abre_arquivo(char* arquivo);
/*a funcao abre o arquivo passado como argumento para 
leitura e escrita se ja existir, se nao, avisa e fecha o programa*/

#endif