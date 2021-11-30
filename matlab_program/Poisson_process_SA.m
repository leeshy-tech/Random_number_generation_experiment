%对泊松分布数据进行统计分析
lambda = 2;e = exp(1);time_max = 1;experiment_numbers = 100000;
%读取数据
file = fopen("..\data\poisson_process.txt");
data=textscan(file,'%f');
event_numbers=data{1};
%均值、方差（实验值、理论值）
mean_value = mean(event_numbers);
variance = var(event_numbers);
disp(["泊松分布数据的均值和方差是：",mean_value,variance]);
disp(["理论值是：",lambda,lambda]);
%理论分布律
x = linspace(0,10,11); 
theoretical_f = (lambda.^x*e^(-lambda))./factorial(x) * experiment_numbers;
%绘图
subplot(211);
cdfplot(event_numbers);
title('累积分布估算');xlabel('x');ylabel('F(x)');

subplot(212);
xbins = 100;histogram(event_numbers,xbins);
title('频率分布直方图');xlabel('x');ylabel('频数');
hold on;plot(x,theoretical_f,'k*');legend('实验值','理论值');