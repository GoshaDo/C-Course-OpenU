#ifndef __TABLE_H
#define __TABLE_H
#include "globals.h"
/**
 * method the build a singal node the symbole table
 * @param value - the adress it's holds
 * @param key - string of the label
 * @param type - which type of instrucation it is
 * */
Node* newNode(long value,char* key,instruction type,instruction entry);
/**
 * @param method the realase a symbole table
 * @param head - the firest node
 * */
void destroy(Node* head);
/**method to print all nodes at table
 * @param head - the first node of the table
 * */
void printTable(Node* head);
/**
 * @param method to porint a single instruction type at symbole table
 * @param head - the first node
 * @param type - which type to search for
 * */
void printTableByType(Node* head,instruction type);
/**
 * method to insert additional row to the table
 * @param head - the first node of the table
 * @param key - the labebl as string
 * @param value - the adress of label
 * @param type - instruction type of the label
 * */
Node* insertToEnd(Node* head,char* key,long value,instruction type,instruction entry);
/**
 * Boolean method to search of given label at symbole table
 * @param haed - the first node
 * @param label - the string to be found
 * */
bool isLabelInTable(Node* head,char* label);
/**
 * Boolean method to check if the label in specifit type of isntuction
 * @param head- the first node
 * @param label - string of the label to search
 * @param type - which type is it
 * */
bool isLabelInTableType(Node* head,char* label,instruction type);
/**
 * a method to extract the label type 
 * @param head - first node
 * @param label - string of the label to be searched
 * @param return - instuction - none_instrucation returend if not found
 * */
instruction getLabelType(Node* head,char* label);
/**
 * method which return the value of single label from symbole table
 * @param head - the first node of the symbole table
 * @param label - string of the label to be searched
 * */
long getLabelValue(Node* head,char* label);

/**
 * Method which builds a single node of machine code table
 * @param adress - the adress of the node
 * @param mw - the data it's hodls
 * @param label - which label to fill / if non - NULL filled
 * @param opType - which kind of operand is it (NONE_OPERAND = command)
 * */
MachineNode* newMWNode(long adress,long mw ,ARE are,char* label,operandType opType);
/**
 * Method the realse and destory machine code table
 * @param head - first node at list
 * */
void destroyMW(MachineNode* head);
/**
 * Method which prints machine code table
 * @param head - the first node of the list
 * */
void printMWTable(MachineNode* head);
/**
 * method which insert a new machine code row to the table (list) with the values
 * @param head - the first node
 * @param adress - the adress of the node
 * @param mw - the machine word
 * @param are - ARE as defined at EX.
 * @param label - which label if none - NULL
 * @param optype - which operand is it by type, none- -1
 * */
MachineNode* insertMWToEnd(MachineNode* head,long adress,long mw,ARE are,char* label,operandType opType);
/**
 * booleand method the change one node at specifit adress if null arg. insert - doesn't change it
 * @param head- the first node of the list(table)
 * @param adress - the user's adress to be changed
 * @param mw - machine word to be changed
 * @param label - the label to be changed
 * @param opType- which operand is it
 * */
bool changeNodeAtAdress(MachineNode** head,long adress,long mw,ARE are,char* label,operandType opType);
/**
 * Method which prints machine code table to file
 * @param head - the first node of the table
 * @param f - file which be writen into
 * @param ICF - counter of the instructions
 * @param DCF - counter of the data
 * */
void printMWTableToFile(MachineNode* head,FILE* f,long* ICF,long* DCF);

#endif
