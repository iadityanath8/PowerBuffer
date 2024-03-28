#ifndef POWER_H
#define POWER_H
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
/* -----------------------------Utility--------------------------- */
#define auto __auto_type
#define PowerInline inline __attribute__((always_inline))

/* -----------------------------Debugging----------------------------- */
#define unimplemented assert(false && "Not implemented yet") 
#define use(x) (void)x


/* ----------------------- TESTING ---------------------------*/
#define TEST_ON 1
#define TEST_OF 0

#define do_assert(bool_val, do_val)\
  if(bool_val){do_val;}


/* ----------------------------Aliasing------------------------ */
typedef uint64_t CursorPosition;

/*---------------------------Constants-------------------------*/

#define DEFAULT_ALLOCA_SIZE 2

/*---------------------------Macros for functions---------------*/

#define left_ele(buff) buff->gap_start
#define gap(buff) buff->gap_end - buff->gap_start + 1
#define right_ele(buff) buff->total_size - gap(buff) - right_ele(buff) 
#define last(buff) buff->total_size - buff->gap_start - gap(buff) + buff->gap_end

/**
 *    Total_size = 10 // this is gap_size
 *    Gs = 0, GE = 9;
 *    rule fill from first or append
 *    m_ _ _ _ _ _ _pq
 *    
 *    total_size = 10 
 *    left_element = gap_start
 *    right_element = 10 - gap_size - left_element
 *    
 *    meow_p  ==> meow_ _ p
 *    -----------------------------------ALLOCATE--------------------------------------
 *    left_buffer = calloc(left_element + right_element + total_size*2,sizeof(char))
 *    NOW introduce a variable gap_size in her
 **/

typedef struct{
  char* buffer;
  size_t total_size;
  size_t gap_size;
  CursorPosition gap_start;
  CursorPosition gap_end;
}PowerBuffer;

/* using Copy value semantics */
PowerBuffer power_init(size_t init_size);

CursorPosition p_idx(PowerBuffer p_buffer[static 1],CursorPosition vidx);

/**
 *        --------------------WORK IN PROGRESS NEEDED A FAST REALLOCATOR----------------------
 *
 *                    NEEDED A DIFFERENT ALLOCATOR FOR STRING VALUE
 */

void p_reallocate(PowerBuffer p_buffer[static 1],int reallocator_size);

size_t p_gap_len(PowerBuffer p_buffer[static 1]);

size_t e_len(PowerBuffer p_buffer[static 1]);

size_t p_gap(PowerBuffer p_buffer[static 1]);

size_t p_left_ele(PowerBuffer p_buffer[static 1]);

size_t p_right_ele(PowerBuffer p_buffer[static 1]);

size_t p_last_index(PowerBuffer p_buffer[static 1]);

/**Most oftenly we will be making these functions into inline functions*/
void p_left(PowerBuffer p_buffer[static 1],CursorPosition pos);

/*Most oftenly we will be making these functions into inline functions*/
void p_right(PowerBuffer p_buffer[static 1],CursorPosition pos);

void p_move_cursor(PowerBuffer p_buffer[static 1],CursorPosition pos);

void p_insert_char_at(PowerBuffer p_buffer[static 1],CursorPosition pos, char ch);


void p_append_char(PowerBuffer p_buffer[static 1],char ch);

// soon we will use better and fast performant strings 
void p_insert_string_at(PowerBuffer p_buffer[static 1],CursorPosition pos,char* str);

void p_append_string(PowerBuffer p_buffer[static 1],char *str);
/**
 *  
 *  DELETING CHARACTER AT GIVEN POSITION
 * 
 */
void p_delete_char_at(PowerBuffer p_buffer[static 1],CursorPosition pos);
#endif // POWER_H
