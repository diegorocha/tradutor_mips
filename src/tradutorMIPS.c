#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Instruções e funções

unsigned char ADD[6] = {1, 0, 0, 0, 0, 0}; //32


//Matriz de registradores [num_reg][bytes]

char REG_MNE[32][6] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
                         "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", 
                         "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                         "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
                       
unsigned char REG[32][5] = {0, 0, 0, 0, 0, //00 = $zero / 
                            0, 0, 0, 0, 1,
                            0, 0, 0, 1, 0,
                            0, 0, 0, 1, 1,
                            0, 0, 1, 0, 0,
                            0, 0, 1, 0, 1,
                            0, 0, 1, 1, 0,
                            0, 0, 1, 1, 1,
                            0, 1, 0, 0, 0,
                            0, 1, 0, 0, 1,
                            0, 1, 0, 1, 0,
                            0, 1, 0, 1, 1,
                            0, 1, 1, 0, 0,
                            0, 1, 1, 0, 1,
                            0, 1, 1, 1, 0,
                            0, 1, 1, 1, 1,
                            1, 0, 0, 0, 0,
                            1, 0, 0, 0, 1,
                            1, 0, 0, 1, 0,
                            1, 0, 0, 1, 1,
                            1, 0, 1, 0, 0,
                            1, 0, 1, 0, 1,
                            1, 0, 1, 1, 0,
                            1, 0, 1, 1, 1,
                            1, 1, 0, 0, 0,
                            1, 1, 0, 0, 1,
                            1, 1, 0, 1, 0,
                            1, 1, 0, 1, 1,
                            1, 1, 1, 0, 0,
                            1, 1, 1, 0, 1,
                            1, 1, 1, 1, 0,
                            1, 1, 1, 1, 1};

unsigned char palavra[32];

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
		printf("%c", palavra[i] + 48);
	}
	printf("\n");
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
		memcpy(bytes, REG[0], 5);
		return 1;
	}
	
	for(i=0; i<32; i++)
	{
		if(strcmp(registrador, REG_MNE[i])==0)
		{
			memcpy(bytes, REG[i], 5);
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
	strncpy(instrucao, linha, lc);
	
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
	strncpy(op1, linha + fc, lc - fc);
	
  //Procura um caractere valido para op2
	fc = lc + 1;
	while(linha[fc] && (linha[fc] == ',' || linha[fc] == ' '))
	{
		fc++;
	}
	lc = fc + 1;
	while(linha[lc] && linha[lc] != ',' && linha[lc] != ' ' && linha[lc] != '(')
	{
		lc++;
	}
	strncpy(op2, linha + fc, lc - fc);
	
	//Procura um caractere valido para op3
	fc = lc + 1;
	while(linha[fc] && (linha[fc] == ',' || linha[fc] == ' ' || linha[fc] == '('))
	{
		fc++;
	}
	lc = fc + 1;
	while(linha[lc] && linha[lc] != ',' && linha[lc] != ' ' && linha[lc] != ')')
	{
		lc++;
	}
	strncpy(op3, linha + fc, lc - fc);
}

void palavraTipoR(unsigned char *rs, unsigned char *rt, unsigned char *rd, unsigned char *func)
{		
		//memcpy(palavra, opcode, 6); -> opcode = 0; OK
		memcpy(palavra + 6, rs, 5);
		memcpy(palavra + 11, rt, 5);
		memcpy(palavra + 16, rd, 5);
		memcpy(palavra + 26, func, 6);
}

/*
Processa a linha.
Retorna 0 em caso de falha, 1 em caso de sucesso.
*/
unsigned char processarLinha(char *linha)
{
	char instrucao[10], op1[5], op2[5], op3[5];
	unsigned char bRS[5], bRT[5], bRD[5];
	printf("%s\n", linha);		
	inicializaPalavra();
	divideInstrucao(linha, instrucao, op1, op2, op3);
	//Verifica o tipo de operação
	if(strcmp(instrucao, "add") == 0)
	{
		//Tipo R: op = 0; rs = op2; rt = op3; rd = op1; shmat = 0; func  = 32;
		
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
		if(!getRegistradorBytes(op1, bRT))
		{
			return 0;
		}
		
		palavraTipoR(bRS, bRT, bRD, ADD);
	}
	//printf("%s\nIns: '%s'\nOp1: '%s'\nOp2: '%s'\nOp3: '%s'\n", linha, instrucao, op1, op2, op3);
}

int main(int argc, char *argv[5]) {
	char line[100] = "add $t0, $s2, $t0";
	int i = 0;
	processarLinha(line);
	escrevePalavra();
	return 0;
};
