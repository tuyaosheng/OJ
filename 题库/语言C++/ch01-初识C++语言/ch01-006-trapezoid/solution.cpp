#include <cstdio>            // 使用 printf / scanf，需包含 cstdio 库
using namespace std;

int main()
{
    double a, b, s;          // 用 double 存储，避免整数除法丢失小数
    scanf("%lf%lf%lf", &a, &b, &s);   // %lf 是读入 double 的格式符
    double h = 2 * s / a;             // 由三角形面积 s = a*h/2 反推高 h
    double ans = (a + b) * h / 2;     // 梯形面积公式：(上底+下底)*高/2
    printf("%.2f\n", ans);            // %.2f：保留两位小数输出
    return 0;
}
