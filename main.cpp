#include "mbed.h"

class Sensor {
private:
    PinName pin;
    AnalogIn sensor_analog_input;
    bool state;       // functionality to be implemented reasearch darlington pairs
    int threshold;

public:
    Sensor() : pin(PA_0), sensor_analog_input(pin), threshold(500) {}
    Sensor(PinName pin, int threshold = 500)
        : pin(pin), sensor_analog_input(pin), threshold(threshold) {}

    int read_value() {
        return static_cast<int>(sensor_analog_input.read() * 1000);
    }

    void set_threshold(int new_threshold) {
        threshold = new_threshold;
    }

    int get_threshold() const {
        return threshold;
    }
};

class SensorRig {
private:
    Sensor sensors[3];

public:
    SensorRig() {
        sensors[0] = Sensor(A0);
        sensors[1] = Sensor(A1);
        sensors[2] = Sensor(A2);
    }

    float calculate_error() {
        int weighted_sum = 0;
        int total_reading = 0;

        for (size_t i = 0; i < 3; ++i) {
            int sensor_value = sensors[i].read_value();
            weighted_sum += (sensor_value * (i - 1));
            total_reading += sensor_value;
        }

        if (total_reading == 0) {
            return 0;
        }

        return static_cast<float>(weighted_sum) / total_reading;
    }

    void get_sensor_data(int* readings) {
        for (size_t i = 0; i < 3; ++i) {
            readings[i] = sensors[i].read_value();
        }
    }
};

int main() {
    SensorRig sensor_rig;

    Timer timer;
    const int log_duration = 10; // Log for 10 seconds
    const float sample_interval_s = 0.2; // Sample every 200 ms (in seconds)
    const int max_samples = log_duration / sample_interval_s;

    int sensor_readings[max_samples][3]; // Log sensor readings
    float error_log[max_samples];       // Log error values
    int sample_count = 0;

    timer.start();
    printf("Time (s),Sensor 0,Sensor 1,Sensor 2,Error\n");
    fflush(stdout);

    while (timer.read() < log_duration && sample_count < max_samples) {
        int readings[3];
        sensor_rig.get_sensor_data(readings);
        float error = sensor_rig.calculate_error();

        for (int i = 0; i < 3; ++i) {
            sensor_readings[sample_count][i] = readings[i];
        }
        error_log[sample_count] = error*1000;

        sample_count++;

        // Sleep for the next sample
        wait(sample_interval_s);
    }

    // Output data in CSV format for easy plotting
    for (int i = 0; i < sample_count; ++i) {
        float time = i * sample_interval_s;
        printf("%.2f,%d,%d,%d,%.3f\n", time, sensor_readings[i][0], sensor_readings[i][1], sensor_readings[i][2], error_log[i]);
        fflush(stdout);
    }

    return 0;
}
