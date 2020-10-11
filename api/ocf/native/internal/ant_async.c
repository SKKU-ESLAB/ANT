#include "ant_async.h"

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
                              gen_fun_t event_queue_data_destroyer) {
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

  return ant_async;
}
static void __destroy_handler_map_node(any_t not_used, any_t js_handler) {
  jerry_release_value(js_handler);
}
void destroy_ant_async(ant_async_t *ant_async) {
  // ant_async->uv_async
  uv_close((uv_handle_t *)&(ant_async->uv_async), NULL);

  // ant_async->handler_map
  hashmap_iterate(ant_async->handler_map, __destroy_handler_map_node, NULL);
  hashmap_free(ant_async->handler_map);

  // ant_async->event_queue
  ll_delete2(ant_async->event_queue);

  // ant_async
  free(ant_async);
}
bool add_js_handler_to_ant_async(ant_async_t *ant_async, int key,
                                 jerry_value_t js_handler) {
  jerry_acquire_value(js_handler);
  return (hashmap_put(ant_async->handler_map, key, (any_t)js_handler) ==
          MAP_OK);
}
bool remove_js_handler_from_ant_async(ant_async_t *ant_async, int key) {
  jerry_value_t js_handler;
  int res = hashmap_get(ant_async->handler_map, key, (any_t *)&js_handler);
  if (res != MAP_OK)
    return false;
  jerry_release_value(js_handler);
  return (hashmap_remove(ant_async->handler_map, key) == MAP_OK);
}
jerry_value_t get_handler_from_ant_async(ant_async_t *ant_async, int key) {
  jerry_value_t js_handler;
  int res = hashmap_get(ant_async->handler_map, key, (any_t *)&js_handler);
  if (res != MAP_OK)
    return jerry_create_undefined();
  return js_handler;
}
bool emit_ant_async_event(ant_async_t *ant_async, int key, void *event) {
  // find js_handler
  jerry_value_t js_handler;
  int result;
  result = hashmap_get(ant_async->handler_map, key, (any_t *)&js_handler);
  if (result != MAP_OK)
    return false;

  // enqueue event to the handler
  enqueue_event_to_ant_async(js_handler, event);

  // emit uv_async event
  uv_async_send(&ant_async->uv_async);

  return true;
}

// event_queue in ant_handler
void enqueue_event_to_ant_async(ant_async_t *ant_async, void *event) {
  if (event != NULL)
    ll_insert_last(ant_async->event_queue, event);
}
void *get_first_event_from_ant_async(ant_async_t *ant_async) {
  return ll_get_first(ant_async->event_queue);
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
  event_queue_item_destroyer(ant_async_event->data);
  free(ant_async_event);
}