#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<math.h>
#include<time.h>
using namespace std;

//(t = x /15)
//f(t) = at^6 + bt^5 + ct^4 + dt^3 + et^2 + ft + 1500 

bool gene[8][180];//유전자
double dataset[31] = {
1500, 1472, 1495, 1364, 1280, 1313,
1388, 1451, 1400, 1418, 1409, 1423,
1393, 1409, 1466, 1400, 1393 ,1409,
1410, 1542, 1565, 1554, 1577, 1637,
1606, 1534, 1630, 1618, 1678, 1809, 1845
};
double term[8][6];//함수 각항
double value[8][31];//함수 값
double diff[8];//오차
double fitness[8];//적합도
double ratio[8];//비율
double accu_ratio[8];//누적비율
double ans_term[6];//최적일때 함수인자
double ans_value[31];//최적일때 함수값
double ans_diff = 1000000000;//최적일때 오차

void set_group()//표본 설정
{
	for(int i = 0; i < 8; i++)
		for (int j = 0; j < 180; j++)
			gene[i][j] = rand() % 2;
}
void get_function()//각 항의 개수 계산
{
	double temp;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 6; j++)
		{
			temp = 0;
			for (int k = 29; k > 0; k--)
			{
				temp *= 2;
				if (gene[i][j * 10 + k])
					temp += 1;
			}
			if (gene[i][j * 10])
				temp *= -1;
			term[i][j] = temp / (double)(1 << 19);
		}
}
void get_function_value()//함수값 계산
{
	double temp;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 31; j++)
		{
			temp = 1500;
			for (int k = 0; k < 6; k++)
				temp += term[i][k] * pow((double)j / 15, 6 - k);
			value[i][j] = temp;
		}
	}
}
void get_diff()//오차 계산
{
	double temp;
	for (int i = 0; i < 8; i++)
	{
		temp = 0;
		for (int j = 0; j < 31; j++)
			temp += abs(value[i][j] - dataset[j]);
		diff[i] = temp;
	}
}
void get_fitness_value()//적합도 계산
{
	double temp = diff[0];
	for (int i = 1; i < 8; i++)
		temp = max(temp, diff[i]);
	for (int i = 0; i < 8; i++)
		fitness[i] = temp - diff[i];
}
void ratio_dist()//비율 분배
{
	double temp = 0;
	for (int i = 0; i < 8; i++)
		temp += fitness[i];
	for (int i = 0; i < 8; i++)
		ratio[i] = fitness[i] / temp;
	accu_ratio[0] = ratio[0];
	for (int i = 1; i < 8; i++)
		accu_ratio[i] = accu_ratio[i - 1] + ratio[i];
}
int choose()
{
	int temp = (double)(rand() % 100) / (double)100;
	
	for (int i = 0; i < 8; i++)
		if ((double)temp < accu_ratio[i])
			return i;
	return 7;
}
void crossover()//교차
{
	int a, b, c;
	double temp[8][180];
	
	for (int i = 0; i < 4; i++)
	{
		a = choose();
		b = choose();
		
		for (int j = 0; j < 6; j++)
		{
			
			c = rand() % 30;

			for (int k = 0; k < c; k++)
			{
				temp[i][j * 30 + k] = gene[a][j * 30 + k];
				temp[i + 1][j * 30 + k] = gene[b][j * 30 + k];
			}
			for (int k = c; k < 30; k++)
			{
				temp[i][j * 30 + k] = gene[b][j * 30 + k];
				temp[i + 1][j * 30 + k] = gene[a][j * 30 + k];
			}
		}
	}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 180; j++)
			gene[i][j] = temp[i][j];
}
void mutantion()//돌연변이
{
	int temp;
	for(int i = 0; i < 8; i++)
		for (int j = 0; j < 180; j++)
		{
			temp = rand() % 100;
			if (temp < 2)
				gene[i][j] = !gene[i][j];
		}
}
bool check()//오차 확인
{
	int temp = 0;
	for (int i = 1; i < 8; i++)
		if (diff[temp] > diff[i])
			temp = i;
	if (diff[temp] <= ans_diff)
	{
		ans_diff = diff[temp];
		for (int i = 0; i < 31; i++)
			ans_value[i] = value[temp][i];
		for (int i = 0; i < 6; i++)
			ans_term[i] = term[temp][i];
		return false;
	}
	return true;
}

int main()
{
	// 데이터 입력;
	srand(time(NULL));
	set_group();//표본설정
	int i = 0;
	while(1)
	{
		i++;
		get_function();
		get_function_value();

		get_diff();
		if(check() && i > 3000)//3000세대 이상을 강제
			break;
		get_fitness_value();
		ratio_dist();
		crossover();
		mutantion();
	}
	printf("함수값\n");
	for (int i = 0; i < 31; i++)
		printf("%d회 %lf점\n", i, ans_value[i]);
	printf("\n그래프\n");
	for (int i = 0; i < 6; i++)
		printf("%lf\n", ans_term[i]);
}