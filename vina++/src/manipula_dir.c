#include "manipula_dir.h"
#include "manipula_conteudo.h"

struct diretorio** le_diretorio(FILE* archive){
	/*a funcao recebe um ponteiro para o archive como argumento e coloca as
	informacoes de cada arquivo em um vetor de ponteiros para structs, retornando tal vetor*/
	struct conteudo* info_conteudo;
	struct diretorio* *v_diretorio;

	info_conteudo = conteudo(archive);
	v_diretorio = malloc(info_conteudo->num_arq * sizeof(struct diretorio*));

	fseek(archive, info_conteudo->diretorio_pos, SEEK_SET);	//vai para o diretorio
	for (int i = 0; i < info_conteudo->num_arq; i++){
		v_diretorio[i] = malloc(sizeof(struct diretorio));
		fread(&v_diretorio[i]->tam_nome, sizeof(int), 1, archive);	//le o tamanho do nome do arquivo
		v_diretorio[i]->nome = malloc(v_diretorio[i]->tam_nome * sizeof(char));
		fread(v_diretorio[i]->nome, sizeof(char), v_diretorio[i]->tam_nome, archive);
		fread(&v_diretorio[i]->tamanho, sizeof(long long int), 1, archive);	//le o tamanho do arquivo
		fread(&v_diretorio[i]->posicao, sizeof(long long int), 1, archive);	//le a posicao do arquivo
		fread(&v_diretorio[i]->uid, sizeof(uid_t), 1, archive);	//le o uid do arquivo
		fread(&v_diretorio[i]->permissoes, sizeof(mode_t), 1, archive);	//le as permissoes do arquivo
		fread(&v_diretorio[i]->ultima_modificacao, sizeof(time_t), 1, archive);	//le a ultima modificacao do arquivo
	}

	return v_diretorio;
}

struct diretorio** adiciona_diretorio(struct diretorio* v_diretorio[], struct conteudo* info_conteudo, struct stat info_arquivo, char* nome_arquivo){
	/*a funcao recebe como argumento um vetor de ponteiros para struct, um ponteiro para struct contendo as informacoes do conteudo, uma struct
	do tipo stat do arquivo novo inserido e, por fim, o nome desse arquivo. Ela adiciona ao fim do vetor as informacoes do novo arquivo que sera
	adicionado ao archive, retornando tal vetor atualizado*/
	v_diretorio = realloc(v_diretorio, info_conteudo->num_arq * sizeof(struct diretorio*));
	v_diretorio[info_conteudo->num_arq - 1] = malloc(sizeof(struct diretorio));

	int tam_nome = strlen(nome_arquivo);
	v_diretorio[info_conteudo->num_arq - 1]->tam_nome = tam_nome;
	v_diretorio[info_conteudo->num_arq - 1]->nome = malloc(sizeof(char) * tam_nome);
	strncpy(v_diretorio[info_conteudo->num_arq - 1]->nome, nome_arquivo, tam_nome);
	v_diretorio[info_conteudo->num_arq - 1]->tamanho = info_arquivo.st_size;
	v_diretorio[info_conteudo->num_arq - 1]->posicao = info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int) - info_arquivo.st_size;
	v_diretorio[info_conteudo->num_arq - 1]->uid = info_arquivo.st_uid;
	v_diretorio[info_conteudo->num_arq - 1]->permissoes = info_arquivo.st_mode;
	v_diretorio[info_conteudo->num_arq - 1]->ultima_modificacao = info_arquivo.st_mtime;

	return v_diretorio;
}

void imprime_diretorio(FILE* archive, struct diretorio* v_diretorio[], int num_arq){
	/*a funcao imprime ao final da area de conteudos do archive o diretorio. 
	O ponteiro para escrever no mesmo deve ser posicionado antes de chamar a funcao.*/
	for (int i = 0; i < num_arq; i++){
		if (v_diretorio[i]){
			fwrite(&v_diretorio[i]->tam_nome, sizeof(int), 1, archive);
			fwrite(v_diretorio[i]->nome, sizeof(char), v_diretorio[i]->tam_nome , archive);
			fwrite(&v_diretorio[i]->tamanho, sizeof(long long int), 1, archive);
			fwrite(&v_diretorio[i]->posicao, sizeof(long long int), 1, archive);
			fwrite(&v_diretorio[i]->uid, sizeof(uid_t), 1, archive);
			fwrite(&v_diretorio[i]->permissoes, sizeof(mode_t), 1, archive);
			fwrite(&v_diretorio[i]->ultima_modificacao, sizeof(time_t), 1, archive);
		}
	}
}

int id_arquivo(FILE *archive, char *nome_arquivo, struct diretorio *v_diretorio[]){
	/*a funcao recebe o archive e o nome de um arquivo e um vetor de ponteiros 
	para structs diretorio, retornando o indice dele no vetor*/

	int achou = 0, num_arq = 0;

	fseek(archive, 0, SEEK_SET);
	if (fread(&num_arq, sizeof(int), 1, archive) == 0)		//le o numero de arquivos
		return -1;	//dir vazio

	while ((achou == 0) && (num_arq > 0)){
		if (strcmp(v_diretorio[num_arq - 1]->nome, nome_arquivo) == 0){
			achou = 1;
		}

		num_arq--;
	}

	if (achou == 0)
		return -1;

	return num_arq;
}

void desaloca_diretorio(FILE* archive, struct diretorio* v_diretorio[]){
	/*a funcao recebe um vetor de ponteiros para structs diretorio e o numero de arquivos
	que ele contem, desalocando a memoria alocada*/
	int num_arq;
	fseek(archive, 0, SEEK_SET);
	fread(&num_arq, sizeof(int), 1, archive);	//le o numero de arquivos
	for (int i = 0; i < num_arq; i++){
		free(v_diretorio[i]->nome);
		free(v_diretorio[i]);
	}
	free(v_diretorio);
}
