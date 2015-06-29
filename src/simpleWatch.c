#include <pebble.h>

static Window *window;
static TextLayer *time_layer;
static TextLayer *message_text_layer;

// on window load, create and add all the layers to the main window
static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // set window background to black
  window_set_background_color(window, GColorBlack);

  // create time layer
  time_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 50 } });
  text_layer_set_text(time_layer, "00:00");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  // set font size to 24 and use gothic font
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));

  // set text color to white
  text_layer_set_background_color(time_layer, GColorBlack);
  text_layer_set_text_color(time_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(time_layer));

  // create message layer
  message_text_layer = text_layer_create((GRect) { .origin = { 0, 50 }, .size = { bounds.size.w, 50 } });
  text_layer_set_text(message_text_layer, "Namasthe!");
  text_layer_set_text_alignment(message_text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(message_text_layer, GColorBlack);
  text_layer_set_text_color(message_text_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(message_text_layer));

}

// on window unload, cleanup all the layers created
static void window_unload(Window *window) {
  text_layer_destroy(time_layer);
  text_layer_destroy(message_text_layer);
}

// time tick handler, not yet using tick_tie and units_changed
static void time_tick_handler(struct tm *tick_time, TimeUnits units_changed){
  // get the current time into the buffer
  static char s_time_buffer[16];
  if (clock_is_24h_style()) {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%H:%M", tick_time);
  } else {
    strftime(s_time_buffer, sizeof(s_time_buffer), "%I:%M", tick_time);
  }

  // update the text layer
  text_layer_set_text(time_layer, s_time_buffer);
}

static void init(void) {
  // create a window
  window = window_create();

  // set window load / unload handlers to window_load / window_unload methods
  // respectively
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  // animated flag
  const bool animated = true;

  // register timer tick event
  // using seconds hand causes a battery drain.
  // Regiter for minute tick
  tick_timer_service_subscribe(MINUTE_UNIT, time_tick_handler);

  // push the window into the stack
  window_stack_push(window, animated);
}

static void deinit(void) {
  // call window destroy which inturn deletes all the layers
  window_destroy(window);
}

int main(void) {

  // initialize
  init();

  // app log in case necessary
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
