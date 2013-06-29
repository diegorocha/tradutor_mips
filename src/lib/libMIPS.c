/*
Biblioteca de tradução assembly MIPS.
Contem funções necessárias para a tradução de uma instrução em bits.
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "libMIPS.h"
#include "MIPSconst.h"
#include "arrayUtils.h"

#define INST_NAO_SUP 200
#define REG_INVAL 201
#define IME_M_GRANDE 202

//Variável globais
unsigned char palavra[32];
char message[30];

void inicializarPalavra()
{
	memset(palavra, 0, 32);
}

void setBinario(unsigned int value, unsigned int bitFinal)
{
	unsigned char bit = bitFinal;
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

char *libMipsErrorMessage(int errorNo)
{
	strcpy(message, "");
	switch (errorNo){
		case INST_NAO_SUP:
			strcpy(message, "Instrução não suportada"); break;
		case REG_INVAL:
			strcpy(message, "Registrador inválido"); break;
		case IME_M_GRANDE:
			strcpy(message, "Imediato muito grande"); break;
		default:
			strcpy(message, "Erro desconhecido"); break;
	}
	return message;
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

void instrucaoTipoR(const unsigned char *rs, const unsigned char *rt, const unsigned char *rd, unsigned int shamt, const unsigned char *func)
{
		copiaBits(rs, 0, 5, palavra, 6);
		copiaBits(rt, 0, 5, palavra, 11);
		copiaBits(rd, 0, 5, palavra, 16);
		setShamt(shamt);
		copiaBits(func, 0, 6, palavra, 26);
}

void instrucaoTipoI(const unsigned char *opCode, unsigned char *rs, unsigned char *rt, unsigned int imediato)
{
		copiaBits(opCode, 0, 6, palavra, 0);
		copiaBits(rs, 0, 5, palavra, 6);
		copiaBits(rt, 0, 5, palavra, 11);
		setImediato(imediato);
}

void instrucaoTipoJ(const unsigned char *opCode, unsigned int imediato)
{
		copiaBits(opCode, 0, 6, palavra, 0);
		setImediato(imediato);
}

/*
Processa a instrução.
Retorna 0 em caso de falha, 1 em caso de sucesso.
*/
unsigned char processarInstrucao(char *linha)
{
	char instrucao[10], op1[5], op2[5], op3[5];
	unsigned char bRS[5], bRT[5], bRD[5];
	unsigned char i = 0;
	unsigned int imediato = 0;
	unsigned int shamt = 0;
	unsigned int retorno = 0; //flag erro
	
	inicializarPalavra();
	divideInstrucao(linha, instrucao, op1, op2, op3);

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
					errno = REG_INVAL;
					return 0;
				}

				//rt
				if(!getRegistradorBytes(op3, bRT))
				{
					errno = REG_INVAL;
					return 0;
				}
			
				//rd
				if(!getRegistradorBytes(op1, bRD))
				{
					errno = REG_INVAL;
					return 0;
				}
				
				//Com rs, rt, rt e sem shamt
				instrucaoTipoR(bRS, bRT, bRD, R_NO_SHAMT, R_FUNC[i]);
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
						errno = REG_INVAL;
						return 0;
					}
					instrucaoTipoR(bRS, REG[0], REG[0], R_NO_SHAMT, R_FUNC[i]);
				}
				else
				{
					//sll e srl: op = 0; rs = op2; rt = 0; rd = op1; shmat = op3; func  = R_FUNC[i];
					//rs = op2
					if(!getRegistradorBytes(op2, bRS))
					{
						errno = REG_INVAL;
						return 0;
					}
				
					//rd = op1
					if(!getRegistradorBytes(op1, bRD))
					{
						errno = REG_INVAL;
						return 0;
					}
					
					//shamt
					sscanf(op3, "%d", &shamt);
					if(shamt > 31)
					{
						errno = IME_M_GRANDE;
						return 0;
					}
					
					instrucaoTipoR(bRS, REG[0], bRD, shamt, R_FUNC[i]);
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
					errno = REG_INVAL;
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
					errno = REG_INVAL;
					return 0;
				}
						
				//imediato
				sscanf(op2, "%d", &imediato);
			}
			
			//rt
			if(!getRegistradorBytes(op1, bRT))
			{
				errno = REG_INVAL;
				return 0;
			}

			if(imediato > 65535)
			{
				errno = IME_M_GRANDE;
				return 0;
			}

			instrucaoTipoI(I_OPCODE[i], bRS, bRT, imediato);
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
				errno = IME_M_GRANDE;
				return 0;
			}
			instrucaoTipoJ(J_OPCODE[i], imediato);
		}
	}
	
	if(retorno == 0)
	{
		errno = INST_NAO_SUP;
	}
	return retorno;
}
