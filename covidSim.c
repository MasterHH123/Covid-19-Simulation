#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CELLS 400
#define RC 20
#define MIN 0

#define HEALTHY 0
#define SICK 1
#define RECOVERED 2
#define DEAD 3

struct CELL{
    char cellStatus;
    int id;
    int age;
    char personStatus;
    char vaccinated;
    char daysSick;
    char mask;
};

struct CELL data[RC][RC];//Matrix
struct CELL *pdata=&data[0][0]; //Pointer used for the matrix
int totalSick;

void init(struct CELL *data);
void age(struct CELL *data);
void ID(struct CELL *data);
int getRandom(int min, int max);
void printMatrix(struct CELL *c);
void cases(struct CELL *data);
void processingCases(struct CELL *data, int cell, int value);

int main(){
    srand(time(NULL));
    init(pdata);
    age(pdata);
    ID(pdata);
    printMatrix(pdata);
    cases(pdata);
}

//Function to initialze the matrix
void init(struct CELL *data)
{
    int success = 0; //flag
    int nCell;
    int d;
    int ocuppiedCells = (0.75*CELLS);
    int percentageVax = (0.25*ocuppiedCells);
    int count = 0;

    while(success<ocuppiedCells) //Initializes 75% of the cells as occupied
    {
        nCell = getRandom(MIN, CELLS);
        d = (data+nCell) -> cellStatus;//stores value of a random cell

        if(d==0)
        {
            (data+nCell) -> cellStatus=1;
            (data+nCell) -> personStatus=HEALTHY;

            if(count<percentageVax)//other 25%
            {
                (data+nCell) -> vaccinated=0;
                (data+nCell) -> mask=0;
                count++;
            }
            else if (count >= percentageVax && count<percentageVax*2)//other 25%
            {
                (data+nCell) -> vaccinated=0;
                (data+nCell) -> mask=1;
                count++;
            }
            else if(count >= percentageVax && count<percentageVax*3)//other 25%
            {
                (data+nCell) -> vaccinated=1;
                (data+nCell) -> mask=0;
            }
            else if(count>=percentageVax*3 && count < percentageVax*4)//other 25%
            {
                (data+nCell) -> vaccinated=1;
                (data+nCell) -> mask=1;
                count++;
            }
            success++;
        }
    }
    nCell=getRandom(MIN, CELLS);//start the epidemic
    (data+nCell) -> cellStatus=1;
    (data+nCell) -> personStatus=SICK;
    (data+nCell) -> daysSick=1;
    totalSick++;
}

void age(struct CELL *data)
{
    int num = 0;
    int i = 0;
    for(i=0;i<CELLS;i++)
    {
        if((data+i) -> cellStatus == 1)//asigning age to a person
        {
            num = getRandom(MIN, 91);//max age 90
            (data+i) -> age = num;
        }
    }
}

void ID(struct CELL *data)
{
    int num = 1;
    for(int i = 0;i<CELLS;i++)
    {
        (data+i) -> id=num;
        num++;
    }
}

int getRandom(int min, int max)
{
    int rNumber = rand() % max + min;
    return rNumber;
}

void printMatrix(struct CELL *c)
{
    int S=0, R=0, empty=0, dead=0;
    int sick=0;

    for(int rows = 0; rows < RC; rows++)
    {
        printf("[");
        for(int columns = 0;columns<RC;columns++)
        {
            if(c -> cellStatus==0)
            {
                printf("  ");
                empty++;
            }
            else{
                if(c -> vaccinated==0)
                {
                    if(c -> personStatus==HEALTHY)
                    {
                        if(c -> mask==0)
                        {
                            printf(" H ");//healthy person without
                        }
                        else{
                            printf(" h ");//healthy person without vax but wiht mask
                        }
                    }
                }
                else
                {
                    if(c -> vaccinated==1)
                    {
                        if(c -> personStatus==HEALTHY)
                        {
                            if(c -> mask==0)
                            {
                                printf(" V ");//vaxed no mask
                            }
                            else{
                                printf(" v ");//vaxed with mask
                            }
                        }
                    }
                }
            }
            if(c -> personStatus==SICK)
            {
                if(c -> mask==0)
                {
                    printf(" S ");
                    S++;
                }
                else{
                    printf(" s ");
                    S++;
                }
            }
            if (c -> personStatus==dead)
            {
                printf(" x ");
                dead++;
            }
            else
            {
                if(c -> personStatus==RECOVERED)
                {
                    printf(" r ");
                    R++;
                }
            }
            c++;
        }
        printf("]\n");
    }
    printf("\nSick people: %d", S);
    printf("\nRecoveries: %d\n", R);
    printf("Empty slots: %d\n", empty);
    printf("Deaths: %d\n", dead);
}

void cases(struct CELL *data)
{
    char position = 0;
    int day = 1;
    while (totalSick>0)//Ends when they're no more people with covid-19
    {
        printf("\nDay %d\n", day);
        for(int i = 0;i<CELLS;i++)
        {
            if((data+i) -> personStatus==SICK)
            {
                (data+i) -> daysSick++;
                if((data+i) -> daysSick==8 && (data+i) -> personStatus != DEAD)
                {
                    (data+i) -> personStatus = RECOVERED;
                    totalSick--;
                }
                int probm = getRandom(0, 100);
                if((data+i) -> vaccinated==0)
                {
                    if(probm <= 10)
                    {
                        (data+i) -> personStatus=DEAD;
                        totalSick--;
                    }
                }
                if ((data+i) -> vaccinated==1)
                {
                    if(probm <= 2)
                    {
                        (data+i) -> personStatus=DEAD;
                        totalSick--;
                    }
                }

                //possible cases without counting the edges
                if(i <= 1 && i>= 18)
                {
                    processingCases(data, i, 1);
                    processingCases(data, i, -1);
                    processingCases(data, i, 20);
                    processingCases(data, i, 21);
                    processingCases(data, i, 19);

                }
                else if(i<=376 && i>=398)
                {
                    processingCases(data, i, 1);
                    processingCases(data, i, -1);
                    processingCases(data, i, -20);
                    processingCases(data, i, -21);
                    processingCases(data, i, -19);
                }
                else if(i % 20 == 0 && i != 0 && i != 380)
                {
                    processingCases(data, i, -20);
                    processingCases(data, i, -19);
                    processingCases(data, i, 1);
                    processingCases(data, i, 21);
                    processingCases(data, i, 19);
                }
                else if((i+20) % 20 == 0 && i != 19 && i != 399)
                {
                    processingCases(data, i, -20);
                    processingCases(data, i, -21);
                    processingCases(data, i, -1);
                    processingCases(data, i, 19);
                    processingCases(data, i, 20);
                }
                else if(i == 0 || i == 19 || i == 375 || i == 399)
                {
                    if(i == 0)
                    {
                        processingCases(data, i, 1);
                        processingCases(data, i, 21);
                        processingCases(data, i, 20);
                    }
                    else if(i == 19)
                    {
                        processingCases(data, i, -1);
                        processingCases(data, i, 19);
                        processingCases(data, i, 20);
                    }
                    else if(i == 375)
                    {
                        processingCases(data, i, -20);
                        processingCases(data, i, -19);
                        processingCases(data, i, 1);
                    }
                    else if(i == 399)
                    {
                        processingCases(data, i, -20);
                        processingCases(data, i, -21);
                        processingCases(data, i, -1);
                    }
                }
                else {
                    processingCases(data, i, -20);
                    processingCases(data, i, -21);
                    processingCases(data, i, -1);
                    processingCases(data, i, 19);
                    processingCases(data, i, 20);
                    processingCases(data, i, 21);
                    processingCases(data, i, 1);
                    processingCases(data, i, -19);
                }
            }
        }
        day++;
        printMatrix(data);
    }
}

void processingCases(struct CELL *data, int cell, int value)
{
    int num = 0;
    if((data+(cell+value)) -> cellStatus==1)
    {
        if((data+cell) -> mask == 0 && (data+(cell+value)) -> mask == 0)//nobody has a mask
        {
            if((data+(cell+value)) -> personStatus == HEALTHY && (data+(cell+value)) -> vaccinated==0)//neighbor cell is healthy and doesnt have a mask
            {
                num=getRandom(1, 100);
                if(num<=90)
                {
                    (data+(cell+value)) -> personStatus=SICK;
                    (data+(cell+value)) -> daysSick=0;
                    totalSick++;
                }
            }
            else
            {
                if((data+(cell+value)) -> personStatus==HEALTHY && (data+(cell+value)) -> vaccinated==1)//healthy and vaxxed
                {
                    num=getRandom(1, 100);
                    if(num<=45)
                    {
                        (data+(cell+value)) -> personStatus=SICK;
                        (data+(cell+value)) -> daysSick=0;
                        totalSick++;
                    }
                }
            }
        }
        else if((data+cell) -> mask == 1 || (data+(cell+value)) -> mask == 1)// at least 1 person has a mask
        {
            if((data+(cell+value)) -> personStatus==HEALTHY && (data+(cell+value)) -> vaccinated==0)
            {
                num=getRandom(1, 100);
                if(num<=20){
                    (data+(cell+value)) -> personStatus=SICK;
                    (data+(cell+value)) -> daysSick=0;
                    totalSick++;
                }
            }
            else 
            {
                if((data+(cell+value)) -> personStatus==HEALTHY && (data+(cell+value)) -> vaccinated==1)//neighbor is sicked but vaxxed
                {
                    num=getRandom(1, 100);
                    if(num <= 15)
                    {
                        (data+(cell+value)) -> personStatus=SICK;
                        (data+(cell+value)) -> daysSick=0;
                        totalSick++;
                    }
                }
            }
        }
        else if((data+cell) -> mask == 1 && (data+(cell+value)) -> mask == 1)//2 masks
        {
            if((data+(cell+value)) -> personStatus==HEALTHY && (data+(cell+value)) -> vaccinated==0)//neighbor isnt vaxxed
            {
                num = getRandom(1, 100);
                if(num<=10)
                {
                    (data+(cell+value)) -> personStatus=SICK;
                    (data+(cell+value)) -> daysSick=0;
                    totalSick++;
                }
            }
            else 
            {
                if((data+(cell+value)) -> personStatus==HEALTHY && (data+(cell+value)) -> vaccinated==1)//neighbor isn't vaxxed
                {
                    num=getRandom(1, 100);
                    if(num<=5)
                    {
                        (data+(cell+value)) -> personStatus=SICK;
                        (data+(cell+value)) -> daysSick=0;
                        totalSick++;
                    }
                }
            }
        }
    }
}