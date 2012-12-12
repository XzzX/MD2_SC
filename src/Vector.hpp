#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include <assert.h>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ostream>

// quick & dirty vector datatype.
// std::vector is more general and much slower than this.
// in general, one should not implement Vector by oneself,
// but use MTL, Blitz++, Eigen etc.
//
// examples
//
// Vector a = Vector (1, 3);
// a[0] = 4;
// a[1] = -1;
//
// Vector b = a;
// Vector c = 2*a + b;
//
// dot product:
// double dot_prod = dot (a, b);
//
// vector norm:
// double n = norm (c);
//
// output:
// std::cerr << a << "\n";

class Vector {
public:
    Vector () {
        // uninitialized
    }

    Vector (double x_, double y_, double z_) {
        d_[0] = x_;
        d_[1] = y_;
        d_[2] = z_;
    }

    Vector (double x_, double y_) {
        d_[0] = x_;
        d_[1] = y_;
        d_[2] = 0;
    }

    //
    // accessors
    //

    double &operator[] (int i) {
        assert (i== 2 || i == 1 || i == 0);
        return d_[i];
    }

    double operator[] (int i) const {
        assert (i == 2 || i == 1 || i == 0);
        return d_[i];
    }

    //
    // operators
    //

    Vector &operator+= (const Vector &rhs) {
        d_[0] += rhs[0];
        d_[1] += rhs[1];
        d_[2] += rhs[2];
        return *this;
    }

    Vector &operator-= (const Vector &rhs) {
        d_[0] -= rhs[0];
        d_[1] -= rhs[1];
        d_[2] -= rhs[2];
        return *this;
    }

    Vector &operator*= (double rhs) {
        d_[0] *= rhs;
        d_[1] *= rhs;
        d_[2] *= rhs;
        return *this;
    }

    Vector &operator/= (double rhs) {
        d_[0] /= rhs;
        d_[1] /= rhs;
        d_[2] /= rhs;
        return *this;
    }

private:
    double d_[3];
};

inline Vector operator+ (const Vector &lhs, const Vector &rhs) {
    return Vector (lhs[0]+rhs[0], lhs[1]+rhs[1] + lhs[2]+rhs[2]);
}

inline Vector operator- (const Vector &lhs, const Vector &rhs) {
    return Vector (lhs[0]-rhs[0], lhs[1]-rhs[1], lhs[2]-rhs[2]);
}

inline Vector operator* (double f, const Vector &v) {
    return Vector (f*v[0], f*v[1], f*v[2]);
}

inline Vector operator* (const Vector &v, double f) {
    return f * v;
}

inline Vector operator/ (const Vector &v, double f) {
    return Vector (v[0]/f, v[1]/f, v[2]/f);
}

inline std::ostream &operator<< (std::ostream &lhs, const Vector &rhs) {
    lhs << rhs[0] << "\t" << rhs[1] << "\t" << rhs[2];
    return lhs;
}

inline bool operator== (const Vector &lhs, const Vector &rhs) {
    return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2];
}

// standard euclidean 2-norm
inline double norm2 (const Vector &v) {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

inline double norm (const Vector &v) {
    return sqrt (norm2 (v));
}

// dot product
inline double dot (const Vector &lhs, const Vector &rhs) {
    return lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2];
}

/*inline bool vector_isnan (const Vector &x) {
    return isnan (x[0]) || isnan (x[1]) || isnan (x[2]);
}*/

#endif // VECTOR_HPP_INCLUDED
