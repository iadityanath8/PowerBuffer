#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../include/powerbuffer.h"

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

//typedef struct{
//  char* buffer;
//  size_t total_size;
//  size_t gap_size;
//  CursorPosition gap_start;
//  CursorPosition gap_end;
//}PowerBuffer;

/* using Copy value semantics */
PowerInline PowerBuffer power_init(size_t init_size){
  
  return (PowerBuffer){
    .buffer = calloc(init_size,sizeof(char)),
    .gap_size = init_size,
    .gap_start = 0,
    .gap_end = init_size - 1,
    .total_size = init_size,
  };
}

PowerInline CursorPosition p_idx(PowerBuffer p_buffer[static 1],CursorPosition vidx){
  assert(vidx < p_buffer->total_size);

  if(vidx > p_buffer->gap_start){
    return vidx;
  }else{
    auto actual_idx = vidx + (p_buffer->gap_end - p_buffer->gap_start + 1);
    assert(actual_idx < p_buffer->total_size);
    return actual_idx;
  }
}

/**
 *        --------------------WORK IN PROGRESS NEEDED A FAST REALLOCATOR----------------------
 *
 *                    NEEDED A DIFFERENT ALLOCATOR FOR STRING VALUE
 */

void p_reallocate(PowerBuffer p_buffer[static 1],int reallocator_size){
  auto alloca_size = DEFAULT_ALLOCA_SIZE;

  if(reallocator_size != -1){
    alloca_size = reallocator_size;
  }

  auto t = p_buffer->total_size;
  auto g_size = p_buffer->gap_end - p_buffer->gap_start + 1; //  current buffer size
  
  auto left_element = p_buffer->gap_start;   // not indices they are element 
  auto right_element = t - left_element - g_size;      // not indices they are no of elements

  CursorPosition new_total = right_element + left_element + p_buffer->gap_size*alloca_size;
  char* new_buffer = calloc(new_total,sizeof(char));
  
  CursorPosition i = 0;
  
  while(i < left_element){
    new_buffer[i] = p_buffer->buffer[i];
    i++;
  }

  i = left_element + g_size;
  CursorPosition j = left_element + p_buffer->gap_size*alloca_size;
  
  while(i < p_buffer->total_size && j < new_total){
    printf("%ld\n",i);
    new_buffer[j] = p_buffer->buffer[i]; 
    i++;
    j++;
  }

  
 
  free(p_buffer->buffer);
  p_buffer->gap_end = left_element + p_buffer->gap_size*alloca_size - 1;
  p_buffer->gap_size = p_buffer->gap_size*alloca_size;
  p_buffer->total_size = new_total;
  p_buffer->buffer = new_buffer; 
}


/**Most oftenly we will be making these functions into inline functions*/
PowerInline void p_left(PowerBuffer p_buffer[static 1],CursorPosition pos){
  while(p_buffer->gap_start > pos){
    p_buffer->gap_start--;
    p_buffer->gap_end--;
    p_buffer->buffer[p_buffer->gap_end + 1] = p_buffer->buffer[p_buffer->gap_start];
    p_buffer->buffer[p_buffer->gap_start] = ' ';
  }
}

/*Most oftenly we will be making these functions into inline functions*/
PowerInline void p_right(PowerBuffer p_buffer[static 1],CursorPosition pos){
  while(p_buffer->gap_end < pos - 1){
    p_buffer->gap_start++;
    p_buffer->gap_end++;
    p_buffer->buffer[p_buffer->gap_start - 1] = p_buffer->buffer[p_buffer->gap_end];
  }
}

void p_move_cursor(PowerBuffer p_buffer[static 1],CursorPosition pos){
  
  do_assert(p_buffer->gap_start >= p_buffer->gap_end,p_reallocate(p_buffer,-1));
  
  if(pos > p_buffer->gap_start){
    p_right(p_buffer,pos);
  }else{
    p_left(p_buffer,pos);
  }
}


void p_insert_char_at(PowerBuffer p_buffer[static 1],CursorPosition pos, char ch){

  do_assert(p_buffer->gap_start >= p_buffer->gap_end,p_reallocate(p_buffer,-1));

  if(pos > p_buffer->gap_start){
    p_right(p_buffer,pos);
  }else if(pos < p_buffer->gap_start){
    p_left(p_buffer,pos);
  }

  /** Append the charaters to the given postion it is naked position **/
  
  p_buffer->buffer[p_buffer->gap_start] = ch;
  p_buffer->gap_start++;
}

void p_append_char(PowerBuffer p_buffer[static 1],char ch){
  
  do_assert(p_buffer->gap_start >= p_buffer->gap_end,p_reallocate(p_buffer,-1));
  
  p_buffer->buffer[p_buffer->gap_start] = ch;
  p_buffer->gap_start++;
}

// soon we will use better and fast performant strings 
void p_insert_string_at(PowerBuffer p_buffer[static 1],CursorPosition pos,char* str){
  auto s_len = strlen(str);
  auto remaining_gap_size = p_buffer->gap_end - p_buffer->gap_start;
 
  do_assert(s_len >= remaining_gap_size, p_reallocate(p_buffer,s_len*2));
  
  p_move_cursor(p_buffer,pos);
  
  uint64_t i = p_buffer->gap_start;
  uint64_t j = 0;

  while(i < i + s_len && j < s_len){
    p_buffer->buffer[i] = str[j];
    p_buffer->gap_start++;i++;j++;
  }
}

void p_append_string(PowerBuffer p_buffer[static 1],char *str){
  auto s_len = strlen(str);
  auto remaining_gap_size = p_buffer->gap_end - p_buffer->gap_start;

  do_assert(s_len >= remaining_gap_size,p_reallocate(p_buffer,s_len*2));
  
  uint64_t i = p_buffer->gap_start;
  uint64_t j = 0;

  while(i < i + s_len && j < s_len){
    p_buffer->buffer[i] = str[j];
    p_buffer->gap_start++;i++;j++;
  }
}

/**
 *  
 *  DELETING CHARACTER AT GIVEN POSITION
 * 
 */
void p_delete_char_at(PowerBuffer p_buffer[static 1],CursorPosition pos){
  
  if(p_buffer->gap_start - 1 == pos){
    p_buffer->gap_start--;
  }else{
    p_move_cursor(p_buffer,pos + 1);
    p_buffer->gap_start--;
  }

  unimplemented;
}

/*
 *  problem with moving right in the direction in here
 * we will make it powerfull and optimized at the end
 * this function is producing error
 * */

#if TEST_OF
int main(){
  
  return 0;
}
#endif

#if TEST_OF
int main(){
  auto p_buffer = power_init(10);
  p_append_char(&p_buffer,'a');
  p_append_char(&p_buffer,'o');
  p_append_char(&p_buffer,'l');
  
  p_delete_char_at(&p_buffer,0);
  p_delete_char_at(&p_buffer,8);
  
  for(size_t i = 0;i < p_buffer.total_size;i++){
    printf("%c->\n",p_buffer.buffer[i]);
  }
  printf("%ld %ld",p_buffer.gap_start,p_buffer.gap_end);
  return 0;
}
#endif

#if TEST_OF
int main(){
  auto p_buffer = power_init(10);

  p_append_char(&p_buffer,'p');
  p_append_char(&p_buffer,'K');
  p_append_char(&p_buffer,'a');
  p_insert_char_at(&p_buffer,0,'o');
  

  for(size_t i = 0;i < p_buffer.total_size;i++){
    printf("%c->\n",p_buffer.buffer[i]);
  }
  
  printf("===================================\n"); 
  p_insert_char_at(&p_buffer,8,'9');
  p_append_string(&p_buffer,"meow");

  for(size_t i = 0;i < p_buffer.total_size;i++){
    printf("%c->\n",p_buffer.buffer[i]);
  }

  printf("%ld %ld",p_buffer.gap_start,p_buffer.gap_end);
  return 0;
}
#endif

#if TEST_OF
int main(){
  auto p_buffer = power_init(1);
  
  p_insert_string_at(&p_buffer,0,"MEOW");
  p_append_string(&p_buffer,"MO");
  p_append_string(&p_buffer,"OP");
  p_insert_char_at(&p_buffer,0,'p');
  for(size_t i =0;i < p_buffer.total_size;i++){
    printf("%c->\n",p_buffer.buffer[i]);
  }
   
  printf("\n");

  printf("%ld\n",p_buffer.total_size);
  printf("%ld %ld\n",p_buffer.gap_start,p_buffer.gap_end);
  return 0;
}
#endif

#if TEST_OF
int main(){
  auto buffer = power_init(1);
  
  //p_reallocate(&buffer);
   p_append_char(&buffer,'a');
   p_insert_char_at(&buffer,0,'o');

  for(size_t i = 0;i < buffer.total_size;i++){
    printf("%c->\n",buffer.buffer[i]);
  }
  
  printf("%ld %ld",buffer.gap_start,buffer.gap_end);
  return 0;
}
#endif

#if TEST_OF

int main(){
  auto p_buffer = power_init(1);  
  
  p_append_char(&p_buffer,'a');
  //p_append_char(&p_buffer,'p');
  //p_insert_char_at(&p_buffer,1,'o');

//  for(size_t i = 0;i < p_buffer.total_size;i++){
//    printf("%c->\n",p_buffer.buffer[i]);
//  }

  printf("%ld %ld\n",p_buffer.gap_start,p_buffer.gap_end);
  return 0;
}
#endif
