#include <cstdio>
using namespace std;

int main()
{
    double f;
    scanf("%lf", &f);
    // 注意：5 和 9 参与的是浮点运算（f 是 double），不会发生整数除法截断
    double c = 5 * (f - 32) / 9;
    printf("%.5f\n", c);     // 保留 5 位小数
    return 0;
}
