/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_vehicule.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abidaux <abidaux@student.42lehavre.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 03:16:44 by abidaux           #+#    #+#             */
/*   Updated: 2025/12/10 03:16:46 by abidaux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_MODEL_H
#define DATA_MODEL_H

struct VehicleState {
    // Capteurs
    float rpm = 0;
    float speed = 0;
    float temp_culasse = 0;
    float temp_eau = 0;
    float battery_voltage = 0;
    
    // État Switches
    bool led_bar_on = false;
    bool long_range_on = false;
    bool work_lights_on = false;
    
    // Tripmaster
    float trip_distance = 0;
    unsigned long trip_time_start = 0;

    // Logique Sortie
    bool fan_status = false; // true = ON
    bool alert_active = false;
};

// Seuils
const float TEMP_WATER_FAN_ON = 90.0;
const float TEMP_WATER_FAN_OFF = 85.0;
const float TEMP_CULASSE_ALARM = 110.0;
const float TEMP_WATER_ALARM = 100.0;
const float V_BATT_LOW = 11.5;

#endif