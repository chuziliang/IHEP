import ROOT
import numpy as np

# 生成示例数据
np.random.seed(42)
data = np.random.normal(5, 2, 10000)

# 创建RooFit数据集（避免分bin）
ROOT.gSystem.Load("libRooFit")
from ROOT import RooRealVar, RooDataSet, RooGaussian, RooFit, RooArgSet

# 定义变量和数据集
x = RooRealVar("x", "x", 0, 10)
data_unbinned = RooDataSet("data", "Unbinned Data", RooArgSet(x))

# 填充未分bin数据
for value in data:
    x.setVal(value)
    data_unbinned.add(RooArgSet(x))

# 定义模型
mean = RooRealVar("mean", "mean", 5, 0, 10)
sigma = RooRealVar("sigma", "sigma", 1, 0.1, 5)
gauss = RooGaussian("gauss", "gaussian", x, mean, sigma)

# 执行无分bin拟合
result = gauss.fitTo(data_unbinned, RooFit.Save())
result.Print()

# 获取参数和误差
print(f"Mean: {mean.getVal()} ± {mean.getError()}")
print(f"Sigma: {sigma.getVal()} ± {sigma.getError()}")