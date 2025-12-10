/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 02:59:25 by abidaux           #+#    #+#             */
/*   Updated: 2025/12/10 03:31:12 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// --- INCLUDES ---
#ifdef SIMULATOR
    // Grâce au fix dans platformio.ini, on peut inclure directement "sdl/sdl.h"
    #include "sdl/sdl.h" 
#else
    #include <Arduino.h> 
#endif

// On inclut la config qu'on vient de créer
#include "lv_conf.h" 
#include "lvgl.h"

// Tes fichiers
#include "data_model.h"
#include "gui.cpp"
// --- FIN DES INCLUDES ---

VehicleState state;
unsigned long last_update = 0;

// --- MODE SIMULATEUR ---
#ifdef SIMULATOR
// Note: On n'inclut PLUS "SDL.h" ici, c'est déjà géré en haut via lv_drivers
// Sliders pour le simulateur
lv_obj_t *slider_temp, *slider_rpm, *slider_speed;

// -----------------------

void logic_update() {
    // 1. Gestion Ventilateur (Hystérésis)
    if (state.temp_eau >= TEMP_WATER_FAN_ON && !state.fan_status) {
        state.fan_status = true;
        // digitalWrite(PIN_RELAY_FAN, HIGH); // Sur ESP32
        printf("FAN ON (Temp: %.1f)\n", state.temp_eau);
    } else if (state.temp_eau <= TEMP_WATER_FAN_OFF && state.fan_status) {
        state.fan_status = false;
        // digitalWrite(PIN_RELAY_FAN, LOW); // Sur ESP32
        printf("FAN OFF\n");
    }

    // 2. Gestion Sécurité
    if (state.temp_eau > TEMP_WATER_ALARM || state.temp_culasse > TEMP_CULASSE_ALARM || state.battery_voltage < V_BATT_LOW) {
        state.alert_active = true;
    } else {
        state.alert_active = false;
    }

    // 3. Tripmaster (Simulation incrément si on roule)
    if (state.speed > 0) {
        // Ajout distance basique pour l'exemple
        state.trip_distance += (state.speed / 3600.0) * (0.1); // Update tous les 100ms
    }
}

void setup() {
    // Init LVGL
    lv_init();
    
    #ifdef SIMULATOR
        // Init SDL (Drivers écran PC)
        // ... Code boiler plate SDL ...
        // create_simulator_controls() est appelé ici
        setup_ui();
        create_simulator_controls(); // Ajoute les sliders de test sur l'écran
    #else
        // Init TFT_eSPI (Drivers écran ESP32)
        // tft.begin();
        // lv_disp_draw_buf_init(...);
        setup_ui();
        // Config GPIO relais ventilateur
        // pinMode(PIN_RELAY_FAN, OUTPUT);
    #endif
}

void loop() {
    lv_timer_handler(); // Gestion graphique LVGL
    
    // Logique toutes les 100ms
    if (millis() - last_update > 100) {
        logic_update();
        update_ui(state);
        
        // Simuler changement de page avec bouton physique (Touche clavier sur PC ?)
        // Si bouton appuyé -> lv_tabview_set_act(tabview, 1, LV_ANIM_ON);
        
        last_update = millis();
    }
    delay(5);
}