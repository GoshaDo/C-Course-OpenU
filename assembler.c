#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "func.h"
#include "firstpass.h"
#include "table.h"
#include "secondpass.h"

static int process_file();

int main(int argc,char *argv[]){
	int i=0;
	int res;
	if (argc <= 1)
	{
		printf("Missing file name\n");
		exit(1);
	}
	for (i=1;i<argc;++i)
	{	
		res = process_file(argv[i]);
		if(!res)
			printf("ERROR in %s skiping the file\n",argv[i]);
	}


	return 1;
}

static int process_file(char *file_name)
{
	char *input_filename;
	FILE *file_des; /* Current assembly file descriptor to process */
	FILE *ObFile;
	FILE *EntFile;
	FILE *ExtFile;
	char Ob[4]=".ob";
	char Ent[5]=".ent";
	char Ext[5]=".ext";
	char* ObName=NULL;
	char* EntName=NULL;
	char* ExtName=NULL;
	line_info curr_line_info;
	char temp_line[MAX_LINE_LENGTH + 2]; /* temporary string for storing line, read from file */
	int temp_c;
	long ic=IC_INIT_VALUE , dc=0,DCF=0,ICF=0,i=0;
	bool error_occurred=FALSE;
	Node* table=NULL;
	MachineNode* table2=NULL;
	long data_image[MAX_IMG_LENGTH];
	fillArr(data_image,SIZE_OF_ARR(data_image,long)); /*intialzing data image with 0*/
	/* Concat extensionless filename with .as extension */
	input_filename = strallocat(file_name, ".as");
	file_des = fopen(input_filename, "r");

	if (file_des == NULL) 
	{
		/* if file couldn't be opened, write to stderr. */
		printf("Error: file \"%s.as\" is inaccessible for reading. continue...\n", file_name);
		free(input_filename); /* The only allocated space is for the full file name */
		return FALSE;
	}
	
	/*first pass*/
	curr_line_info.file_name = input_filename;
	curr_line_info.content = temp_line; 
	/*loop throw each line at the file*/
	for (curr_line_info.line_number = 1;fgets(temp_line, MAX_LINE_LENGTH + 2, file_des) != NULL; curr_line_info.line_number++)
	{
		/*check if the line too long*/
		if (strchr(temp_line, '\n') == NULL && !feof(file_des)) 
		{

			print_error(curr_line_info,"Maximum line length reached");
			error_occurred = TRUE;
			/*go the the end of the line*/
			while (temp_c != '\n' && temp_c != EOF)
			{
				temp_c = fgetc(file_des);
			}
		}else{
				
				/*Initate first pass at first line*/
				 if(!process_line_fpss(curr_line_info,&ic,&dc,&table,data_image,&table2))
				 {
					error_occurred = TRUE;
				 }
		     }
	}
	if(error_occurred)
		return FALSE;
	
	ICF=ic;
	DCF=dc;
	/*insert data image to machine table*/
	while(i<=dc)
	{
		table2=insertMWToEnd(table2,ic+i,data_image[i],A,NULL,NONE_OPERAND);
		i++;	
	}
	
	/*insert parameters to table*/
	fillTable(&table,&table2,ICF,DCF);
	/*second pass*/
	rewind(file_des);
	for (curr_line_info.line_number = 1;fgets(temp_line, MAX_LINE_LENGTH + 2, file_des) != NULL; curr_line_info.line_number++)
	{
		
		if(!process_line_spss(curr_line_info,&table,&table2))
		{
			error_occurred = TRUE;
		}
	}
	if(error_occurred)
		return FALSE;
	/*writing file - no errors till now*/
	


	
	ObName=strallocat(file_name,Ob);
	EntName=strallocat(file_name,Ent);
	ExtName=strallocat(file_name,Ext);
	ObFile=fopen(ObName,"w");
	if (ObFile==NULL)
	{
		printf("Can't open %s file\n",ObName);
		exit(1);
	}
	EntFile=fopen(EntName,"w");
	if (EntFile==NULL)
	{
		printf("Can't open %s file\n",EntName);
		exit(1);
	}
	ExtFile=fopen(ExtName,"w");
	if (ExtFile==NULL)
	{
		printf("Can't open %s file\n",ExtName);
		exit(1);
	}
	/*writing*/
	printMWTableToFile(table2,ObFile,&ICF,&DCF);/*prints object file*/
	printExtEntfiles(table2,table,EntFile,ExtFile,&ICF,&DCF);/*prints ent file and enr file*/
	/*closing files*/
	
	if(ObName) free(ObName);
	if(EntName) free(EntName);
	if(ExtName) free(ExtName);
	ObName=NULL;
	EntName=NULL;
	ExtName=NULL;

	if(fclose(file_des)!=0){printf("Eror in closing file %s\n",file_name);exit(1);}
	if(fclose(ObFile)!=0){printf("Eror in closing file %s\n",ObName);exit(1);}
	if(fclose(ExtFile)!=0){printf("Eror in closing file %s\n",EntName);exit(1);}
	if(fclose(EntFile)!=0){printf("Eror in closing file %s\n",ExtName);exit(1);}
	


	destroyMW(table2);
	destroy(table);
	free(input_filename);
	return !error_occurred;
}
