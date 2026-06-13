#ifndef SIGNAL_ANALYSER_COMPLEX_H
#define SIGNAL_ANALYSER_COMPLEX_H
#include <complex>
#include <list>
#include <optional>


class Complex {
public:
    // Cartesian
    struct CartCoord {
        std::optional<double >real;
        std::optional<double> imag;
    } cartesian;
    // Polar
    struct PolarCoord {
        std::optional<double> theta;
        std::optional<double> r;
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
        this->cartesian.real.value() += num->cartesian.real.value();
        this->cartesian.imag.value() += num->cartesian.imag.value();

        updatePolar();
        return this;
    }

    // Multiples another complex number by self, returns self
    Complex* multiply_s(const Complex* num) {
        this->polar.theta = this->polar.theta.value() + num->polar.theta.value();
        this->polar.r = this->polar.r.value() * num->polar.r.value();

        updateCart();
        return this;
    }

    // Bulk add complex numbers and returns a new complex number
    static Complex add(const std::list<Complex>& nums) {
        CartCoord coordinate{};
        coordinate.real = 0;
        coordinate.imag = 0;

        for (Complex num : nums) {
            coordinate.real.value() += num.cartesian.real.value();
            coordinate.imag.value() += num.cartesian.imag.value();
        }

        return Complex(coordinate);
    }

    static Complex multiply(const std::list<Complex>& nums) {
        PolarCoord coordinate{};

        for (Complex num : nums) {
            if (!coordinate.r.has_value()) {
                coordinate.r = num.polar.r;
                coordinate.theta = num.polar.theta;
                continue;
            }

            coordinate.r.value()  = coordinate.r.value() * num.polar.r.value();
            coordinate.theta = coordinate.theta.value() + num.polar.theta.value();
        }

        return Complex(coordinate);
    }

    private:
    void updatePolar() {
        this->polar.r =  std::sqrt(this->cartesian.real.value() * this->cartesian.real.value() + this->cartesian.imag.value() * this->cartesian.imag.value());
        this->polar.theta = atan2(this->cartesian.imag.value(), this->cartesian.real.value());
    }

    void updateCart() {
        this->cartesian.real = this->polar.r.value() * cos(this->polar.theta.value());
        this->cartesian.imag = this->polar.r.value() * sin(this->polar.theta.value());
    }
};


#endif //SIGNAL_ANALYSER_COMPLEX_H