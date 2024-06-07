#ifndef _SQUARELINE_UI_H
#define _SQUARELINE_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "ui_events.h"

extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_time_label;
extern lv_obj_t * ui_date_label;
void ui_event_count_button(lv_event_t * e);
extern lv_obj_t * ui_count_button;
extern lv_obj_t * ui_count_label;

LV_FONT_DECLARE(ui_font_clock_80);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif
