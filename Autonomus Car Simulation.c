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
    STATE_COUNT         
} State_t;

typedef State_t (*StateFunction)(void);

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
    handle_night_mode
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

State_t handle_idle(void) {
    printf("[IDLE] System on standby. Waiting for start signal...\n");
    
    if (system_start_signal) {
        return STATE_SYS_CHECK; 
    }
    return STATE_IDLE;
}

State_t handle_sys_check(void) {
    printf("[SYS_CHECK] Performing hardware and sensor calibration...\n");
    
    if (sensors_ok) {
        return STATE_NAVIGATE; 
    } else {
        printf("[SYS_CHECK] Error: Sensors are not ready!\n");
        return STATE_EMERGENCY;
    }
}

State_t handle_navigate(void) {
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
    
    // Once battery is no longer low, return to system check
    if (!battery_low) {
        printf(">> [CHARGING] Battery fully charged. Resuming system check...\n");
        return STATE_SYS_CHECK;
    }
    return STATE_CHARGING;
}

State_t handle_calibration(void){
    printf("[CALIBRATION] Sensor recalibration in progress...\n");
    
    // Once calibration is complete, return to navigation
    if (!calibration_needed){
        printf(">> [CALIBRATION] Calibration completed successfully.\n");
        return STATE_NAVIGATE;
    }
    return STATE_CALIBRATION;
}

State_t handle_manual_override(void){
    printf("[MANUAL OVERRIDE] Operator took control. Manual driving active...\n");
    
    // When remote control is released, return to navigation
    if (!remote_control_active){
        printf(">> [MANUAL OVERRIDE] Remote control disabled. Resuming autonomous mode.\n");
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

int main(void) {
    srand(time(NULL)); 

    State_t current_state = STATE_IDLE;

    printf("--- Randomized Autonomous System FSM Simulation Started ---\n");

    int step;
    for (step = 1; step <= 100; step++) {
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
        

        printf(">> [SENSOR STATES] start:%d | sensors_ok:%d | obstacle:%d | dead_end:%d | battery_fault:%d | battery_low:%d | calibration_needed:%d | remote_control_active:%d | light_red:%d | park_wait:%d | night_mode_active:%d\n",
               system_start_signal, sensors_ok, obstacle_detected, dead_end_detected, critical_battery_fault, battery_low, calibration_needed, remote_control_active, light_red, park_wait, night_mode_active);
        
        current_state = state_table[current_state]();
    }

    return 0;
}
