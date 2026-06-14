#include <iostream>
#include <vector>

#include "Complex.h"

class FourierTransformer {
private:
    static double testFrequency(const std::vector<Complex>& values, double frequency) {
        std::vector<Complex> sum;

        for (Complex value : values) {
            double point_x = value.cartesian.real.value();
            double point_y = value.cartesian.imag.value();

            Complex::PolarCoord winding;
            winding.r = point_y;
            winding.theta = point_x * frequency;

            sum.emplace_back(winding);
        }

        return Complex::add(sum).cartesian.imag.value() * 2 / values.size();
    }

public:

    static void fourier(const std::vector<double>& data, double duration, double frameRate) {
        // Prepare the data to be processed
        std::vector<Complex> values; // stores all the complex numbers

        double resolution =  duration / data.size(); // generate the real values

        for (int i = 0; i < data.size(); i++) { // Construct the numbers
            Complex::CartCoord coord;
            coord.real = resolution * i;
            coord.imag = data[i];

            values.emplace_back(coord); // Constructs the object inside the container basically runs faster
        }

        std::cout << "[";
        for (int i = 0; i < 500; i++) {
            double t = i / 20.0 * std::numbers::pi;
            std::cout << "(" << t << "," << testFrequency(values,t) << "),";
        }
        std::cout << "]";

    }
};
