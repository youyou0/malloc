#include "include/malloc.h"

void		show_alloc_mem()
{
  void		*ptr;
  t_meta_data	*tmp;

  ptr = sbrk(0);
  tmp = start;
  printf("break : %p pool : %p\n", ptr, tmp->pool);
  while (tmp && tmp != ptr)
    {
      if (tmp->used == 1)
	printf("%p - %p : %u bytes\n",
	       tmp, ((char*)tmp + (1 << tmp->bin)), (1 << tmp->bin));
      tmp = (t_meta_data*)(((char*)tmp + (1 << tmp->bin)));
    }
}

void		show_free_mem()
{
  void		*ptr;
  t_meta_data	*tmp;

  ptr = sbrk(0);
  tmp = start;
  printf("break : %p pool : %p\n", ptr, tmp->pool);
  while (tmp && tmp != ptr)
    {
      if (tmp->used == 0)
	printf("%p - %p : %u bytes\n",
	       tmp, ((char*)tmp + (1 << tmp->bin)), (1 << tmp->bin));
      tmp = (t_meta_data*)(((char*)tmp + (1 << tmp->bin)));
    }
  printf("\n");
}
