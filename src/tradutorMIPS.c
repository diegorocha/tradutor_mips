#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Registradores
unsigned char R_ZERO[5] = {0, 0, 0, 0, 0}; //00
unsigned char R_AT[5]   = {0, 0, 0, 0, 1}; //01
unsigned char R_V0[5]   = {0, 0, 0, 1, 0}; //02
unsigned char R_V1[5]   = {0, 0, 0, 1, 1}; //03
unsigned char R_A0[5]   = {0, 0, 1, 0, 0}; //04
unsigned char R_A1[5]   = {0, 0, 1, 0, 1}; //05
unsigned char R_A2[5]   = {0, 0, 1, 1, 0}; //06
unsigned char R_A3[5]   = {0, 0, 1, 1, 1}; //07
unsigned char R_T0[5]   = {0, 1, 0, 0, 0}; //08
unsigned char R_T1[5]   = {0, 1, 0, 0, 1}; //09
unsigned char R_T2[5]   = {0, 1, 0, 1, 0}; //10
unsigned char R_T3[5]   = {0, 1, 0, 1, 1}; //11
unsigned char R_T4[5]   = {0, 1, 1, 0, 0}; //12
unsigned char R_T5[5]   = {0, 1, 1, 0, 1}; //13
unsigned char R_T6[5]   = {0, 1, 1, 1, 0}; //14
unsigned char R_T7[5]   = {0, 1, 1, 1, 1}; //15
unsigned char R_S0[5]   = {1, 0, 0, 0, 0}; //16
unsigned char R_S1[5]   = {1, 0, 0, 0, 1}; //17
unsigned char R_S2[5]   = {1, 0, 0, 1, 0}; //18
unsigned char R_S3[5]   = {1, 0, 0, 1, 1}; //19
unsigned char R_S4[5]   = {1, 0, 1, 0, 0}; //20
unsigned char R_S5[5]   = {1, 0, 1, 0, 1}; //21
unsigned char R_S6[5]   = {1, 0, 1, 1, 0}; //22
unsigned char R_S7[5]   = {1, 0, 1, 1, 1}; //23
unsigned char R_T8[5]   = {1, 1, 0, 0, 0}; //24
unsigned char R_T9[5]   = {1, 1, 0, 0, 1}; //25
unsigned char R_K0[5]   = {1, 1, 0, 1, 0}; //26
unsigned char R_K1[5]   = {1, 1, 0, 1, 1}; //27
unsigned char R_GP[5]   = {1, 1, 1, 0, 0}; //28
unsigned char R_SP[5]   = {1, 1, 1, 0, 1}; //29
unsigned char R_FP[5]   = {1, 1, 1, 1, 0}; //30
unsigned char R_RA[5]   = {1, 1, 1, 1, 1}; //31

unsigned char palavra[32];

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

void toLowerCase(char *s)
{
	for(; *s; ++s)
	{
		*s = tolower(*s);
	}
}

void divideInstrucao(char *linha, char *instrucao, char *op1, char *op2, char *op3)
{
	//Separa a linha em instrução e três operandos
	unsigned char i = 0;
	strcpy(instrucao, "");
	strcpy(op1, "");
	strcpy(op2, "");
	strcpy(op3, "");
	toLowerCase(linha);
	//Pega a instrucao. Para no primeiro espaço
	while(linha[i] && linha[i] != 32)
	{
		i++;
	}
	strncpy(instrucao, linha, i);
}

void processarLinha(char *linha)
{
	char *instrucao, *op1, *op2, *op3;
	divideInstrucao(linha, instrucao, op1, op2, op3);
	printf("%s\n%s\n%s\n%s\n", instrucao, op1, op2, op3);
}

int main(int argc, char *argv[5]) {
	char line[100] = "ADD $t0, $s0, $s1";
	inicializaPalavra();
	processarLinha(line);
	escrevePalavra();
};
