#ifndef __GLOBALS_H
#define __GLOBALS_H

/** Maximum length of a single source line  */
#define MAX_LINE_LENGTH 80

/** Initial IC value */
#define IC_INIT_VALUE 100

/* Maxium length of a single label*/
#define MAX_LABEL_LENGTH 31

#define MAX_IMG_LENGTH 4096

#define MAX_OPERAND_LENGTH 10
/*move the pointer to the next word*/
#define SKIP_NEXT(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)));

/*macro to size of array*/
#define SIZE_OF_ARR(arr,type) sizeof(arr)/sizeof(type)

/** Boolean (t/f) definition */
typedef enum booleans {
	FALSE = 0, TRUE = 1
} bool;

/**Represents a single source line, including it's details*/
typedef struct line_info {
	/** Line number in file */
	long line_number;
	/** File name */
	char *file_name;
	/** Line content (source) */
	char *content;
} line_info;

/** Operand addressing type */
typedef enum addressing_types {
	/** Immediate addressing (0) */
	IMMEDIATE_ADDR = 0,
	/** Direct addressing (1) */
	DIRECT_ADDR = 1,
	/** Relative addressing (2) */
	RELATIVE_ADDR = 2,
	/** Register addressing */
	REGISTER_ADDR = 3,
	/** Failed/Not detected addressing */
	NONE_ADDR = -1
} addressing_type;

/** Commands opcode */
typedef enum opcodes {
	/* First Group */
	MOV_OP = 0,
	CMP_OP = 1,

	ADD_OP = 2,
	SUB_OP = 2,

	LEA_OP = 4,
	/* END First Group */

	/* Second Group */
	CLR_OP = 5,
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,

	JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,

	RED_OP = 12,
	PRN_OP = 13,
	/* END Second Group */

	/* Third Group */
	RTS_OP = 14,
	STOP_OP = 15,
	/* END Third Group */

	/** Failed/Error */
	NONE_OP = -1
} opcode;

/** Commands funct */
typedef enum functs {
	/* OPCODE 2 */
	ADD_FUNCT = 10,
	SUB_FUNCT = 11,

	/* OPCODE 5 */
	CLR_FUNCT = 10,
	NOT_FUNCT = 11,
	INC_FUNCT = 12,
	DEC_FUNCT = 13,

	/* OPCODE 9 */
	JMP_FUNCT = 10,
	BNE_FUNCT = 11,
	JSR_FUNCT = 12,

	/** Default (No need/Error) */
	NONE_FUNCT = 0
} funct;

/**command elemnt*/
struct cmd_element {
	char *cmd;
	opcode op;
	funct fun;
};

/**enum of the registers*/
typedef enum registers {
	r0 = 0,
	r1 = 1,
	r2 = 2,
	r3 = 3,
	r4 = 4,
	r5 = 5,
	r6 = 6,
	r7 = 7,
	NONE_REG = -1
} reg;
/**enum of the instrucions*/
typedef enum instruction {
	/** .data instruction - 0 */
	DATA_INST=0,
	/** .string instruction - 1 */
	STRING_INST=1,
	/** .entry instruction - 2 */
	ENTRY_INST=2,
	/** .extern instruction - 3*/
	EXTERN_INST=3,
	/** Not found - 4 */
	NONE_INST=4,
	/*ERROR instuction*/
	ERROR_INST
} instruction;

/**struct of the instruction item*/
struct instruction_item {
	char *name;
	instruction value;
};
/**enum of all diffrent operands type*/
typedef enum operandType{
	IMD_OPERAND,
	DIR_OPERAND,
	REL_OPERAND,
	REG_OPERAND,
	NONE_OPERAND=-1
}operandType;
/**stuct of symbole node*/
typedef struct Node{
	long value;
	char* key;
	instruction type;
	instruction entry;
	struct Node* next;
}Node;


/**packed code_word struct*/
typedef struct __attribute__((__packed__)) code_word{

unsigned int cdes:2;
unsigned int csrc:2;
unsigned int cfun:4;
unsigned int copc:4;

}code_word;

/**enum of the ARE symboles*/
typedef enum ARE{
	A,
	R,
	E,
	NONE_ARE=-1
}ARE;
/**machine code node structer*/
typedef struct MachineNode{
	long adress;
	unsigned long machineWord;
	ARE are;
	char* label;
	operandType opType;
	struct MachineNode* next; /*next node at list*/
}MachineNode;

#endif

