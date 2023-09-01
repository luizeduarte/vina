#ifndef MANIPULA_DIR_H
#define MANIPULA_DIR_H
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 1024

struct diretorio{
	int tam_nome;
	char* nome;
	long long int tamanho;
	long long int posicao;
	uid_t uid;
	mode_t permissoes;
	time_t ultima_modificacao;
};

struct conteudo{
	unsigned int num_arq;
	long long int tam_conteudo, diretorio_pos;
};

struct diretorio** le_diretorio(FILE* archive);
	/*a funcao recebe um ponteiro para o archive como argumento e coloca as
	informacoes de cada arquivo em um vetor de ponteiros para structs, retornando tal vetor*/

struct diretorio** adiciona_diretorio(struct diretorio* v_diretorio[], struct conteudo* info_conteudo, struct stat info_arquivo, char* nome_arquivo);
	/*a funcao recebe como argumento um vetor de ponteiros para struct, um ponteiro para struct contendo as informacoes do conteudo, uma struct
	do tipo stat do arquivo novo inserido e, por fim, o nome desse arquivo. Ela adiciona ao fim do vetor as informacoes do novo arquivo que sera
	adicionado ao archive, retornando tal vetor atualizado*/

void imprime_diretorio(FILE* archive, struct diretorio* v_diretorio[], int num_arq);
	/*a funcao imprime ao final da area de conteudos do archive o diretorio. 
	O ponteiro para escrever no mesmo deve ser posicionado antes de chamar a funcao.*/

int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[]);
	/*a funcao recebe o archive e o nome de um arquivo e um vetor de ponteiros 
	para structs diretorio, retornando o indice dele no vetor*/

void desaloca_diretorio(FILE* archive, struct diretorio* v_diretorio[]);
	/*a funcao recebe um vetor de ponteiros para structs diretorio e o numero de arquivos
	que ele contem, desalocando a memoria alocada*/
#endif