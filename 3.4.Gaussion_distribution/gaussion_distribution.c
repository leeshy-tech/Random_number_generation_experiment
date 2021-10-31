/*!
	*
	* @file     gaussion_distribution.c
	*
	* @brief    ��Ϣ���罨ģ�����γ� ʵ��һ ����3 4
	*           �����ļ��޶�������Box-Muller�㷨�ֱ������׼��˹�ֲ������
	*			����Ϸ����ɻ�ϸ�˹�ֲ�����
	*			�洢��data.txt�У����matlab����������ݷ�����
	* 
	* @company  BUPT
	*
	* @author   ����  saili@bupt.edu.cn
	*
	* @note     1> �����㷨��˼·д�ں���ע����
	*			
	* @version  final  2021/10/31
	*
	* @date     2021/10/28 ������
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "..\mt19937ar.sep\mt19937ar.h"

#define pi acos(-1.0)
#define data_file "..\\data\\gaussion_distribution.txt"

/*
���ܣ������ļ��޶��������׼��˹�ֲ������
˼·��X = B*sum(1->N)(Ui-1/2)���Ʒ��ӱ�׼��˹�ֲ���UiΪ(0,1)���ȷֲ������
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
���ܣ���Box-Muller�㷨������׼��˹�ֲ������
˼·����������01���ȷֲ��������x1,x2�����˹����ֲ�����Ϊ��
	  X = (-2*lnx1)^0.5*cos(2*pi*x2)
	  Y = (-2*lnx1)^0.5*sin(2*pi*x2)  ʹ��һ�����ɡ�
*/
double Box_Muller_algorithm(void){
   double rand01[2] = {genrand_real3(),genrand_real3()};
   double rand_standard_gauss = pow(-2*log(rand01[0]),0.5) * cos(2*pi*(rand01[1]));
   return rand_standard_gauss;
}

/*
���ܣ�����Ϸ����ɻ�ϸ�˹�ֲ�����
˼·����ϸ�˹�ֲ�f(x) = sum(1->3)pi*N(ai,bi^2)
	1> ���ɱ�׼��˹�ֲ�N(0,1),�����˹�ֲ�Ni = N(ai,bi^2) = N(0,1)*bi + ai
	2> ����(0,1)���ȷֲ�picki,��sum(1->i-1)pi <= picki < sum(1->i)pi,��ȡ��Ni
	3> ������ȡ�����¼�pi*Ni���
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
	//���û�������������������
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
	
	FILE *fp;//��writeģʽ���ļ�ʱ��������ļ����ļ������ڵĻ����ȴ����ļ�������Ҫ�˹�����data.txt
    fp = fopen(data_file,"w");
    if (fp == NULL) printf("fail to open the data.text,program interrupted.");
    else {
		for (int i=1;i<=number;i++){
			double rand_gauss;
			if (method == 'a')  rand_gauss = central_limit_theorem();
            else if(method == 'b') rand_gauss = Box_Muller_algorithm();
            else rand_gauss = combinaion_method();
			
			//��double����ת��Ϊ�ַ�������Ϊ�ļ�����ֱ��д��double����
			char string_rand_gauss[50] = {0};
			gcvt(rand_gauss,16,string_rand_gauss);

			//��������洢���ļ��У�ÿ��һ��
			fputs(string_rand_gauss,fp);
			fputs("\n",fp);
		}
		//������
        printf(
            "Gaussion distribution random numbers generated using the %s are stored in pareto_distribution.txt\n"
			"You can use matlab program for statistical analysis of data",
			Method
        );
	}
}