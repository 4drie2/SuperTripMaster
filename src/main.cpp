/* ************************************************************************** */
/* */
/* main.cpp - VERSION FINALE (Avec main() pour le PC)                       */
/* */
/* ************************************************************************** */

// --- INCLUDES ---
#ifdef SIMULATOR
    #include "sdl/sdl.h" 
    
    /* --- Émulation Arduino pour le PC --- */
    uint32_t millis() {
        return SDL_GetTicks();
    }
    
    void delay(uint32_t ms) {
        SDL_Delay(ms);
    }
    /* ------------------------------------ */

#else
    #include <Arduino.h> 
#endif

#include "lv_conf.h" 
#include "lvgl.h"
#include "data_model.h"

// Déclarations
void setup_ui();
void update_ui(VehicleState &state);

VehicleState state;
unsigned long last_update = 0;

// --- MODE SIMULATEUR ---
#ifdef SIMULATOR
// Sliders pour le simulateur
lv_obj_t *slider_temp, *slider_rpm, *slider_speed;

void create_simulator_controls() {
    printf("Simulateur démarré.\n");
}
#endif
// -----------------------

void logic_update() {
    // 1. Gestion Ventilateur
    if (state.temp_eau >= TEMP_WATER_FAN_ON && !state.fan_status) {
        state.fan_status = true;
        printf("FAN ON (Temp: %.1f)\n", state.temp_eau);
    } else if (state.temp_eau <= TEMP_WATER_FAN_OFF && state.fan_status) {
        state.fan_status = false;
        printf("FAN OFF\n");
    }

    // 2. Gestion Sécurité
    if (state.temp_eau > TEMP_WATER_ALARM || state.temp_culasse > TEMP_CULASSE_ALARM || state.battery_voltage < V_BATT_LOW) {
        state.alert_active = true;
    } else {
        state.alert_active = false;
    }

    // 3. Tripmaster
    if (state.speed > 0) {
        state.trip_distance += (state.speed / 3600.0) * (0.1); 
    }
}

void setup() {
    lv_init();
    
    #ifdef SIMULATOR
        sdl_init(); 
        
        // Configuration Écran & Souris pour SDL
        static lv_disp_draw_buf_t draw_buf;
        static lv_color_t buf[SDL_HOR_RES * SDL_VER_RES]; 
        lv_disp_draw_buf_init(&draw_buf, buf, NULL, SDL_HOR_RES * SDL_VER_RES);

        static lv_disp_drv_t disp_drv;
        lv_disp_drv_init(&disp_drv);
        disp_drv.draw_buf = &draw_buf;
        disp_drv.flush_cb = sdl_display_flush;
        disp_drv.hor_res = SDL_HOR_RES;
        disp_drv.ver_res = SDL_VER_RES;
        lv_disp_drv_register(&disp_drv);

        static lv_indev_drv_t indev_drv;
        lv_indev_drv_init(&indev_drv);
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        indev_drv.read_cb = sdl_mouse_read;
        lv_indev_drv_register(&indev_drv);

        setup_ui();
        create_simulator_controls();
    #else
        setup_ui();
    #endif
}

void loop() {
    lv_timer_handler(); 
    
    if (millis() - last_update > 100) {
        logic_update();
        update_ui(state);
        last_update = millis();
    }
    
    delay(5);
}

// --- LE FIX FINAL : Point d'entrée pour le PC ---
#ifdef SIMULATOR
int main(int argc, char *argv[]) {
    (void)argc; // Pour éviter les warnings "unused variable"
    (void)argv;

    setup();

    while(1) {
        loop();
    }

    return 0;
}
#endif
// ------------------------------------------------