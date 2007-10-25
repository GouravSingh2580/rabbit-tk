#ifndef global_hpp
#define global_hpp

#include <xcb/xcb.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xcb.h>

extern cairo_scaled_font_t * menu_font;
extern cairo_font_extents_t menu_font_extents;

extern xcb_connection_t * rtk_xcb_connection;
extern xcb_screen_t * rtk_xcb_screen;

extern void rtk_global_init(int argc, char ** argv);

#endif
