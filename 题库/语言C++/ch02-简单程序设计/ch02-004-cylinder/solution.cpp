#include <cstdio>
using namespace std;

const double PI = 3.14159;   // 符号常量：见名知意，且要改只需改这一处

int main()
{
    double r, h;
    scanf("%lf%lf", &r, &h);
    // 表面积 = 上下底面积(2*PI*r*r) + 侧面积(2*PI*r*h)
    double s = 2 * PI * r * r + 2 * PI * r * h;
    printf("%.3f\n", s);     // 保留 3 位小数
    return 0;
}
