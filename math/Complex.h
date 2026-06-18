#ifndef SIGNAL_ANALYSER_COMPLEX_H
#define SIGNAL_ANALYSER_COMPLEX_H
#include <complex>
#include <list>
#include <optional>


class Complex {
public:
    // Cartesian
    struct CartCoord {
        double real = 0;
        double imag = 0;
    } cartesian;
    // Polar
    struct PolarCoord {
        double theta = 0;
        double r = 0;
    } polar;

    // Initialises the variables
    explicit Complex(const CartCoord &coordinate) {
        this->cartesian.real = coordinate.real;
        this->cartesian.imag = coordinate.imag;

        updatePolar();
    }

    explicit Complex(const PolarCoord &coordinate) {
        this->polar.r = coordinate.r;
        this->polar.theta = coordinate.theta;

        updateCart();
    }

    // Adds another complex number to this object, returns self
    Complex* add_s(const Complex* num) {
        this->cartesian.real += num->cartesian.real;
        this->cartesian.imag += num->cartesian.imag;

        updatePolar();
        return this;
    }

    // Multiples another complex number by self, returns self
    Complex* multiply_s(const Complex *num) {
        this->polar.theta = this->polar.theta + num->polar.theta;
        this->polar.r = this->polar.r * num->polar.r;

        updateCart();
        return this;
    }

    // Bulk add complex numbers and returns a new complex number
    static Complex add(const std::vector<Complex>& nums) {
        CartCoord coordinate{};
        coordinate.real = 0;
        coordinate.imag = 0;

        for (Complex num : nums) {
            coordinate.real += num.cartesian.real;
            coordinate.imag += num.cartesian.imag;
        }

        return Complex(coordinate);
    }

    static Complex multiply(const std::vector<Complex>& nums) {
        PolarCoord coordinate{};

        for (Complex num : nums) {
            if (coordinate.r == 0) {
                coordinate.r = num.polar.r;
                coordinate.theta = num.polar.theta;
                continue;
            }

            coordinate.r  = coordinate.r * num.polar.r;
            coordinate.theta = coordinate.theta + num.polar.theta;
        }

        return Complex(coordinate);
    }

    private:
    void updatePolar() {
        this->polar.r =  std::sqrt(this->cartesian.real * this->cartesian.real + this->cartesian.imag * this->cartesian.imag);
        this->polar.theta = atan2(this->cartesian.imag, this->cartesian.real);
    }

    void updateCart() {
        this->cartesian.real = this->polar.r * cos(this->polar.theta);
        this->cartesian.imag = this->polar.r * sin(this->polar.theta);
    }
};


#endif //SIGNAL_ANALYSER_COMPLEX_H