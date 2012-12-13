#ifndef D_COMPLEX_H_INCLUDED
#define D_COMPLEX_H_INCLUDED
#include <complex>
#include <sstream>
#include <cmath>

template<class T>
class D_Complex:public complex<T>//(const T& re, const T& im)
{
public:
    explicit D_Complex(const T& re, const T& im);
    //explicit D_Complex(const T& x):complex<T>(x,x){};
    D_Complex():complex<T>(0,0){}
    template<class S> friend ostream& operator<< (ostream& in_cout,const D_Complex<S>& in_complex);
    T ati() const;
    T angl() const;
    D_Complex<T>& operator=(const complex<T>& x);
    explicit D_Complex(const complex<T>& x):complex<T>(x){}

private:
    static const T PI;//=3.14159265354;
};
template<class T>
T const D_Complex<T>::PI=3.14159265354;
template<class T>
D_Complex<T>::D_Complex(const T& re, const T& im):complex<T>(re,im)//,PI(3.14159265354)
{

}

template<class T>
ostream& operator<<(ostream& in_cout,const D_Complex<T>& in_complex)
{
    stringstream t_strstream;
    string buff_str;
    t_strstream<<in_complex.ati()<<"'"<<in_complex.angl();
    t_strstream>>buff_str;
    in_cout<<buff_str;
    return in_cout;
}

template<class T>
T D_Complex<T>::ati() const
{
    T t;
    T re=complex<T>::real();
    T im=complex<T>::imag();
    t=sqrt(re*re+im*im);
    return t;
}
template<class T>
T D_Complex<T>::angl() const
{
    T re=complex<T>::real();
    T im=complex<T>::imag();

    if(re<0 && im >=0 )
    {
        return PI+atan(im/re) ;
    }

    if(re<0 && im<=0)
    {
        return -PI+atan(im/re) ;
    }
    if(re==0 && im>0) return PI/2;
    if(re==0 && im<0) return -PI/2;
    if(re==0 && im==0) return 0;

    return atan(im/re);
}

template<class T>
D_Complex<T>& D_Complex<T>::operator=(const complex<T>& x)
{
    complex<T>::real(x.real());
    complex<T>::imag(x.imag());
    return *this;
}


#endif // D_COMPLEX_H_INCLUDED
