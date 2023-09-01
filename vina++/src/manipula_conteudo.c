#include "manipula_conteudo.h"

struct conteudo* conteudo(FILE* archive){
	/*a funcao recebe um ponteiro para o archive e le as informacoes do conteudo, sendo estas tamanho, numero de arquivos e onde acaba,
	retornando um ponteiro para uma struct com essas informacoes*/
	struct conteudo* info_conteudo = malloc(sizeof(struct conteudo));

	//as informacoes se localizam no comeco do archive
	fseek(archive, 0, SEEK_SET);
	if (fread(&info_conteudo->num_arq, sizeof(int), 1, archive) == 0){
		//se estiver vazio, inicializa as variaveis
		info_conteudo->num_arq = 0;
		info_conteudo->tam_conteudo = 0;
		info_conteudo->diretorio_pos = sizeof(long long int) + sizeof(int);
		return info_conteudo;
	}

	fread(&info_conteudo->tam_conteudo, sizeof(long long int), 1, archive);
	//o final dos conteudos, ou onde o diretorio comeca, se localiza apos o tamanho do conteudo
	info_conteudo->diretorio_pos = info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int);
	return info_conteudo;
}

void copia_texto(FILE* arq_le, FILE* arq_escreve, long long int tam_texto){
	/*a funcao recebe ponteiros para os arquivos que deseja ler e escrever,
	alem do tamanho do texto que sera copiado de um para outro.
	Os ponteiros das funcoes de leitura e escrita devem ser posicionados fora da funcao*/
	char* buffer = malloc(sizeof(char) * MAX_STRING);

	//calcula o numeros de blocos a serem lidos
	long long int num_blocos = tam_texto / MAX_STRING;
	int resto = tam_texto % MAX_STRING;

	for (int i = 0; i < num_blocos; i++){
		fread(buffer, sizeof(char), MAX_STRING, arq_le);
		fwrite(buffer, sizeof(char), MAX_STRING, arq_escreve);
	}
	fread(buffer, sizeof(char), resto, arq_le);
	fwrite(buffer, sizeof(char), resto, arq_escreve);
	free(buffer);
}

void att_info_conteudo(FILE* archive, struct conteudo* info_conteudo){
	/*a funcao recebe um ponteiro para o archive e uma struct do tipo
	info_conteudo, atualizando ao inicio do archive as informacoes contidas nela*/
	//escreve no archive os valores atualizados
	fseek(archive, 0, SEEK_SET);
	fwrite(&info_conteudo->num_arq, sizeof(int), 1, archive);	
	fwrite(&info_conteudo->tam_conteudo, sizeof(long long int), 1, archive);
}


void move_conteudo(FILE* archive, struct diretorio* arquivo, long long int diretorio_pos){
	/*a funcao recebe um ponteiro para o archive e para a struct do arquivo a ser excluido,
	alem da posicao do diretorio. Ela move para tras o conteudo localizado apos o arquivo 
	que deseja remover, escrevendo por cima do mesmo*/

	long long int nova_pos = arquivo->posicao;	//onde comeca o arquivo a ser removido
	long long int antiga_pos = nova_pos + arquivo->tamanho;	//onde comeca o arquivo seguinte dele 
	long long int tam_mover = diretorio_pos - antiga_pos;	//tamanho do conteudo a ser movido
	if (tam_mover == 0){	//o arquivo eh o ultimo do archive
		fseek(archive, nova_pos + 1, SEEK_SET);
		return;
	}
	//calcula o carregamento em blocos 
	long long int num_blocos = tam_mover / MAX_STRING;
	long long int resto = tam_mover % MAX_STRING;

	char* buffer = malloc(sizeof(char) * MAX_STRING);
	//copia os blocos para a nova posicao no archive 
	for (int i = 0; i < num_blocos; i++){
		fseek(archive, antiga_pos, SEEK_SET);
		fread(buffer, sizeof(char), MAX_STRING, archive);
		fseek(archive, nova_pos, SEEK_SET);
		fwrite(buffer, sizeof(char), MAX_STRING, archive);
		nova_pos += MAX_STRING;
		antiga_pos += MAX_STRING;
	}

	//copia o resto para a nova posicao no archive
	fseek(archive, antiga_pos, SEEK_SET);
	fread(buffer, sizeof(char), resto, archive);
	fseek(archive, nova_pos, SEEK_SET);
	fwrite(buffer, sizeof(char), resto, archive);

	free(buffer);
}