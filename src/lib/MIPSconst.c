/*
Biblioteca de constantes da arquitetura MIPS.
Contem os nomes e valores das instruções e dos registradores.
*/

#include "MIPSconst.h"

//Instruções Tipo R
const char R_MNE[8][4] = {"sll", "srl", "jr", "add", "sub", "and", "or", "slt"};

const unsigned char R_FUNC[8][6] = {0, 0, 0, 0, 0, 0, //00 = sll
															      0, 0, 0, 0, 1, 0, //02 = srl
														      	0, 0, 0, 1, 0, 0, //08 = jr
															      1, 0, 0, 0, 0, 0, //32 = add
															      1, 0, 0, 0, 1, 0, //34 = sub
															      1, 0, 0, 1, 0, 0, //36 = and
															      1, 0, 0, 1, 0, 1, //37 = or
															      1, 0, 1, 0, 1, 0};//42 = slt

const unsigned int R_NO_SHAMT = 0;

//Instruções Tipo I
const char I_MNE[5][5] = {"beq", "bne", "addi", "lw", "sw"};

const unsigned char I_OPCODE[5][6] = {0, 0, 0, 1, 0, 0, //04 = beq
															        0, 0, 0, 1, 0, 1, //05 = bne
															        0, 0, 1, 0, 0, 0, //08 = addi
															        1, 0, 0, 0, 1, 1, //35 = lw
															        1, 0, 1, 0, 1, 1};//43 = sw

//Instruções Tipo J
const char J_MNE[2][4] = {"j", "jal"};

const unsigned char J_OPCODE[2][6] = {0, 0, 0, 0, 1, 0, //02 = j
															        0, 0, 0, 0, 1, 1};//03 = jal


//Registradores [num_reg][bytes]
const char REG_MNE[32][6] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
                             "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                             "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                             "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

const unsigned char REG[32][5] = {0, 0, 0, 0, 0, //00 = $zero
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
