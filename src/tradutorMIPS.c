#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "lib/libMIPS.h"

#define MAX_CHAR_LINE 100 

void escrevePalavra(FILE *fOutput, unsigned char newLine)
{
	unsigned char i = 0;
	for(i=0; i<32; i++)
	{
		//Adiciona 48 ao valor contido no vetor, para ser representado como o ASCII '0' ou '1'
		fprintf(fOutput, "%c", palavra[i] + 48);
	}
	if(newLine)
	{
		fputs("\n", fOutput);
	}
}

int main(int argc, char **argv) {
	FILE *fInput;
	FILE *fOutput;
	unsigned char newLine = 0;
	char linha[MAX_CHAR_LINE];
	char inputFilePath;
	char *errorMessage;
	
	//Tratamento dos argumentos
	if(argc < 3)
	{
		fprintf(stderr, "Argumentos invalidos.\nUso: tradutorMIPS INPUTFILE OUTPUTFILE [--newline]\n\tINPUTFILE: Caminho para o arquivo de entrada\n\tOUTPUTFILE: Caminho onde será salvo o arquivo de saida, se já existir será sobreescrito.\n\t--newline: Opção que habilita a quebra de linha ao cada 32 bits.\n");
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
		fclose(fInput);
		return errno;
	}
	
	//Trata o argumento --newline (somente na posição 4!)
	if(argc > 3)
	{
		//Só seta o valor 1 se a strcmp retornar 0
		newLine = !strcmp(argv[3], "--newline");	
	}
	
	while(fgets(linha, MAX_CHAR_LINE, fInput) != NULL)
	{
		if(processarInstrucao(linha))
		{
			escrevePalavra(fOutput, newLine);
		}
		else
		{
			fprintf(stderr, "Não foi possível processar a instrução %s%s\n", linha, libMipsErrorMessage(errno));
			return 1;
		}
	}
	fclose(fInput);
	fclose(fOutput);
	printf("%s\n", strerror(errno));
	return 0;
};
