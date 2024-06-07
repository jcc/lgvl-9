#include "ui.h"

lv_obj_t * ui_Screen1;
lv_obj_t * ui_time_label;
lv_obj_t * ui_date_label;
void ui_event_count_button(lv_event_t * e);
lv_obj_t * ui_count_button;
lv_obj_t * ui_count_label;

void ui_event_count_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        button_pressed(e);
    }
}

void ui_init(void)
{
  lv_display_t * dispp = lv_display_get_default();
  lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);

  lv_display_set_theme(dispp, theme);

  ui_Screen1 = lv_obj_create(NULL);
  lv_obj_remove_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
  lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_time_label = lv_label_create(ui_Screen1);
  lv_obj_set_width(ui_time_label, LV_SIZE_CONTENT);   /// 1
  lv_obj_set_height(ui_time_label, LV_SIZE_CONTENT);    /// 1
  lv_obj_set_x(ui_time_label, 0);
  lv_obj_set_y(ui_time_label, -60);
  lv_obj_set_align(ui_time_label, LV_ALIGN_CENTER);
  lv_label_set_text(ui_time_label, "12:25:45");
  lv_obj_set_style_text_color(ui_time_label, lv_color_hex(0x293062), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(ui_time_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_time_label, &lv_font_montserrat_48, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_date_label = lv_label_create(ui_Screen1);
  lv_obj_set_width(ui_date_label, LV_SIZE_CONTENT);   /// 1
  lv_obj_set_height(ui_date_label, LV_SIZE_CONTENT);    /// 1
  lv_obj_set_x(ui_date_label, 0);
  lv_obj_set_y(ui_date_label, -10);
  lv_obj_set_align(ui_date_label, LV_ALIGN_CENTER);
  lv_label_set_text(ui_date_label, "Mon 26 Dec 2023");
  lv_obj_set_style_text_color(ui_date_label, lv_color_hex(0x9C9CD9), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_opa(ui_date_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_date_label, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_count_button = lv_button_create(ui_Screen1);
  lv_obj_set_width(ui_count_button, 191);
  lv_obj_set_height(ui_count_button, 39);
  lv_obj_set_x(ui_count_button, 0);
  lv_obj_set_y(ui_count_button, 60);
  lv_obj_set_align(ui_count_button, LV_ALIGN_CENTER);
  lv_obj_add_flag(ui_count_button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
  lv_obj_remove_flag(ui_count_button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
  lv_obj_set_style_bg_color(ui_count_button, lv_color_hex(0x293062), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_count_button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_count_label = lv_label_create(ui_count_button);
  lv_obj_set_width(ui_count_label, LV_SIZE_CONTENT);   /// 1
  lv_obj_set_height(ui_count_label, LV_SIZE_CONTENT);    /// 1
  lv_obj_set_align(ui_count_label, LV_ALIGN_CENTER);
  lv_label_set_text(ui_count_label, "Button Pressed: 0");

  lv_obj_add_event_cb(ui_count_button, ui_event_count_button, LV_EVENT_ALL, NULL);

  lv_screen_load(ui_Screen1);
}
