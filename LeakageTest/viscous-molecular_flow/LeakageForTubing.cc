//这个脚本用于计算波纹管的允许漏率
//#define TotalTime 100000
constexpr int TotalTime = 100000;//计算的总时间
constexpr double miuSF6 = 1.53;//SF6的粘度系数，单位1E-5 Pa*s
constexpr double miuAir = 1.81;//空气的粘度系数，单位1E-5 Pa*s
constexpr double MAir = 28.96E-3;//空气的相对分子质量
constexpr double MSF6 = 146.055E-3;//SF6的相对分子质量

//粘滞-分子流的漏率
double vmLeakRate(double ph, double pl, double d, double miu, double M, double L)//ph 高压压强；pl 低压压强；d 管道直径；miu 粘滞系数；M 气体摩尔质量；L 漏孔长度
{
    double R = 8.3143;//摩尔气体常数，8.3143J/(K*mol)
    double T = 300;//气体温度，K
    double pbar = (ph+pl)/2.0;
    double DeltaP = (ph-pl); 
    double pi = TMath::Pi();
    double ConstA = sqrt(2*pi*R*T/M);
    double ConstB = sqrt(M/(R*T));
    double LeakRate = DeltaP*(pi/128.0)*d*d*d*d*pbar/(miu*L)
                        + DeltaP*(ConstA/6.0)*(d*d*d/L)*((1+ConstB*d*pbar/miu)/(1+1.24*ConstB*d*pbar/miu));
    return LeakRate;
}

void LeakageForTubing()
{
    double leakrate;
    leakrate = vmLeakRate(1.25E5,1.0E5,6.6E-7,miuSF6*1E-5,MSF6,1E-3);
    cout<<leakrate<<endl;
}