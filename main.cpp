#include "mbed.h"

class Sensor {
private:
    PinName pin;      // The GPIO pin to which the sensor is connected
    AnalogIn sensor_analog_input; // Used for analog sensors (if the sensor output is analog)
    bool state;       // Tracks the sensor's current state (on/off)
    int threshold;    // Threshold for determining if the sensor detects the line

public:
    Sensor() : pin(PA_0), sensor_analog_input(pin), state(false), threshold(500) {} // Default pin and threshold
    // Constructor: Initializes the sensor with a pin and defaults for state and threshold
    Sensor(PinName pin, int threshold = 500)
        : pin(pin), sensor_analog_input(pin), state(false), threshold(threshold) {}

    // Method to read the value from the sensor pin
    // Returns a scaled value (0 to 1000 for analog sensor)
    int read_value() {
        return static_cast<int>(sensor_analog_input.read() * 1000);  // Read as an analog input and scale it to integer (0-1000)
    }

    // Method to set the threshold (in case it's needed for line detection or other purposes)
    void set_threshold(int new_threshold) {
        threshold = new_threshold;
    }

    // Getter for the threshold
    int get_threshold() const {
        return threshold;
    }
};

class SensorRig {
private:
    Sensor sensors[3];  // Array of 3 sensors (left, center, right)

public:
    // Constructor: Initializes the SensorRig with an array of sensors
    SensorRig() {
        sensors[0] = Sensor(A0);  // Left sensor
        sensors[1] = Sensor(A1);  // Center sensor
        sensors[2] = Sensor(A2);  // Right sensor
    }


    // Method to calculate the error based on sensor readings
    float calculate_error() {
        int weighted_sum = 0;
        int total_reading = 0;

        // Example: Compute weighted sum of sensor readings
        // Assuming sensor 0 is the leftmost sensor, sensor 1 is the center, and sensor 2 is the rightmost
        for (size_t i = 0; i < 3; ++i) {
            int sensor_value = sensors[i].read_value();
            weighted_sum += (sensor_value * (i - 1)); // Weighted by position (left = -1, center = 0, right = 1)
            total_reading += sensor_value;
        }

        // Calculate error: If all sensors are on the line, error should be zero
        if (total_reading == 0) {
            return 0;  // In case no sensor detects the line
        }

        return static_cast<float>(weighted_sum) / total_reading; // Normalize by total reading
    }

    // Method to collect readings from all sensors
    void get_sensor_data() {
        for (size_t i = 0; i < 3; ++i) {
            printf("Sensor %d Value: %d\n", i, sensors[i].read_value());
        }
    }

    // Method to calibrate the sensors (adjust thresholds)
    void calibrate() {
        for (size_t i = 0; i < 3; ++i) {
            int calibration_value = sensors[i].read_value(); // Read value from sensor to set threshold
            sensors[i].set_threshold(calibration_value);
        }
    }
};

int main() {
    // Create a SensorRig object
    SensorRig sensor_rig;  // Fixed: no parentheses needed

    while (true) {
        // Calculate error based on the sensor readings
        float error = sensor_rig.calculate_error();
        printf("Line Error: %.3f\n", error);

        // Get sensor data (useful for diagnostics)
        sensor_rig.get_sensor_data();

        // Calibrate sensors periodically
        sensor_rig.calibrate();

        // Wait for 1 second
        wait(1);  // Use the correct wait function
    }
}
