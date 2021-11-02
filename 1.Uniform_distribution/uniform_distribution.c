/*!
    *
    * @file     uniform_distribution.c
    *
    * @brief    ��Ϣ���罨ģ�����γ� ʵ��һ ����1
    *           �˳������[0,1]���ȷֲ���������洢��data.txt��
    *           ���matlab������о�ֵ����������ܶȷ�����
    *
    * @company  BUPT
    *
    * @author   ����  saili@bupt.edu.cn
    *
    * @note     1> ���ַ�������[0,1]���ȷֲ������������ĿҪ�����Ҽ���һ�ַ���������<stdlib.h>�����rand()�����������������
    *              ÷ɭ��ת�㷨�ǵ��ÿ�Դ�⣬����ͬ�෨��MRG32k3a�㷨�Ǹ��ݵ���ʽ�Լ���д�ġ�
    *           2> ����ʽ�ļ���һ�������ַ������ݹ����ѭ�����ʼ�İ汾���õݹ�д�ģ����ǵ�MRG32k3a�㷨����100�����������Ҫ�ܳ���ʱ�䣬
    *              ����Ȼ�����ʣ����Դ���һ�ַ������forѭ�����������ʽ����������ڲ�ע�ͣ�
    *              �ҽ����ַ�����Ϊ���������ڷ��������������tcpЭ�飬
    *              ����ڿ�һ���ǳ�������飬ʹ�����ַ������Լ���ļ�С�������ʽʱ�Ŀռ临�Ӷȡ�
    *           3> �˳��������c++д�Ļ�һЩ�ط����Լ���һ�£�����д�������Ѿ����Ѻܳ�ʱ�䲢��ʵ�ֹ����ˣ������ա�
    *           4> matlab��c���Կ�ѧ�������ĸ�ʽ��һ���ģ�C�������ɵ�С������ֱ�Ӹ�matlab�������Բ����ر�����data�ļ����С��λ������ʽ��
    *
    * @version  final  2021/10/31
    *
    * @date     2021/10/21 ������
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../mt19937ar.sep/mt19937ar.h"

#define ul unsigned long//����͵���ĺ궨��
#define ll long long

#define data_file "..\\data\\uniform_distribution.txt"

/*
    ���ܣ�ȡһ��С����С������
    ˼·����double����װ����int����ʱ���ȥС�����֣�Ҳ����ȡ�������֣��������
*/
double xiaoshu (double x){
    return (x-(int)x);
}

/*
    ���ܣ�������ͬ�෨����(0,1)���ȷֲ������
    ˼·�����õ���ʽx(n) = A*x(n-1) mod M������x(seed)/M��Ϊ(0,1)���ȷֲ������
*/
double linear_congruential_method(long seed){
    ul A = 16807,M = 2147483647;
    ul series[2]={2,0};
    for(long i=1;i<seed;i++){
        /*
        ����ʽ���Ƶ�˼·��
        ʹ��һ������Ϊ2�����鴢��x(n-1)��x(n)
        1.���õ���ʽ���x(n)����ֵ������ڶ�λ
        2.������������һλ����һλ�������ڶ�λ����
        3.ѭ��12
        */
        series[1] = A*series[0] % M;
        series[0] = series[1];
    }
    return series[0]/(double)M;
}

/*
    ���ܣ�ʹ��MRG32k3a�㷨����[0,1]�����
    ˼·�����õ���ʽ
                x(n) = { A*x(n-2)-B*x(n-3) } mod m1
                y(n) = { C*y(n-2)-D*y(n-3) } mod m2
                ���x(n),y(n)��
        [0,1]����� = {|x(n)|/m1 + |y(n)|/m2} mod 1 

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
            ����ʽ���Ƶ�˼·��xy˼·��ͬ����xΪ����
            ʹ��һ������Ϊ�ĵ����鴢��x(n-3),x(n-2),x(n-1),x(n)
            1.���õ���ʽ���x(n)����ֵ���������һλ
            2.������������һλ����һλ���������һλ����
            3.ѭ��12
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
    //���û������ȡ����������������ͷ���
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

    //�����������������data.txt�ļ���
    FILE *fp;//��writeģʽ���ļ�ʱ��������ļ����ļ������ڵĻ����ȴ����ļ�������Ҫ�˹�����data.txt
    fp = fopen(data_file,"w");
    if (fp == NULL) printf("fail to open the data.text,program interrupted.");
    else {
        for (int i=1;i<=number;i++){
            double rand_01;
            if (method == 'a')  rand_01 = linear_congruential_method(i);
            else if(method == 'b') rand_01 = genrand_real1();
            else if(method == 'c') rand_01 = MRG32k3a(i);
            else rand_01 = (double)rand()/RAND_MAX;
            
            //��double����ת��Ϊ�ַ�������Ϊ�ļ�����ֱ��д��double����
            //ʹ��c++�Ļ���΢��һ�£���Ϊc++��string����
            char string_rand_01[50] = {0};
            gcvt(rand_01,16,string_rand_01);

            //��������洢���ļ��У�ÿ��һ��
            fputs(string_rand_01,fp);
            fputs("\n",fp);
        }
        //������
        printf(
            "Uniformly distributed random numbers generated using the %s are stored in uniform_distribution.txt,"
            "you can use matlab program for statistical analysis of data",
            Method
        );
    }
    return 0;
}