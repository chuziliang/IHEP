#include<cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
int main()
{
    double diedai(double d,double goal);
    double func(double d, double goal);
    double diffunc(double d);
    double d=5E-7;
    double goal = 7E-8;
    d=diedai(d,goal);
    cout<<"漏率 "<<goal<<" Pa*m3/s"<<endl;
    cout<<"漏孔直径 "<<d<<" m"<<endl;
    return 0;
}
double func(double d,double goal)
{
    double funcd;
    funcd = (1341.0*0.5E10/2E-3)*d*d*d*d+(121.0*1E5/2E-3)*d*d*d*((1+189*d*0.5E5)/(1+234*d*0.5E5))-goal;
    return funcd;
}
double diffunc(double d)
{
    double diffuncd;
    diffuncd = (1341.0*0.5E10/2E-3)*4*d*d*d+(121.0*1E5/2E-3)*((3*d*d+189*0.5E5*4*d*d*d)/(1+234*d*0.5E5))
                -(121.0*1E5/2E-3)*d*d*d*189*0.5E5*((1+189*d*0.5E5)/((1+234*d*0.5E5)*(1+234*d*0.5E5)));
    return diffuncd;
}
double diedai(double d, double goal)
{
    double func(double d, double goal);
    double diffunc(double d);
    int i = 0;
    while(abs(func(d,goal))>0.01*goal)
    {
        d=d-(func(d,goal))/(diffunc(d));
        i++;
    }
    cout<<"迭代次数 "<<i<<endl;
    return d;
}