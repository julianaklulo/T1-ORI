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


/* Função que cria o registro, lendo do teclado os dados
   Retorna:
   registro com os dados inseridos */
Tregistro criaRegistro() {
	// lê do teclado os dados do registro
	Tregistro aluno;

	printf("Digite o ID do aluno: ");
	scanf("%d", &aluno.id);
	printf("Digite o Nome do aluno: ");
	scanf("%s", &aluno.nome);
	printf("Digite a Nota do aluno: ");
	scanf("%d", aluno.nota);

	return aluno;
}


/* Função que escreve um registro no bloco
   Parâmetros: registro a ser gravado
   Retorna:
   1 se a escrita ocorreu
   0 caso tenha ocorrido erro */
int insercao(Tregistro *registro) {
	printf("\nInserção\n\n");

	// tenta abrir o arquivo
	FILE *arquivo = fopen("trabalho_ORI.bin", "a+b");
	if (arquivo == NULL) { // caso dê erro, avisa e encerra
		printf("Houve um problema na abertura do arquivo, tente novamente!");
		return 0;
	}

	// lê o último bloco
	fseek(arquivo, -sizeof(Tbloco), SEEK_END);
	Tbloco ultimoBloco;
	fread(&ultimoBloco, sizeof(Tbloco), 1, arquivo);

	// insere no último bloco
	ultimoBloco.ultimoRegistroDoBloco++;
	ultimoBloco.registrosDoBloco[ultimoBloco.ultimoRegistroDoBloco] = *registro;

	// grava o bloco modificado
	fseek(arquivo, -sizeof(Tbloco), SEEK_END);
	fwrite(&ultimoBloco, sizeof(Tbloco), 1, arquivo);

	// fecha o arquivo
	fclose(arquivo);

	// confere se o bloco ficou cheio
	if (ultimoBloco.ultimoRegistroDoBloco == registrosPorBloco - 1) {
		// tenta abrir o arquivo
			FILE *arquivo = fopen("trabalho_ORI.bin", "a+b");
			if (arquivo == NULL) { // caso dê erro, avisa e encerra
				printf("Houve um problema na abertura do arquivo, tente novamente!");
				return 0;
			}

		// cria o bloco novo
		Tbloco novoBloco;
		novoBloco.id = ultimoBloco.id + 1;
		novoBloco.ultimoRegistroRemovido = -1;
		novoBloco.ultimoRegistroDoBloco = -1;

		// grava o novo bloco
		fseek(arquivo, 0, SEEK_END);
		fwrite(&novoBloco, sizeof(Tbloco), 1, arquivo);

		// fecha o arquivo
		fclose(arquivo);

		// atualiza o cabeçalho
		atualizaCabecalho();
	}

	return 1;
}
