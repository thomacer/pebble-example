#include <pebble.h>

static Window *s_window;	
static TextLayer* summary_layer;
static TextLayer* location_layer;
static TextLayer* date_layer;
  
// Keys for AppMessage Dictionary
// These should correspond to the values you defined in appinfo.json/Settings
enum {
	STATUS_KEY = 0,	
	MESSAGE_KEY = 1,
  SUMMARY_KEY = 2,
  LOCATION_KEY = 3,
  DATE_KEY = 4,
};

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {  
  Tuple *tuple;
	
	tuple = dict_find(received, SUMMARY_KEY);
	if (tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received summary: %s", tuple->value->cstring);
    text_layer_set_text (summary_layer, tuple->value->cstring);
	} else {
  }
	
	tuple = dict_find(received, LOCATION_KEY);
	if (tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received location: %s", tuple->value->cstring);
    text_layer_set_text (location_layer, tuple->value->cstring);
  }
  
  tuple = dict_find(received, DATE_KEY);
	if (tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received location: %s", tuple->value->cstring);
    text_layer_set_text (date_layer, tuple->value->cstring);
  }
}


// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Incoming message dropped");

}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Outbox failed");
}

static void init(void) {
	s_window = window_create();
	window_stack_push(s_window, true);

  Layer* window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  
  summary_layer = text_layer_create(GRect(0, 0, bounds.size.w, bounds.size.h / 3));
  text_layer_set_text(summary_layer, "Summary");  
  layer_add_child(window_layer, text_layer_get_layer(summary_layer));
  
  location_layer = text_layer_create(GRect(0, bounds.size.h / 3, bounds.size.w, bounds.size.h / 3));  
  text_layer_set_text(location_layer, "Location");
  layer_add_child(window_layer, text_layer_get_layer(location_layer));
  
  date_layer = text_layer_create(GRect(0, 2 * bounds.size.h / 3, bounds.size.w, bounds.size.h / 3));  
  text_layer_set_text(date_layer, "Date");
  layer_add_child(window_layer, text_layer_get_layer(date_layer));

	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
  // app_message_register_outbox_sent(outbox_sent_callback);


  // Initialize AppMessage inbox and outbox buffers with a suitable size
  const int inbox_size = 1024;
  const int outbox_size = 1024;
	app_message_open(inbox_size, outbox_size);
}

static void deinit(void) {
	app_message_deregister_callbacks();
  text_layer_destroy(summary_layer);
  text_layer_destroy(location_layer);
	window_destroy(s_window);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}