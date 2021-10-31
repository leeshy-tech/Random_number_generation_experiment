/*!
	*
	* @file     gaussion_distribution.c
	*
	* @brief    信息网络建模与仿真课程 实验一 内容3 4
	*           用中心极限定理方法和Box-Muller算法分别产生标准高斯分布随机数
	*			用组合法生成混合高斯分布变量
	*			存储在data.txt中，配合matlab程序进行数据分析。
	* 
	* @company  BUPT
	*
	* @author   李赛  saili@bupt.edu.cn
	*
	* @note     1> 三种算法的思路写在函数注释中
	*			
	* @version  final  2021/10/31
	*
	* @date     2021/10/28 星期四
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\mt19937ar.sep\mt19937ar.h"

#define pi acos(-1.0)
#define data_file "..\\data\\gaussion_distribution.txt"

/*
功能：用中心极限定理产生标准高斯分布随机数
思路：X = B*sum(1->N)(Ui-1/2)近似服从标准高斯分布，Ui为(0,1)均匀分布随机数
*/
double central_limit_theorem(void){
    int N = 12;
    double B = pow(12.0/N,0.5),rand_standard_gauss = 0;
    for (int i=1;i<=N;i++){
        rand_standard_gauss += genrand_real3() - 0.5;
    }
    rand_standard_gauss *= B;
    return rand_standard_gauss;
}

/*
功能：用Box-Muller算法产生标准高斯分布随机数
思路：生成两个01均匀分布随机变量x1,x2，则高斯随机分布变量为：
	  X = (-2*lnx1)^0.5*cos(2*pi*x2)
	  Y = (-2*lnx1)^0.5*sin(2*pi*x2)  使用一个即可。
*/
double Box_Muller_algorithm(void){
   double rand01[2] = {genrand_real3(),genrand_real3()};
   double rand_standard_gauss = pow(-2*log(rand01[0]),0.5) * cos(2*pi*(rand01[1]));
   return rand_standard_gauss;
}

/*
功能：用组合法生成混合高斯分布变量
思路：混合高斯分布f(x) = sum(1->3)pi*N(ai,bi^2)
	1> 生成标准高斯分布N(0,1),常规高斯分布Ni = N(ai,bi^2) = N(0,1)*bi + ai
	2> 生成(0,1)均匀分布picki,若sum(1->i-1)pi <= picki < sum(1->i)pi,则取样Ni
	3> 对所有取样的事件pi*Ni求和
*/
double combinaion_method(void){
	double p[3] = {1/2.0 , 1/3.0 , 1/6.0},a[3] = {-1.0 , 0 , 1.0},b[3] = {1/4.0 , 1.0 , 1/2.0},pick[3] = {0};
	double rand_gauss[3] = {0},rand_mixed_gauss = 0;

	for(int i=0;i<3;i++){
		rand_gauss[i] = Box_Muller_algorithm() * b[i] + a[i];	//1
		if(genrand_real3()<p[0]) pick[0] = 1;					//2
		else if(genrand_real3()<p[0]+p[1]) pick[1] = 1;
		else pick[2] = 1;
	}

	for(int j=0;j<3;j++){
		rand_mixed_gauss += pick[j] * rand_gauss[j];			//3
	}
	return rand_mixed_gauss;
}

int main(){
	//从用户获得生成随机数的数量
	int number = 0;
    printf("input the number of random numbers: ");
    scanf("%d",&number);
    getchar();

	char method = 'a';
    printf(
        "input a letter to choose the method to produce gaussion distribution random numbers:\n"
        "a:Central limit theorem\n" 
        "b:Box Muller algorithm\n" 
        "else:Combinaion method"
    );
    scanf("%c",&method);

    char *Method = "";
    if (method == 'a')   Method = "Central limit theorem";
	else if(method == 'b')  Method = "Box Muller algorithm";
	else  Method = "Combinaion method";
	
	FILE *fp;//以write模式打开文件时会先清空文件，文件不存在的话会先创造文件，不需要人工操作data.txt
    fp = fopen(data_file,"w");
    if (fp == NULL) printf("fail to open the data.text,program interrupted.");
    else {
		for (int i=1;i<=number;i++){
			double rand_gauss;
			if (method == 'a')  rand_gauss = central_limit_theorem();
            else if(method == 'b') rand_gauss = Box_Muller_algorithm();
            else rand_gauss = combinaion_method();
			
			//将double类型转化为字符串，因为文件不能直接写入double类型
			char string_rand_gauss[50] = {0};
			gcvt(rand_gauss,16,string_rand_gauss);

			//将随机数存储在文件中，每行一个
			fputs(string_rand_gauss,fp);
			fputs("\n",fp);
		}
		//结束语
        printf(
            "Gaussion distribution random numbers generated using the %s are stored in pareto_distribution.txt\n"
			"You can use matlab program for statistical analysis of data",
			Method
        );
	}
}