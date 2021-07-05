#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "table.h"
#include "globals.h"
#include "firstpass.h"
#include "func.h"

bool process_line_fpss(line_info line,long* IC,long* DC,Node** table,long* data_image,MachineNode** table2){
	int i=0;
	int caseX=-1;
	int tmp=0;
	char* label = NULL;
	char tmp_istr[MAX_LINE_LENGTH];
	const char space[2]=" ";
	bool LABEL_FLAG = FALSE;
	bool NO_ERROR = TRUE;
	char *token;
	label = (char*)malloc_with_check(sizeof(char)*MAX_LINE_LENGTH); 
	/*go to first non-whitespace occurdend*/
	SKIP_NEXT(line.content,i);

	/*checks if the line a comment or empety*/
	if (!line.content[i] || line.content[i] == '\n' || line.content[i] == EOF || line.content[i] == ';')
	{
		free(label);
		label=NULL;
		return TRUE; /*return true if the line a comment or empety*/
	}
	
	if(!find_label(line,&label)) /*search for valid label stores it in label*/
	{
		free(label);
		label=NULL;
		NO_ERROR=FALSE;
	}
	
	/*Label detected*/
	if(label!=NULL)
	{
		i = i+strlen(label)+1;
		LABEL_FLAG = TRUE;
		/*check if in table*/
		if(isLabelInTable((*table),label))
		{
			print_error(line,"the label already declared");	
			free(label);
			label=NULL;
			NO_ERROR=FALSE;
			LABEL_FLAG = FALSE;
		}		
	}
	SKIP_NEXT(line.content,i);
	
	if (line.content[i] == '\n') /*the line contains label only*/
	{
		if(LABEL_FLAG) /*insert label to table as code label*/
		{
			(*table) = insertToEnd((*table),label,*IC,NONE_INST,ERROR_INST);
			free(label);
			label=NULL;
			return TRUE; 
		}
	}
	
	if (line.content[i] == '.') /*checks is it instraction*/
	{
		
		strcpy(tmp_istr,line.content+i+1);
		token = strtok(tmp_istr,space);
		/*checks which instruction*/
		caseX = find_instruction_by_name(token);
		
			switch(caseX)
			{
			case 0: /*DATA_INST*/
			case 1: /*STRING_INST*/
					if(label)
					{
						(*table) = insertToEnd((*table),label,*DC,caseX,ERROR_INST);
						LABEL_FLAG=FALSE; /*label used*/
						
					}
					/*read and write the values*/
					i=i+strlen(token)+1;
					SKIP_NEXT(line.content,i);
					
					if(caseX==1)
					{
						if(line.content[i]!='"') /*checks if valid start of string*/
						{
							print_error(line,"String not starts with \" char");
							NO_ERROR = FALSE;
						}else
						{
						/*string starts properly*/
							
							tmp=(long)fillStringFromIndex(data_image,MAX_IMG_LENGTH,*DC,line.content+i);
							if(tmp==-1)
							{
								print_error(line,"String error");
								NO_ERROR=FALSE;
							}else
							{
								(*DC)+=tmp;
								free(label);
								label=NULL;
								return TRUE;
							}
						}
					}
					else if(caseX==0)
					{
						tmp = (long)fillDataFromIndex(data_image,MAX_IMG_LENGTH,*DC,line.content+i);
						if(tmp==-1)
						{
								print_error(line,"Data eror");
								NO_ERROR=FALSE;
						}else
						{
						(*DC)+=tmp;
						free(label);
						label=NULL;
						return TRUE;
						}
					}
				break;
			case 2: /*ENTRY_INST*/
				if(label)
					printf("WARNING at file %s line number %ld the label \"%s\" meaningless\n",line.file_name,line.line_number,label);
				free(label);
				label=NULL;
				return TRUE; /*continue to next line*/
				break;
			case 3: /*EXTERN_INST*/
				if(label)
				{
					printf("WARNING at file %s line number %ld the label \"%s\" meaningless\n",line.file_name,line.line_number,label);
					free(label);
					label=NULL;
				}
				i=i+strlen(token)+1;
				SKIP_NEXT(line.content,i);
				label = (char*)copyStringOnly(line.content+i);
				
				if(isLabelInTableType((*table),label,EXTERN_INST))
				{
					print_error(line,"Undeclred external label");
					free(label);
					label=NULL;
					NO_ERROR=FALSE;
				}
				(*table) = insertToEnd((*table),label,0,EXTERN_INST,ERROR_INST);
				free(label);
				label=NULL;
				return TRUE;
				break;
			case 4: /*NONE_INST*/ 
				print_error(line,"Instarcation not found");
				NO_ERROR = FALSE;
				break;
			}
		
		
	}
	/* not instrucation so code*/
	if(LABEL_FLAG && label)
		(*table) = insertToEnd((*table),label,*IC,NONE_INST,ERROR_INST); /*NONE_INST == code symbol*/
	free(label);
	label=NULL;
	
	/*code process*/
	
	/*printf("%s\n",line.content+i);*/
	  if(!processCode(line.content+i,line,table2,IC))
	  {
	  	NO_ERROR=TRUE;
	  
	  }
	
	
	return NO_ERROR;
}

bool processCode(char* s1,line_info line,MachineNode** table2,long* IC)
{
	bool NO_ERROR=TRUE;
	char* token=NULL;
	char* ptr=NULL;
	char* ptr2=NULL;
	char* operand1=NULL;
	char* operand2=NULL;
	int fun, opc;
	const char space[2] =" ";
	const char comma[2] =",";
	char stringSaved[MAX_LINE_LENGTH];
	operand1=(char*)malloc_with_check(sizeof(char)*MAX_OPERAND_LENGTH);
	operand2=(char*)malloc_with_check(sizeof(char)*MAX_OPERAND_LENGTH);
	
	if(!s1)
		return FALSE;
	strcpy(stringSaved,s1);/*saves original string*/
	token = strtok(stringSaved,space); /*tokenize the cmd*/
	ptr = copyStringOnly(token); /*cleans the cmd token out of other symboles*/
	get_opcode_func(ptr, &opc, (funct *) &fun);
	free(ptr);
	ptr=NULL;
	
	/*checks if the command in the command table*/
	if(opc==NONE_OP)
	{
		print_error(line,"invalid command");
		NO_ERROR = FALSE;
		return FALSE;
	}
	ptr = cleanSpaces(s1+strlen(token));
	ptr2 = copyStringOnly(ptr);

	if (ptr2==NULL) /*no operand found*/
	{
		free(operand1);
		operand1=NULL;
		free(operand2);
		operand2=NULL;
	}
	else /*there is operands*/
	{
		token = strtok(ptr2,comma);
		if(token==NULL)
		{
			free(operand1);
			operand1=NULL;
		}else{strcpy(operand1,token);}
		
		token = strtok(NULL,comma);
		if(token==NULL)
		{
			free(operand2);
			operand2=NULL;
		}else{strcpy(operand2,token);}
	}
	
	free(ptr2);
	ptr2=NULL;

		
	/*check if valid command*/
	
	if (!cheackIfValidCommand(line,opc,fun,operand1,operand2,table2,IC))
		NO_ERROR = FALSE;
	
	if(operand1!=NULL) free(operand1);
	if(operand2!=NULL) free(operand2);
	
	if(ptr!=NULL)free(ptr);
	ptr=NULL;
	operand2=NULL;
	operand1=NULL;
	
	return NO_ERROR;
}

char* getNextOperand(char* s)
{
	char* ptr=NULL;
	char* token=NULL;
	int i=0;
	const char comma[2]=",";
	if (strlen(s)<=0)
		return NULL;
	token = strtok(s,comma);
	while(token[i]==' ')
		i++;
	token = copyStringOnly(token+i);
	ptr = (char*)malloc_with_check(sizeof(char)*strlen(token));
	strcpy(ptr,token);
	free(token);
	token=NULL;
	
	return ptr;
	
	
}

bool cheackIfValidCommand(line_info line,int opc,int fun,char* operand1,char* operand2,MachineNode** table2,long* IC)
{
	char* pop1=NULL;
	char* pop2=NULL;
	int src = NONE_OPERAND;
	int des = NONE_OPERAND;
	int val=1;
	int L=0;
	bool NO_ERROR=TRUE;
	code_word* mw=NULL;
	mw = (code_word*)malloc_with_check(sizeof(code_word));

	
	pop1 = cleanSpaces(operand1);
	pop2 = cleanSpaces(operand2);
	
	if(pop1!=NULL && pop2!=NULL)
	{
		src = indentifyOperand(line,pop1);
		des = indentifyOperand(line,pop2);
	}else if(pop1!=NULL && pop2==NULL)
	{
		des = indentifyOperand(line,pop1);
	}
	
	switch(opc)
	{
	case 0: /*MOV 2 operands*/
		if(src==REL_OPERAND || src==NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des==IMD_OPERAND || des==REL_OPERAND || des==NONE_OPERAND) /*check destiantion operand in-direct*/
		{
			print_error(line,"ilegal destaion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 1:/*CMP 2 operands*/
		if(src==REL_OPERAND || src==NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des==REL_OPERAND || des==NONE_OPERAND) /*check destiantion operand in-direct*/
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 3: /*checks add or sub 2 operands*/
		if(src==REL_OPERAND || src==NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des==REL_OPERAND || des==NONE_OPERAND) /*check destiantion operand in-direct*/
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 4:
		if(src!=DIR_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des!=DIR_OPERAND && des!=REG_OPERAND)
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 5:
		if(src!=NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des!=DIR_OPERAND && des!=REG_OPERAND)
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 9:
		if(src!=NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des!=DIR_OPERAND && des!=REL_OPERAND)
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 12:
		if(src!=NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des!=DIR_OPERAND && des!=REG_OPERAND)
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 13:
		if(src!=NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des==REL_OPERAND || des==NONE_OPERAND)
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	case 15:
		if(src!=NONE_OPERAND)
		{
			print_error(line,"ilegal source operand");
			NO_ERROR=FALSE;
		}
		if(des!=NONE_OPERAND)
		{
			print_error(line,"ilegal destantion operand");
			NO_ERROR=FALSE;
		}
		break;
	}
	
	if(src!=NONE_OPERAND)
		L++;
	if(des!=NONE_OPERAND)
		L++;
	
	if(des==NONE_OPERAND)
		{des=IMD_OPERAND;}
	if(src==NONE_OPERAND)
		{src=IMD_OPERAND;}
	/*DONE CEHCKING - CREATING MEACHINE WORD*/
	
	
	mw->copc=(unsigned int)opc;
	mw->cfun=(unsigned int)fun;
	mw->csrc=(unsigned int)src;
	mw->cdes=(unsigned int)des;
	/*printf("opc:%d fun:%d src:%d des:%d L:%d\n",mw->copc,mw->cfun,mw->csrc,mw->cdes,L);*/
	val = codeWordToInt(&mw);
	
	/*insert command machine word to code image*/
	(*table2)=insertMWToEnd((*table2),*IC,val,A,NULL,NONE_OPERAND);
	(*IC)++;
	/*insert the other words to code image*/
	/*ONE operand*/
	if(L==1)
	{
		fillMWTableOperands(pop1,table2,IC,des);
	}
	/*Two operands*/
	if(L==2)
	{
		fillMWTableOperands(pop1,table2,IC,src);
		fillMWTableOperands(pop2,table2,IC,des);
		
	}
	
	/*printf("----------------------------\n");*/	
	if((*IC)>=MAX_IMG_LENGTH)
	{
		NO_ERROR=FALSE;
		print_error(line,"Maximun image length reached");
		
	}
	if(pop1!=NULL) free(pop1);
	if(pop2!=NULL) free(pop2);
	if(mw!=NULL) free(mw);
	pop1=NULL;
	pop2=NULL;	
	mw=NULL;
	return NO_ERROR;
}

int indentifyOperand(line_info line,char* oper)
{
	operandType type = NONE_OPERAND;
	if(oper==NULL)
	{
		return type;
	}
	if(oper[0]=='#') /*checks if direct operand*/
	{
		if((oper[1]=='-' || oper[1]=='+') && isdigit(oper[2]))
		{
				type=IMD_OPERAND;
				return type;
			
		}
		else if(isdigit(oper[1]))
		{
			type = IMD_OPERAND;
			return type;
		}
	}
	
	if(oper[0]=='%') /*checks if relative operand*/
	{

		if(is_valid_label(line,oper+1))
			type=REL_OPERAND;
			return type;
	}
	
	if(get_register_by_name(oper)!=NONE_REG)/*checks if register operand*/
	{
		type=REG_OPERAND;
		return type;
	}
	
	if(is_valid_label(line,oper));/*checks if direct operand*/
	{
		type=DIR_OPERAND;
		return type;
	}
	
	return type;
}

void fillMWTableOperands(char* operand,MachineNode** table2,long* IC,int des)
{

	switch(des)
		{
			case IMD_OPERAND:
				/*printf("IMD %d\n",textToNum(operand,1,strlen(operand)-1));*/
				(*table2)=insertMWToEnd((*table2),*IC,textToNum(operand,1,strlen(operand)-1),A,NULL,IMD_OPERAND);
				(*IC)++;
			break;
			case DIR_OPERAND:
				/*printf("DIR %s\n",operand);*/
				(*table2)=insertMWToEnd((*table2),*IC,(long)NULL,NONE_ARE,operand,DIR_OPERAND);
				(*IC)++;
			break;
			case REL_OPERAND:
				/*printf("REL %s\n",operand+1);*/
				(*table2)=insertMWToEnd((*table2),*IC,(long)NULL,NONE_ARE,operand+1,REL_OPERAND);
				(*IC)++;
			break;
			case REG_OPERAND:
				/*printf("REG %d\n",textToNum(operand,1,strlen(operand)-1));*/
				(*table2)=insertMWToEnd((*table2),*IC,1<<(textToNum(operand,1,strlen(operand)-1)),A,NULL,REG_OPERAND);
				(*IC)++;
			break;
			
		}
	
}
