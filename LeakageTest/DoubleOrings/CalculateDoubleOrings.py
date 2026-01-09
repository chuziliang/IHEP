import numpy as np

def calculate_PM(t, A1, A2, PH, PL, V, PM0):
    # 计算分子 A1 * PH^2 + A2 * PL^2
    numerator = A1 * PH**2 + A2 * PL**2
    # 计算分母 A1 + A2
    denominator = A1 + A2
    # 计算根号内的值
    sqrt_term1 = np.sqrt(numerator / denominator)
    # 计算双曲余切函数的参数
    coth_arg = (np.sqrt(numerator * denominator) / (2 * V)) * t
    # 计算双曲余切函数值
    coth_value =  np.tanh(coth_arg)
    # 计算最终结果
    PM_t = sqrt_term1 * coth_value + PM0
    return PM_t

def calculate_LeakRate(A2,PL,PM):
    LeakRate = 0.5*A2*(PM**2 - PL**2)
    return LeakRate

# 示例参数值
t = 49000
A1 = 3.6e-17
A2 = 3.6e-17
PH = 1.25e5
PL = 1e5
V = 2e-7
PM0 = 1e5

# 调用函数计算 PM(t)
PM = calculate_PM(t, A1, A2, PH, PL, V, PM0)
print(f"P_M(t) 的值为: {PM}")

# 调用函数计算 LeakRate
LeakRate = calculate_LeakRate(A2, PL, PM)
print(f"LeakRate 的值为: {LeakRate}")