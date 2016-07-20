/*
Biblioteca de utilidades para arrays
Contem funções auxiliares para transferença de dados entre arrays e tratamentos de strings.
*/

#include "arrayUtils.h"

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
void copiaBits(const unsigned char *origem, unsigned char inicio, unsigned char qtd, unsigned char *destino, unsigned char inicioDestino)
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
