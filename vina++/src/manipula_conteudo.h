#ifndef MANIPULA_CONTEUDO_H
#define MANIPULA_CONTEUDO_H
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include "manipula_dir.h"
#define MAX_STRING 1024

struct conteudo* conteudo(FILE* archive);
	/*a funcao recebe um ponteiro para o archive e le as informacoes do conteudo, sendo estas tamanho, numero de arquivos e onde acaba,
	retornando um ponteiro para uma struct com essas informacoes*/

void copia_texto(FILE* arq_le, FILE* arq_escreve, long long int tam_texto);
/*a funcao recebe ponteiros para os arquivos que deseja ler e escrever,
	alem do tamanho do texto que sera copiado de um para outro.
	Os ponteiros das funcoes de leitura e escrita devem ser posicionados fora da funcao*/

void att_info_conteudo(FILE* archive, struct conteudo* info_conteudo);
	/*a funcao recebe um ponteiro para o archive e uma struct do tipo
	info_conteudo, atualizando ao inicio do archive as informacoes contidas nela*/

void move_conteudo(FILE* archive, struct diretorio* diretorio, long long int diretorio_pos);
	/*a funcao recebe um ponteiro para o archive e para a struct do arquivo a ser excluido,
	alem da posicao do diretorio. Ela move para tras o conteudo localizado apos o arquivo 
	que deseja remover, escrevendo por cima do mesmo*/

#endif