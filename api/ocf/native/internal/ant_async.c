/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ant_async.h"

// #define DEBUG_PRINT_ANT_ASYNC_NAME

// ant_async list
ll_t *g_ant_async_list = NULL;
void ant_async_list_item_destroyer(void *item) {
  destroy_ant_async((ant_async_t *)item);
}
void init_ant_async_list() {
  g_ant_async_list = ll_new(ant_async_list_item_destroyer);
}
void destroy_ant_async_list() {
  if (g_ant_async_list != NULL)
    ll_delete(g_ant_async_list);
}
void insert_ant_async_to_list(ant_async_t *async) {
  ll_insert_last(g_ant_async_list, (void *)async);
}

// ant_async
void ant_async_event_queue_item_destroyer(void *item, void *data_destroyer) {
  destroy_ant_async_event((ant_async_event_t *)item, (gen_fun_t)data_destroyer);
}
ant_async_t *create_ant_async(uv_async_cb uv_handler_fn,
                              gen_fun_t event_queue_data_destroyer,
                              const char *name) {
  // ant_async
  ant_async_t *ant_async = (ant_async_t *)malloc(sizeof(ant_async_t));

  // ant_async->uv_async
  iotjs_environment_t *env = iotjs_environment_get();
  uv_loop_t *loop = iotjs_environment_loop(env);
  uv_async_init(loop, &ant_async->uv_async, uv_handler_fn);

  // ant_async->handler_map
  ant_async->handler_map = hashmap_new();

  // ant_async->event_queue
  ant_async->event_queue = ll_new2(ant_async_event_queue_item_destroyer,
                                   (void *)event_queue_data_destroyer);

  // ant_async->name
  ant_async->name = (char *)malloc(strlen(name) + 1);
  strncpy(ant_async->name, name, strlen(name) + 1);

  return ant_async;
}
static int __destroy_handler_map_node(any_t not_used, any_t js_handler) {
  jerry_release_value((jerry_value_t)js_handler);
  return MAP_OK;
}
void destroy_ant_async(ant_async_t *ant_async) {
  // ant_async->uv_async
  uv_close((uv_handle_t *)&(ant_async->uv_async), NULL);

  // ant_async->handler_map
  hashmap_iterate(ant_async->handler_map, __destroy_handler_map_node, NULL);
  hashmap_free(ant_async->handler_map);

  // ant_async->event_queue
  ll_delete(ant_async->event_queue);

  // ant_async->name
#ifdef DEBUG_PRINT_ANT_ASYNC_NAME
  printf("Destroy ant async: %s\n", ant_async->name);
  free(ant_async->name);
#endif

  // ant_async
  free(ant_async);
}
bool add_js_handler_to_ant_async(ant_async_t *ant_async, int key,
                                 jerry_value_t js_handler) {
  // Remove if js_handler has already existed in the hashmap entry
  jerry_value_t old_handler = get_js_handler_from_ant_async(ant_async, key);
  if (!jerry_value_is_undefined(old_handler)) {
    remove_js_handler_from_ant_async(ant_async, key);
  }

  jerry_acquire_value(js_handler);
  return (hashmap_put(ant_async->handler_map, (unsigned int)key,
                      (any_t)js_handler) == MAP_OK);
}
bool remove_js_handler_from_ant_async(ant_async_t *ant_async, int key) {
  jerry_value_t js_handler;
  int res = hashmap_get(ant_async->handler_map, (unsigned int)key,
                        (any_t *)&js_handler);
  if (res != MAP_OK)
    return false;
  jerry_release_value(js_handler);
  return (hashmap_remove(ant_async->handler_map, (unsigned int)key) == MAP_OK);
}
jerry_value_t get_js_handler_from_ant_async(ant_async_t *ant_async, int key) {
  jerry_value_t js_handler;
  int res = hashmap_get(ant_async->handler_map, (unsigned int)key,
                        (any_t *)&js_handler);
  if (res != MAP_OK)
    return jerry_create_undefined();
  return js_handler;
}
bool emit_ant_async_event(ant_async_t *ant_async, int key, void *event_data,
                          bool sync_mode) {
  // create ant_async_event
  ant_async_event_t *ant_async_event = create_ant_async_event(key, event_data);

  // enqueue event to the handler
  enqueue_event_to_ant_async(ant_async, ant_async_event);

  // emit uv_async event
  if (sync_mode) {
    // sync mode: emit event repeatedly until JS thread processes the event.
    struct timespec waiting_timeout;
    waiting_timeout.tv_sec = 1;
    waiting_timeout.tv_nsec = 0;

    pthread_mutex_init(&ant_async_event->sync_mutex, NULL);
    pthread_cond_init(&ant_async_event->sync_cond, NULL);

    pthread_mutex_lock(&ant_async_event->sync_mutex);
    do {
      // emit uv_async event
      uv_async_send(&ant_async->uv_async);
      pthread_cond_timedwait(&ant_async_event->sync_cond,
                             &ant_async_event->sync_mutex, &waiting_timeout);
    } while (get_first_event_from_ant_async(ant_async) != NULL);
    pthread_mutex_unlock(&ant_async_event->sync_mutex);
  } else {
    // async mode: do not wait JS thread's processing
    uv_async_send(&ant_async->uv_async);
  }

  return true;
}

void wakeup_ant_async_sender(ant_async_event_t *ant_async_event) {
  // wake up ant_async sender thread
  pthread_mutex_lock(&ant_async_event->sync_mutex);
  pthread_cond_signal(&ant_async_event->sync_cond);
  pthread_mutex_unlock(&ant_async_event->sync_mutex);
}

// event_queue in ant_handler
void enqueue_event_to_ant_async(ant_async_t *ant_async,
                                ant_async_event_t *ant_async_event) {
  if (ant_async_event != NULL)
    ll_insert_last(ant_async->event_queue, (void *)ant_async_event);
}
ant_async_event_t *get_first_event_from_ant_async(ant_async_t *ant_async) {
  return (ant_async_event_t *)ll_get_first(ant_async->event_queue);
}
void remove_first_event_from_ant_async(ant_async_t *ant_async) {
  ll_remove_first(ant_async->event_queue);
}

// ant_event
ant_async_event_t *create_ant_async_event(int key, void *data) {
  ant_async_event_t *ant_async_event =
      (ant_async_event_t *)malloc(sizeof(ant_async_event_t));
  ant_async_event->key = key;
  ant_async_event->data = data;
  return ant_async_event;
}
void destroy_ant_async_event(ant_async_event_t *ant_async_event,
                             gen_fun_t event_queue_item_destroyer) {
  if (event_queue_item_destroyer != NULL) {
    event_queue_item_destroyer(ant_async_event->data);
  }
  if (ant_async_event != NULL) {
    free(ant_async_event);
  }
}
