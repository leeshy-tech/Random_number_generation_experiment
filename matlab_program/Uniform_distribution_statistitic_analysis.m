%对01均匀分布数据进行统计分析
%读取数据
file = fopen("..\data\uniform_distribution.txt");
data = textscan(file,'%f');
random_numbers = data{1};
%方差、均值（实际、理论）
mean_value = mean(random_numbers);
variance = var(random_numbers);
disp(["01均匀分布数据的均值和方差是：",mean_value,variance]);
disp(["理论值是：",1/2,1/12]);
%概率密度、分布函数（实际、理论）
x = linspace(-1,2,10000); 
estimated_f = ksdensity(random_numbers,x,'function','pdf');
estimated_F = ksdensity(random_numbers,x,'function','cdf');
theoretical_f = x./x;
theoretical_F = x;
%修正理论值
for i=1:length(x)
    if x(i) <= 0
        theoretical_f(i)=0;
        theoretical_F(i)=0;
    elseif x(i) >= 1
        theoretical_f(i)=0;
        theoretical_F(i)=1;
    end
end
%绘图
subplot(311);
plot(x,estimated_f);hold on;plot(x,theoretical_f);
title('概率密度函数');xlabel('x');ylabel('f(x)');
legend('估算曲线','理论曲线');

subplot(312);
plot(x,estimated_F);hold on;plot(x,theoretical_F);
title('分布函数');xlabel('x');ylabel('F(x)');
legend('估算曲线','理论曲线');

subplot(313);
xbins = 50;
histogram(random_numbers,xbins);
title('频率分布直方图');xlabel('x');ylabel('F(x)');