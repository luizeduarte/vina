#include "funcoes_principais.h"
#include <unistd.h>
#include <pwd.h>
#include <time.h>

int verifica_sub(time_t ultima_modificacao, char* nome_arquivo){
	/*a funcao verifica se o arquivo deve ser substituido no archive,
	dependendo de sua data de modificacao. Para isso ele recebe a data
	do arquivo ja presente no archive e seu nome, retornando 1 caso deve e 0 caso nao*/

	//pega a data de modificacao do arquivo 
	struct stat info_arquivo;
	stat(nome_arquivo, &info_arquivo);

	//se for maior que a do arquivo no archive, substitui
	if (info_arquivo.st_mtime > ultima_modificacao)
		return 1;
	
	return 0;
}

struct diretorio** substitui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int argc, char** argv, int optind){
	/*a funcao atualiza o archive inserindo no fim dos conteudos e no diretorio as informacoes 
	do arquivo novo apenas se ele for mais recente, para isso ela recebe o nome de tal arquivo, um ponteiro para ele e o archive,
	alem de um vetor de ponteiros para structs diretorio*/
	FILE *novo_arq;
	int id_arq;
	struct conteudo* info_conteudo = conteudo(archive);

	//insere todos os arquivos passados como argumento
	for (int i = optind; i < argc; i++){
		novo_arq = fopen(argv[i], "r");
		if (!novo_arq){
			fprintf(stderr, "erro ao abrir o arquivo %s\n", argv[i]);
		} else {
			id_arq = id_arquivo(archive, argv[i], v_diretorio);
			if (id_arq < 0){
				v_diretorio = insere(argv[i], novo_arq, archive, v_diretorio, info_conteudo);
			} else { 
				if (verifica_sub(v_diretorio[id_arq]->ultima_modificacao, argv[i])){
					//se deve ser substituido, exclui o arquivo antigo e insere o novo
					exclui(nome_archive, archive, v_diretorio, id_arq, info_conteudo);
					v_diretorio = insere(argv[i], novo_arq, archive, v_diretorio, info_conteudo);
				}
			}
			fclose(novo_arq);
		}
	}

	//atualiza as informacoes do conteudo e o diretorio
	if (info_conteudo->num_arq == 0){
		remove(nome_archive);
		return v_diretorio;
	}
	att_info_conteudo(archive, info_conteudo);
	fseek(archive, 0, SEEK_END);
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
    
	return v_diretorio;
}

struct diretorio** insere(char* nome_arquivo, FILE* arq_novo, FILE* archive, struct diretorio* v_diretorio[], struct conteudo* info_conteudo){
	/*a funcao atualiza o archive inserindo no fim dos conteudos e no diretorio as informacoes 
	do arquivo novo, para isso ela recebe o nome de tal arquivo, um ponteiro para ele e o archive,
	alem de um vetor de ponteiros para structs diretorio*/
	struct stat info_arquivo;
	stat(nome_arquivo, &info_arquivo);	//pega as informacoes do arquivo novo

	//atualiza o numero de arquivos e o tamanho total dos conteudos no comeco do archive
	info_conteudo->num_arq++;
	info_conteudo->diretorio_pos = info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int);
	info_conteudo->tam_conteudo += info_arquivo.st_size;

	//posiciona o ponteiro no fim dos conteudos
	fseek(archive, info_conteudo->diretorio_pos, SEEK_SET);
	//copia o texto do arquivo novo para o archive
	copia_texto(arq_novo, archive, info_arquivo.st_size);
	ftruncate(fileno(archive), ftell(archive) - 1);	//corta o archive no final do texto	

	//atualiza o diretorio com o novo arquivo
	return adiciona_diretorio(v_diretorio, info_conteudo, info_arquivo, nome_arquivo);
}

struct diretorio** insere_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind){
	/*a funcao insere no archive todos os arquivos passados como argumentos, 
	substituindo os ja existentes pela outra versao*/
	FILE* novo_arq;
	int id_arq;
	struct conteudo* info_conteudo = conteudo(archive);

	for (int i = optind; i < num_arq; i++){
		//exclui se ele ja estiver no archive
		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if(id_arq != -1){
			exclui(nome_archive, archive, v_diretorio, id_arq, info_conteudo);
		}

		novo_arq = fopen(argv[i], "r");
		if (!novo_arq)
			fprintf(stderr, "erro ao abrir o arquivo %s, ele nao sera inserido\n", argv[i]);
		else {
			v_diretorio = insere(argv[i], novo_arq, archive, v_diretorio, info_conteudo);
			fclose(novo_arq);
		}
	}

	//atualiza as informacoes do conteudo e o diretorio
	if (info_conteudo->num_arq == 0){
		remove(nome_archive);
		return v_diretorio;
	}
	att_info_conteudo(archive, info_conteudo);
	fseek(archive, info_conteudo->tam_conteudo + sizeof(int) + sizeof(long long int), SEEK_SET);
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
	return v_diretorio;
}

void cria_dir(char* caminho){
	/*a funcao cria um diretorio com o nome passado como argumento, caso ele nao exista*/
	char* fim_dir, *nome_dir;

	nome_dir = strchr(caminho, '/');
	while(nome_dir != NULL){
		*nome_dir = '\0';	//delimita ate onde vai o nome do dir para criar
		mkdir(caminho, 0777);
		*nome_dir = '/';	//libera para diretorios intermediarios
		fim_dir = nome_dir + 1;
		nome_dir = strchr(fim_dir, '/');	//pega o nome do proximo dir
	}
}

void extrai(FILE* archive, char* nome_arquivo, struct diretorio* arquivo){
	/* a funcao recebe um ponteiro para o archive, o nome do arquivo a 
	ser extraido e um onteiro para ele do vetor de diretorios*/

	cria_dir(nome_arquivo);		//cria o diretorio caso ele nao exista 
	FILE* arq_novo = fopen(nome_arquivo, "w+");

	fseek(archive, arquivo->posicao, SEEK_SET);	//vai ate o comeco do texto a ser copiado
	copia_texto(archive, arq_novo, arquivo->tamanho);
	chmod(nome_arquivo, arquivo->permissoes);	//atribui as permissoes ao arquivo criado
	fclose(arq_novo);
}

void extrai_arg(int argc, char* argv[], FILE* archive, struct diretorio* v_diretorio[]){
	/* a funcao recebe as informacoes de argumentos passados para o programa, alem de 
	um ponteiro para o archive e o vetor de diretorios, visaando extrair todos os arquivos pedidos*/
	int num_arquivos, id_arq;

	if (argc == 3){		//extrai todos os arquivos
        fseek(archive, 0, SEEK_SET);
		fread(&num_arquivos, sizeof(int), 1, archive);
		for (int i = 0; i < num_arquivos; i++)
			extrai(archive, v_diretorio[i]->nome, v_diretorio[i]);

	} else {
		for (int i = 3; i < argc; i++){
			id_arq = id_arquivo(archive, argv[i], v_diretorio);
			if (id_arq < 0)
				fprintf(stderr, "arquivo nao encontrado no archive\n");
			else 
				extrai(archive, v_diretorio[id_arq]->nome, v_diretorio[id_arq]);
		}
	}
}

void exclui(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int id_arq, struct conteudo* info_conteudo){
	/*a funcao exclui na area de conteudos um arquivo, movendo para cima os que vem apos ele. Para isso, ela recebe o nome do archive, 
	um ponteiro para ele, um vetor de diretorio o indice nele do arquivo a ser removido e uma struct com as informacoes do conteudo*/
	//atualiza o numero de arquivos e o tamanho do conteudo
	info_conteudo->num_arq--;
	info_conteudo->tam_conteudo -= v_diretorio[id_arq]->tamanho;

	//se nao houver mais arquivos no archive, basta corta-lo no comeco
	if (info_conteudo->num_arq == 0)
		remove(nome_archive);

	//move o restante dos conteudos para o lugar do excluido
	move_conteudo(archive, v_diretorio[id_arq], info_conteudo->diretorio_pos);
	info_conteudo->diretorio_pos -= v_diretorio[id_arq]->tamanho;

	//retira o arquivo do diretorio e atualiza a posicao do restante
	for (int i = id_arq + 1; i <= info_conteudo->num_arq; i++){
		v_diretorio[i]->posicao -= v_diretorio[id_arq]->tamanho;
	}
	for (int i = id_arq; i < info_conteudo->num_arq; i++){
		v_diretorio[i] = v_diretorio[i + 1];
	}
	
	//corta o archive no final dos conteudos
	ftruncate(fileno(archive), ftell(archive) - 1);
}

void exclui_arg(char* nome_archive, FILE* archive, struct diretorio* v_diretorio[], int num_arq, char** argv, int optind){
	/*a funcao exclui todos os arquivos passados como argumento, para isso ela recebe o nome do archive, um ponteiro para 
	ele, o vetor de diretorio e as informcoes dos argumentos*/
	int id_arq;
	struct conteudo* info_conteudo = conteudo(archive);

	for (int i = optind; i < num_arq; i++){
		id_arq = id_arquivo(archive, argv[i], v_diretorio);
		if (id_arq == -1)	//arquivo nao esta no archive
			fprintf(stderr, "arquivo %s nao existe no archive\n", argv[i]);
		else 
			exclui(nome_archive, archive, v_diretorio, id_arq, info_conteudo);
	}

	//atualiza o comeco dos conteudos e adiciona o diretorio
	att_info_conteudo(archive, info_conteudo);
	fseek(archive, 0, SEEK_END);
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
}

void move(FILE* archive, char* nome_target, char* nome_arquivo, struct diretorio* v_diretorio[]){
	/*a funcao move nos diretorios os arquivos passados como argumento, para isso ela recebe um ponteiro 
	para o archive, o nome do target e do atquivo a ser movido e o vetor de diretorio*/
	int target = id_arquivo(archive, nome_target, v_diretorio);
	int arquivo = id_arquivo(archive, nome_arquivo, v_diretorio);
	if ((target < 0) || (arquivo < 0)){
		fprintf(stderr, "arquivo nao encontrado\n");
		exit(1);
	}

	struct diretorio* temp = v_diretorio[arquivo];

	if (arquivo > target){	//se o arquivo estiver a frente do target, move para tras
		for (int i = arquivo; i > target; i--){
			v_diretorio[i] = v_diretorio[i - 1];
		}
		v_diretorio[target + 1] = temp;

	} else if (arquivo < target){	//se o arquivo estiver atras do target, move para frente
		for (int i = arquivo; i < target; i++){
			v_diretorio[i] = v_diretorio[i + 1];
		}
		v_diretorio[target] = temp;
	}

	//imprime o diretorio no fim do archive
	struct conteudo* info_conteudo = conteudo(archive);
	fseek(archive, info_conteudo->tam_conteudo + sizeof(long long int) + sizeof(int), SEEK_SET);
	imprime_diretorio(archive, v_diretorio, info_conteudo->num_arq);
}

void lista_arq(FILE* archive, struct diretorio* v_diretorio[]){
	/*a funcao lista os arquivos presentes no archive, para 
	isso ela recebe um ponteiro para ele e um vetor de diretorios*/
	char permissoes[10];
	char tempo[20];
	int num_arq;

	fseek(archive, 0, SEEK_SET);
	fread(&num_arq, sizeof(int), 1, archive);
	for (int i = 0; i < num_arq; i++){
		//prepara as informacoes de permissoes 
		permissoes[0] = (v_diretorio[i]->permissoes & S_IRUSR) ? 'r' : '-';
		permissoes[1] = (v_diretorio[i]->permissoes & S_IWUSR) ? 'w' : '-';
		permissoes[2] = (v_diretorio[i]->permissoes & S_IXUSR) ? 'x' : '-';
		permissoes[3] = (v_diretorio[i]->permissoes & S_IRGRP) ? 'r' : '-';
		permissoes[4] = (v_diretorio[i]->permissoes & S_IWGRP) ? 'w' : '-';
		permissoes[5] = (v_diretorio[i]->permissoes & S_IXGRP) ? 'x' : '-';
		permissoes[6] = (v_diretorio[i]->permissoes & S_IROTH) ? 'r' : '-';
		permissoes[7] = (v_diretorio[i]->permissoes & S_IWOTH) ? 'w' : '-';
		permissoes[8] = (v_diretorio[i]->permissoes & S_IXOTH) ? 'x' : '-';
		permissoes[9] = '\0';

		//transforma uid em string
		struct passwd *pw = getpwuid(v_diretorio[i]->uid);
		strftime(tempo, 20, "%Y-%m-%d %H:%M", localtime(&v_diretorio[i]->ultima_modificacao));

		printf("%s %s %8lld %s %s\n", permissoes, pw->pw_name, 
			v_diretorio[i]->tamanho, tempo, v_diretorio[i]->nome);
	}
}
