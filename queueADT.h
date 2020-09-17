/**
 * \queueADT.h
 * \Himani Munshi - (hxm3443@rit.edu)
 * \CSCI - 243 - 05
 * \Date: 11/3/2018
**/

#ifndef QUEUEADT_H
#define QUEUEADT_H 

/// General Notes on queueADT Operation
/// The operations identified below are the public interface to queueADT.

/// The Queue data type is opaque to clients.

typedef struct Queue_S * Queue;

/// createQueue constructs an empty queue.
/// @param capacity represents the initial given capacity of the queue
/// @return a queue object (pointer)
//  @post the queue is not NULL (i.e. there was enough storage)
Queue createQueue( size_t capacity );

/// enqueue inserts an into the queue. 
/// insertion takes place always from the end.
/// @param item the item to insert into the queue
/// @pre queue is a valid Queue data type instance.
/// @post queue assumes (temporary) ownership/custody of the item, and
/// the client must not delete the memory while queue owns it.
void enqueue( Queue queue, void * item );

/// isEmpty checks if the queue is empty or not 
/// @param queue is a valid Queue data type instance.
/// @return 0 if not empty, otherwise 1. 
int isEmpty( Queue queue );

/// dequeue removes the item at the start of the queue.
/// deletion takes place from the front in a queue.
/// @queue is a valid Queue data type instance.
/// @pre queue is a non-empty, valid Queue data type instance.
/// @post client assumes ownership of the item removed.
void *dequeue( Queue queue );

/// destroyQueue deletes all dynamic queue storage.
/// @param queue the subject queue 
/// @pre queue is a valid Queue data type reference.
/// @pre client previously removed and deleted all entries that
/// involved dynamic memory allocation.
/// @post the queue reference is no longer valid.
void destroyQueue( Queue queue );

#endif
