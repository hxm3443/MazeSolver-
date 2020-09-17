/**
 * \queueADT.c
 * \Himani Munshi - (hxm3443@rit.edu)
 * \CSCI - 243 - 05
 * \Date: 11/3/2018
**/
#define QUEUE_ALLOC_UNIT 10

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * A structure representing a queue where-data is an array storing
 *                                        the queue
 *                                       -maxCapacity is the initial
 *                                        maximum capacity of the array
 *                                       -currSize is the current size of the
 *                                        array
**/
struct Queue_S {
    void **data;
    size_t maxCapacity;
    size_t currSize;
};

#include "queueADT.h"

/**
 * createQueue constructs an empty queue.
 * @param capacity represents the initial given capacity of the queue
 * @return a queue object (pointer)
 * @post the queue is not NULL (i.e. there was enough storage)
**/
Queue createQueue( size_t capacity ) {
    Queue new;
    new = (Queue) malloc( sizeof(struct Queue_S) );

    if( new != 0 ) {
        new->data = 0;
        new->maxCapacity = capacity;
        new->currSize = 0;
    }

    return ( new );
}

/**
 * isEmpty checks if the queue is empty or not
 * @param queue is a valid Queue data type instance.
 * @return 0 if not empty, otherwise 1.
**/
int isEmpty( Queue queue ) {
    return queue->currSize == 0;
}

/**
 * enqueue inserts an into the queue.
 * insertion takes place always from the end.
 * @param item the item to insert into the queue
 * @pre queue is a valid Queue data type instance.
 * @post queue assumes (temporary) ownership/custody of the item, and
 * the client must not delete the memory while queue owns it.
**/
void enqueue( Queue queue, void * item ) {
    if( queue->data == 0 ) {
        queue->data =  malloc( sizeof(void *)*QUEUE_ALLOC_UNIT );
        queue->maxCapacity = QUEUE_ALLOC_UNIT;
    }

    if( queue->currSize >= queue->maxCapacity ) {
        queue->data = realloc( queue->data, sizeof(void *) * 
			(queue->maxCapacity + QUEUE_ALLOC_UNIT) );
        queue->maxCapacity += QUEUE_ALLOC_UNIT;
    }

    queue->data[queue->currSize] = item;
    queue->currSize += 1;
}

/**
 * dequeue removes the item at the start of the queue.
 * deletion takes place from the front in a queue.
 * @queue is a valid Queue data type instance.
 * @pre queue is a non-empty, valid Queue data type instance.
 * @post client assumes ownership of the item removed.
**/
void * dequeue( Queue queue ) {
    if( !isEmpty( queue ) ) {
        void *temp = queue->data[0];
        unsigned int i = 1;
        while( i < queue->currSize ) {
            queue->data[i-1] = queue->data[i];
            i++;
        }
	queue->data[queue->currSize-1] = NULL;
	queue->currSize--;
        return temp;
    }
    return NULL;
}

/**
 * destroyQueue deletes all dynamic queue storage.
 * @param queue the subject queue
 * @pre queue is a valid Queue data type reference.
 * @pre client previously removed and deleted all entries that
 * involved dynamic memory allocation.
 * @post the queue reference is no longer valid.
**/
void destroyQueue( Queue queue ) {
    assert( queue != 0 );
    if( queue->data != 0 ) {
        free( queue->data );
    }
    free( queue );
}

