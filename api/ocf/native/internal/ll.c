/**
 * Thread-safe linked-list data-structure for C.
 *
 * See `../README.md` and `main()` in this file for usage.
 *
 * @file ll.c contains the implementatons of the functions outlined in `ll.h` as
 * well as all the functions necessary to manipulate and handle nodes (which are
 * not exposed to the user).
 *
 * @author r-medina
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 r-medina
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "ll.h"

/* macros */

// for locking and unlocking rwlocks along with `locktype_t`
#define RWLOCK(lt, lk)                                                         \
  ((lt) == l_read) ? pthread_rwlock_rdlock(&(lk)) : pthread_rwlock_wrlock(&(lk))
#define RWUNLOCK(lk) pthread_rwlock_unlock(&(lk));

/* type definitions */

typedef enum locktype locktype_t;

// locktype enumerates the two typs of rw locks. This isused in the macros above
// for simplifying all the locking/unlocking that goes on.
enum locktype { l_read, l_write };

// ll_node models a linked-list node
struct ll_node {
  // pointer to the value at the node
  void *val;

  // pointer to the next node
  ll_node_t *nxt;

  // rw mutex
  pthread_rwlock_t m;
};

/**
 * @function ll_new
 *
 * Allocates a new linked list and initalizes its values.
 *
 * @param val_teardown - the `val_teardown` attribute of the linked list will be
 * set to this
 *
 * @returns a pointer to a new linked list
 */
ll_t *ll_new(gen_fun_t val_teardown) {
  ll_t *list = (ll_t *)malloc(sizeof(ll_t));
  list->hd = NULL;
  list->len = 0;
  list->val_teardown = val_teardown;
  pthread_rwlock_init(&list->m, NULL);

  return list;
}

/**
 * @function ll_delete
 *
 * Traversesthe whole linked list and deletes/deallocates the nodes then frees
 * the linked list itself.
 *
 * @param list - the linked list
 */
void ll_delete(ll_t *list) {
  ll_node_t *node = list->hd;
  ll_node_t *tmp;
  RWLOCK(l_write, list->m);
  while (node != NULL) {
    RWLOCK(l_write, node->m);
    if (list->val_teardown != NULL) {
      list->val_teardown(node->val);
    }
    RWUNLOCK(node->m);
    tmp = node;
    node = node->nxt;
    pthread_rwlock_destroy(&(tmp->m));
    free(tmp);
    (list->len)--;
  }
  list->hd = NULL;
  list->val_teardown = NULL;
  list->val_printer = NULL;
  RWUNLOCK(list->m);

  pthread_rwlock_destroy(&(list->m));

  free(list);
}

/**
 * @function ll_new_node
 *
 * Makes a new node with the given value.
 *
 * @param val - a pointer to the value
 *
 * @returns a pointer to the new node
 */
ll_node_t *ll_new_node(void *val) {
  ll_node_t *node = (ll_node_t *)malloc(sizeof(ll_node_t));
  node->val = val;
  node->nxt = NULL;
  pthread_rwlock_init(&node->m, NULL);

  return node;
}

/**
 * @function ll_select_n_min_1
 *
 * Actually selects the n - 1th element. Inserting and deleting at the front of
 * a list do NOT really depend on this.
 *
 * @param list - the linked list
 * @param node - a pointer to set when the node is found
 * @param n - the index
 *
 * @returns 0 if successful, -1 otherwise
 */
int ll_select_n_min_1(ll_t *list, ll_node_t **node, int n, locktype_t lt) {
  if (n < 0) // don't check against list->len because threads can add length
    return -1;

  if (n == 0)
    return 0;

  // n > 0

  *node = list->hd;
  if (*node == NULL) // if head is NULL, but we're trying to go past it,
    return -1;       // we have a problem

  RWLOCK(lt, (*node)->m);

  ll_node_t *last;
  for (; n > 1; n--) {
    last = *node;
    *node = last->nxt;
    if (*node ==
        NULL) { // happens when another thread deletes the end of a list
      RWUNLOCK(last->m);
      return -1;
    }

    RWLOCK(lt, (*node)->m);
    RWUNLOCK(last->m);
  }

  return 0;
}

/**
 * @function ll_insert_n
 *
 * Inserts a value at the nth position of a linked list.
 *
 * @param list - the linked list
 * @param val - a pointer to the value
 * @param n - the index
 *
 * @returns 0 if successful, -1 otherwise
 */
int ll_insert_n(ll_t *list, void *val, int n) {
  ll_node_t *new_node = ll_new_node(val);

  if (n == 0) { // nth_node is list->hd
    RWLOCK(l_write, list->m);
    new_node->nxt = list->hd;
    list->hd = new_node;
    RWUNLOCK(list->m);
  } else {
    ll_node_t *nth_node;
    if (ll_select_n_min_1(list, &nth_node, n, l_write)) {
      free(new_node);
      return -1;
    }
    new_node->nxt = nth_node->nxt;
    nth_node->nxt = new_node;
    RWUNLOCK(nth_node->m);
  }

  RWLOCK(l_write, list->m);
  (list->len)++;
  RWUNLOCK(list->m);

  return list->len;
}

/**
 * @function ll_insert_first
 *
 * Just a wrapper for `ll_insert_n` called with 0.
 *
 * @param list - the linked list
 * @param val - a pointer to the value
 *
 * @returns the new length of thew linked list on success, -1 otherwise
 */
int ll_insert_first(ll_t *list, void *val) { return ll_insert_n(list, val, 0); }

/**
 * @function ll_insert_last
 *
 * Just a wrapper for `ll_insert_n` called with the index being the length of
 * the linked list.
 *
 * @param list - the linked list
 * @param val - a pointer to the value
 *
 * @returns the new length of thew linked list on success, -1 otherwise
 */
int ll_insert_last(ll_t *list, void *val) {
  return ll_insert_n(list, val, list->len);
}

/**
 * @function ll_remove_n
 *
 * Removes the nth element of the linked list.
 *
 * @param list - the linked list
 * @param n - the index
 *
 * @returns the new length of thew linked list on success, -1 otherwise
 */
int ll_remove_n(ll_t *list, int n) {
  ll_node_t *tmp;
  if (n == 0) {
    RWLOCK(l_write, list->m);
    tmp = list->hd;
    list->hd = tmp->nxt;
  } else {
    ll_node_t *nth_node;
    if (ll_select_n_min_1(list, &nth_node, n,
                          l_write)) // if that node doesn't exist
      return -1;

    tmp = nth_node->nxt;
    nth_node->nxt = nth_node->nxt == NULL ? NULL : nth_node->nxt->nxt;
    RWUNLOCK(nth_node->m);
    RWLOCK(l_write, list->m);
  }

  (list->len)--;
  RWUNLOCK(list->m);

  if (list->val_teardown != NULL) {
    list->val_teardown(tmp->val);
  }
  free(tmp);

  return list->len;
}

/**
 * @function ll_remove_first
 *
 * Wrapper for `ll_remove_n`.
 *
 * @param list - the linked list
 *
 * @returns 0 if successful, -1 otherwise
 */
int ll_remove_first(ll_t *list) { return ll_remove_n(list, 0); }

/**
 * @function ll_remove_search
 *
 * Removes the first item in the list whose value returns 1 if `cond` is called
 * on it.
 *
 * @param list - the linked list
 * @param cond - a function that will be called on the values of each node. It
 * should return 1 of the element matches.
 *
 * @returns the new length of thew linked list on success, -1 otherwise
 */
int ll_remove_search(ll_t *list, int cond(void *)) {
  ll_node_t *last = NULL;
  ll_node_t *node = list->hd;
  while ((node != NULL) && !(cond(node->val))) {
    last = node;
    node = node->nxt;
  }

  if (node == NULL) {
    return -1;
  } else if (node == list->hd) {
    RWLOCK(l_write, list->m);
    list->hd = node->nxt;
    RWUNLOCK(list->m);
  } else {
    RWLOCK(l_write, last->m);
    last->nxt = node->nxt;
    RWUNLOCK(last->m);
  }

  if (list->val_teardown != NULL) {
    list->val_teardown(node->val);
  }
  free(node);

  RWLOCK(l_write, list->m);
  (list->len)--;
  RWUNLOCK(list->m);

  return list->len;
}

/**
 * @function ll_get_n
 *
 * Gets the value of the nth element of a linked list.
 *
 * @param list - the linked list
 * @param n - the index
 *
 * @returns the `val` attribute of the nth element of `list`.
 */
void *ll_get_n(ll_t *list, int n) {
  ll_node_t *node;
  if (ll_select_n_min_1(list, &node, n + 1, l_read))
    return NULL;

  RWUNLOCK(node->m);
  return node->val;
}

/**
 * @function ll_get_first
 *
 * Wrapper for `ll_get_n`.
 *
 * @param list - the linked list
 *
 * @returns the `val` attribute of the first element of `list`.
 */
void *ll_get_first(ll_t *list) { return ll_get_n(list, 0); }

/**
 * @function ll_map
 *
 * Calls a function on the value of every element of a linked list.
 *
 * @param list - the linked list
 * @param f - the function to call on the values.
 */
void ll_map(ll_t *list, gen_fun_t f) {
  ll_node_t *node = list->hd;

  while (node != NULL) {
    RWLOCK(l_read, node->m);
    f(node->val);
    ll_node_t *old_node = node;
    node = node->nxt;
    RWUNLOCK(old_node->m);
  }
}

/**
 * @function ll_print
 *
 * If `val_printer` has been set on the linked list, that function is called on
 * the values of all the elements of the linked list.
 *
 * @param list - the linked list
 */
void ll_print(ll_t list) {
  if (list.val_printer == NULL)
    return;

  printf("(ll:");
  ll_map(&list, list.val_printer);
  printf("), length: %d\n", list.len);
}

/**
 * @function ll_no_teardown
 *
 * A generic taredown function for values that don't need anything done.
 *
 * @param n - a pointer
 */
void ll_no_teardown(void *n) {
  n += 0; // compiler won't let me just return
}

#ifdef LL
/* this following code is just for testing this library */

void num_teardown(void *n) {
  *(int *)n *= -1; // just so we can visually inspect removals afterwards
}

void num_printer(void *n) { printf(" %d", *(int *)n); }

int num_equals_3(void *n) { return *(int *)n == 3; }

int main() {
  int *_n; // for storing returned ones
  int test_count = 1;
  int fail_count = 0;
  int a = 0;
  int b = 1;
  int c = 2;
  int d = 3;
  int e = 4;
  int f = 5;
  int g = 6;
  int h = 3;
  int i = 3;

  ll_t *list = ll_new(num_teardown);
  list->val_printer = num_printer;

  ll_insert_first(list, &c); // 2 in front

  _n = (int *)ll_get_first(list);
  if (!(*_n == c)) {
    fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, c,
            *_n);
    fail_count++;
  } else
    fprintf(stderr, "PASS Test %d!\n", test_count);
  test_count++;

  if (list->len != 1) {
    fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 1,
            list->len);
    fail_count++;
  } else
    fprintf(stderr, "PASS Test %d!\n", test_count);
  test_count++;

  ll_insert_first(list, &b); // 1 in front
  ll_insert_first(list, &a); // 0 in front -> 0, 1, 2

  _n = (int *)ll_get_first(list);
  if (!(*_n == a)) {
    fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, a,
            *_n);
    fail_count++;
  } else
    fprintf(stderr, "PASS Test %d!\n", test_count);
  test_count++;

  if (!(list->len == 3)) {
    fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 3,
            list->len);
    fail_count++;
  } else
    fprintf(stderr, "PASS Test %d!\n", test_count);
  test_count++;

  ll_insert_last(list, &d); // 3 in back
  ll_insert_last(list, &e); // 4 in back
  ll_insert_last(list, &f); // 5 in back

  _n = (int *)ll_get_n(list, 5);
  if (!(*_n == f)) {
    fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, f,
            *_n);
    fail_count++;
  } else
    fprintf(stderr, "PASS Test %d!\n", test_count);
  test_count++;

  if (!(list->len == 6)) {
    fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", test_count, 6,
            list->len);
    fail_count++;
  } else
    fprintf(stderr, "PASS Test %d!\n", test_count);
  test_count++;

  ll_insert_n(list, &g, 6); // 6 at index 6 -> 0, 1, 2, 3, 4, 5, 6

  int _i;
  for (_i = 0; _i < list->len; _i++) { // O(n^2) test lol
    _n = (int *)ll_get_n(list, _i);
    if (!(*_n == _i)) {
      fail_count++;
      fprintf(stderr, "FAIL Test %d: Expected %d, but got %d.\n", 1, _i, *_n);
    } else
      fprintf(stderr, "PASS Test %d!\n", test_count);
    test_count++;
  }

  // (ll: 0 1 2 3 4 5 6), length: 7

  ll_remove_first(list); // (ll: 1 2 3 4 5 6), length: 6
  ll_remove_n(list, 1);  // (ll: 1 3 4 5 6),   length: 5
  ll_remove_n(list, 2);  // (ll: 1 3 5 6),     length: 4
  ll_remove_n(list, 5);  // (ll: 1 3 5 6),     length: 4; does nothing
  ll_remove_search(list, num_equals_3); // (ll: 1 5 6),       length: 3
  ll_insert_first(list, &h);            // (ll: 3 1 5 6),     length: 5
  ll_insert_last(list, &i);             // (ll: 3 1 5 6 3),   length: 5
  ll_remove_search(list, num_equals_3); // (ll: 1 5 6 3),     length: 4
  ll_remove_search(list, num_equals_3); // (ll: 1 5 6),       length: 3

  ll_print(*list);

  ll_delete(list);

  if (fail_count) {
    fprintf(stderr, "FAILED %d tests of %d.\n", fail_count, test_count);
    return fail_count;
  }

  fprintf(stderr, "PASSED all %d tests!\n", test_count);
}
#endif