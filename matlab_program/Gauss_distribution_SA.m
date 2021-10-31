%对高斯分布数据进行统计分析
%读取数据
file = fopen("..\data\gaussion_distribution.txt");
data=textscan(file,'%f');
random_numbers=data{1};
mean_value = mean(random_numbers);
variance = var(random_numbers);
disp(["高斯分布数据的均值和方差是：",mean_value,variance]);

%绘图
subplot(311);
[f1,x1] = ksdensity(random_numbers,'function','pdf');
plot(x1,f1);
title('概率密度函数');xlabel('x');ylabel('f(x)');

subplot(312);
[f2,x2] = ksdensity(random_numbers,'function','cdf');
plot(x2,f2);
title('累积分布函数');xlabel('x');ylabel('F(x)');

subplot(313);
xbins = 50;
histogram(random_numbers,xbins);
title('频率分布直方图');xlabel('x');ylabel('F(x)');