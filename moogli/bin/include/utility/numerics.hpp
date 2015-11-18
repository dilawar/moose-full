#ifndef __NUMERICS_HPP__
#define __NUMERICS_HPP__

template<typename T>
T constrain(T left, T value, T right)
{
    if(value < left) return left;
    if(value > right) return right;
    return value;
}

#endif /* __NUMERICS_HPP__ */
