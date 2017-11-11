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


/* Função que inicializa o arquivo de dados
   Retorna:
   1 se a criação ocorreu
   0 caso tenha ocorrido erro */
int criacao() {
	printf("\nCriação\n\n");

	// cria um arquivo novo
	FILE *arquivo = fopen("trabalho_ORI.bin", "w+b");
	if (arquivo == NULL) {
		printf("Houve um problema na abertura do arquivo, tente novamente!");
		return 0;
	}
	
	// cria o cabeçalho
	Tcabecalho cabecalho;
	cabecalho.ultimoBlocoComRemocao = -1;
	cabecalho.ultimoBlocoDoArquivo = 0;
	
	// cria o primeiro bloco
	Tbloco primeiroBloco;
	primeiroBloco.id = 0;
	primeiroBloco.ultimoRegistroRemovido = -1;
	primeiroBloco.ultimoRegistroDoBloco = -1;
	
	// grava o cabeçalho no arquivo
	fseek(arquivo, 0, SEEK_SET);
	fwrite(&cabecalho, sizeof(Tcabecalho), 1,  arquivo);

	// grava o primeiro bloco no arquivo
	fwrite(&primeiroBloco, sizeof(Tbloco), 1, arquivo);
	
	// fecha o arquivo
	fclose(arquivo);

	printf("\nArquivo criado!\n\n");

	return 1;
}	


/* Função que atualiza o cabeçalho quando um novo bloco é criado
   Retorna:
   1 se a atualização ocorreu
   0 caso tenha ocorrido erro */
int atualizaCabecalho() {
	// tenta abrir o arquivo
	FILE *arquivo = fopen("trabalho_ORI.bin", "r+b");
	if (arquivo == NULL) { // caso dê erro, avisa e encerra
		printf("Houve um problema na abertura do arquivo, tente novamente!");
		return 0;
	}

	// lê o cabeçalho
	Tcabecalho cabecalho;
	fread(&cabecalho, sizeof(Tcabecalho), 1, arquivo);
	
	// atualiza o cabecalho
	cabecalho.ultimoBlocoDoArquivo++;
	
	// grava o cabeçalho atualizado
	fseek(arquivo, 0, SEEK_SET);
	fwrite(&cabecalho, sizeof(Tcabecalho), 1, arquivo);
	
	// fecha o arquivo
	fclose(arquivo);

	return 1;
}