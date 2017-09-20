
#ifndef __COMM_QUEUE_H__
#define __COMM_QUEUE_H__
#include <stddef.h>
typedef struct _comm_queue_t {
	struct _comm_queue_t *prev;
	struct _comm_queue_t *next;
} comm_queue_t;

#define comm_queue_init(q) do{ \
	(q)->prev = (q); \
	(q)->next = (q);} while(0)

#define comm_queue_init_node(x) do{	\
	(x)->prev = NULL;	\
	(x)->next = NULL;} while(0)

#define comm_queue_empty(h) (h == (h)->prev)

#define comm_queue_insert_head(h, x) do{ \
	(x)->next = (h)->next; \
	(x)->next->prev = (x); \
	(x)->prev = (h); \
	(h)->next = (x);} while(0)

#define comm_queue_insert_after(p, x) do{ \
	(x)->next = (p)->next; \
	(x)->prev = (p); \
	(p)->next->prev = (x); \
	(p)->next = (x);} while(0)

#define comm_queue_insert_tail(h, x) do{ \
	(x)->prev = (h)->prev; \
	(x)->next = (h); \
	(x)->prev->next = (x); \
	(h)->prev = (x);} while(0)

#define comm_queue_head(h) ((h)->next)

#define comm_queue_tail(h) ((h)->prev)

#define comm_queue_next(q) ((q)->next)

#define comm_queue_prev(q) ((q)->prev)

#define comm_queue_remove(x) do{\
	(x)->next->prev = (x)->prev; \
	(x)->prev->next = (x)->next; \
	(x)->next = NULL; \
	(x)->prev = NULL; } while(0)

#define comm_queue_data(q, type, link) \
	(type *) ((unsigned char *) q - offsetof(type, link))

#define comm_queue_foreach(q, h) \
	for((q) = comm_queue_head(h); (q) != (h); (q) = comm_queue_next(q))

inline int comm_queue_length(comm_queue_t *head)
{
	int i = 0;

	comm_queue_t *walker;

	comm_queue_foreach(walker, head)
		i++;

	return i;
}
#endif
