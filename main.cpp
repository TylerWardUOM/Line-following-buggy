#include "mbed.h"

class Sensor {
  private:
    PinName pin;      // The GPIO pin to which the sensor is connected
    AnalogIn sensor_analog_input; // Used for analog sensors (if the sensor output is analog)
    bool state;       // Tracks the sensor's current state (on/off)
    int threshold;    // Threshold for determining if the sensor detects the line

  public:
   // Constructor: Initializes the sensor with a pin and defaults for state and threshold
    Sensor(PinName pin, int threshold = 500)
        : pin(pin), sensor_analog_input(pin), state(false), threshold(threshold) {}

        // Method to read the value from the sensor pin
    // Returns a digital value (1 or 0) if it's a digital sensor
    // Or returns a scaled analog value (0.0 to 1.0) if it's an analog sensor
    int read_value() {
      return static_cast<int>(sensor_analog_input.read() * 1000);  // Read as an analog input and scale it to integer (0-1000)
    }

    // Method to turn the sensor on (this could involve setting the GPIO pin high or initializing it)
    void turn_on() {
        state = true;
        // Logic to turn on the sensor can go here
        // For example, setting the GPIO pin high for a digital sensor or enabling it in the case of analog
        // This is just a placeholder for real hardware initialization
    }

    // Method to turn the sensor off (this could involve setting the GPIO pin low or disabling it)
    void turn_off() {
        state = false;
        // Logic to turn off the sensor can go here
        // For example, setting the GPIO pin low or disabling the sensor
        // This is just a placeholder for real hardware deactivation
    }

    // Method to set the threshold (in case it's needed for line detection or other purposes)
    void set_threshold(int new_threshold) {
        threshold = new_threshold;
    }

    // Method to check if the sensor has detected the line based on the threshold
    bool is_line_detected() {
        int value = read_value();
        return value >= threshold;
    }

    // Getter for the state
    bool get_state() {
        return state;
    }

    // Getter for the threshold
    int get_threshold() {
        return threshold;
    }
}


// Declare an AnalogIn object for pin A0 (PA0)
AnalogIn sensor_analog_input(A0);

int main() {
    while (true) {
        // Read the value from the sensor pin (scaled to 0-1 range)
        float sensor_value = sensor_analog_input.read(); 
        
        // Print the sensor value (0.0 to 1.0 range)
        printf("Sensor Value: %.3f\n", sensor_value);
        
        wait(1);  // Wait for 1 second
    }
}