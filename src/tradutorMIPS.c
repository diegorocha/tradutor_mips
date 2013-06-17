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
unsigned int R_NO_SHAMT = 0;

//Instruções Tipo I
char I_MNE[5][5] = {"beq", "bne", "addi", "lw", "sw"};
unsigned char I_OPCODE[5][6] = {0, 0, 0, 1, 0, 0, //04 = beq
															  0, 0, 0, 1, 0, 1, //05 = bne
															  0, 0, 1, 0, 0, 0, //08 = addi
															  1, 0, 0, 0, 1, 1, //35 = lw
															  1, 0, 1, 0, 1, 1};//43 = sw

//Instruções Tipo J
char J_MNE[2][4] = {"j", "jal"};
unsigned char J_OPCODE[5][6] = {0, 0, 0, 0, 1, 0, //02 = j
															  0, 0, 0, 0, 1, 1};//03 = jal


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
	memset(palavra, 0, 32);
}

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

void setBinario(unsigned int value, unsigned int bitInicial)
{
	unsigned char bit = bitInicial;
	//Converte para binario atraves das divisoes sucessivas
	while(value > 0)
	{
		palavra[bit] = value % 2;
		value /= 2;
		bit--;
	}
}

void setShamt(unsigned int shamt)
{
	setBinario(shamt, 25);
}

void setImediato(unsigned int imediato)
{
	setBinario(imediato, 31);
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
	while(linha[lc] && linha[lc] != ',' && linha[lc] != ' ' && linha[lc] != '\n')
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

void palavraTipoR(unsigned char *rs, unsigned char *rt, unsigned char *rd, unsigned int shamt, unsigned char *func)
{
		copiaBits(rs, 0, 5, palavra, 6);
		copiaBits(rt, 0, 5, palavra, 11);
		copiaBits(rd, 0, 5, palavra, 16);
		setShamt(shamt);
		copiaBits(func, 0, 6, palavra, 26);
}

void palavraTipoI(unsigned char *opCode, unsigned char *rs, unsigned char *rt, unsigned int imediato)
{
		copiaBits(opCode, 0, 6, palavra, 0);
		copiaBits(rs, 0, 5, palavra, 6);
		copiaBits(rt, 0, 5, palavra, 11);
		setImediato(imediato);
}

void palavraTipoJ(unsigned char *opCode, unsigned int imediato)
{
		copiaBits(opCode, 0, 6, palavra, 0);
		setImediato(imediato);
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
	unsigned int imediato = 0;
	unsigned int shamt = 0;
	unsigned int retorno = 0; //flag erro
	
	printf("%s", linha);
	
	inicializaPalavra();
	divideInstrucao(linha, instrucao, op1, op2, op3);
	
	//printf("Resultado: Ins = '%s'\tOp1 = '%s'\tOp2 = '%s'\tOp3 = '%s'\n\n", instrucao, op1, op2, op3);

	//Verifica se a instrução é do tipo R
	for(i = 0; i<8; i++)
	{
		if(strcmp(instrucao, R_MNE[i]) == 0)
		{
			retorno = 1; //Identificou a instrução
			if(i > 2)
			{
				//Tipo R: op = 0; rs = op2; rt = op3; rd = op1; shmat = 0; func  = R_FUNC[i]; (sll, srl e jr são exceções)
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
				
				//Com rs, rt, rt e sem shamt
				palavraTipoR(bRS, bRT, bRD, R_NO_SHAMT, R_FUNC[i]);
			}
			else
			{
				//Exceções
				if(i == 2)
				{
					//jr: op = 0; rs = op1; rt = 0; rd = 0; shmat = 0; func  = R_FUNC[i];
					//rs = op1
					if(!getRegistradorBytes(op1, bRS))
					{
						return 0;
					}
					palavraTipoR(bRS, REG[0], REG[0], R_NO_SHAMT, R_FUNC[i]);
				}
				else
				{
					//sll e srl: op = 0; rs = op2; rt = 0; rd = op1; shmat = op3; func  = R_FUNC[i];
					//rs = op2
					if(!getRegistradorBytes(op2, bRS))
					{
						return 0;
					}
				
					//rd = op1
					if(!getRegistradorBytes(op1, bRD))
					{
						return 0;
					}
					
					//shamt
					sscanf(op3, "%d", &shamt);
					if(shamt > 31)
					{
						return 0;
					}
					
					palavraTipoR(bRS, REG[0], bRD, shamt, R_FUNC[i]);
				}
			}
			break;
		}
	}
	
	//Verifica se a instrução é do tipo I
	for(i = 0; i<5; i++)
	{
		if(strcmp(instrucao, I_MNE[i]) == 0)
		{		
			retorno = 1; //Identificou a instrução
			if(i < 3)
			{
				//Tipo I (regular): op = I_OPCODE[i]; rs = op2; rt = op1; imediato = op3 (lw e sw são exceções)

				//rs
				if(!getRegistradorBytes(op2, bRS))
				{
					return 0;
				}
						
				//imediato
				sscanf(op3, "%d", &imediato);
			}
			else
			{
				//lw/sw : op = I_OPCODE[i]; rs = op3; rt = op1; imediato = op2

				//rs
				if(!getRegistradorBytes(op3, bRS))
				{
					return 0;
				}
						
				//imediato
				sscanf(op2, "%d", &imediato);
			}
			
			//rt
			if(!getRegistradorBytes(op1, bRT))
			{
				return 0;
			}

			if(imediato > 65535)
			{
				return 0;
			}

			palavraTipoI(I_OPCODE[i], bRS, bRT, imediato);
			break;
		}
	}
	
	//Verifica se a instrução é do tipo J
	for(i = 0; i<2; i++)
	{
		if(strcmp(instrucao, J_MNE[i]) == 0)
		{
			retorno = 1; //Identificou a instrução
			//imediato
			sscanf(op1, "%d", &imediato);
			if(imediato > 67108863)
			{
				return 0;
			}
			palavraTipoJ(J_OPCODE[i], imediato);
		}
	}
	
	return retorno;
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
		if(processarLinha(linha))
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
