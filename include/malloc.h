/*
** malloc.h for  in /home/hamdi_y/rendu/PSU_2015_malloc
** 
** Made by youness hamdi
** Login   <hamdi_y@epitech.net>
** 
** Started on  Sat Feb 13 20:26:14 2016 youness hamdi
** Last update Sat Feb 13 22:00:47 2016 youness hamdi
*/

#ifndef _MALLOC_H_
# define _MALLOC_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct		s_meta_data
{
  short			bin;
  int			used;
  char			*pool;
  short			peak;
  struct s_meta_data	*next;
}			t_meta_data;

# define META_SIZE (sizeof(t_meta_data))
# define MIN_SIZE (27)
# define UNIT_SIZE (1 << MIN_SIZE)
# define MAX_SIZE (31)

extern t_meta_data	*free_lists[28];
extern t_meta_data	*start;
extern t_meta_data	*break_pos;

void			show_alloc_mem();
void			show_free_mem();
t_meta_data		*init_chunk(int bin);
t_meta_data		*split_chunk(t_meta_data *chunk);
t_meta_data		*get_chunk(int bin);
void			remove_elem(t_meta_data *chunk, int index);
t_meta_data		*fusion(t_meta_data *chunk, t_meta_data *buddy);
void			replace_chunk(t_meta_data *chunk,
				      t_meta_data *buddy, char *offset);
void			*malloc(size_t size);
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t nmemb, size_t size);

#endif /* !_MALLOC_H_ */
