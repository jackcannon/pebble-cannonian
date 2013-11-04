#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "stdio.h"

#define MY_UUID { 0x38, 0xBB, 0x01, 0xE6, 0x10, 0x24, 0x4D, 0x2A, 0xB9, 0x94, 0x9A, 0x1A, 0xEF, 0xB4, 0x76, 0x57 }
PBL_APP_INFO(MY_UUID, "Cannonian Time Digital", "Cannonian Ltd", 0x5, 0x0, RESOURCE_ID_IMAGE_MENU_ICON, APP_INFO_WATCH_FACE);

Window window;
TextLayer time_cann;
TextLayer time_stan;
TextLayer label_cann;
TextLayer label_stan;

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

unsigned long get_total_seconds(PblTm *tick_time) {
  return (tick_time->tm_hour * 60 * 60) + (tick_time->tm_min * 60) + (tick_time->tm_sec);
}

void display_time(PblTm *tick_time) {
  unsigned long totalSeconds = get_total_seconds(tick_time);
  static char display[] = "00.00";
	
  double thousandth = ((double)totalSeconds / 86400) * 10000;
  long cann_hour = ((long)thousandth) / 100;
  long cann_min = ((long)thousandth) % 100;
	
  itoa2(cann_hour, &display[0]);
  itoa2(cann_min, &display[3]);
	
  text_layer_set_text(&time_cann, display);
	
  static char time_text[] = "00:00";
  if(clock_is_24h_style())
    string_format_time(time_text, sizeof(time_text), "%R", tick_time);
  else
    string_format_time(time_text, sizeof(time_text), "%I:%M", tick_time);
  text_layer_set_text(&time_stan, time_text);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)t;
  (void)ctx;

  display_time(t->tick_time);
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Cannonian Time Digital");
  window_stack_push(&window, true /* Animated */);
  window_set_background_color(&window, GColorBlack);
	
  text_layer_init(&label_cann, GRect(0, 0, 144, 30));
  text_layer_set_background_color(&label_cann, GColorClear);
  text_layer_set_text_color(&label_cann, GColorWhite);
  text_layer_set_text_alignment(&label_cann, GTextAlignmentCenter);
  text_layer_set_text(&label_cann, "Cannonian Time");
  text_layer_set_font(&label_cann, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &label_cann.layer);
	
  text_layer_init(&label_stan, GRect(0, 84, 144, 30));
  text_layer_set_background_color(&label_stan, GColorClear);
  text_layer_set_text_color(&label_stan, GColorWhite);
  text_layer_set_text_alignment(&label_stan, GTextAlignmentCenter);
  text_layer_set_text(&label_stan, "Standard Time");
  text_layer_set_font(&label_stan, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &label_stan.layer);
	
  text_layer_init(&time_cann, GRect(0, 20, 144, 50));
  text_layer_set_background_color(&time_cann, GColorClear);
  text_layer_set_text_color(&time_cann, GColorWhite);
  text_layer_set_text_alignment(&time_cann, GTextAlignmentCenter);
  text_layer_set_font(&time_cann, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(&window.layer, &time_cann.layer);

  text_layer_init(&time_stan, GRect(0, 104, 144, 50));
  text_layer_set_background_color(&time_stan, GColorClear);
  text_layer_set_text_color(&time_stan, GColorWhite);
  text_layer_set_text_alignment(&time_stan, GTextAlignmentCenter);
  text_layer_set_font(&time_stan, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  layer_add_child(&window.layer, &time_stan.layer);
	
	
  // Avoids a blank screen on watch start.
  PblTm tick_time;
  get_time(&tick_time);
  display_time(&tick_time);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,

    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = SECOND_UNIT
    }

  };
  app_event_loop(params, &handlers);
}
