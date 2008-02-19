#ifndef config_hpp
#define config_hpp

#include <string>
#include <cairo/cairo.h>
#include "yaml/yaml.hpp"

extern "C" Yval rtk_config_query(const std::string path);
extern "C" void rtk_config_set_color(cairo_t * cr, const char * path);

extern Yval rtk_global_config;

#endif
