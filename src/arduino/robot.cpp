
#include "robot.h"

namespace robot {
	robot::Activity *current_activity = nullptr;
	uint32_t current_activity_cooldown = 0;
	uint32_t current_activity_clock = 0;
	uint32_t current_activity_last_call = 0;

	void setup() {

	}

	void loop() {
		check_distance_sensors();
		update_motor_speeds();
		run_activity();
		read_message_buffer();
	}

	void check_distance_sensors() {
		
	}

	void update_motor_speeds() {

	}

	void run_activity() {
		if (current_activity == nullptr) return;

		uint32_t current_time = micros();
		uint32_t delta_time = current_time - current_activity_last_call;

		// update clock and cooldown
		current_activity_cooldown -= delta_time;
		current_activity_clock += delta_time;

		// if activity has timed out then stop it
		if (current_activity_clock > current_activity->timeout && current_activity->timeout != 0) {
			current_activity = nullptr;
			return;
		}

		// if activity predicate exists and returns false, stop activity
		if (current_activity->predicate != nullptr && !(*current_activity->predicate)()) {
			current_activity = nullptr;
			return;
		}

		// if cooldown is satisfied
		if (delta_time >= current_activity_cooldown) {
			// run the activity
			current_activity->callback();
			// note the last call time
			current_activity_last_call = current_time;
			// reset the cooldown (TODO: review this)
			current_activity_cooldown = delta_time >= current_activity->cooldown
			                          ? current_activity->cooldown
			                          : current_activity->cooldown - delta_time + current_activity_cooldown;
		}
	}

	void read_message_buffer() {

	}
}
