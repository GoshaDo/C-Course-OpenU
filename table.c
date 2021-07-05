#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "table.h"
#include "func.h"

Node* newNode(long value,char* key,instruction type,instruction entry)
{
	Node* tmp=NULL;
	char* tmp_key=NULL;
	tmp = (Node*)malloc_with_check(sizeof(Node));
	if(tmp == NULL)
	{
		printf("Out of memory");
		exit(1);
	}
	tmp_key = (char*)malloc(sizeof(char)*(strlen(key)+1));
	if(tmp_key == NULL)
	{
		printf("Out of memory");
		exit(1);
	}
	strcpy(tmp_key,key);
	tmp->entry=entry;
	tmp->value = value;
	tmp->key = tmp_key;
	tmp->type = type;
	tmp->next = NULL;
	return tmp;
}

void destroy(Node* head)
{
	if(head==NULL)
		return;
	while(head!=NULL)
	{
		Node* tmp = head;
		free(tmp->key);
		tmp->key = NULL;
		head = head->next;
		free(tmp);
		tmp = NULL;
	}

}

void printTable(Node* head)
{
	while(head!=NULL)
	{
		printf("%ld %s %d %d\n",head->value,head->key,head->type,head->entry);
		head = head->next;
	}

}

void printTableByType(Node* head,instruction type)
{

	while(head!=NULL)
	{
		if(head->type == type || head->entry ==type)
			printf("%ld %s %d %d\n",head->value,head->key,head->type,head->entry);
		head = head->next;
	}

}



Node* insertToEnd(Node* head,char* key,long value,instruction type,instruction entry)
{
	if(head==NULL)
		return newNode(value,key,type,entry);
	else{
		Node* tmp = head;
		while(tmp->next!=NULL)
			tmp=tmp->next;
		tmp->next = newNode(value,key,type,entry);
	}

return head;
}

bool isLabelInTable(Node* head,char* label)
{
	if(head==NULL)
		return FALSE;
	else{
		while(head!=NULL)
		{
			if(!strcmp(label,head->key))
				return TRUE;
			head = head->next;	
		}
	return FALSE;
	}
	
}

instruction getLabelType(Node* head,char* label)
{
	instruction tmp = ERROR_INST;
	if(head==NULL)
		return FALSE;
	else{
		while(head!=NULL)
		{
			if(!strcmp(label,head->key))
			{
				tmp=head->type;
				return tmp;
			}
			head = head->next;	
		}
	return tmp;
	}
}

long getLabelValue(Node* head,char* label)
{
	unsigned long tmp = 0;
	if(head==NULL)
		return FALSE;
	else{
		while(head!=NULL)
		{
			if(!strcmp(label,head->key))
			{
				tmp=head->value;
				return tmp;
			}
			head = head->next;	
		}
	return tmp;
	}
}

bool isLabelInTableType(Node* head,char* label,instruction type)
{
	if(head==NULL)
		return FALSE;
	else{
		while(head!=NULL)
		{
			if(!strcmp(label,head->key) && (head->type == type || head->entry == type))
				return TRUE;
			head = head->next;	
		}
	return FALSE;
	}
	
}

/*ANOTHER----------------------------------------------- TABLE */
MachineNode* newMWNode(long adress,long mw ,ARE are,char* label,operandType opType)
{
	MachineNode* tmp=NULL;
	char* tmpLabel=NULL;
	tmp = (MachineNode*)malloc_with_check(sizeof(MachineNode));
	if(label!=NULL)
	{
		 tmpLabel=(char*)malloc_with_check(sizeof(char)*strlen(label)+1);
		 strcpy(tmpLabel,label);
	 }
	
	tmp->adress = adress;
	tmp->machineWord =(unsigned long)mw;
	tmp->are = are;
	if(opType) 
		tmp->opType=opType;
	else
		tmp->opType=NONE_OPERAND;
	
	tmp->next = NULL;
	tmp->label = tmpLabel;
	
	return tmp;
}

void destroyMW(MachineNode* head)
{
	if(head==NULL)
		return;
	while(head!=NULL)
	{
		MachineNode* tmp = head;
		if(tmp->label!=NULL)
			free(tmp->label);
		tmp->label=NULL;
		head = head->next;
		free(tmp);
		tmp = NULL;
	}

}

void printMWTable(MachineNode* head)
{
	while(head->next!=NULL)
	{
		printf("%04d %03x %d %s %d\n",(int)head->adress,(unsigned int)(head->machineWord&0xFFF),head->are,head->label,head->opType);
		head = head->next;
	}

}

void printMWTableToFile(MachineNode* head,FILE* f,long* ICF,long* DCF)
{
	char are;
	fprintf(f,"   %ld %ld\n",*ICF-100,*DCF);
	while(head->next!=NULL)
	{
		switch(head->are)
		{
			case A:
				are='A';
				break;
			case R:
				are='R';
				break;
			case E:
				are='E';
				break;
			case NONE_ARE:
				are='!';
				break;
		}
		fprintf(f,"%04d %03x %c\n",(int)head->adress,(unsigned int)(head->machineWord&0xFFF),are);
		head = head->next;
	}

}

MachineNode* insertMWToEnd(MachineNode* head,long adress,long mw,ARE are,char* label,operandType opType)
{
	
	if(head==NULL)
	{
		return newMWNode(adress,mw,are,label,opType);
	}
	else{
		MachineNode* tmp = head;
		while(tmp->next!=NULL)
			tmp=tmp->next;
		tmp->next = newMWNode(adress,mw,are,label,opType);
	}
return head;
}

bool changeNodeAtAdress(MachineNode** head,long adress,long mw,ARE are,char* label,operandType opType)
{
	MachineNode** tmp =NULL;
	if((*head)==NULL)
		return FALSE;
	else{
		tmp=head;
		while((*tmp)!=NULL)
		{
			if(adress==(*tmp)->adress)
			{
				if(mw)
					(*tmp)->machineWord=mw;
				if(are)
					(*tmp)->are=are;
				if(label)
					strcpy((*tmp)->label,label);
				if(opType)
					(*tmp)->opType=opType;
				return TRUE;
			}
			(*tmp) = (*tmp)->next;	
		}
	return FALSE;
	}
	
}
