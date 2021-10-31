/*!
	*
	* @file     poisson_process.c
	*
	* @brief    ��Ϣ���罨ģ�����γ� ʵ��һ ����5
	*           ����һ�������˵���2�Ĳ��ɹ��̣��洢�¼�������ʱ�䣬ͳ�Ƶ�λʱ�����¼������Ĵ������ж����벴�ɷֲ��Ƿ��Ǻϡ�
	*			 
	* @company  BUPT
	*
	* @author   ����  saili@bupt.edu.cn
	*
	* @note     1> ���ɹ��̣�P{N(t+s)-N(s)=k} = (lambda*t)^k * e^(-lambda*t) / (k!)
	*			2> ���ɷֲ���P{X=k} = lambda^k * e^(-lambda) / (k!)
	*			3> ���ɷֲ����������¼������ĸ��ʷ���ָ���ֲ����� P{N(t+s)-N(s)=0} = e^(-lambda*t)
	*			4> ����(0,1)�������ʹ�÷��任��ģ�������¼�������ʱ�����������ۼӼ��ɻ�ȡĳȷ��ʱ��η����¼���ʱ������С�
	*			5> ���ж���ʵ�飬�����¼���������������matlab��֤�Ƿ���Ӳ��ɷֲ���ע�����lambda��time_maxҪ��ͬ��
	*			
	* @version  final  2021/10/31
	*
	* @date     2021/10/29 ������
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\mt19937ar.sep\mt19937ar.h"

#define data_file "..\\data\\poisson_process.txt"

int main(){
	double lambda = 2.0;
	int time_max = 20,number = 0;
    printf("input the numbers of poisson experiment: ");
    scanf("%d",&number);
    getchar();

    FILE *fp;//��writeģʽ���ļ�ʱ��������ļ�
    fp = fopen(data_file,"w");
    for(int i=0;i<number;i++){
		int n = 0;
		double t = 0;
		double time[2] = {0};
		while(t<time_max){
			double rand01 = genrand_real3();
			if(n==1) time[1] = -1/lambda*log(rand01) ;//ʹ�û������ڷ�
			else time[1] = time[0]-1/lambda*log(rand01);
			time[0] = time[1];
			t = time[0];
			n++;
		}

		//��double����ת��Ϊ�ַ�������Ϊ�ļ�����ֱ��д��double����
		char string_n[50] = {0};
		gcvt((double)(n-2),16,string_n);

		//�������洢���ļ��У�ÿ��һ��
		fputs(string_n,fp);
		fputs("\n",fp);
	}
	return 0;
}
