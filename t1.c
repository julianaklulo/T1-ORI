#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define registrosPorBloco 5


typedef struct {
	int id;
	char nome[92];
	int nota;
} Tregistro; // cada registro possui 100 bytes


typedef struct {
	int id;
	int ultimoRegistroRemovido;
	int ultimoRegistroDoBloco;
	Tregistro registrosDoBloco[registrosPorBloco];
} Tbloco; // cada bloco possui 512 bytes, acomodando 5 registros


typedef struct {
	int ultimoBlocoComRemocao;
	int ultimoBlocoDoArquivo;
} Tcabecalho; // cabeçalho do arquivo possui 8 bytes