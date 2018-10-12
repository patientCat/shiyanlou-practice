//c++ program
//design by >^.^<
//@date    2018-10-11 15:14:29
#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

#include <cstdlib> 
#include <cmath> 

using RandomType = enum { UNIFORM, EXPONENTIAL, POISSON };

class Random 
{ 
public: 
    Random (); 
    ~Random (); 

    // 给定分布类型和参数，获得随机值
    static double getRandom(RandomType type, double parameter)
    {
        switch(type)
        {
            case UNIFORM:
                return uniform(parameter);
                break;
            case EXPONENTIAL:
                return exponential(parameter);
                break;
            case POISSON:
                return poisson(parameter);
                break;
            default:
                return 0;
                break;
        }
    }
            
    // [0, 1) 之间的服从均匀分布的随机值
    static double uniform(double max = 1)
    {
        return ((double)std::rand() / (RAND_MAX)) * max;
    } 

    // 服从 lambda-指数分布的随机值
    // 这里采用逆分布函数的方法求出服从某一分布的随机数
    static double exponential(double lambda)
    {
        return -log(1 - uniform()) / lambda;
    }
     
    // 服从 lambda-泊松分布的随机值
    static double poisson(double lambda)
    {
        int t = 0;
        double p = exp(-lambda);
        double f = p;
        double u = uniform();
        while(true)
        {
            if(f > u)
                break;
            t++;
            p = p * lambda / t;
            f += p;
        }
        return t;
    }
private: 
}; 

#endif /* RANDOM_HPP */
