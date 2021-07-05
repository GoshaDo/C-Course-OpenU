#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "table.h"
#include "globals.h"
#include "secondpass.h"
#include "func.h"

bool process_line_spss(line_info line,Node** table,MachineNode** table2)
{
	bool NO_ERROR=TRUE;
	char tmp_istr[MAX_LINE_LENGTH];
	char* token=NULL;
	const char space[2]=" ";
	Node* head=NULL;
	bool entryFound = FALSE;
	bool entryToken = FALSE;
	int i=0;
	
	SKIP_NEXT(line.content,i);
	if(line.content[i]=='.')
	{
		strcpy(tmp_istr,line.content+i+1);
		token = strtok(tmp_istr,space);
		if(find_instruction_by_name(token)==ENTRY_INST)
		{
			token = strtok(NULL,space);
			token = copyStringOnly(token);
			if(token) entryToken=TRUE;
			head = (*table);
			while(head!=NULL)/*loop throw symbole table*/
			{
				if(!strcmp(head->key,token))/*entry found*/
				{
					entryFound=TRUE;
					head->entry = ENTRY_INST; /*mark entry as entry in the node*/	
				}
				head = head->next;
			}
			free(token);
			token=NULL;
		}
	}
	if(entryToken && !entryFound)
	{
		print_error(line,"Entry can't be found in file");
		NO_ERROR=FALSE;
	}
	return NO_ERROR;
}

bool fillTable(Node** table,MachineNode** table2,long ICF,long DCF)
{
	bool NO_ERROR = TRUE;
	instruction InstrucationType=NONE_OPERAND;
	MachineNode* head=NULL;
	long value =0;
	operandType opType = NONE_OPERAND;
	if((*table2)==NULL)
		return FALSE;
	head=(*table2);

	/*Loop throw all the table*/
	while(head!=NULL)
	{
		if(head->label) /*if label saved at the machine word table*/
		{
			if(isLabelInTable((*table),head->label))/*if label in symbol table*/
			{
					InstrucationType = getLabelType((*table),head->label);
					value = getLabelValue((*table),head->label);
					opType = head->opType;
					
					if(opType==DIR_OPERAND)
					{
						if(InstrucationType==DATA_INST || InstrucationType==STRING_INST)
						{
							head->machineWord = (int)ICF+(unsigned long)value;
							head->are = R;
						}else{
							head->machineWord =(unsigned long)value;
							if(InstrucationType==NONE_INST)
								head->are = A;
							else
								head->are = E;
								
						}
					}
					if(opType==REL_OPERAND)
					{
						if(InstrucationType==DATA_INST || InstrucationType==STRING_INST)
						{
							head->machineWord = (int)ICF+(unsigned long)value-((int)head->adress-1);
							head->are = R;
						}else{
							head->machineWord = (unsigned long)value-((unsigned int)head->adress-1);
							head->are = R;
						}
						
					}
					
			}else
			{
				printf("the %s label haven't declred at code\n",head->label);
				NO_ERROR=FALSE;
			}
			
		}
		
		head=head->next;
		
	}
	
	return NO_ERROR;
}
