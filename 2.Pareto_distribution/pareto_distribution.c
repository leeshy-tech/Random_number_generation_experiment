/*!
	*
	* @file     pareto_distribution.c
	*
	* @brief    ��Ϣ���罨ģ�����γ� ʵ��һ ����2
	*           ���÷��任����Pareto�ֲ���������洢��data.txt��
	*           ���matlab����������ݷ�����
	* 
	* @company  BUPT
	*
	* @author   ����  saili@bupt.edu.cn
	*
	* @note     1> ˼·����������MT�㷨����[0,1]�����������Pareto�ۻ��ֲ������ķ�������Ϊ����Pareto�ֲ��������
	*			2> Pareto�ֲ���
	*						�����ܶȺ��� f(x) = ( a*b^a )/( x^(a+1) ) [x>=b]
	*						�ۻ��ֲ����� F(x) = 1 - (x/b)^(-a)        [x>=b]
	*						�䷴����Ϊ      x = b / ( (1-y)^(1/a) )   [y<=1]	
	*			3> ����matlab���������ݷ���ʱҪע��Pareto�ֲ�����a��bһ��
	*
	* @version  final  2021/10/31
	*
	* @date     2021/10/28 ������
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../mt19937ar.sep/mt19937ar.h"

#define data_file "..\\data\\pareto_distribution.txt"

int main(){
	double a = 3.0,b = 5.0;//Pareto�ֲ�����
	double rand01,rand_Pareto;
	//���û�������������������
	int number = 0;
    printf("input the number of random numbers: ");
    scanf("%d",&number);
    getchar();

	FILE *fp;//��writeģʽ���ļ�ʱ��������ļ����ļ������ڵĻ����ȴ����ļ�������Ҫ�˹�����data.txt
    fp = fopen(data_file,"w");
    
    if (fp == NULL) printf("fail to open the data.text,program interrupted.");
    else {
		for (int i=1;i<=number;i++){
			rand01 = genrand_real3();
			rand_Pareto = b / pow(1-rand01,1/a);

			//��double����ת��Ϊ�ַ�������Ϊ�ļ�����ֱ��д��double����
			char string_rand_Pareto[50] = {0};
			gcvt(rand_Pareto,16,string_rand_Pareto);

			//��������洢���ļ��У�ÿ��һ��
			fputs(string_rand_Pareto,fp);
			fputs("\n",fp);
		}
		//������
        printf(
            "Pareto distribution Random numbers generated using the transformation method are stored in pareto_distribution.txt\n"
			"you can use matlab program for statistical analysis of data"
        );
	}
	return 0;
}