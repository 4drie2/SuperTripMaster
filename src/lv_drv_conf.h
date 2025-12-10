#ifndef LV_DRV_CONF_H
#define LV_DRV_CONF_H

#include "lv_conf.h"

// --- Configuration pour le Simulateur (SDL) ---
#ifndef USE_SDL
# define USE_SDL 1
#endif

// Résolution de la fenêtre du simulateur
#ifndef SDL_HOR_RES
# define SDL_HOR_RES 320
#endif

#ifndef SDL_VER_RES
# define SDL_VER_RES 240
#endif

// Active le contrôle par la souris
#ifndef USE_MOUSE
# define USE_MOUSE 1
#endif

#ifndef USE_MOUSEWHEEL
# define USE_MOUSEWHEEL 1
#endif

#ifndef USE_KEYBOARD
# define USE_KEYBOARD 1
#endif

#endif /*LV_DRV_CONF_H*/