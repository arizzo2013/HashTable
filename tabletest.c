/*
NAME: ANTHONY RIZZO
COURSE NUMBER: ECE 2230
SECTION: 001
SEMESTER: SPRING 2017
ASSIGNMENT NUMBER: #4
HASHTABLE TESTING PROGRAM
*/

#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define SIZE 4000000

typedef struct d
{
    int number1;
} *d_t;

// function to create memory for new data and will store integer value in data
d_t createnode(int);
// this function will generate a random integer with decent uniformity over a certain range
int randominteger(int);
// function for creating an array of unique numbers
void CreateKeyList(int);
// this will return a unique key that hasn't been used yet for input into hashtable
int getrandomkey(int);

int keypicks[SIZE];     /*array of unique keys*/

int main (void)
{
    int Tablesize, choice, key, nput;
    int result;
    d_t data = NULL;
    d_t hreturn = NULL;
    srand(time(NULL));

    // this will set the size of the hashtable
    printf("Enter size of table: ");
    scanf("%d", &Tablesize);

    // creates list of unique keys. SIZE can be changed depending how many requests will be given.
    CreateKeyList(SIZE);
    // initializing hashtable
    table_t hashtable = Table_new(Tablesize);

    //int count = 0;
    // setting the number of requests to the hashtable
    int loops = 4000000;
    while(1)
    {
        //printf("Loop: %d\n", count++);
        //printf("******************************************************************\n\n");
        //printf("Please input one of the following NUMBERS:\n0. INSERT\n1. FIND\n2. DEL\n3. UPDATE\n4. QUIT\n5. Random Number\n\n");
        //scanf("%d", &choice);

        // counts down number of requests, last request will quit and free table
        if (loops > 0)
        {
            loops--;
            choice = randominteger(3);
        }
        else choice = 4;

        switch (choice)
        {
            // insert
            case 0:
                //printf("Enter key value: ");
                //scanf("%d", &key);
                //printf("Input a number: ");
                //scanf("%d", &nput);
                // random data value is genertated along. memory is created and stores data value.
                nput = randominteger(10000000);
                data = createnode(nput);
                // this will return a unique key not in use.
                key = getrandomkey(SIZE);
                result = Table_insert(hashtable, key, data);
                //if (result == 0) printf("key: %d, value: %d\n", key, nput);
                if (result < 0) printf("key-value pair not stored\n");
                break;
            // find
            case 1:
                //printf("Enter key value to lookup: ");
                //scanf("%d", &key);
                // searching for random key data
                key = randominteger(SIZE);
                hreturn = Table_find(hashtable, key);
                if (hreturn != NULL) printf("Data value found: %d\n", hreturn->number1);
                /*
                if (hreturn == NULL)
                {
                    printf("No data was found at this location\n\n");
                }
                else printf("Data value: %d\n", hreturn->number1);
                */
                break;
            // delete
            case 2:
                //printf("Enter key value to delete: ");
                //scanf("%d", &key);
                // searches table to see if random key value holds data.
                key = randominteger(SIZE);
                hreturn = Table_remove(hashtable, key);
                if (hreturn != NULL)
                {
                    printf("Value: %d has been deleted\n", hreturn->number1);
                    free(hreturn);
                }
                break;
            // update data
            case 3:
                printf("Enter key: ");
                scanf("%d", &key);
                printf("Enter new data: ");
                scanf("%d", &nput);
                data = createnode(nput);
                result = Table_update(hashtable, key, data);
                if (result < 0) printf("Could not update data!!\n");
                break;
            // quit
            case 4:
                Table_free(hashtable);
                exit(0);
                break;
            // random number generator
            case 5:
                printf("Random number: %d\n", randominteger(3));
                break;

            default:
                printf("Incorrect choice!!\n");
                break;
        }
    }
    return 0;
}

// random number generator function
int randominteger(int range)
{
    if((range-1) == RAND_MAX) return rand();
    else
    {
        long end = RAND_MAX / range;
        assert(end > 0L);
        end *= range;
        int r;
        while((r = rand()) >= end);
        return r % range;
    }
}

// creating memory for data structure
d_t createnode(int data)
{
    d_t d1 = (struct d *)malloc(sizeof(struct d));
    if (d1 == NULL) return NULL;
    d1->number1 = data;
    return d1;
}

// filling a list of unique keys
void CreateKeyList(int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        keypicks[i] = i;
    }
}
// returns a random unique key value
int getrandomkey(int size)
{
    int p = -1;
    int i;
    while (p < 0)
    {
        i = randominteger(size);
        p = keypicks[i];
    }
    keypicks[i] = -1;
    return p;
}
