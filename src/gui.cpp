/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gui.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 02:59:04 by abidaux           #+#    #+#             */
/*   Updated: 2025/12/10 02:59:07 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lvgl.h"
#include "data_model.h"
#include <stdio.h>

// Widgets globaux pour mise à jour
lv_obj_t *lbl_rpm_sidebar;
lv_obj_t *lbl_trip_sidebar;
lv_obj_t *meter_rpm;
lv_obj_t *lbl_speed;
lv_obj_t *lbl_temp_eau;
lv_obj_t *lbl_temp_culasse;
lv_obj_t *lbl_batt;
lv_obj_t *alert_overlay;
lv_obj_t *lbl_alert_msg;

// Styles
lv_style_t style_baroudeur;
lv_style_t style_alert;

void init_styles() {
    lv_style_init(&style_baroudeur);
    lv_style_set_bg_color(&style_baroudeur, lv_color_hex(0x202020)); // Gris foncé
    lv_style_set_text_color(&style_baroudeur, lv_color_hex(0xFFFFFF));
    lv_style_set_border_width(&style_baroudeur, 2);
    lv_style_set_border_color(&style_baroudeur, lv_color_hex(0xE65100)); // Orange Rallye

    lv_style_init(&style_alert);
    lv_style_set_bg_color(&style_alert, lv_color_hex(0xFF0000));
    lv_style_set_text_color(&style_alert, lv_color_hex(0xFFFFFF));
}

// Création de la Sidebar (Toujours visible)
void create_sidebar(lv_obj_t *parent) {
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 80, 240); // 80px de large
    lv_obj_align(cont, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_style(cont, &style_baroudeur, 0);

    // RPM Petit
    lv_obj_t *l1 = lv_label_create(cont);
    lv_label_set_text(l1, "RPM");
    lv_obj_align(l1, LV_ALIGN_TOP_MID, 0, 10);
    
    lbl_rpm_sidebar = lv_label_create(cont);
    lv_label_set_text(lbl_rpm_sidebar, "0000");
    lv_obj_align(lbl_rpm_sidebar, LV_ALIGN_TOP_MID, 0, 30);

    // Trip
    lv_obj_t *l2 = lv_label_create(cont);
    lv_label_set_text(l2, "TRIP");
    lv_obj_align(l2, LV_ALIGN_BOTTOM_MID, 0, -50);

    lbl_trip_sidebar = lv_label_create(cont);
    lv_label_set_text(lbl_trip_sidebar, "0.0km");
    lv_obj_align(lbl_trip_sidebar, LV_ALIGN_BOTTOM_MID, 0, -30);
}

// Page Principale (Conduite)
void create_drive_page(lv_obj_t *parent) {
    // Vitesse Réelle (Gros)
    lbl_speed = lv_label_create(parent);
    lv_obj_set_style_text_font(lbl_speed, &lv_font_montserrat_48, 0); // Grande police
    lv_label_set_text(lbl_speed, "0");
    lv_obj_align(lbl_speed, LV_ALIGN_CENTER, 40, -20); // Décalé à droite de la sidebar

    lv_obj_t *unit = lv_label_create(parent);
    lv_label_set_text(unit, "km/h");
    lv_obj_align_to(unit, lbl_speed, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

// Page Capteurs (Détails)
void create_sensor_page(lv_obj_t *parent) {
    lv_obj_t *grid = lv_obj_create(parent);
    lv_obj_set_size(grid, 220, 220);
    lv_obj_align(grid, LV_ALIGN_RIGHT_MID, 0, 0);
    
    // Temp Eau
    lbl_temp_eau = lv_label_create(grid);
    lv_label_set_text(lbl_temp_eau, "Eau: -- C");
    lv_obj_align(lbl_temp_eau, LV_ALIGN_TOP_LEFT, 0, 20);

    // Temp Culasse
    lbl_temp_culasse = lv_label_create(grid);
    lv_label_set_text(lbl_temp_culasse, "Culasse: -- C");
    lv_obj_align(lbl_temp_culasse, LV_ALIGN_LEFT_MID, 0, 0);

    // Batterie
    lbl_batt = lv_label_create(grid);
    lv_label_set_text(lbl_batt, "Batt: -- V");
    lv_obj_align(lbl_batt, LV_ALIGN_BOTTOM_LEFT, 0, -20);
}

// Overlay d'Alerte (Sécurité)
void create_alert_overlay() {
    alert_overlay = lv_obj_create(lv_scr_act());
    lv_obj_set_size(alert_overlay, 200, 100);
    lv_obj_align(alert_overlay, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(alert_overlay, &style_alert, 0);
    lv_obj_add_flag(alert_overlay, LV_OBJ_FLAG_HIDDEN); // Caché par défaut

    lbl_alert_msg = lv_label_create(alert_overlay);
    lv_label_set_text(lbl_alert_msg, "STOP !");
    lv_obj_align(lbl_alert_msg, LV_ALIGN_CENTER, 0, 0);
}

// Tabview principal
lv_obj_t *tabview;

void setup_ui() {
    init_styles();
    
    // Création d'un écran de fond noir
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);

    // Sidebar fixe (ne bouge pas quand on change d'onglet)
    create_sidebar(lv_scr_act());

    // Zone de contenu (Tabview sans boutons, géré par boutons physiques)
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);
    lv_obj_set_pos(tabview, 80, 0); // Décalé après la sidebar
    lv_obj_set_size(tabview, 240, 240);
    
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Drive");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Sensors");

    create_drive_page(tab1);
    create_sensor_page(tab2);
    
    create_alert_overlay();
}

// Fonction de mise à jour UI (appelée dans la loop)
void update_ui(VehicleState &state) {
    // Update Labels
    lv_label_set_text_fmt(lbl_rpm_sidebar, "%d", (int)state.rpm);
    lv_label_set_text_fmt(lbl_trip_sidebar, "%.1f", state.trip_distance);
    lv_label_set_text_fmt(lbl_speed, "%d", (int)state.speed);
    
    lv_label_set_text_fmt(lbl_temp_eau, "Eau: %.0f C", state.temp_eau);
    lv_label_set_text_fmt(lbl_temp_culasse, "Culasse: %.0f C", state.temp_culasse);
    lv_label_set_text_fmt(lbl_batt, "Batt: %.1f V", state.battery_voltage);

    // Gestion Alertes (Clignotement simulé par visibilité)
    static uint32_t blink_timer = 0;
    if (state.alert_active) {
        if (lv_tick_get() - blink_timer > 500) {
            bool hidden = lv_obj_has_flag(alert_overlay, LV_OBJ_FLAG_HIDDEN);
            if (hidden) lv_obj_clear_flag(alert_overlay, LV_OBJ_FLAG_HIDDEN);
            else lv_obj_add_flag(alert_overlay, LV_OBJ_FLAG_HIDDEN);
            blink_timer = lv_tick_get();
        }
        
        // Texte alerte dynamique
        if (state.temp_eau > TEMP_WATER_ALARM) lv_label_set_text(lbl_alert_msg, "SURCHAUFFE\nEAU");
        else if (state.temp_culasse > TEMP_CULASSE_ALARM) lv_label_set_text(lbl_alert_msg, "SURCHAUFFE\nCULASSE");
    } else {
        lv_obj_add_flag(alert_overlay, LV_OBJ_FLAG_HIDDEN);
    }
}