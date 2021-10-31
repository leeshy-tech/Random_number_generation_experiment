%对泊松过程数据进行统计分析
%读取数据
file = fopen("..\data\poisson_process.txt");
data=textscan(file,'%f');
event_numbers=data{1};
%概率密度（实际、理论）
lambda = 2;e = exp(1);time_max = 20;
x = linspace(1,2*lambda*time_max,2*lambda*time_max); 
estimated_f = ksdensity(event_numbers,x,'function','pdf');
theoretical_f = ((lambda*time_max).^x*e^(-lambda*time_max))./(factorial(x));
%绘图
subplot(211);
plot(x,estimated_f);hold on;plot(x,theoretical_f);
title('概率密度函数');xlabel('x');ylabel('f(x)');
legend('估算曲线','理论曲线');

subplot(212);
xbins = 50;
histogram(event_numbers,xbins);
title('频率分布直方图');xlabel('x');ylabel('频数');
