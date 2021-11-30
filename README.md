
# 随机变量与随机过程产生实验
## 项目概述
项目来源于BUPT课程“信息网络建模与仿真”实验一  
本项目不仅要重视随机数产生算法的思路，还有数据操作的细节，比如数据类型的选择，不注意就会发生计算溢出。  
项目结构：  
```
└─ Random_number_generation_experiment
    ├─ .vscode                  //vscode编辑器的配置文件夹
    ├─ 1.Uniform_distribution
    ├─ 2.Pareto_distribution
    ├─ 3.4.Gaussion_distribution
    ├─ 5.Poisson_process        //实验内容1-5对应的C语言代码文件
    ├─ data                     //C语言代码生成的随机数数据文件
    ├─ matlab_program           //对应数据文件的matlab处理程序
    └─ mt19937ar.sep            //老师给的代码，梅森旋转算法的C语言实现
```

## 项目详解
### 1 均匀分布
基础知识：  
[a,b]上的均匀分布，均值为(b+a)/2，方差为(b-a)^2 / 12。  
概率密度函数为
$$
f(x)=\left\{\begin{array}{c}
\frac{1}{b-a}, a<x<b \\
0, \text { else }
\end{array}\right.
$$
累积分布函数为：  
$$
F(x)=\left\{\begin{array}{c}
0, x<a \\
\frac{x-a}{b-a}, a \leq x \leq b \\
1, x>b
\end{array}\right.
$$

实验内容：  
（1）用线性同余法生成(0, 1)区间上的随机数，计算其期望、方差和概率密度与理论值的吻合程度，递推式：x(i+1) = (16807*x(i)) mod (2147483647)  
思路：用滑动窗口法计算该递推式的第n项x(n)，生成的(0,1)均匀分布随机数即为 x(n) / 2147483647
```c
double linear_congruential_method(long seed){
    unsigned long A = 16807,M = 2147483647;
    unsigned long series[2]={2,0};
    for(long i=1;i<seed;i++){
        series[1] = A*series[0] % M;
        series[0] = series[1];
    }
    return series[0]/(double)M;
}
```
（2）分析Mersenne Twister产生均匀分布随机数的代码，并用该代码产生一组(0,1)区间上的随机数，计算其期望、方差和概率密度与理论值的吻合程度。  
思路：引用头文件mt19937ar.h，调用对应的函数即可
```c
/* generates a random number on [0,1]-real-interval */
double genrand_real1(void);

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void);

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void);

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void);
```
（3）编写程序，实现NS-3中采用的MRG32k3a算法生成随机数，并计算其期望、方差和概率密度与理论值的吻合程度。  
思路：  
计算递推式   
x(n) = { A*x(n-2)-B*x(n-3) } mod m1  
y(n) = { C*y(n-2)-D*y(n-3) } mod m2  
输出随机数为：  
$$
Un=\left\{\begin{array}{c}
\frac{xn-yn+m1}{m1+1}, xn \leq yn \\
\frac{xn-yn}{m1+1}, xn>yn
\end{array}\right.
$$
```c
double MRG32k3a(long seed){
    ll A = 1403580,B = 810728,C = 527612,D = 1370589;
    ll m1 = 4294967087,m2 = 4294944443;
    ll X[4] = {1,1,1,0},Y[4] = {1,1,1,0};
    ll xn,yn;
    if (seed<=3) {
        xn = X[seed-1];yn = Y[seed-1];
    }
    else{
        for (long i = 1;i<=seed-3;i++){
            X[3] = ( A*X[1] - B*X[0] ) % m1;
            X[0] = X[1];X[1] = X[2];X[2] = X[3];

            Y[3] = ( C*Y[2] - D*Y[0] ) % m2;
            Y[0] = Y[1];Y[1] = Y[2];Y[2] = Y[3];
        }
        xn = X[2];yn = Y[2];
    }

    double rand01;
    rand01 = xiaoshu(llabs(xn)/(double)m1+llabs(yn)/(double)m2);
    //rand01 = ((xn>yn)  ?  (xn-yn+m1)/((double)m1+1.0)  :  (yn-xn)/((double)m1+1.0));
    return rand01;
}
```
但是根据这个算式得出的数据经matlab分析与预期相差的很远，所以我更改了随机数的算式，得出了一些比较好的数据，更改的算式是：  
$$
\left\{\frac{X(n)}{m1}+\frac{Y(n)}{m2}\right\} \bmod 1
$$
### 2 Pareto分布
基础知识：  
Pareto分布的均值是 b * a / (a-1),方差是 b^2 * a / (a-1)^2 / (a-2)  
概率密度函数是：  
$$
f(x)=\left\{\begin{array}{c}
\frac{a b^{a}}{x^{a+1}}, x \geq b \\
0, x<b
\end{array}\right.
$$
累积分布函数是：  
$$
F(x)=\left\{\begin{array}{c}
\left(\frac{x}{b}\right)^{-a}, x \geq b \\
1, x<b
\end{array}\right.
$$
实验内容：  
以Mersenne Twister随机数产生器为基础产生服从Pareto分布的随机变量，数据导入到Matlab中画出其概率密度函数分布图和累积分布图。（自行尝试a和b的不同取值）  
思路：  
使用反变换法，将(0,1)均匀分布随机数带入Pareto累积分布函数的反函数，即可得到符合Pareto分布的随机变量数据。其反函数为：  
$$
x=\frac{b}{(1-y)^{\frac{1}{a}}}, y \leq 1
$$
```c
rand01 = genrand_real3();
rand_Pareto = b / pow(1-rand01,1/a);
```
### 3 4 高斯分布
基础知识：  
均值为a，方差为b^2的高斯分布：  
$$
f(x)=\frac{1}{\sqrt{2 \pi} b} \exp \left(-\frac{(x-a)^{2}}{2 b^{2}}\right)
$$
标准高斯分布与高斯分布的关系：N(a,b^2) = N(0,1)*b + a
### 5 泊松过程
## 知识点碎碎念
1> C语言没有string类型，以后能用c++就用c++  
2> C语言中 "/" 运算符的两个操作数均是整数时，结果也是整数，即做除法取整，要想获得小数结果，需要把其中一个操作数转换为float或double。  
3> cmd命令 [tree命令生成目录树](https://blog.csdn.net/qq_42623156/article/details/110184553)  
4> vscode在编译c文件时只会编译该文件夹里的文件，所以如果跨文件夹引用mt19937ar.h，需要在配置文件tasks.json加入一行,目的是额外编译mt19937ar.c
```
"${workspaceRoot}\\mt19937ar.sep\\mt19937ar.c", 
```  
5> 相对路径：  ./ 表示该文件夹../ 表示上一级文件夹  
6> 滑动窗口法计算递推式  
虽然我很想说这是我由tcp协议得到的灵感自己想出来的，但是我google了一下居然真的有这种方法，现加以总结：  
> 递归式的计算一般有两种方法，递归和循环  
递归的时间复杂度很高，在随机数生成这种场景就不要想了。  
最初级的循环法求递推式怎样实现呢？  
开一个大数组，循环计算递推式，算一个存一个。但是这样的空间复杂度太高了。

则引出了滑动窗口法：  
基于开大数组法，想象每算出来一个数，就丢掉前面一个数，我们只需要维护一个长度为w的数组即可，w的大小满足计算下一个递推项即可。以均匀分布用到的MRG32k3a算法为例：
> 我们需要计算 x(n) = { A*x(n-2)-B*x(n-3) } mod m1  
> 维护一个长度为4的数组，存储：
> x(n-3) x(n-2) x(n-1) x(n)  
> 初始化之后，进行以下循环：  
1.计算x(n)，存入数组最后一位  
2.将整个数组向左移动一位，丢弃x(n-3)  

通过这种循环我们就能一直计算递推式，大大减小了数组造成的空间开支