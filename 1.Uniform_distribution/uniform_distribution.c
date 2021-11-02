/*!
    *
    * @file     uniform_distribution.c
    *
    * @brief    信息网络建模与仿真课程 实验一 内容1
    *           此程序产生[0,1]均匀分布随机数并存储在data.txt中
    *           配合matlab程序进行均值、方差、概率密度分析。
    *
    * @company  BUPT
    *
    * @author   李赛  saili@bupt.edu.cn
    *
    * @note     1> 四种方法生成[0,1]均匀分布随机数，在题目要求外我加了一种方法：利用<stdlib.h>库里的rand()函数来产生随机数，
    *              梅森旋转算法是调用开源库，线性同余法和MRG32k3a算法是根据递推式自己编写的。
    *           2> 递推式的计算一般是两种方法，递归或者循环，最开始的版本是用递归写的，但是到MRG32k3a算法生成100个随机数就需要很长的时间，
    *              这显然不合适，我自创了一种方法配合for循环来计算递推式，详见程序内部注释，
    *              我将此种方法称为“滑动窗口法”，灵感来自于tcp协议，
    *              相比于开一个非常大的数组，使用这种方法可以极大的减小计算递推式时的空间复杂度。
    *           3> 此程序如果用c++写的话一些地方可以简练一下，但是写到这里已经花费很长时间并且实现功能了，故作罢。
    *           4> matlab与c语言科学计数法的格式是一样的，C语言生成的小数可以直接给matlab处理，所以不用特别设置data文件里的小数位数及格式。
    *
    * @version  final  2021/10/31
    *
    * @date     2021/10/21 星期四
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../mt19937ar.sep/mt19937ar.h"

#define ul unsigned long//用来偷懒的宏定义
#define ll long long

#define data_file "..\\data\\uniform_distribution.txt"

/*
    功能：取一个小数的小数部分
    思路：在double类型装换到int类型时会截去小数部分，也就是取整数部分，做个差即可
*/
double xiaoshu (double x){
    return (x-(int)x);
}

/*
    功能：用线性同余法生成(0,1)均匀分布随机数
    思路：利用递推式x(n) = A*x(n-1) mod M，返回x(seed)/M作为(0,1)均匀分布随机数
*/
double linear_congruential_method(long seed){
    ul A = 16807,M = 2147483647;
    ul series[2]={2,0};
    for(long i=1;i<seed;i++){
        /*
        递推式的推导思路：
        使用一个长度为2的数组储存x(n-1)，x(n)
        1.利用递推式求出x(n)，赋值给数组第二位
        2.数组整体左移一位，第一位丢弃，第二位保留
        3.循环12
        */
        series[1] = A*series[0] % M;
        series[0] = series[1];
    }
    return series[0]/(double)M;
}

/*
    功能：使用MRG32k3a算法生成[0,1]随机数
    思路：利用递推式
                x(n) = { A*x(n-2)-B*x(n-3) } mod m1
                y(n) = { C*y(n-2)-D*y(n-3) } mod m2
                求出x(n),y(n)。
        [0,1]随机数 = {|x(n)|/m1 + |y(n)|/m2} mod 1 

*/
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
            /*
            递推式的推导思路，xy思路相同，以x为例：
            使用一个长度为四的数组储存x(n-3),x(n-2),x(n-1),x(n)
            1.利用递推式求出x(n)，赋值给数组最后一位
            2.数组整体左移一位，第一位丢弃，最后一位保留
            3.循环12
            */
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


int main(){
    //从用户那里获取生成随机数的数量和方法
    int number = 0;
    printf("input the number of random numbers: ");
    scanf("%d",&number);
    getchar();

    char method = 'a';
    printf(
        "input a letter to choose the method to produce [0,1] random numbers:\n \
        a:linear congruential method\n \
        b:Mersenne Twister algorithm\n \
        c:MRG32k3a algorithm\n \
        else:C library function rand() "
    );
    scanf("%c",&method);

    char *Method = "";
    if (method == 'a')  { Method = "linear congruential method";}
    else if(method == 'b') { Method = "Mersenne Twister algorithm";}
    else if(method == 'c') { Method = "MRG32k3a algorithm";}
    else { Method = "C library function rand()";}

    //生成随机数并储存在data.txt文件中
    FILE *fp;//以write模式打开文件时会先清空文件，文件不存在的话会先创造文件，不需要人工操作data.txt
    fp = fopen(data_file,"w");
    if (fp == NULL) printf("fail to open the data.text,program interrupted.");
    else {
        for (int i=1;i<=number;i++){
            double rand_01;
            if (method == 'a')  rand_01 = linear_congruential_method(i);
            else if(method == 'b') rand_01 = genrand_real1();
            else if(method == 'c') rand_01 = MRG32k3a(i);
            else rand_01 = (double)rand()/RAND_MAX;
            
            //将double类型转化为字符串，因为文件不能直接写入double类型
            //使用c++的话稍微简化一下，因为c++有string类型
            char string_rand_01[50] = {0};
            gcvt(rand_01,16,string_rand_01);

            //将随机数存储在文件中，每行一个
            fputs(string_rand_01,fp);
            fputs("\n",fp);
        }
        //结束语
        printf(
            "Uniformly distributed random numbers generated using the %s are stored in uniform_distribution.txt,"
            "you can use matlab program for statistical analysis of data",
            Method
        );
    }
    return 0;
}