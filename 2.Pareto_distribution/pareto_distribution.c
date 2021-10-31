/*!
	*
	* @file     pareto_distribution.c
	*
	* @brief    信息网络建模与仿真课程 实验一 内容2
	*           利用反变换法求Pareto分布随机数，存储在data.txt中
	*           配合matlab程序进行数据分析。
	* 
	* @company  BUPT
	*
	* @author   李赛  saili@bupt.edu.cn
	*
	* @note     1> 思路：首先利用MT算法产生[0,1]随机数，带入Pareto累积分布函数的反函数即为满足Pareto分布的随机数
	*			2> Pareto分布：
	*						概率密度函数 f(x) = ( a*b^a )/( x^(a+1) ) [x>=b]
	*						累积分布函数 F(x) = 1 - (x/b)^(-a)        [x>=b]
	*						其反函数为      x = b / ( (1-y)^(1/a) )   [y<=1]	
	*			3> 利用matlab程序做数据分析时要注意Pareto分布参数a，b一致
	*
	* @version  final  2021/10/31
	*
	* @date     2021/10/28 星期四
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../mt19937ar.sep/mt19937ar.h"

#define data_file "..\\data\\pareto_distribution.txt"

int main(){
	double a = 3.0,b = 5.0;//Pareto分布参数
	double rand01,rand_Pareto;
	//从用户获得生成随机数的数量
	int number = 0;
    printf("input the number of random numbers: ");
    scanf("%d",&number);
    getchar();

	FILE *fp;//以write模式打开文件时会先清空文件，文件不存在的话会先创造文件，不需要人工操作data.txt
    fp = fopen(data_file,"w");
    
    if (fp == NULL) printf("fail to open the data.text,program interrupted.");
    else {
		for (int i=1;i<=number;i++){
			rand01 = genrand_real3();
			rand_Pareto = b / pow(1-rand01,1/a);

			//将double类型转化为字符串，因为文件不能直接写入double类型
			char string_rand_Pareto[50] = {0};
			gcvt(rand_Pareto,16,string_rand_Pareto);

			//将随机数存储在文件中，每行一个
			fputs(string_rand_Pareto,fp);
			fputs("\n",fp);
		}
		//结束语
        printf(
            "Pareto distribution Random numbers generated using the transformation method are stored in pareto_distribution.txt\n"
			"you can use matlab program for statistical analysis of data"
        );
	}
	return 0;
}