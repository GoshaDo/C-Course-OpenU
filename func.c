#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "globals.h"
#include "func.h"

static struct cmd_element cmd_table[] = {
		{"mov", MOV_OP, NONE_FUNCT},
		{"cmp",CMP_OP, NONE_FUNCT},
		{"add",ADD_OP, ADD_FUNCT},
		{"sub",SUB_OP, SUB_FUNCT},
		{"lea",LEA_OP, NONE_FUNCT},
		{"clr",CLR_OP, CLR_FUNCT},
		{"not",NOT_OP, NOT_FUNCT},
		{"inc",INC_OP, INC_FUNCT},
		{"dec",DEC_OP, DEC_FUNCT},
		{"jmp",JMP_OP, JMP_FUNCT},
		{"bne",BNE_OP, BNE_FUNCT},
		{"jsr",JSR_OP, JSR_FUNCT},
		{"red",RED_OP, NONE_FUNCT},
		{"prn",PRN_OP, NONE_FUNCT},
		{"rts",RTS_OP, NONE_FUNCT},
		{"stop",STOP_OP, NONE_FUNCT},
		{NULL, NONE_OP, NONE_FUNCT}
};

static struct instruction_item instructions_table[] = {
	{"string",STRING_INST},
	{"data",DATA_INST},
	{"entry",ENTRY_INST},
	{"extern",EXTERN_INST},
	{NULL,NONE_INST}
};

char *strallocat(char *s0, char* s1) {
	char *str = (char *)malloc_with_check(strlen(s0) + strlen(s1) + 1);
	strcpy(str, s0);
	strcat(str, s1);
	return str;
}

void *malloc_with_check(long size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}

int print_error(line_info line,char* msg){ 
	printf("Error at file %s line %ld : %s\n",line.file_name,line.line_number,msg);
	return 0;	
}


reg get_register_by_name(char *name) {
	int digit;
	if (name[0] == 'r' && isdigit(name[1]) && name[2] == '\0') {
		digit = name[1] - '0'; /* convert digit ascii char to actual single digit */
		if (digit >= 0 && digit <= 7) return digit;
	}
	return NONE_REG; /* No match */
}

instruction find_instruction_by_name(char *name) {
	struct instruction_item *curr_item;
	for (curr_item = instructions_table; curr_item->name != NULL; curr_item++) {
		if (strcmp(curr_item->name, name) == 0) {
			return curr_item->value;
		}
	}
	return NONE_INST;
}

bool is_alphanumeric_str(char *string) {
	int i;
	/*check for every char in string if it is non alphanumeric char if it is function returns true*/
	for (i = 0; string[i]; i++) {
		if (!isalpha(string[i]) && !isdigit(string[i])) return FALSE;
	}
	return TRUE;
}


bool is_reserved_word(char *name) {
	int fun, opc;
	get_opcode_func(name, &opc, (funct *) &fun);
	if (opc != NONE_OP || get_register_by_name(name) != NONE_REG || find_instruction_by_name(name) != NONE_INST) 
		return TRUE;

	return FALSE;
}


bool is_valid_label(line_info line,char *name) {
	/* Check length, first char is alpha and all the others are alphanumeric, and not saved word */
	if(!name[0]){print_error(line,"Empety label");return FALSE;}
	if(strlen(name) > MAX_LABEL_LENGTH){print_error(line,"Label name too long");return FALSE;}
	if(!isalpha(name[0])){print_error(line,"Label name starts with non-alphabetic char");return FALSE;}
	if(!is_alphanumeric_str(name + 1)){print_error(line,"Label name contains ilegal char");return FALSE;}
	if(is_reserved_word(name)){print_error(line,"Label named after reserved word");return FALSE;}
	return TRUE;
}

bool find_label(line_info line,char** label){
	const char space[2]=":";
	const char ch = ':';
	char* token;
	char* ret;
	char* tmp_line=NULL;
	
	tmp_line = (char*)malloc_with_check(strlen(line.content)+1);
	strcpy(tmp_line,line.content);
	
	
	ret = strchr(tmp_line,ch);
	if(ret==NULL) /*no : found*/
	{
		free(*label);
		*label=NULL;
		free(tmp_line);
		tmp_line=NULL;
		return TRUE;
	}
	/*if found : countinue*/
	ret = strchr(ret+1,ch); /*seeking for another : */
	if(ret!=NULL) /*more then one occurdent of : */
	{
		print_error(line,"invalid char ':' in code");
		free(*label);
		*label=NULL;
		free(tmp_line);
		tmp_line=NULL;
		return FALSE;
	}
	/*found ONLY one : in string*/
	token = strtok(tmp_line,space);
	/*check if label(token) valid*/
	if(!is_valid_label(line,token))
	{
		/*error occurded*/
		free(*label);
		*label=NULL;
		free(tmp_line);
		tmp_line=NULL;
		return FALSE;
	}
	strcpy((*label),token);
	free(tmp_line);
	tmp_line=NULL;
	return TRUE;
}


void fillArr(long* arr,long arrSize)
{
	int i=0;
	while(i<arrSize)
	{
		arr[i]=0;
		i++;
	}
}

int* fillStringFromIndex(long* arr,long arrSize,long index,char* string)
{
	int i=0;
	int j=-1;
	if(index+strlen(string) >= arrSize)
	{
		printf("Maximum arr length reached\n");
		return (int*)j;
	}
	if(string[i]!='"')
	{
		printf("ilegal string entry");
		return (int*)j;
	}

	while(i<strlen(string))
	{
		
		if(string[i+1]=='"')
		{
			string[i++]='\0';
			break;
		}
		arr[index+i]=string[i+1];
		i++;
	}

	return (int*)i;
}

int textToNum(char *text, int from, int end){
    int i = 0;
    int result = 0,start=from;
	while(text[end] && (text[end]==' ' || text[end]==EOF || text[end]=='\t' || text[end]=='\n'))
		end--;
    if(text[start] == '-' || text[start] == '+')
        start++;
    while(end-i >= start)
    {
	
        result += pow(10,i)*((int)(text[end-i] - '0'));
        i++;
	
    }
    return (text[from] == '-')? -result: result;
}

int* fillDataFromIndex(long* arr,long arrSize,long index,char* string)
{
	char* token;
	int end;
	int start;
	long num;
	int i=0;
	const char comma[2]=",";
	
	token = strtok(string,comma);
	while(token!=NULL)
	{
		end = token-string+strlen(token)-1;
		start = token-string;
		num = (unsigned long)textToNum(string,start,end);
		arr[index+i]=num;
		i++;
		token=strtok(NULL,comma);
		while(token!=NULL && token[0]==' ')
			token++;
	}
	return(int*)i;
}

void printArr(long* arr,int arrSize)
{
	int i=0;
	while(i<arrSize)
	{
		printf("%ld ",arr[i]);
		i++;
	}
	printf("\n");
	return;
}

char* copyStringOnly(char* string)
{
	int i=0,j=0;
	char* arr;
	arr = (char*)malloc_with_check(sizeof(char)*(strlen(string)+1));
	j=strlen(string)-1;
	while(string[j] && string[j]!=' ' && string[j]!='\n' && string[j]=='\t' && string[j]!='\r' && string[j]!=EOF)
		j--;
	if (j<=0 || string[j]=='\0' || string[j]==' ') return NULL;
	if(string[j]=='\n') string[j]=0;
	arr = (char*)malloc_with_check((j+1)*sizeof(char));
	while(i!=j+1)
	{
		arr[i]=string[i];
		i++;
	}
		arr[j+1]='\0';

	return arr;
	
	
}
void get_opcode_func(char *cmd, opcode *opcode_out, funct *funct_out) {
	struct cmd_element *e;
	*opcode_out = NONE_OP;
	*funct_out = NONE_FUNCT;
	/* iterate through the cmd table, if commands are same return the opcode of found. */
	for (e = cmd_table; e->cmd != NULL; e++) {
		if (strcmp(e->cmd, cmd) == 0) {
			*opcode_out = e->op;
			*funct_out = e->fun;
			return;
		}
	}
}

char* cleanSpaces(char* string)
{
	
	char* result=NULL;
	int i=0,j=0;
	bool space = FALSE;
	if(string==NULL)
		return result;
	for(i=0;i<strlen(string);i++)
	{
		if(string[i]==' ')
			space=TRUE;
	}
	result = (char*)malloc_with_check(sizeof(char)*(strlen(string)));
	if(space==FALSE)
	{ 
		strcpy(result,string);
		return result;
	}
	
	
	for(i=0;i<strlen(string);i++)
	{
		if(string[i]!=' ')
		{
			result[j]=string[i];
			j++;
		}
	}
	result[j]='\0';

	return result;
}

int codeWordToInt(code_word** w)
{
	
	unsigned int val;
	val = ((*w)->copc<<8) | ((*w)->cfun<<4) | ((*w)->csrc<<2) | ((*w)->cdes);
	/*val = (unsigned int)(*w);*/
	/*printf("%03x\n",val&0xFFF);*/
	return val&0xFFF;

}

void printExtEntfiles(MachineNode* table2,Node* table,FILE* EntFile,FILE* ExtFile,long* ICF,long* DCF)
{
	MachineNode* mwHead=NULL;
	Node* head =NULL;
	head = table;
	while(head!=NULL)
	{
		mwHead=table2;
		if(head->type==ENTRY_INST || head->entry==ENTRY_INST)
		{
			if(head->type==DATA_INST || head->type==STRING_INST)
				fprintf(EntFile,"%s %04ld\n",head->key,head->value+*ICF);
			else
				fprintf(EntFile,"%s %04ld\n",head->key,head->value);
		}
		if(head->type==EXTERN_INST || head->entry==EXTERN_INST)
		{
			while(mwHead!=NULL)/*Loop throw machine code table and file all apperance of labels*/
			{
				if(mwHead->label)
					if(strcmp(head->key,mwHead->label)==0)
						fprintf(ExtFile,"%s %04ld\n",head->key,mwHead->adress);
				
				mwHead=mwHead->next;
			}
		}
		
		head=head->next;
	}
	
	
	
}


