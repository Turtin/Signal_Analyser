#include <iomanip>
#include <iostream>
#include <vector>

#include "Complex.h"

class FourierTransformer {
private:
    struct WaveComponent {
        double phase;
        double amp;
        double shift;
    };

    // This function run a sample at a specific time
    // It will output the x-axis dilation of a sine curve
    // To convert the output to a frequency, divide the input t by 2pi
    static Complex fourierSample(const std::vector<Complex> &values, double t) {
        std::vector<Complex> sum;

        for (Complex value : values) {
            double point_x = value.cartesian.real;
            double point_y = value.cartesian.imag;

            Complex::PolarCoord winding;
            winding.r = point_y;
            winding.theta = point_x * t;

            sum.emplace_back(winding);
        }

        // setup to scale the output to the correct size
        Complex::CartCoord scale;
        scale.real = 2.0 / values.size();
        scale.imag = 0;
        Complex scaleFactor = Complex(scale);

        return Complex(Complex::add(sum).multiply_s(&scaleFactor)->cartesian);
    }

    static double stDev(const std::vector<std::pair<double, Complex>> &values) {
        std::cout << "Computing standard deviation" << std::endl;

        const unsigned int n = values.size();
        double mean = 0;

        for (const std::pair<double, Complex>& value : values) {
            mean += value.second.cartesian.imag;
        }

        mean /= n; // compute the mean

        double variance = 0;

        for (const std::pair<double, Complex>& value : values) {
            const double difference = (value.second.cartesian.imag - mean);
            variance += difference * difference;
        }

        double stDev = sqrt(variance / (n - 1));

        return stDev;
    }

public:
    static std::vector<WaveComponent> run(const std::vector<double> &data, double duration) {
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
        // std::cout << "[";
        // for (int i = 0; i < 500; i++) {
        //     double t = i / 10.0;
        //     std::cout << "(" << t  / (2 * std::numbers::pi)<< "," << fourierSample(values,t)->cartesian.imag << "),";
        // }
        // std::cout << "]";

        std::vector<std::pair<double, Complex>> transformedValues;

        std::cout << "Transforming data..." << std::endl;
        std::cout << "0% complete" << std::flush;
        for (int i = 0; i < 500; i++) { // temp magic numbers
            double completion = round(i / 5.0);
            std::cout << "\r" << completion << "% complete" << std::flush;

            double t = i / 10.0;
            transformedValues.emplace_back(t, fourierSample(values,t));
        }
        std::cout << std::endl << "done!" << std::endl;

        double threshHold = stDev(transformedValues);

        std::cout << "Searching for phases above the threshold amplitude: " << threshHold << std::endl;

        std::vector<WaveComponent> results;
        double last = threshHold;
        bool peakFound = false; // Prevents the program collecting phases trailing the peak

        std::cout << "Phases found: 0" << std::flush;
        for (const std::pair<double, Complex>& value : transformedValues) {
            if (value.second.cartesian.imag <= threshHold) {
                last = value.second.cartesian.imag;
                peakFound = false;
                continue;
            }

            if (last >= value.second.cartesian.imag && !peakFound) {
                peakFound = true;
                results.push_back({value.first});
                std::cout << "\r" << "Phases found: " << results.size() << std::flush;
            }

            last = value.second.cartesian.imag;
        }
        std::cout << std::endl;

        //temp
        for (WaveComponent& wave: results) {
            Complex sample = fourierSample(values, wave.phase); // Get the exact data from the graph

            wave.amp = sqrt(sample.cartesian.real * sample.cartesian.real + sample.cartesian.imag * sample.cartesian.imag);
            wave.shift = atan(sample.cartesian.real / sample.cartesian.imag);
        }
        std::cout << "done!" << std::endl;
        return results;
    }
};
