//Malloc is dynamic memory allocation that is used to dynamically allocate space
//Free frees up that space in memory

//Includes the libaries/header files needed
#include "memory_management.h"
#include <string.h>
#include <unistd.h>

//Defines needed
#define BLOCK_SIZE sizeof(struct block_information_data) //Size of the data block

struct block_information_data *find_next_free_space(struct block_information_data **last_block, int size) { //Function to find the next free block. Passes in the last memory block and the size of the new memory block
    void *head = NULL; //Pointer for the head of the linked list
    struct block_information_data *current_block = head; //Creates a structure for the current data block
    while (current_block && !(current_block->free && current_block->size >= size)) { //While there isnt a free space
        *last_block = current_block; //Sets the last block of memory to the current block (we're done with this block)
        current_block = current_block->next_block; //Sets the current block to the next one in memory
    }
    return current_block; //Returns the current block of memory
}

struct block_information_data *requesting_new_space(struct block_information_data *last_block, int size) {//Function to request space from the Operating System
    struct block_information_data *current_block; //Creates a struct for the current block of memory
    current_block = sbrk(0); //Gets the current end of the heap
    void *request_pointer = sbrk(size + BLOCK_SIZE); //Pointer for the new location of memory
    if (request_pointer == (void*) -1) { //If the cast failed
        return NULL; //Cast failed so return nothing
    }

    if (last_block) { //If its NULL on the first request
        last_block = current_block; //Sets the last block as the current block - done with this block
    }

    //Setting the current block
    current_block->size = size;
    current_block->next_block = NULL;
    current_block->free = 0;

    return current_block; //Returns it
}

struct block_information_data *get_block_pointer(void *pointer) { //Gets the block pointer
    return (struct block_information_data*)pointer -1; //Returns the pointer 
}

void *_malloc(int size) { //Function used to call malloc, passes in a size for the amount of space needed to be allocated
    struct block_information_data *current_block; //Creates a structure for the current block
    void *head = NULL; //Pointer to the head

    if (size <= 0) { //If the size passed into malloc is 0 (no memory is needed to be allocated)
        return NULL; //Returns nothing as no memory has been allocated
    }

    if (!head) { //Inital call of _malloc()
        current_block = requesting_new_space(NULL, size); //Requests space
        if (!current_block) { //If theres an error
            return NULL; //Returns nothing
        }
        head = current_block; //Sets the head pointer to point to the structure
    }
    else {
        struct block_information_data *last_block = head; //Creates a structure for the last block of memory
        current_block = requesting_new_space(last_block, size); //Requests space from the last pointer for how big it is
    }

    return (current_block+1); //Returns the block of memory +1 address
}

//Now malloc is made, need a free function to free the space that malloc used

void _free(void *pointer) { //Function to call free, passes in a pointer to the space in memory
    if (!pointer) { //If pointer cast failed
        return; //Returns nothing since there is nothing to free
    }
    struct block_information_data *block_to_free= get_block_pointer(pointer); //Gets the memroy block information of the block that the pointer passed in points to
    block_to_free->free = 1; //Sets this memory block as free so it can be used again by something else
}
