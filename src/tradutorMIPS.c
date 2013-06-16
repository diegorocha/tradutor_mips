#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//Variáveis globais
FILE *fInput;
FILE *fOutput;
unsigned char newLine = 0;
unsigned char palavra[32];

//Instruções Tipo R
char R_MNE[8][4] = {"sll", "srl", "jr", "add", "sub", "and", "or", "slt"};
unsigned char R_FUNC[8][6] = {0, 0, 0, 0, 0, 0, //00 = sll
															0, 0, 0, 0, 1, 0, //02 = srl
															0, 0, 0, 1, 0, 0, //08 = jr
															1, 0, 0, 0, 0, 0, //32 = add
															1, 0, 0, 0, 1, 0, //34 = sub
															1, 0, 0, 1, 0, 0, //36 = and
															1, 0, 0, 1, 0, 1, //37 = or
															1, 0, 1, 0, 1, 0};//42 = slt


//Registradores [num_reg][bytes]
char REG_MNE[32][6] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
                         "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                         "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                         "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
unsigned char REG[32][5] = {0, 0, 0, 0, 0, //00 = $zero
                            0, 0, 0, 0, 1, //01 = $at
                            0, 0, 0, 1, 0, //02 = $v0
                            0, 0, 0, 1, 1, //03 = $v1
                            0, 0, 1, 0, 0, //04 = $a0
                            0, 0, 1, 0, 1, //05 = $a1
                            0, 0, 1, 1, 0, //06 = $a2
                            0, 0, 1, 1, 1, //07 = $a3
                            0, 1, 0, 0, 0, //08 = $t0
                            0, 1, 0, 0, 1, //09 = $t1
                            0, 1, 0, 1, 0, //10 = $t2
                            0, 1, 0, 1, 1, //11 = $t3
                            0, 1, 1, 0, 0, //12 = $t4
                            0, 1, 1, 0, 1, //13 = $t5
                            0, 1, 1, 1, 0, //14 = $t6
                            0, 1, 1, 1, 1, //15 = $t7
                            1, 0, 0, 0, 0, //16 = $s0
                            1, 0, 0, 0, 1, //17 = $s1
                            1, 0, 0, 1, 0, //18 = $s2
                            1, 0, 0, 1, 1, //19 = $s3
                            1, 0, 1, 0, 0, //20 = $s4
                            1, 0, 1, 0, 1, //21 = $s5
                            1, 0, 1, 1, 0, //22 = $s6
                            1, 0, 1, 1, 1, //23 = $s7
                            1, 1, 0, 0, 0, //24 = $t8
                            1, 1, 0, 0, 1, //25 = $t9
                            1, 1, 0, 1, 0, //26 = $k0
                            1, 1, 0, 1, 1, //27 = $k1
                            1, 1, 1, 0, 0, //28 = $gp
                            1, 1, 1, 0, 1, //29 = $sp
                            1, 1, 1, 1, 0, //30 = $fp
                            1, 1, 1, 1, 1};//31 = $ra


/*
void escreveRegistrador(unsigned char *reg)
{
	unsigned int i = 0;
	for(i = 0; i<5; i++)
	{
		printf("%c", reg[i] + 48);
	}
	puts("\n");
}*/

/*
Função auxiliar que converte uma cadeia de caracteres em minúsculo
*/
void toLowerCase(char *s)
{
	for(; *s; ++s)
	{
		*s = tolower(*s);
	}
}

/*
Função que copia um subcadeia de bits de um vetor para outro.
*/
void copiaBits(unsigned char *origem, unsigned char inicio, unsigned char qtd, unsigned char *destino, unsigned char inicioDestino)
{
	//Tive que implementar essa função pois o memcpy não estava funcionando corretamente.
	unsigned char i = 0;
	unsigned char fim = inicio + qtd;
	unsigned char destinoB = inicioDestino;
	for(i=inicio; i<fim; i++)
	{
		destino[destinoB] = origem[i];
		destinoB++;
	}
}

/*
Função que copia um subcadeia de caracteres de um vetor para outro.
*/
void copiaString(char *origem, char *destino, unsigned char inicio, unsigned char qtd)
{
	//Tive que implementar essa função pois o strncpy não estava funcionando corretamente.
	unsigned char i = 0;
	unsigned char fim = inicio + qtd;
	unsigned char destinoC = 0;
	for(i=inicio; i<fim; i++)
	{
		destino[destinoC] = origem[i];
		destinoC++;
	}
	destino[destinoC] = '\0';
}

void inicializaPalavra()
{
	//Inicializa os 32 bytes do vetor com o valor 0
	memset(palavra, 0, 32);
}

void escrevePalavra()
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

/*
Escreve o valor contido em imediato nos últimos bits da palavra
*/
unsigned char setImediato(int imediato)
{
	unsigned char bit = 31;
	int num = imediato;
	//Converte para binario atraves das divisoes sucessivas
	while(imediato > 0)
	{
		palavra[bit] = imediato % 2;
		imediato /= 2;
		bit--;
	}
}

/*
Verifica se a string registrador possui o nome de um registrador valido.
Em caso positivo retorna 1 e numero do registrador em bytes.
Em caso negativo retorna 0 e bytes é completamente zerado.
*/
unsigned char getRegistradorBytes(char *registrador, unsigned char *bytes)
{
	unsigned char i = 0;
	//Inicializa o vetor de saída com valor zero.
	memset(bytes, 0, 5);

	if(strcmp(registrador, "$0")==0)
	{
		copiaBits(REG[0], 0, 5, bytes, 0);
		return 1;
	}

	for(i=0; i<32; i++)
	{
		if(strcmp(registrador, REG_MNE[i])==0)
		{
			copiaBits(REG[i], 0, 5, bytes, 0);
			return 1;
		}
	}
	
	return 0;
}

/*
Separa a instrução continda em *linha
Em *instrução e três operandos *op1,*op2 e *op3.
Caso um operando não exista ele ficará com a string vazia.
Obs: Esse método não verifica a sintáxe.
*/
void divideInstrucao(char *linha, char *instrucao, char *op1, char *op2, char *op3)
{
	//Separa a linha em instrução e três operandos
	unsigned char fc = 0, lc = 0;
	strcpy(instrucao, "");
	strcpy(op1, "");
	strcpy(op2, "");
	strcpy(op3, "");
	toLowerCase(linha);
	//Pega a instrucao. Para no primeiro espaço
	while(linha[lc] && linha[lc] != ' ')
	{
		lc++;
	}
	//Copia a substring linha do inicio até lc (LastCharacter).
	copiaString(linha, instrucao, 0, lc);

	//Procura um caractere valido para op1
	fc = lc + 1;
	while(linha[fc] && (linha[fc] == ',' || linha[fc] == ' '))
	{
		fc++;
	}
	lc = fc + 1;
	while(linha[lc] && linha[lc] != ',' && linha[lc] != ' ')
	{
		lc++;
	}
	copiaString(linha, op1, fc, lc - fc);

	//Procura um caractere valido para op2
	fc = lc + 1;
	while(linha[fc] && (linha[fc] == ',' || linha[fc] == ' '))
	{
		fc++;
	}
	lc = fc + 1;
	while(linha[lc] && linha[lc] != ',' && linha[lc] != ' ' && linha[lc] != '(' && linha[lc] != '\n')
	{
		lc++;
	}
	copiaString(linha, op2, fc, lc - fc);

	//Procura um caractere valido para op3
	fc = lc + 1;
	while(linha[fc] && (linha[fc] == ',' || linha[fc] == ' ' || linha[fc] == '('))
	{
		fc++;
	}
	lc = fc + 1;
	while(linha[lc] && linha[lc] != ',' && linha[lc] != ' ' && linha[lc] != ')' && linha[lc] != '\n')
	{
		lc++;
	}
	copiaString(linha, op3, fc, lc - fc);
}

void palavraTipoR(unsigned char *rs, unsigned char *rt, unsigned char *rd, unsigned char *func)
{
		copiaBits(rs, 0, 5, palavra, 6);
		copiaBits(rt, 0, 5, palavra, 11);
		copiaBits(rd, 0, 5, palavra, 16);
		copiaBits(func, 0, 6, palavra, 26);
}

/*
Processa a linha.
Retorna 0 em caso de falha, 1 em caso de sucesso.
*/
unsigned char processarLinha(char *linha)
{
	char instrucao[10], op1[5], op2[5], op3[5];
	unsigned char bRS[5], bRT[5], bRD[5];
	unsigned char i = 0;
	
	//TODO: Retirar
	printf("Processando linha: %s", linha);
	
	inicializaPalavra();
	divideInstrucao(linha, instrucao, op1, op2, op3);
	
	//TODO: Retirar
	printf("Resultado: Ins = '%s'\tOp1 = '%s'\tOp2 = '%s'\tOp3 = '%s'\n", instrucao, op1, op2, op3);

	//Verifica se a instrução é do tipo R
	for(i = 0; i<8; i++)
	{
		if(strcmp(instrucao, R_MNE[i]) == 0)
		{
			//Tipo R: op = 0; rs = op2; rt = op3; rd = op1; shmat = 0; func  = 6 bits;
			//rs
			if(!getRegistradorBytes(op2, bRS))
			{
				return 0;
			}

			//rt
			if(!getRegistradorBytes(op3, bRT))
			{
				return 0;
			}

			//rd
			if(!getRegistradorBytes(op1, bRD))
			{
				return 0;
			}
			palavraTipoR(bRS, bRT, bRD, R_FUNC[i]);
		}
	}
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
		processarLinha(linha);
		escrevePalavra();
	}
	fclose(fInput);
	fclose(fOutput);
	return 0;
};
