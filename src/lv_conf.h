#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

#define LV_COLOR_DEPTH 32
#define LV_COLOR_16_SWAP 0
#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (128 * 1024U)
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "SDL2/SDL.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (SDL_GetTicks())
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_48 1
#define LV_USE_LOG 1
#define LV_LOG_PRINTF 1
#define LV_USE_USER_DATA 1

#endif