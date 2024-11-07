//Libraries needed
#include <stddef.h>

//Initalising functions
void *_malloc(int size);
void _free(void *pointer);
struct block_information_data *find_next_free_space(struct block_information_data **last_block, int size);
struct block_information_data *requesting_new_space(struct block_information_data *last_block, int size);
struct block_information_data *get_block_pointer(void *pointer);

//Structure used to store block information data
struct block_information_data {
    int free; //If the memory block is free (0) or not (1)
    int size; //Stores the size of the memory block
    struct block_information_data *next_block; //Structure to hold the information of the next memory block

};