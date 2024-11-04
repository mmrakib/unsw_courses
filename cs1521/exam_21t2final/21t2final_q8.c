// COMP1521 21T2 ... final exam, question 8

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "21t2final_q8.h"
#include "21t2final_q8_opcodes.h"

#define INSTRUCTION_MASK_MSB 0xFC000000
#define INSTRUCTION_MASK_LSB 0x0000003F

#define SPECIAL_RS_MASK 0x03E00000
#define SPECIAL_RT_MASK 0x001F0000
#define SPECIAL_RD_MASK 0x0000F800

#define IMM_MASK 0x0000FFFF
#define IMM_RS_MASK 0x03E00000
#define IMM_RT_MASK 0x001F0000

Instruction
decode_instruction (Word insn_word)
{
	Instruction insn;

	bool uses_immediate = (((insn_word & INSTRUCTION_MASK_MSB) >> 26) == OP_SPECIAL) ? false : true;

	if (uses_immediate) {
		int opcode = (insn_word & INSTRUCTION_MASK_MSB) >> 26;
		
		if (opcode == OP_ADDI) {
			//insn.op = "addi";
			strcpy(insn.op, "addi");
		} else if (opcode == OP_ADDIU) {
			//insn.op = "addiu";
			strcpy(insn.op, "addiu");
		} else if (opcode == OP_LB) {
			//insn.op = "lb";
			strcpy(insn.op, "lb");
		} else if (opcode == OP_LW) {
			//insn.op = "lw";
			strcpy(insn.op, "lw");
		}

		insn.uses_rs = false;
		insn.uses_rt = true;
		insn.uses_rd = false;
		insn.uses_base = true;
		insn.uses_imm = true;

		insn.imm = insn_word & IMM_MASK;
		insn.rt = (insn_word & IMM_RT_MASK) >> 16;
		insn.base = (insn_word & IMM_RS_MASK) >> 21;
	} else {
		int opcode = insn_word & INSTRUCTION_MASK_LSB;
		
		if (opcode == OP_SPECIAL_ADD) {
			//insn.op = "add";
			strcpy(insn.op, "add");
		} else if (opcode == OP_SPECIAL_ADDU) {
			//insn.op = "addu";
			strcpy(insn.op, "addu");
		} else if (opcode == OP_SPECIAL_SUB) {
			//insn.op = "sub";
			strcpy(insn.op, "sub");
		} else if (opcode == OP_SPECIAL_SUBU) {
			//insn.op = "subu";
			strcpy(insn.op, "subu");
		}

		insn.uses_rs = true;
		insn.uses_rt = true;
		insn.uses_rd = true;
		insn.uses_base = false;
		insn.uses_imm = false;

		insn.rs = (insn_word & SPECIAL_RS_MASK) >> 21;
		insn.rt = (insn_word & SPECIAL_RT_MASK) >> 16;
		insn.rd = (insn_word & SPECIAL_RD_MASK) >> 11;
	}

	return insn;
}
