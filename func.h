#ifndef _FUNC_H
#define _FUNC_H


/**
 * Concatenates both string to a new allocated memory.
 * @param s0 The first string.
 * @param s1 The second string.
 * @return A pointer to the new, allocated string.
 */
char *strallocat(char *s0, char* s1);

/**
 * Allocates memory in the required size. Exits the program if failed.
 * @param size The size to allocate in bytes.
 * @return A generic pointer to the allocated memory if succeeded.
 */
void *malloc_with_check(long size);

/**
 * Print error to stdout. including line_info's file name, number of line and the error.
 * @param line which line_info struct.
 * @param msg the error message the user gets.
 */
int print_error(line_info line,char* msg);
/**
 * Methid which find a label in single line
 * @param line which line_info struct.
 * @param label where the label be stored
 * @return boolean if sucessful
 * */
bool find_label(line_info line,char** label);
/**
 * Boolean method to check if a label in line valid
 * @param line which line_info struct.
 * @param name the label name
 * @return boolean if succesful
 * */
bool is_valid_label(line_info line,char *name); 
/**
 * Boolean method to check if label is reserved
 * @param name the label
 * @return boolean if succeful
 * */
bool is_reserved_word(char *name);
/**
 * Boolean meathod to check if string is alpha or numeric
 * @return boolean
 * */
bool is_alphanumeric_str(char *string);
/**
 * Method the find an instuction by it name
 * @param name the name of the instrucation
 * @return instruction item
 * */
instruction find_instruction_by_name(char *name);
/**
 * Method that returns register object by it name
 * @param name the name of the register
 * @return reg as defined at globals
 * */
reg get_register_by_name(char *name);
/**
 * A methoc which fill opcdoe/funct out by the command imported
 * @param cmd the command which the user want to get the opcode and funct
 * @param opcode_out the opcode to be filled
 * @param funct_out the funct to be filled
 * 
 * */
void get_opcode_func(char *cmd, opcode *opcode_out, funct *funct_out);
/**
 * Method which print an error
 * @param line where the error occurded
 * @param msg the msg to be printed to stdout
 * */
int print_error(line_info line,char* msg);
/**
 * method which fills a array with nulls
 * @param arr the array
 * @param arrSize the length of the array
 * */
void fillArr(long* arr,long arrSize);
/**
 * Method which fills a string from specifict index as longs in array
 * @param arr - the array to be filled
 * @param arrSize - the size of the array
 * @param index the index to begin with
 * @param string to be filled as long to the array
 * @return the new index
 * */
int* fillStringFromIndex(long* arr,long arrSize,long index,char* string);
/**
 * Method which converts text number to number
 * @param text to be filled
 * @param from the index wheres the number in the string
 * @param end the index wheres the number ends
 * @return int number
 * */
int textToNum(char *text, int from, int end);
/**
 * Method which fills data to the data_image from given indedx
 * @param arr the data image
 * @param arrSize the maximum size of the array
 * @param index the index wheres to begin the fill
 * @param data which be filled in the array
 * @return new index
 * */
int* fillDataFromIndex(long* arr,long arrSize,long index,char* data);
/**
 * method which prints given array
 * @param arr the array
 * @param arrSize where to stop
 * */
void printArr(long* arr,int arrSize);
/**
 * Method which return string only w/o heads and talis 
 * @param string which be cut
 * @return new string (should be free later)
 * */
char* copyStringOnly(char* string);
/**
 * method which clean all spaces from a string
 * @param string to be cleaned
 * @return the new string (should be free later)
 * */
char* cleanSpaces(char* string);
/**
 * Method which coverts a code word object to intger
 * @param w code word
 * @return intger of the machine word
 * */
int codeWordToInt(code_word** w);

/**
 * method which creates .ext and .ent file out of all tables
 * @param table2 machinecode table
 * @param table symbole table
 * @param EntFile the pointer to the file which be writen the entries
 * @param ExtFile the pointer to the file which be writen the enternals
 * @param ICF counter of instrucations
 * @param DCF counter of data 
 * */
void printExtEntfiles(MachineNode* table2,Node* table,FILE* EntFile,FILE* ExtFile,long* ICF,long* DCF);

#endif

