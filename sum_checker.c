#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS 11 /* Hint */
#define PUZZLE_SIZE 9

int sum=0;

/* example puzzle */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0}
		};

/* data structure for passing data to threads */
typedef struct
{
	int thread_number;
	int x;
	int y;
} parameters;

/* print puzzle */ 
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10])
{
    int i,j;
    printf(LINE);
    printf(NAME);
    printf(LINE);

	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
	        printf("|%1d |",grid[i][j]);
		}
        printf(COLLINE);
	}
    printf("\n");
}
// read file to check sudoku
void SetPuzzle(char filename[]){
    FILE *file = fopen(filename,"r");
    int i,j,ch,num;
    for (i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            while(((ch = getc(file)) != EOF)){
                if(ch == EOF) break;
                if(ch == ',') break;
                if(ch == '\n') break;
                ungetc(ch,file);
                fscanf(file,"%d",&num);
                if(num!=-1)
                    puzzle[i][j] = num;            
            } 
        }
    }
    print_grid(puzzle);
    return;
}

void* checkRow()
{
    for(int i=1;i<PUZZLE_SIZE+1;i++)
    {
        int sumR=0;
        for(int j=1;j<PUZZLE_SIZE+1;j++)
        {
            sumR+=puzzle[i][j];
        }
        if(i==1)
        {
            sum=sumR;
        }
        if(sum!=sumR)
        {
            //printf("Row %d\n",i);
            pthread_exit((void*) 0);
        }
        else
        {
            continue;
        }
    }
    //sleep(10);
    pthread_exit((void*) 1);
}

void* checkCol()
{
    for(int i=1;i<PUZZLE_SIZE+1;i++)
    {
        int sumC=0;
        for(int j=1;j<PUZZLE_SIZE+1;j++)
        {
            sumC+=puzzle[j][i];
        }
        if(sum!=sumC)
        {
            //printf("Col %d\n",i);
            pthread_exit((void*) 0);
        }
        else
        {
            continue;
        }
    }
    //sleep(10);
    pthread_exit((void*) 1);
}

void* checkBlock(void *input)
{
    int block=(int*) input;
    int sumB=0;
    for(int i=(block%3)*3+1; i<(block%3)*3+4; i++)
    {
        
        for(int j=(block/3)*3+1; j<(block/3)*3+4; j++)
        {
            sumB+=puzzle[i][j];
        }
    }
    //sleep(10);
    if(sum!=sumB)
        pthread_exit((void*) 0);
    else
        pthread_exit((void*) 1);
}

int main(int argc, char* argv[])
{
	int rv = 1; // flag to check answer
    // input the sudoku file
    pthread_t tr,tc,tb[9];
    void *re;
    SetPuzzle("test2.txt");
    pthread_create(&tr, NULL, checkRow, NULL);
    pthread_create(&tc, NULL, checkCol, NULL);
    for (int i=0; i<PUZZLE_SIZE ; i++)
    {
        pthread_create(&tb[i], NULL, checkBlock, (void*)i);
    }
    pthread_join(tr, &re);
    rv=rv&&(int*)re;
    pthread_join(tc, &re);
    rv=rv&&(int*)re;
    for (int i=0; i<PUZZLE_SIZE ; i++)
    {
        pthread_join(tb[i], &re);
        rv=rv&&(int*)re;
    }
    
	if (rv == 1)
		printf("Successful :) \n");
	else
		printf("Must check again! :( \n");

	return 0;
}
