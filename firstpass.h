#ifndef _FIRSTPASS_H
#define _FIRSTPASS_H


 /**
  * method to process a single line in the .as file
  * @param  line - struct of the line
  * @param IC - counter
  * @param DC - counter
  * @param table - symbole tyble
  * @param data_image - the data image
  * @param table2 - machine code table
  * */
bool process_line_fpss(line_info line,long* IC ,long* DC,Node** table,long* data_image,MachineNode** table2);
/**
 * method to analyze the code define the operand and insert to table
 * @param string - the string w/0 cmd
 * @param line - struct of the whole line
 * @param table2 - machinde code table
 * @param IC - counter
 * */
bool processCode(char* string,line_info line,MachineNode** table2,long* IC);
/**
 * method to extract the next operand from a string
 * seprates with comma
 * @param string to be annalyzed
 * */
char* getNextOperand(char* string);
/**
 * boolean method check if the command vlaid
 * if yes insert to the tbale
 * @param line - the line struct
 * @param opc - opcode as int
 * @param fun - funct as int
 * @param operand1 - string of the first operand
 * @param operand2 - string the the second operand
 * @param table2 - machineCode table
 * @param IC - coutner
 * */
bool cheackIfValidCommand(line_info line,int opc,int fun,char* operand1,char* operand2,MachineNode** table2,long* IC);
/**
 * fill the table depands on the operand passed
 * @param operand- string of the operand
 * @param table2 - machine code table
 * @param IC - counter
 * @param des - destantion/source command
 * */
void fillMWTableOperands(char* operand,MachineNode** table2,long* IC,int des);
/**
 * method which analyize the operand passed as string
 * and return the operand code number
 * @param line as line_info struct
 * @param oper the operand
 * */
int indentifyOperand(line_info line,char* oper);
#endif
