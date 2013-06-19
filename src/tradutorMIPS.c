#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "lib/libMIPS.h"

FILE *fInput;
FILE *fOutput;
unsigned char newLine = 0;

void escrevePalavra()
{
	unsigned char i = 0;
	for(i=0; i<32; i++)
	{
		//Adiciona 48 ao valor contido no vetor, para ser representado como o ASCII '0' ou '1'
		fprintf(fOutput, "%c", palavra[i] + 48);
		printf("%c", palavra[i]+48);
	}
	if(newLine)
	{
		fputs("\n", fOutput);
	}
	printf("\n");
}

int main(int argc, char **argv) {
	char linha[100];
	char inputFilePath;
	
	//Tratamento dos argumentos
	if(argc < 3)
	{
		fprintf(stderr, "Argumentos invalidos.\nUso: tradutorMIPS INPUTFILE OUTPUTFILE [--newline]\n\tINPUTFILE: Caminho para o arquivo de entrada\n\tOUTPUTFILE: Caminho onde será salvo o arquivo de saida, se já existir será sobreescrito.\n\t--newline: Opção que habilita a quebra de linha ao final de cada palavra.\n");
		return EINVAL;  //Invalid argument
	}
	
	//Arquivo de leitura
	fInput = fopen(argv[1], "r");
	if(fInput == NULL)
	{
		fprintf(stderr, "Houve um erro na leitura do arquivo '%s'\n%s\n", argv[1], strerror(errno));
		return errno;
	}
	
	//Arquivo de saida
	fOutput = fopen(argv[2], "w+");
	if(fOutput == NULL)
	{
		fprintf(stderr, "Houve um erro na escrita do arquivo '%s'\n%s\n", argv[2], strerror(errno));
		return errno;
	}
	
	//Trata o argumento --newline (somente na posição 4!)
	if(argc > 3)
	{
		//Só seta o valor 1 se a strcmp retornar 0
		newLine = !strcmp(argv[3], "--newline");	
	}
	
	while(fgets(linha, 100, fInput) != NULL)
	{
		if(processarInstrucao(linha))
		{
			escrevePalavra();
		}
		else
		{
			fprintf(stderr, "Não foi possível processar a instrução %s%s\n", linha, strerror(errno));
			return 1;
		}
	}
	fclose(fInput);
	fclose(fOutput);
	return 0;
};
