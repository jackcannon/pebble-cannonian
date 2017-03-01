#include <pebble.h>

Window *window;
Layer *window_layer;
TextLayer *label_cann;
TextLayer *label_stan;
TextLayer *time_cann;
TextLayer *time_stan;

TextLayer *label_cann_shadow_1;
TextLayer *label_stan_shadow_1;
TextLayer *time_cann_shadow_1;
TextLayer *time_stan_shadow_1;

TextLayer *label_cann_shadow_2;
TextLayer *label_stan_shadow_2;
TextLayer *time_cann_shadow_2;
TextLayer *time_stan_shadow_2;

int label_shadow_1_h = 2;
int label_shadow_1_v = 2;
int time_shadow_1_h = 3;
int time_shadow_1_v = 3;

int label_shadow_2_h = 3;
int label_shadow_2_v = 3;
int time_shadow_2_h = 4;
int time_shadow_2_v = 4;

void prepare_layer(TextLayer *layer, GColor color, const char *text, const char *font_key) {
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, color);
  text_layer_set_text_alignment(layer, GTextAlignmentCenter);
  text_layer_set_text(layer, text);
  text_layer_set_font(layer, fonts_get_system_font(font_key));
  layer_add_child(window_layer, text_layer_get_layer(layer));
}

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

  static char display[] = "99.99";
//   itoa2(cann_hour, &display[0]);
//   itoa2(cann_min, &display[3]);
  text_layer_set_text(time_cann_shadow_2, display);
  text_layer_set_text(time_cann_shadow_1, display);
  text_layer_set_text(time_cann, display);

  static char time_text[] = "23:59";
//   itoa2(get_display_hour(tick_time->tm_hour), &time_text[0]);
//   itoa2(tick_time->tm_min, &time_text[3]);
  text_layer_set_text(time_stan_shadow_2, time_text);
  text_layer_set_text(time_stan_shadow_1, time_text);
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
  window_layer = window_get_root_layer(window);
  
  #ifdef PBL_COLOR
    label_cann_shadow_2 = text_layer_create(GRect(0 + label_shadow_2_h, 0 + label_shadow_2_v, 144, 30));
    label_stan_shadow_2 = text_layer_create(GRect(0 + label_shadow_2_h, 84 + label_shadow_2_v, 144, 30));
    time_cann_shadow_2 = text_layer_create(GRect(0 + time_shadow_2_h, 20 + time_shadow_2_v, 144, 50));
    time_stan_shadow_2 = text_layer_create(GRect(0 + time_shadow_2_h, 104 + time_shadow_2_v, 144, 50));

    prepare_layer(label_cann_shadow_2, GColorDarkCandyAppleRed, "Cannonian Time", FONT_KEY_ROBOTO_CONDENSED_21); /* GColorDarkCandyAppleRed */
    prepare_layer(label_stan_shadow_2, GColorCobaltBlue, "Standard Time", FONT_KEY_ROBOTO_CONDENSED_21); /* GColorCobaltBlue */
    prepare_layer(time_cann_shadow_2, GColorDarkCandyAppleRed, "", FONT_KEY_BITHAM_42_BOLD); /* GColorDarkCandyAppleRed */
    prepare_layer(time_stan_shadow_2, GColorCobaltBlue, "", FONT_KEY_BITHAM_42_BOLD); /* GColorCobaltBlue */

    label_cann_shadow_1 = text_layer_create(GRect(0 + label_shadow_1_h, 0 + label_shadow_1_v, 144, 30));
    label_stan_shadow_1 = text_layer_create(GRect(0 + label_shadow_1_h, 84 + label_shadow_1_v, 144, 30));
    time_cann_shadow_1 = text_layer_create(GRect(0 + time_shadow_1_h, 20 + time_shadow_1_v, 144, 50));
    time_stan_shadow_1 = text_layer_create(GRect(0 + time_shadow_1_h, 104 + time_shadow_1_v, 144, 50));

    prepare_layer(label_cann_shadow_1, GColorBlack, "Cannonian Time", FONT_KEY_ROBOTO_CONDENSED_21);
    prepare_layer(label_stan_shadow_1, GColorBlack, "Standard Time", FONT_KEY_ROBOTO_CONDENSED_21);
    prepare_layer(time_cann_shadow_1, GColorBlack, "", FONT_KEY_BITHAM_42_BOLD);
    prepare_layer(time_stan_shadow_1, GColorBlack, "", FONT_KEY_BITHAM_42_BOLD);

  #endif

  label_cann = text_layer_create(GRect(0, 0, 144, 30));
  label_stan = text_layer_create(GRect(0, 84, 144, 30));
  time_cann = text_layer_create(GRect(0, 20, 144, 50));
  time_stan = text_layer_create(GRect(0, 104, 144, 50));

  prepare_layer(label_cann, GColorWhite, "Cannonian Time", FONT_KEY_ROBOTO_CONDENSED_21);
  prepare_layer(label_stan, GColorWhite, "Standard Time", FONT_KEY_ROBOTO_CONDENSED_21);
  prepare_layer(time_cann, GColorWhite, "", FONT_KEY_BITHAM_42_BOLD);
  prepare_layer(time_stan, GColorWhite, "", FONT_KEY_BITHAM_42_BOLD);

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
