#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "manipula_conteudo.h"
#include "manipula_dir.h"
#include "funcoes_principais.h"
#include "abre_arquivos.h"

int main(int argc, char **argv){
	int cont = 0, inserir = 0, substituir = 0, extrair = 0, excluir = 0, mover = 0, listar = 0;
	char *opcoes = "iam:xrch", *nome_archive, *nome_target, opt;
	FILE *archive;
	struct diretorio* *v_diretorio;
	//processa os argumentos da linha de comando
	while((opt = getopt(argc, argv, opcoes)) != -1){
		cont++;
		if (cont > 1){
			fprintf(stderr, "Apenas uma opcao pode ser passada\n");
			return 1;
		}

		switch (opt){
			case 'i':
				inserir = 1;
				break;
			case 'a':
				substituir = 1;
				break;
			case 'm':
				mover = 1;
				nome_target = optarg;
				break;
			case 'x':
				extrair = 1;
				break;
			case 'r':
				excluir = 1;
				break;
			case 'c':
				listar = 1;
				break;
			case 'h':
				printf("O programa deve ser executado da seguinte forma:\n./vina++ <opção> <archive> [membro1 membro2 ...]\n");
				return 0;
				break;
			//avisa se passou a especificacao errada
			default:
				fprintf(stderr, "opcao invalida: %c\n", optopt);
				return 1;
		}
	}

	if ((optind >= argc) && (!listar)){ 	//nao foi passado o nome do arquivo
		fprintf(stderr, "nome do arquivo faltando\n");
		return 1;
	}
	nome_archive = argv[optind];

	if (inserir){
		archive = cria_arquivo(nome_archive);
		v_diretorio = le_diretorio(archive);

		v_diretorio = insere_arg(nome_archive, archive, v_diretorio, argc, argv, optind + 1);
	} else if (substituir){
		archive = cria_arquivo(nome_archive);
		v_diretorio = le_diretorio(archive);

		v_diretorio = substitui_arg(nome_archive, archive, v_diretorio, argc, argv, optind + 1);
	} else if (mover){
		archive = abre_arquivo(nome_archive);
		v_diretorio = le_diretorio(archive);

		move(archive, nome_target, argv[optind + 1], v_diretorio);
	} else if (extrair){
		archive = abre_arquivo(nome_archive);
		v_diretorio = le_diretorio(archive);

		extrai_arg(argc, argv, archive, v_diretorio);
	} else if (excluir){
		archive = abre_arquivo(nome_archive);
		v_diretorio = le_diretorio(archive);

		exclui_arg(nome_archive, archive, v_diretorio, argc, argv, optind + 1);
	} else if (listar){
		archive = abre_arquivo(nome_archive);
		v_diretorio = le_diretorio(archive);

		lista_arq(archive, v_diretorio);
	}

	desaloca_diretorio(archive, v_diretorio);
	fclose(archive);
}
