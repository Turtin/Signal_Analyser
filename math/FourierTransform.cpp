#include <iostream>
#include <vector>

#include "Complex.h"

class FourierTransformer {
private:
    // This function run a sample at a specific time
    // It will output the x-axis dilation of a sine curve
    // To convert the output to a frequency, divide the input t by 2pi
    static double fourierSample(const std::vector<Complex>& values, double t) {
        std::vector<Complex> sum;

        for (Complex value : values) {
            double point_x = value.cartesian.real.value();
            double point_y = value.cartesian.imag.value();

            Complex::PolarCoord winding;
            winding.r = point_y;
            winding.theta = point_x * t;

            sum.emplace_back(winding);
        }

        return Complex::add(sum).cartesian.imag.value() * 2 / values.size();
    }

public:

    static void run(const std::vector<double>& data, double duration) {
        // Prepare the data to be processed
        std::vector<Complex> values; // stores all the complex numbers

        double resolution =  duration / data.size(); // generate the real values

        for (int i = 0; i < data.size(); i++) { // Construct the numbers
            Complex::CartCoord coord;
            coord.real = resolution * i;
            coord.imag = data[i];

            values.emplace_back(coord); // Constructs the object inside the container basically runs faster
        }
        // temp for testing
        std::cout << "[";
        for (int i = 0; i < 500; i++) {
            double t = i / 10.0;
            std::cout << "(" << t  / (2 * std::numbers::pi)<< "," << fourierSample(values,t)<< "),";
        }
        std::cout << "]";

    }
};
