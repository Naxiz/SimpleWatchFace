#include <pebble.h>

static Window *s_main_window;

static TextLayer *s_week_year_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_day_layer;
static TextLayer *s_bluetooth_layer;

static char s_week_year_buffer[15];
static char s_battery_buffer[5];
static char s_time_buffer[6];
static char s_date_buffer[13];
static char s_day_buffer[10];
static char s_bluetooth_buffer[5];

static void update_time() {
  // Get current time
  time_t temp_time = time(NULL); 
  struct tm *tick_time = localtime(&temp_time);

  // Format time to strings
  strftime(s_week_year_buffer, sizeof(s_week_year_buffer), "Week %W / %Y", tick_time);
  strftime(s_time_buffer, sizeof(s_time_buffer), "%H:%M", tick_time);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%e %B", tick_time);
  strftime(s_day_buffer, sizeof(s_day_buffer), "%A", tick_time);

  // Update the display
  text_layer_set_text(s_week_year_layer, s_week_year_buffer);
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
  text_layer_set_text(s_day_layer, s_day_buffer);
}

static void update_battery() {
  BatteryChargeState charge_state = battery_state_service_peek();

  if (charge_state.is_charging) {
    snprintf(s_battery_buffer, sizeof(s_battery_buffer), "CHRG");
  } else {
    snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", charge_state.charge_percent);
  }

  text_layer_set_text(s_battery_layer, s_battery_buffer);
}

static void update_bluetooth() {
  if (bluetooth_connection_service_peek()) {
    strncpy(s_bluetooth_buffer, "BT:C", sizeof(s_bluetooth_buffer));
  } else {
    strncpy(s_bluetooth_buffer, "BT:D", sizeof(s_bluetooth_buffer));
  }

  text_layer_set_text(s_bluetooth_layer, s_bluetooth_buffer);
}

static void main_window_load(Window *window) {
  // Create week/year layer
  s_week_year_layer = text_layer_create(GRect(0, 0, 115, 20));
  text_layer_set_background_color(s_week_year_layer, GColorBlack);
  text_layer_set_text_color(s_week_year_layer, GColorWhite);
  text_layer_set_text(s_week_year_layer, s_week_year_buffer);
  text_layer_set_font(s_week_year_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_week_year_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_week_year_layer));

  // Create battery layer
  s_battery_layer = text_layer_create(GRect(115, 0, 29, 20));
  text_layer_set_background_color(s_battery_layer, GColorBlack);
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_text(s_battery_layer, s_battery_buffer);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_battery_layer));

  // Create time layer
  s_time_layer = text_layer_create(GRect(0, 43, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorWhite);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, s_time_buffer);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));

  // Create date layer
  s_date_layer = text_layer_create(GRect(0, 93, 144, 32));
  text_layer_set_background_color(s_date_layer, GColorWhite);
  text_layer_set_text_color(s_date_layer, GColorBlack);
  text_layer_set_text(s_date_layer, s_date_buffer);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_layer));

  // Create day layer
  s_day_layer = text_layer_create(GRect(0, 148, 122, 20));
  text_layer_set_background_color(s_day_layer, GColorBlack);
  text_layer_set_text_color(s_day_layer, GColorWhite);
  text_layer_set_text(s_day_layer, s_day_buffer);
  text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_day_layer, GTextAlignmentLeft);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_layer));

  // Create bluetooth layer
  s_bluetooth_layer = text_layer_create(GRect(122, 148, 22, 20));
  text_layer_set_background_color(s_bluetooth_layer, GColorBlack);
  text_layer_set_text_color(s_bluetooth_layer, GColorWhite);
  text_layer_set_text(s_bluetooth_layer, s_bluetooth_buffer);
  text_layer_set_font(s_bluetooth_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_bluetooth_layer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_bluetooth_layer));

  // Initial update for display
  update_time();
  update_battery();
  update_bluetooth();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_week_year_layer);
  text_layer_destroy(s_battery_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_bluetooth_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void battery_handler(BatteryChargeState charge_state) {
  update_battery();
}

void bluetooth_handler(bool connected) {
  update_bluetooth();
}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  #ifdef PBL_PLATFORM_APLITE
    // Fullscreen is only available on Aplite
    window_set_fullscreen(s_main_window, true);
  #endif

  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);
  bluetooth_connection_service_subscribe(bluetooth_handler);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}