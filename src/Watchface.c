#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

static void update_time() {
  //get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //create a long-lived buffer
  static char buffer[] = "00:00:00";
  
  //write the current houres and minutes unto buffer
  if(clock_is_24h_style() == true) {
    //use 24h time
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  }
  else {
    //12h time
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
  }
  
  //display time on textlayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *ticktime, TimeUnits units_changed){
  update_time();
}

static void main_window_load(Window *window) {
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  //display time at start
  update_time();
  
  //register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}