/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2017
 */
#include "camelCaser.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
void removeSpace(char*str){

int count = 0;

for(int i = 0; str[i];i++)
	if(str[i]!=' ')
		str[count++] = str[i];
str[count] = '\0';
}



char **camel_caser(const char *input_str) 
{ 
	if(!input_str)
		return NULL;
	char*temp = strdup(input_str);
	int loc = 0;
	int punctNum = 0;
	//for tokenlize
	while(temp[loc])
   {
	if(ispunct(temp[loc]))
	{
	temp[loc] = '.';
	punctNum++;	
	}
	loc++;
   }
   //punctNum--;

	//mallocSpace Section1
	char** result = malloc((punctNum+1)*sizeof(char*));
	result[punctNum] = NULL;//last one is null
	//tokenized
	 char* start;
	int s1Temp = 0;
	while((start = strsep(&temp,"."))!= NULL)
	{
	//for extra \0
	//printf("%s\n",start);
	char* emp = "";
	if(!strcmp(start,emp))
	{
	result[s1Temp] = "";
	s1Temp++;
	continue;
	}	
	char* token = strdup(start);
	result[s1Temp]  = token;
	s1Temp++;
	}
	result[punctNum] = NULL;

	//we have to CamleCaseEach Sec2
	int s2Temp = 0;
//	int spaceCounter =0;
//	int NotTheF = 0;

	while(s2Temp < s1Temp)
	{
	int spaceCounter = 0;
	int NotTheF = 0;
	char* Curr = result[s2Temp];
	if(Curr == NULL)
	{
		s2Temp++;
		continue;
	}
	char currChar;
	char* empty = "";
		if(!strcmp(Curr ,empty))
			{
			s2Temp++;
			continue;
			}

		for(int i = 0;Curr[i]!='\0';i++)
		{	
		currChar = Curr[i];
		if(i == 0&& isalpha(currChar))
			{
			Curr[i] = tolower(currChar);
			NotTheF = 1;
			}	
		else
			{
			
			if(isspace(currChar)&& NotTheF == 1)
				{
				spaceCounter = 1;
				continue;
				
				}
			if(isdigit(currChar))
				{
				NotTheF = 1;
				continue;	
				}
			if(spaceCounter==1 && isalpha(currChar))
				{
				Curr[i] = toupper(currChar);
				spaceCounter = 0;
				NotTheF = 1;
				}
			else if(isalpha(currChar))
				{
				Curr[i] = tolower(currChar);
				NotTheF = 1;//check
				}
			}		
		}
//	printf("%s\n",Curr);
	fflush(stdout);
	removeSpace(Curr);
	s2Temp++;
	}
	return result;
	}


