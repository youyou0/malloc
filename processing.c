#include "include/malloc.h"

/*
** split the block and return new one
*/
t_meta_data		*split_chunk(t_meta_data *chunk)
{
  int			index;
  t_meta_data		*buddy;

  chunk->bin--;
  index = chunk->bin - 5;
  buddy = (t_meta_data*)(((char*)chunk) + (1 << chunk->bin));
  buddy->next = free_lists[index];
  buddy->pool = chunk->pool;
  buddy->peak = chunk->peak;
  free_lists[index] = buddy;
  free_lists[index]->bin = chunk->bin;
  free_lists[index]->next = NULL;
  free_lists[index]->used = 0;
  return (chunk);
}

/*
** Get block if exist and return it
** else create one and split it
*/
t_meta_data		*get_chunk(int bin)
{
  int			index;
  t_meta_data		*chunk;

  index = bin - 5;
  while (!free_lists[index] && index < MAX_SIZE - 4)
    index++;
  chunk = free_lists[index];
  if (!chunk)
    {
      if ((chunk = init_chunk(bin)) == NULL)
        return (NULL);
    }
  else
    free_lists[index] = free_lists[index]->next;
  chunk->used = 1;
  while (chunk->bin > bin)
    chunk = split_chunk(chunk);
  return (chunk);
}

void			remove_elem(t_meta_data *chunk,
				    int index)
{
  t_meta_data		*node;

  node = free_lists[index];
  if (node == chunk)
    free_lists[index] = free_lists[index]->next;
  else
    {
      while (node && node->next != chunk)
	node = node->next;
      if (node)
	node->next = node->next->next;
    }
}

t_meta_data		*fusion(t_meta_data *chunk,
				t_meta_data *buddy)
{
  int			index;

  index = chunk->bin - 5;
  remove_elem(chunk, index);
  remove_elem(buddy, index);
  chunk->bin++;
  index++;
  chunk->next = free_lists[index];
  free_lists[index] = chunk;
  return (free_lists[index]);
}

void			replace_chunk(t_meta_data *chunk,
				      t_meta_data *buddy, char *offset)
{
  int		index;

  index = chunk->bin - 5;
  if (chunk && chunk->bin < chunk->peak && buddy->used == 0)
    {
      while (chunk->bin < chunk->peak && buddy->used == 0
	     && buddy->bin == chunk->bin)
	{
	  if (chunk < buddy)
	    chunk = fusion(chunk, buddy);
	  else if (chunk > buddy)
	    chunk = fusion(buddy, chunk);
	  offset = (char*)(((char*)chunk -
			    (char*)chunk->pool) ^ (1 << chunk->bin));
	  buddy = (t_meta_data*)((size_t)chunk->pool + offset);
	}
    }
  else
    {
      chunk->next = free_lists[index];
      free_lists[index] = chunk;
      chunk->used = 0;
    }
}
