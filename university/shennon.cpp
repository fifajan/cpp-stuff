#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

//0.0166, 0.0379, 0.0127, 0.0212, 0.0486, 0.0499, 0.0558, 0.0347, 0.0505, 0.0170, 0.0274, 0.0102, 0.0147, 0.0140, 0.0256, 0.0544, 0.0197, 0.0439, 0.0539, 0.0501, 0.0344, 0.0444, 0.0173, 0.0028, 0.0475, 0.0448, 0.0036, 0.0373, 0.0458, 0.0385, 0.0234, 0.0014, 
#define _ 0.5
#define STR_LEN_32 16
#define STR_LEN_64 32
int params;
int str_len;
int paramParser(int argc, char **argv)
{
	int result= 0;

	if(getopt(argc, argv, "o")== 'o') 
	{
		result= 1;
		if(getopt(argc, argv, "p")== 'p')
			result++;
	}
	return(result);
}

float sort(float *first, int length)
{
	int i, j;
	float _first, _second, sum= 0.0;
	for(i= 0; i< length; i++)
		for(j= i+ 1; j< length; j++)
		{
			_first= *(first + i);
			_second= *(first + j);
			if(_second > _first)
			{	
				*(first + i)= _second;
				*(first + j)= _first;
			}
		}
	for(i= 0; i< length; i++) sum+= *(first + i);
	return(sum);
}
void clearCodes(char *code)
{
	int i;
	for(i= 0; i< 32; i++)
		strcpy(code+ 32*i, "\0");
}
void clearCodes1024(char *code)
{
	int i;
	for(i= 0; i< 1024; i++)
		strcpy(code+ 32*i, "\0");
}

void appendNewBit(int startIndex, int middleIndex, int stopIndex, char *code)
{
	int i;
	if(startIndex== stopIndex)
		strcat(code+ 32*startIndex, "1\0");
	else
	{
		for(i= startIndex; i<= stopIndex; i++)
			if(i<= middleIndex) strcat(code+ 32*i, "1\0");
			else 				strcat(code+ 32*i, "0\0");
	}

}
void printStep(int startIndex, int middleIndex, int stopIndex, float *probability, char *code, int depth)
{
	int i, j, value= 1;
	char space[]= "    ";


	for(j= 0; j< depth; j++) printf("%s", space);
	printf("_______________________________\n");

	value= 1;	
	for(i= startIndex; i<= middleIndex; i++)
	{
		for(j= 0; j< depth; j++) printf("%s", space);

		printf("| %02i | %1i | %9s | %1.4f |\n", i, value, code + 32*i, *(probability + i));
	}

	if(startIndex!= stopIndex)
	{
		for(j= 0; j< depth; j++) printf("%s", space);
		printf("|----|---|-----------|--------|\n");
		value= 0;
		for(i= middleIndex+ 1; i<= stopIndex; i++)
		{
			for(j= 0; j< depth; j++) printf("%s", space);

			printf("| %02i | %1i | %9s | %1.4f |\n", i, value, code + 32*i, *(probability + i));
		}
	}

	for(j= 0; j< depth; j++) printf("%s", space);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

}
void countPairProbability(float *probability1024, float *probability, int length)
{
	int i, j, k;
	for(k= 0; k< length*length; k++)
		for(i= 0; i< length; i++)
			for(j= 0; j< length; j++)
				*(probability1024 + i*length + j)= *(probability + i) * ( *(probability + j));
}
float enthropy(float *probability, int length)
{
	float result= 0.0;
	int i;
	for(i= 0; i< length; i++)
		result+= *(probability + i)*log2f(*(probability + i));

	return(-result);

}
float midCodeLength(char *code, float *probability, int length)
{
	float result= 0;
	int i;
	for(i= 0; i< length; i++)
		result+= strlen(code + 32*i)* (*(probability + i));

	return(result);
}



int process(int startIndex, int stopIndex, float neededProbability, float *probability, char *code, int depth)
{
	float sumProbability;
	int i, j;
	bool last= 0;
	if(stopIndex == startIndex) return 0;
	else
	{
		sumProbability= 0.0;
		for(i= startIndex; i<= stopIndex; i++)
		{
			sumProbability+= *(probability + i);
			//printf("= %4.4f\n", sumProbability);
			last= (i+1 == stopIndex);
			if((sumProbability>= neededProbability)|| last)
			{
				if(fabs(neededProbability - sumProbability) <= 
				   fabs(neededProbability - (sumProbability - (*(probability + i)))))
					j= i;
				else
				{
					j= i-1;
					sumProbability-= *(probability + i);
				}
			
				appendNewBit(startIndex, j, stopIndex, code);
				if(params >= 1) 
				{
					printStep(startIndex, j, stopIndex, probability, code, depth);
					if(params >= 2) getchar();
				}
				process(startIndex, j, 0.5*sumProbability, probability, code, depth+ 1);
				if(params >= 1) 
				{
				 	printStep(startIndex, j, stopIndex, probability, code, depth);
					if(params >= 2) getchar();
				}
				process(j+1, stopIndex, neededProbability- 0.5*sumProbability, probability, code, depth+ 1);
				break;
			}
		}
	}
}
int main(int argc, char **argv)
{
	float probability[]= 
	{
		0.0166, 0.0379, 0.0014, 
		0.0212, 0.0486, 0.0499, 
		0.0558, 0.0347, 0.0505, 
		0.0170, 0.0274, 0.0102, 
		0.0147, 0.0140, 0.0256, 
		0.0544, 0.0197, 0.0439, 
		0.0539, 0.0501, 0.0344, 
		0.0444, 0.0173, 0.0028, 
		0.0475, 0.0448, 0.0036, 
		0.0373, 0.0458, 0.0385, 
		0.0234, 0.0127 
	};

	float probability1024[1024];

	char code[1024]; //32 by 16
	char code1024[32768]; //1024 by 16

	int i;
	float totalProbability, totalProbability1024;
	params= paramParser(argc, argv);

	totalProbability= sort(&probability[0], 32);
	countPairProbability(&probability1024[0], &probability[0], 32);
	totalProbability1024= sort(&probability1024[0], 1024);
	clearCodes(&code[0]);
	process(0, 31, 0.5*totalProbability, &probability[0], &code[0], 0);

	if(params >= 1) 
		for(i=0; i< 32; i++)
			printf("N= %02i | %4.4f | %s\n", i, probability[i], code+ 32*i);

	//params= 0;
	clearCodes(&code1024[0]);
	process(0, 1023, 0.5*totalProbability1024, &probability1024[0], &code1024[0], 0);

	//getchar();
	printf("Enthropy= %4.4f; MiddleCodeLength= %4.4f; MiddleCodeLength1024= %4.4f\n", \
			enthropy(&probability[0], 32), midCodeLength(&code[0], probability, 32), _ * midCodeLength(&code1024[0], probability1024, 1024));
	printf("1st length= %5.5f\n", totalProbability1024);
	return(0);
}
