#ifndef FUNCOES_PRINCIPAIS_H
#define FUNCOES_PRINCIPAIS_H
#include "manipula_conteudo.h"
#include "manipula_dir.h"

int verifica_sub(time_t ultima_modificacao, char* nome_arquivo);
	/*a funcao verifica se o arquivo deve ser substituido no archive,
	dependendo de sua data de modificacao. Para isso ele recebe a data
	do arquivo ja presente no archive e seu nome, retornando 1 caso deve e 0 caso nao*/
struct diretorio** substitui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv, int optind);
/*a funcao atualiza o archive inserindo no fim dos conteudos e no diretorio as informacoes 
	do arquivo novo apenas se ele for mais recente, para isso ela recebe o nome de tal arquivo, um ponteiro para ele e o archive,
	alem de um vetor de ponteiros para structs diretorio*/

struct diretorio** insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[], struct conteudo* info_conteudo);
/*a funcao atualiza o archive inserindo no fim dos conteudos e no diretorio as informacoes 
	do arquivo novo, para isso ela recebe o nome de tal arquivo, um ponteiro para ele e o archive,
	alem de um vetor de ponteiros para structs diretorio*/

struct diretorio** insere_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);
/*a funcao insere no archive todos os arquivos passados como argumentos, 
	substituindo os ja existentes pela outra versao*/

void cria_dir(char* caminho);
	/*a funcao cria um diretorio com o nome passado como argumento, caso ele nao exista*/

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* arquivo);
	/* a funcao recebe um ponteiro para o archive, o nome do arquivo a 
	ser extraido e um onteiro para ele do vetor de diretorios*/

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]);
	/* a funcao recebe as informacoes de argumentos passados para o programa, alem de 
	um ponteiro para o archive e o vetor de diretorios, visaando extrair todos os arquivos pedidos*/

void exclui(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int id_arq, struct conteudo* info_conteudo);
	/*a funcao exclui na area de conteudos um arquivo, movendo para cima os que vem apos ele. Para isso, ela recebe o nome do archive, 
	um ponteiro para ele, um vetor de diretorio o indice nele do arquivo a ser removido e uma struct com as informacoes do conteudo*/

void exclui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind);
	/*a funcao exclui todos os arquivos passados como argumento, para isso ela recebe o nome do archive, um ponteiro para 
	ele, o vetor de diretorio e as informcoes dos argumentos*/

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]);
	/*a funcao move nos diretorios os arquivos passados como argumento, para isso ela recebe um ponteiro 
	para o archive, o nome do target e do atquivo a ser movido e o vetor de diretorio*/

void lista_arq(FILE* archive, struct diretorio* v_diretorio[]);
	/*a funcao lista os arquivos presentes no archive, para 
	isso ela recebe um ponteiro para ele e um vetor de diretorios*/

#endif