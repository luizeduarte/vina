CC = gcc
CFLAGS = -g -Wall

all: vina++

vina++: main.o funcoes_principais.o manipula_dir.o manipula_conteudo.o abre_arquivos.o
	$(CC) $^ -o vina++

main.o: src/main.c
	$(CC) -c $(CFLAGS) src/main.c -o main.o

funcoes_principais.o: src/funcoes_principais.c
	$(CC) -c $(CFLAGS) src/funcoes_principais.c -o funcoes_principais.o

manipula_dir.o: src/manipula_dir.c
	$(CC) -c $(CFLAGS) src/manipula_dir.c -o manipula_dir.o

manipula_conteudo.o: src/manipula_conteudo.c
	$(CC) -c $(CFLAGS) src/manipula_conteudo.c -o manipula_conteudo.o

abre_arquivos.o: src/abre_arquivos.c
	$(CC) -c $(CFLAGS) src/abre_arquivos.c -o abre_arquivos.o

clean:
	rm -f *.o

purge:
	rm -f *.o vina++