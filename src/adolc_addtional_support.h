#ifndef ADOLC_ADDTIONAL_SUPPORT_H_INCLUDED
#define ADOLC_ADDTIONAL_SUPPORT_H_INCLUDED



#include <adolc/adouble.h>
#include <Eigen/Core>
namespace Eigen {
template<> struct NumTraits<adouble>
: NumTraits<double> // permits to get the epsilon, dummy_precision, lowest, highest functions
{
typedef adouble Real;
typedef adouble NonInteger;
typedef adouble Nested;
enum {
IsComplex = 0,
IsInteger = 0,
IsSigned = 1,
RequireInitialization = 1,
ReadCost = 1,
AddCost = 3,
MulCost = 3
};
};
}

inline const adouble& conj(const adouble& x) { return x; }
inline const adouble& real(const adouble& x) { return x; }
inline adouble imag(const adouble&) { return 0.; }
inline adouble abs(const adouble& x) { return fabs(x); }
inline adouble abs2(const adouble& x) { return x*x; }




#endif // ADOLC_ADDTIONAL_SUPPORT_H_INCLUDED
