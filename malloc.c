#include "include/malloc.h"

t_meta_data		*free_lists[28];
t_meta_data		*start;
t_meta_data		*break_pos;
static pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

/*
**
*/
t_meta_data		*init_chunk(int bin)
{
  t_meta_data		*new_ptr;
  size_t		size;

  size = 1 << bin;
  if (size < UNIT_SIZE && !start)
    {
      if ((new_ptr = sbrk((UNIT_SIZE))) == (void*) - 1)
	return (NULL);
      bin = MIN_SIZE;
    }
  else if ((new_ptr = sbrk(((size)))) == (void*) - 1)
    return (NULL);
  if (!start)
    start = new_ptr;
  if ((break_pos = sbrk(0)) == ((void*) - 1))
    return (NULL);
  new_ptr->pool = (char*)new_ptr;
  new_ptr->bin = bin;
  new_ptr->peak = bin;
  new_ptr->used = 0;
  new_ptr->next = NULL;
  return (new_ptr);
}

/*
**
*/
void			*malloc(size_t size)
{
  int			bin;
  t_meta_data		*chunk;
  void			*ptr;

  bin = 5;
  while (((size_t)(1 << bin)) < (size + META_SIZE) && bin < MAX_SIZE)
    bin++;
  if (pthread_mutex_lock(&mutex) != 0)
    write(2, "Can't lock mutex\n", 17);
  if ((chunk = get_chunk(bin)) == NULL)
    {
      if (pthread_mutex_unlock(&mutex) != 0)
	write(2, "Can't unlock mutex\n", 19);
      return (NULL);
    }
  ptr = (void*)(((char*)chunk) + (META_SIZE));
  if ((pthread_mutex_unlock(&mutex) != 0))
      write(2, "Can't unlock mutex\n", 19);
  return (ptr);
}

/*
** look if ptr is a valid address
** which means : if ptr is a malloced address
** get the corresponding t_meta_data block. (ptr = (t_meta_data)ptr);
** if there is a buddy matching, fusion
**    if x % x + 2^k == 0 then
**      buddy at the left
**    else if == 2k then
**      buddy at the right
**    if fusion
**       add chunk to list
**       del chunk from prev list
*/
void			free(void *ptr)
{
  t_meta_data		*chunk;
  t_meta_data		*buddy;
  char			*offset;

  if (!ptr)
    return ;
  if (pthread_mutex_lock(&mutex) != 0)
    write(2, "Can't lock mutex\n", 17);
  chunk = (t_meta_data*)(((char*)ptr) - (META_SIZE));
  if (!chunk || (chunk && (chunk < start || chunk > break_pos)))
    {
      if (pthread_mutex_unlock(&mutex) != 0)
	write(2, "Can't unlock mutex\n", 17);
      return ;
    }
  chunk->used = 0;
  offset = (char*)(((char*)chunk - (char*)chunk->pool) ^ (1 << chunk->bin));
  buddy = (t_meta_data*)((size_t)chunk->pool + offset);
  replace_chunk(chunk, buddy, offset);
  if (pthread_mutex_unlock(&mutex) != 0)
    write(2, "Can't unlock mutex\n", 19);
}

void			*realloc(void *ptr, size_t size)
{
  void			*new_ptr;

  if (!ptr)
    return (malloc(size));
  if ((new_ptr = malloc(size)) == NULL)
    return (NULL);
  memcpy(new_ptr, ptr, size);
  free(ptr);
  return (new_ptr);
}

void			*calloc(size_t nmemb, size_t size)
{
  size_t		size_total;
  void			*data;

  size_total = nmemb * size;
  if ((data = malloc(size_total)) == NULL)
    return (NULL);
  memset(data, 0, size_total);
  return (data);
}
