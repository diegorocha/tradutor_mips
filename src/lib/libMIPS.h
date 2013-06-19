/*
Biblioteca de tradução assembly MIPS.
Contem funções necessárias para a tradução de uma instrução em bits.
*/

//Variável globais
extern unsigned char palavra[32];

//Processa a instrução em *linha salvando os bits codificados no array global palavra.
//Retorna 0 em caso de falha, 1 em caso de sucesso.
unsigned char processarInstrucao(char *linha);
