/*!
	*
	* @file     poisson_process.c
	*
	* @brief    信息网络建模与仿真课程 实验一 内容5
	*           产生一个参数λ等于2的泊松过程，存储事件发生的时间，统计单位时间内事件发生的次数，判断其与泊松分布是否吻合。
	*			 
	* @company  BUPT
	*
	* @author   李赛  saili@bupt.edu.cn
	*
	* @note     1> 泊松过程：P{N(t+s)-N(s)=k} = (lambda*t)^k * e^(-lambda*t) / (k!)
	*			2> 泊松分布：P{X=k} = lambda^k * e^(-lambda) / (k!)
	*			3> 泊松分布相邻两次事件发生的概率服从指数分布，即 P{N(t+s)-N(s)=0} = e^(-lambda*t)
	*			4> 基于(0,1)随机数，使用反变换法模拟相邻事件发生的时间间隔，计数累加即可获取某确定时间段发生事件的时间点序列。
	*			5> 进行多组实验，储存事件发生次数，导入matlab验证是否服从泊松分布，注意参数lambda，time_max要相同。
	*			
	* @version  final  2021/11/30
	*
	* @date     2021/10/29 星期五
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\mt19937ar.sep\mt19937ar.h"

#define data_file "..\\data\\poisson_process.txt"

/*
    功能：模拟一次泊松过程，返回单位时间内事件发生的次数
    思路：1> 生成01均匀分布随机数
		2> 利用反变换法生成指数分布随机数
		3> 以指数分布随机数为时间间隔，累加直到时间结束
*/
int poisson(){
	int n = 0, average_n;
	double t = 0;
	double lambda = 2.0,time_max = 1.0;

	while(t<time_max){
		double rand01 = genrand_real3();
		t += -1/lambda*log(rand01);
		n++;
	}
	average_n = floor((n-1)/time_max);

	return average_n;
}

int main(){
	int experiment_numbers = 100000;

    FILE *fp;//以write模式打开文件时会先清空文件
    fp = fopen(data_file,"w");
    for(int i=0;i<experiment_numbers;i++){
		//将double类型转化为字符串，因为文件不能直接写入double类型
		char string_n[50] = {0};
		gcvt(poisson(),16,string_n);

		//将次数存储在文件中，每行一个
		fputs(string_n,fp);
		fputs("\n",fp);
	}
	return 0;
}
