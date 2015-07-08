#include <pebble.h>

Window *window;
TextLayer *label_cann;
TextLayer *label_stan;
TextLayer *time_cann;
TextLayer *time_stan;

TextLayer *label_cann_shadow;
TextLayer *label_stan_shadow;
TextLayer *time_cann_shadow;
TextLayer *time_stan_shadow;

int label_shadow_h = 2;
int label_shadow_v = 2;
int time_shadow_h = 3;
int time_shadow_v = 3;

unsigned long get_total_seconds(struct tm *tick_time) {
  return (tick_time->tm_hour * 60 * 60) + (tick_time->tm_min * 60) + (tick_time->tm_sec);
}

void itoa1(int num, char* buffer) {
  const char digits[10] = "0123456789";
  buffer[0] = digits[num % 10];
}

void itoa2(int num, char* buffer) {
  const char digits[10] = "0123456789";
  if(num > 99) {
    buffer[0] = '9';
    buffer[1] = '9';
    return;
  } else if(num > 9) {
    buffer[0] = digits[num / 10];
  } else {
    buffer[0] = '0';
  }
  buffer[1] = digits[num % 10];
}

static unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }
  unsigned short display_hour = hour % 12;
  return display_hour ? display_hour : 12;
}

static void display_time(struct tm *tick_time) {
  unsigned long totalSeconds = get_total_seconds(tick_time);
  double thousandth = ((double)totalSeconds / 86400) * 10000;
  long cann_hour = ((long)thousandth) / 100;
  long cann_min = ((long)thousandth) % 100;

  static char display[] = "00.00";
  itoa2(cann_hour, &display[0]);
  itoa2(cann_min, &display[3]);
  text_layer_set_text(time_cann_shadow, display);
  text_layer_set_text(time_cann, display);

  static char time_text[] = "00:00";
  itoa2(get_display_hour(tick_time->tm_hour), &time_text[0]);
  itoa2(tick_time->tm_min, &time_text[3]);
  text_layer_set_text(time_stan_shadow, time_text);
  text_layer_set_text(time_stan, time_text);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
  display_time(tick_time);
}

void handle_init(void) {
  window = window_create();

  #ifdef PBL_COLOR
  #else
    window_set_fullscreen(window, true);
  #endif
  window_set_background_color(window, GColorBlack);
  Layer *window_layer = window_get_root_layer(window);
  
  #ifdef PBL_COLOR
    label_cann_shadow = text_layer_create(GRect(0 + label_shadow_h, 0 + label_shadow_v, 144, 30));
    text_layer_set_background_color(label_cann_shadow, GColorClear);
    text_layer_set_text_color(label_cann_shadow, GColorDarkCandyAppleRed);
    text_layer_set_text_alignment(label_cann_shadow, GTextAlignmentCenter);
    text_layer_set_text(label_cann_shadow, "Cannonian Time");
    text_layer_set_font(label_cann_shadow, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    layer_add_child(window_layer, text_layer_get_layer(label_cann_shadow));

    label_stan_shadow = text_layer_create(GRect(0 + label_shadow_h, 84 + label_shadow_v, 144, 30));
    text_layer_set_background_color(label_stan_shadow, GColorClear);
    text_layer_set_text_color(label_stan_shadow, GColorCobaltBlue);
    text_layer_set_text_alignment(label_stan_shadow, GTextAlignmentCenter);
    text_layer_set_text(label_stan_shadow, "Standard Time");
    text_layer_set_font(label_stan_shadow, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    layer_add_child(window_layer, text_layer_get_layer(label_stan_shadow));

    time_cann_shadow = text_layer_create(GRect(0 + time_shadow_h, 20 + time_shadow_v, 144, 50));
    text_layer_set_background_color(time_cann_shadow, GColorClear);
    text_layer_set_text_color(time_cann_shadow, GColorDarkCandyAppleRed);
    text_layer_set_text_alignment(time_cann_shadow, GTextAlignmentCenter);
    text_layer_set_font(time_cann_shadow, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(time_cann_shadow));

    time_stan_shadow = text_layer_create(GRect(0 + time_shadow_h, 104 + time_shadow_v, 144, 50));
    text_layer_set_background_color(time_stan_shadow, GColorClear);
    text_layer_set_text_color(time_stan_shadow, GColorCobaltBlue);
    text_layer_set_text_alignment(time_stan_shadow, GTextAlignmentCenter);
    text_layer_set_font(time_stan_shadow, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(time_stan_shadow));
  #endif

  label_cann = text_layer_create(GRect(0, 0, 144, 30));
  text_layer_set_background_color(label_cann, GColorClear);
  text_layer_set_text_color(label_cann, GColorWhite);
  text_layer_set_text_alignment(label_cann, GTextAlignmentCenter);
  text_layer_set_text(label_cann, "Cannonian Time");
  text_layer_set_font(label_cann, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(label_cann));

  label_stan = text_layer_create(GRect(0, 84, 144, 30));
  text_layer_set_background_color(label_stan, GColorClear);
  text_layer_set_text_color(label_stan, GColorWhite);
  text_layer_set_text_alignment(label_stan, GTextAlignmentCenter);
  text_layer_set_text(label_stan, "Standard Time");
  text_layer_set_font(label_stan, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(window_layer, text_layer_get_layer(label_stan));

  time_cann = text_layer_create(GRect(0, 20, 144, 50));
  text_layer_set_background_color(time_cann, GColorClear);
  text_layer_set_text_color(time_cann, GColorWhite);
  text_layer_set_text_alignment(time_cann, GTextAlignmentCenter);
  text_layer_set_font(time_cann, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(time_cann));

  time_stan = text_layer_create(GRect(0, 104, 144, 50));
  text_layer_set_background_color(time_stan, GColorClear);
  text_layer_set_text_color(time_stan, GColorWhite);
  text_layer_set_text_alignment(time_stan, GTextAlignmentCenter);
  text_layer_set_font(time_stan, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(time_stan));

  window_stack_push(window, true);

  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);
  display_time(tick_time);

  tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
}

void handle_deinit(void) {
  text_layer_destroy(label_cann);
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
