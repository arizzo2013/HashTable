/*
NAME: ANTHONY RIZZO
COURSE NUMBER: ECE 2230
SECTION: 001
SEMESTER: SPRING 2017
ASSIGNMENT NUMBER: #4
Seperate chaining Hashtable
*/

#include "table.h"
#include <stdio.h>
#include <stdlib.h>

// structure singly linked list input
typedef struct node_s
{
    data_t data;
    key_t key;
    struct node_s *next;
} *node_t;

// structure for individual buckets
typedef struct hashnode_s
{
    int count;
    struct node_s * head;
} *hashnode_t;

typedef struct table
{
    struct hashnode_s * bucket;
    int TableCount;
    int TableSize;
} table;

node_t NewNode(key_t, data_t);
int hashSlingingSlasher(key_t, int);

// Creating new hash table
table_t Table_new(int table_size)
{
    int i;
    // allocate memory for chain array of linked lists
    if (table_size < 1) return NULL;
    table_t htable = (struct table *)calloc(1, sizeof(struct table));
    //table_t htable = (struct table *)calloc(table_size, sizeof(struct table));
    if (htable == NULL) return NULL;
    htable->TableSize = table_size;
    htable->TableCount = 0;

    htable->bucket = (struct hashnode_s *)calloc(table_size, sizeof(struct hashnode_s));
    if (htable->bucket == NULL) return NULL;
    for (i = 0; i < table_size; i++)
    {
        htable->bucket[i].count = 0;
        htable->bucket[i].head = NULL;
    }
    return htable;
}

// inserting a node into hashtable, return -1 for fail, 0 for pass
int Table_insert(table_t htable, key_t key, data_t data)
{
    // get hash index and create the node
    int index = hashSlingingSlasher(key, htable->TableSize);
    node_t node_in = NewNode(key, data);
    if (node_in == NULL) return -1;
    // insert new node into table_t
    if ((htable->bucket[index]).head == NULL)
    {
        // or htable->bucket[index]->head->next == NULL??
        htable->bucket[index].head = node_in;
        htable->bucket[index].count = 1;
        htable->TableCount++;
        return 0;
    }
    // not the only element at this bucket
    node_in->next = htable->bucket[index].head;
    htable->bucket[index].head = node_in;
    htable->bucket[index].count++;
    htable->TableCount++;
    return 0;
}

// find data given a specific key
data_t Table_find(table_t htable, key_t key)
{
    if (key < 0 || htable == NULL) return NULL;
    int index = hashSlingingSlasher(key, htable->TableSize);
    node_t temp;
    int i;
    if (htable->bucket[index].count == 0) return NULL;
    if (htable->bucket[index].count == 1)
    {
        temp = htable->bucket[index].head;
        if (key == temp->key) return temp->data;
        else return NULL;
    }
    //  must be more than one element chained at this bucket
    temp = htable->bucket[index].head;
    i = 0;
    while(i < htable->bucket[index].count)
    {
        if (key != temp->key)
        {
            if (temp->next != NULL)
            {
                temp = temp->next;
                i++;
            }
            else break;
        }
        else break;
    }
    if (key != temp->key) return NULL;
    return temp->data;
}

// update the data for an item already entered. return -1 for failure, 0 for pass
int Table_update(table_t htable, key_t key, data_t data)
{
    node_t temp;
    data_t del;
    if (key < 0 || htable == NULL) return -1;
    int index = hashSlingingSlasher(key, htable->TableSize);
    int i;
    if (htable->bucket[index].count == 0) return -1;
    if (htable->bucket[index].count == 1)
    {
        if ((htable->bucket[index].head)->key == key)
        {
            del = (htable->bucket[index].head)->data;
            (htable->bucket[index].head)->data = data;
            free(del);
            return 0;
        }
        else return -1;
    }
    //  must be more than one element chained at this bucket
    temp = htable->bucket[index].head;
    i = 0;
    while(i < htable->bucket[index].count)
    {
        if (key != temp->key)
        {
            if (temp->next != NULL)
            {
                temp = temp->next;
                i++;
            }
            else break;
        }
        else break;
    }
    if (key != temp->key) return -1;
    del = temp->data;
    temp->data = data;
    free(del);
    return 0;
}

// searches for index of the key given, if the found with data, will return data and free node.
data_t Table_remove(table_t htable, key_t key)
{
    if (key < 0 || htable == NULL) return NULL;
    int index = hashSlingingSlasher(key, htable->TableSize);
    node_t temp, node1;
    data_t data = NULL;
    if (htable->bucket[index].count == 0) return NULL;
    node1 = htable->bucket[index].head;
    temp = node1;
    while (node1 != NULL)
    {
        if (node1->key == key)
        {
            if (node1 == htable->bucket[index].head) htable->bucket[index].head = node1->next;
            else
            {
                // if temp == head check necessary????
                if (temp == htable->bucket[index].head) (htable->bucket[index].head)->next = node1->next;
                else temp->next = node1->next;
            }
            data = node1->data;
            free(node1);
            htable->bucket[index].count--;
            htable->TableCount--;
            break;
        }
        temp = node1;
        node1 = node1->next;
    }
    // wiil return a NULL pointer if data was never set to anything else
    return data;
}

// free all resources used by the hashtable
void Table_free (table_t htable)
{
    int i, j;
    node_t temp, delete;
    // freeing all bucket chains
    for (i = 0; i < htable->TableSize; i++)
    {
        temp = htable->bucket[i].head;
        for (j = 0; j < htable->bucket[i].count; j++)
        {
            delete = temp;
            temp = temp->next;
            free(delete->data);
            free(delete);
            delete = NULL;
        }
    }
    free(htable->bucket);
    free(htable);
}

// hashing function
int hashSlingingSlasher(key_t key, int size)
{
    return key % size;
}

// creating a new node function
node_t NewNode(key_t key, data_t data)
{
    node_t new = (struct node_s *)calloc(1, sizeof(struct node_s));
    if (new == NULL) return NULL;
    new->key = key;
    new->data = data;
    new->next = NULL;
    return new;
}
