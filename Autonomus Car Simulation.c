#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    STATE_IDLE = 0,
    STATE_SYS_CHECK,    
    STATE_NAVIGATE,
    STATE_AVOID,
    STATE_REVERSE,      
    STATE_EMERGENCY,
    STATE_CHARGING,
    STATE_CALIBRATION,
    STATE_MANUAL_OVERRIDE,
    STATE_TRAFFIC_LIGHT,
    STATE_PARKING,
    STATE_NIGHT_MODE,
    STATE_OBSTACLE_AVOIDENCE,
    STATE_INTERSECTION_PASS,
    STATE_LANE_CHANGE,
    STATE_SPEED_LIMIT_ADJUSTE,
    STATE_WEATHER_HAZARD,
    STATE_DIAGNOSTICS,
    STATE_GPS_LOST,
    STATE_SOS_BEACON,
    STATE_SLEEP_POWER_SAVE,
    STATE_FIRMWARE_UPDATE,
    STATE_TRAILER_ATTACHED,
    STATE_BLIND_SPOT_ALERT,
    STATE_COUNT         
} State_t;

typedef State_t (*StateFunction)(void); //!!!!!

State_t handle_idle(void);
State_t handle_sys_check(void);
State_t handle_navigate(void);
State_t handle_avoid(void);
State_t handle_reverse(void);
State_t handle_emergency(void);
State_t handle_charging(void);
State_t handle_calibration(void);
State_t handle_manual_override(void);
State_t handle_traffic_light(void);
State_t handle_parking(void);
State_t handle_night_mode(void);
State_t handle_obstacle_avoidence(void);
State_t handle_intersection_pass(void);
State_t handle_lane_change(void);
State_t handle_speed_limit_adjuste(void);
State_t handle_weather_hazard(void);
State_t handle_diagnostics(void);
State_t handle_gps_lost(void);
State_t handle_sos_beacon(void);
State_t handle_sleep_power_save(void);
State_t handle_firmware_update(void);
State_t handle_trailer_attached(void);
State_t handle_blind_spot_alert(void);

StateFunction state_table[STATE_COUNT] = {
    handle_idle,
    handle_sys_check,
    handle_navigate,
    handle_avoid,
    handle_reverse,
    handle_emergency,
    handle_charging,
    handle_calibration,
    handle_manual_override,
    handle_traffic_light,
    handle_parking,
    handle_night_mode,
    handle_obstacle_avoidence,
    handle_intersection_pass,
    handle_lane_change,
    handle_speed_limit_adjuste,
    handle_weather_hazard,
    handle_diagnostics,
    handle_gps_lost,
    handle_sos_beacon,
    handle_sleep_power_save,
    handle_firmware_update,
    handle_trailer_attached,
    handle_blind_spot_alert
};

int system_start_signal = 0;
int sensors_ok = 0;
int obstacle_detected = 0;
int dead_end_detected = 0;
int critical_battery_fault = 0;
int battery_low = 0;
int calibration_needed = 0;
int remote_control_active = 0;
int light_red = 0;
int park_wait = 0;
int night_mode_active = 0;
int is_lane_left_available = 0;
int is_lane_right_available = 0;
int passing_order = 0;
int over_speed_limit = 0;
int defrost_system_active = 0;
int obd2_censor_active = 0;
int is_gps_lost = 0;
int emergency_mode_active = 0;
int long_nonactive_time = 0;
int available_ota_sw = 0;
int trailer_connected = 0;

State_t handle_idle(void){
    printf("[IDLE] System on standby. Waiting for start signal...\n");
    
    if (system_start_signal){
        return STATE_SYS_CHECK; 
    }
    return STATE_IDLE;
}

State_t handle_sys_check(void){
    printf("[SYS_CHECK] Performing hardware and sensor calibration...\n");
    
    if (sensors_ok) {
        return STATE_NAVIGATE; 
    } else {
        printf("[SYS_CHECK] Error: Sensors are not ready!\n");
        return STATE_EMERGENCY;
    }
}

State_t handle_navigate(void){
    printf("[NAVIGATE] Autonomous driving active, following route...\n");
    
    if(critical_battery_fault){
        return STATE_EMERGENCY;
    }
    if(battery_low){
        return STATE_CHARGING;
    }
    if(calibration_needed){
        return STATE_CALIBRATION;
    }
    if(remote_control_active){
        return STATE_MANUAL_OVERRIDE;
    }
    if(dead_end_detected){
        return STATE_REVERSE;   
    }
    if(obstacle_detected){
        return STATE_AVOID;     
    }
    if(light_red){
    	return STATE_TRAFFIC_LIGHT;
	}
	if(park_wait){
		return STATE_PARKING;
	}
	if(night_mode_active){
		return STATE_NIGHT_MODE;
	}
	if((is_lane_left_available == 0) || (is_lane_left_available == 1) || (is_lane_right_available == 0) || (is_lane_right_available == 1)){
		return STATE_OBSTACLE_AVOIDENCE;
		return STATE_LANE_CHANGE;
		return STATE_BLIND_SPOT_ALERT;
	}
	if(passing_order){
		return STATE_INTERSECTION_PASS;
	}
	if(over_speed_limit){
		return STATE_SPEED_LIMIT_ADJUSTE;
	}
	if(defrost_system_active){
		return STATE_WEATHER_HAZARD;
	}
	if(obd2_censor_active){
		return STATE_DIAGNOSTICS;
	}
	if(is_gps_lost){
		return STATE_GPS_LOST;
	}
	if(emergency_mode_active){
		return STATE_SOS_BEACON;
	}
	if(long_nonactive_time){
		return STATE_SLEEP_POWER_SAVE;
	}
	if(available_ota_sw){
		return STATE_FIRMWARE_UPDATE;
	}
	if(trailer_connected){
		return STATE_TRAILER_ATTACHED;
	}
    return STATE_NAVIGATE;
}

State_t handle_avoid(void){
    printf("[AVOID] Obstacle detected! Executing safe maneuver...\n");
    return STATE_NAVIGATE; 
}

State_t handle_reverse(void){
    printf("[REVERSE] Dead end! Shifting into reverse and backing up...\n");
    return STATE_NAVIGATE; 
}

State_t handle_emergency(void){
    printf("[EMERGENCY] Critical fault! Motors stopped, safe mode active.\n");
    
    if (critical_battery_fault == 0){
        printf(">> [RESET] Fault resolved! Restarting system...\n");
        return STATE_IDLE;
    }
    return STATE_EMERGENCY;
}

State_t handle_charging(void){
    printf("[CHARGING] Battery is low! Redirecting to charging station...\n");
    
    if (!battery_low) {
        printf(">> [CHARGING] Battery fully charged. Resuming system check...\n");
        return STATE_SYS_CHECK;
    }
    return STATE_CHARGING;
}

State_t handle_calibration(void){
    printf("[CALIBRATION] Sensor recalibration in progress...\n");
    
    if (!calibration_needed){
        printf("[CALIBRATION] Calibration completed successfully.\n");
        return STATE_NAVIGATE;
    }
    return STATE_CALIBRATION;
}

State_t handle_manual_override(void){
    printf("[MANUAL OVERRIDE] Operator took control. Manual driving active...\n");
    
    if (!remote_control_active){
        printf("[MANUAL OVERRIDE] Remote control disabled. Resuming autonomous mode.\n");
        return STATE_NAVIGATE;
    }
    return STATE_MANUAL_OVERRIDE;
}

State_t handle_traffic_light(void){
	if(light_red){
		printf("[TRAFFIC LIGHT] Car will wait light till it turns to green...\n");
		return STATE_TRAFFIC_LIGHT;
	}
	else{
		printf(">> [TRAFFIC LIGHT] Car is starting to remove!!!\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_parking(void){
	if(park_wait){
		printf("[PARKING] Car will be parked...\n");
		return STATE_PARKING;
	}
	else{
		return STATE_NAVIGATE;
	}
}

State_t handle_night_mode(void){
	if(night_mode_active){
		printf("[NIGHT MODE : ON] It's night and car mode turns to Night!!!\n");
		return STATE_NIGHT_MODE;
	}
	else{
		printf("[NIGHT MODE : OFF] It's day and car mode is Normal...\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_obstacle_avoidence(void){
	if(is_lane_left_available == 0 && is_lane_right_available == 1){
		printf("[OBSTACLE AVOIDENCE] Car detected an obstacle and crossing to the right lane.\n");
		return STATE_OBSTACLE_AVOIDENCE;
	}
	if(is_lane_left_available == 1 && is_lane_right_available == 0){
		printf("[OBSTACLE AVOIDENCE] Car detected an obstacle and crossing to the left lane.\n");
		return STATE_OBSTACLE_AVOIDENCE;
	}
	if(is_lane_left_available == 0 && is_lane_right_available == 0){
		printf("[OBSTACLE AVOIDENCE] Car detected an obstacle but there's no available lane and car will wait!!!\n");
		return STATE_NAVIGATE;
	}
	if(is_lane_left_available == 1 && is_lane_right_available == 1){
		printf("[OBSTACLE AVOIDENCE] Car detected an obstacle and crossing left or right lane.\n");
		printf(">>[!!!CROSS!!!] Car will decide left or right lane.\n");
		return STATE_OBSTACLE_AVOIDENCE;
	} 
}

State_t handle_intersection_pass(void){
	if(passing_order == 1){
		printf("[INTERSECTION PASS] Car will remove according to passing order...\n");
		return STATE_INTERSECTION_PASS;
	}
	else{
		printf("[INTERSECTION PASS] Car will continue to stop while its order is not coming...\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_lane_change(void){
	if(is_lane_left_available == 1 && is_lane_right_available == 1){
		printf(">>[!!!CROSS!!!] Car will change lane left or right!\n");
		return STATE_LANE_CHANGE;
	}
	if(is_lane_left_available == 1 && is_lane_right_available == 0){
		printf("[LANE CHANGE] Car will change lane and will cross to left.\n");
		return STATE_LANE_CHANGE;
	}
	if(is_lane_left_available == 0 && is_lane_right_available == 1){
		printf("[LANE CHANGE] Car will change lane and will cross to right.\n");
		return STATE_LANE_CHANGE;
	}
	if(is_lane_left_available == 0 && is_lane_right_available == 0){
		printf("[LANE CHANGE] Car will continue its move on its lane.\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_speed_limit_adjuste(void){
	if(over_speed_limit){
		printf(">>>[!!!WARNING!!!] Car is over the speed limit!!!\n");
		printf(">>>[!!!CAUTION!!!] Release the gas!!!\n");
		return STATE_SPEED_LIMIT_ADJUSTE;
	}
	else{
		printf("[SPEED LIMIT ADJUSTE] Car driving is safe.\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_weather_hazard(void){
	if(defrost_system_active){
		printf("[WEATHER HAZARD] Defroster system is active...Please drive careful!!!\n");
		printf(">>>[DEFROSTER SYSTEM : ON]\n");
		return STATE_WEATHER_HAZARD;
	}
	else{
		printf("[WEATHER HAZARD] Weather is OK.Have a nice drive!!!\n");
		printf(">>>[DEFROSTER SYSTEM : OFF]\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_diagnostics(void){
	if(obd2_censor_active){
		printf("[ISSUE] OBD2 Censor detected and ERROR!!! Please go to Service ASAP!!!\n");
		return STATE_DIAGNOSTICS;
	}
	else{
		printf("[CLEAR] Car is healthy.\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_gps_lost(void){
	if(is_gps_lost){
		printf("[GPS LOST] GPS Signal is lost!!!\n");
		return STATE_GPS_LOST;
	}
	else{
		printf("[ACTIVE GPS] GPS signal is active.\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_sos_beacon(void){
	if(emergency_mode_active){
		printf("[!!!SOS - EMERGENCY!!!] Call an ambulance or police!!!\n");
		return STATE_SOS_BEACON;
	}
	else{
		return STATE_NAVIGATE;
	}
}

State_t handle_sleep_power_save(void){
	if(long_nonactive_time){
		printf("[SAVE MODE : ON] Car changed mode to save for saving charge.\n");
		return STATE_SLEEP_POWER_SAVE;
	}
	else{
		return STATE_NAVIGATE;
	}
}

State_t handle_firmware_update(void){
	if(available_ota_sw){
		printf("[FIRMWARE UPDATE] OTA Software detected...Please update car's SW.\n");
		return STATE_FIRMWARE_UPDATE;
	}
	else{
		printf("[VERSUS : CURRENT] Car SW is on the newest versus.\n");
		return STATE_NAVIGATE;
	}
}

State_t handle_trailer_attached(void){
	if(trailer_connected){
		printf("[TRAILER] A trailer is connected to car and changed weight center.\n");
		return STATE_TRAILER_ATTACHED;
	}
	else{
		return STATE_NAVIGATE;
	}
}

State_t handle_blind_spot_alert(void){
	if(is_lane_left_available == 0 && is_lane_right_available == 0){
		printf("[!!!BLIND SPOT ALERT!!!] Blind Spot Alert detected cars both of 2 lanes!!!\n");
		printf(">>>[SAVE LANE] Car will save its lane...\n");
		return STATE_BLIND_SPOT_ALERT;
	}
	if(is_lane_left_available == 1 && is_lane_right_available == 0){
		printf("[BLIND SPOT : RIGHT] Car detected from right lane.\n");
		return STATE_BLIND_SPOT_ALERT;
	}
	if(is_lane_left_available == 0 && is_lane_right_available == 1){
		printf("[BLIND SPOT : LEFT] Car detected from left lane.\n");
		return STATE_BLIND_SPOT_ALERT;
	}
	if(is_lane_left_available == 1 && is_lane_right_available == 1){
		return STATE_NAVIGATE;
	}
}

int main(void){
    srand(time(NULL)); 

    State_t current_state = STATE_IDLE;

    printf("--- Randomized Autonomous System FSM Simulation Started ---\n");

    int step;
    for (step = 1; step <= 100; step++){
        printf("\n----------------------------------------\n");
        printf("[Loop Step %d]\n", step);

        system_start_signal = rand()% 2;
        sensors_ok = rand()% 2;
        obstacle_detected = rand()% 2;
        dead_end_detected = rand()% 2;
        critical_battery_fault = (rand() % 10) == 0;
        battery_low = rand()% 2;
        calibration_needed = rand()% 2;
        remote_control_active = rand()% 2;
        light_red = rand()% 2;
        park_wait = rand()% 2;
        night_mode_active = rand()% 2;
        is_lane_left_available = rand()% 2;
        is_lane_right_available = rand()% 2;
        passing_order = rand()% 2;
        over_speed_limit = rand()% 2;
        defrost_system_active = rand()% 2;
        is_gps_lost = rand()% 2;
        emergency_mode_active = rand()% 2;
        long_nonactive_time = rand()% 2;
        available_ota_sw = rand()% 2;
        trailer_connected = rand()% 2;
        
        printf(">> [SENSOR STATES : 1st STAGE] start:%d | sensors_ok:%d | obstacle:%d | dead_end:%d | battery_fault:%d | battery_low:%d | calibration_needed:%d | remote_control_active:%d | light_red:%d | park_wait:%d | night_mode_active:%d\n",
               system_start_signal, sensors_ok, obstacle_detected, dead_end_detected, critical_battery_fault, battery_low, calibration_needed, remote_control_active, light_red, park_wait, night_mode_active);
    	printf(">> [SENSOR STATES : 2ns STAGE] lane_left:%d | lane_right:%d | passing_order:%d | over_speed_limit:%d | defrost_system_active:%d | is_gps_lost:%d | emergency_mode_active:%d | long_nonactive_time:%d | available_ota_sw:%d | trailer_connected:%d\n",
				is_lane_left_available, is_lane_right_available, passing_order, over_speed_limit, defrost_system_active, is_gps_lost, emergency_mode_active, long_nonactive_time, available_ota_sw, trailer_connected);
        
        current_state = state_table[current_state]();
    }

    return 0;
}
