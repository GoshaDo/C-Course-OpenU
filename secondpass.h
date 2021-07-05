#ifndef _SECONDPASS_H
#define _SECONDPASS_H
/**
 * method which anaylzing each line
 * @param line - struct of the line
 * @param table - symbole table
 * @param table2- machine code table
 * */
bool process_line_spss(line_info line,Node** table,MachineNode** table2);
/**
 * method which fills the missing labels at machine code table
 * @param table - symbol/labels table
 * @param table2 - machine code table
 * @param ICF - copy of the instraction counter
 * @param DCF - copy of the data counter
 * */
bool fillTable(Node** table,MachineNode** table2,long ICF,long DCF);
#endif
