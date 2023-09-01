#include "abre_arquivos.h"

FILE* cria_arquivo(char* arquivo){
	/*a funcao abre o arquivo passado como argumento para 
	leitura e escrita se ja existir, ou para escrita caso contrario*/
	FILE* p_arquivo = fopen(arquivo, "r+b");

	if (!p_arquivo)
		p_arquivo = fopen(arquivo, "w+b");

	return p_arquivo;
}

FILE* abre_arquivo(char* arquivo){
	/*a funcao abre o arquivo passado como argumento para 
	leitura e escrita se ja existir, se nao, avisa e fecha o programa*/
	FILE* p_arquivo = fopen(arquivo, "r+b");

	if (!p_arquivo){
		fprintf(stderr, "arquivo %s nao existe\n", arquivo);
		exit(1);
	}

	fseek(p_arquivo, 0, SEEK_END);
	if (ftell(p_arquivo) == 0){
		fprintf(stderr, "archive vazio\n");
		exit(1);
	}
	fseek(p_arquivo, 0, SEEK_SET);

	return p_arquivo;
}