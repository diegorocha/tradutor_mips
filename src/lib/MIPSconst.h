/*
Biblioteca de constantes da arquitetura MIPS.
Contem os nomes e valores das instruções e dos registradores.
*/

//Instruções Tipo R
extern const char R_MNE[8][4];
extern const unsigned char R_FUNC[8][6];
extern const unsigned int R_NO_SHAMT;

//Instruções Tipo I
extern const char I_MNE[5][5];
extern const unsigned char I_OPCODE[5][6];

//Instruções Tipo J
extern const char J_MNE[2][4];
extern const unsigned char J_OPCODE[2][6];

//Registradores [num_reg][bytes]
extern const char REG_MNE[32][6];
extern const unsigned char REG[32][5];
