/*
Biblioteca de utilidades para arrays
Contem funções auxiliares para transferença de dados entre arrays e tratamentos de strings.
*/

//Função auxiliar que converte uma cadeia de caracteres em minúsculo
void toLowerCase(char *s);

//Função que copia um subcadeia de bits de um vetor para outro.
void copiaBits(const unsigned char *origem, unsigned char inicio, unsigned char qtd, unsigned char *destino, unsigned char inicioDestino);

//Função que copia um subcadeia de caracteres de um vetor para outro.
void copiaString(char *origem, char *destino, unsigned char inicio, unsigned char qtd);
