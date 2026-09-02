// 伤害结算
// 考点：double、混合运算、保留两位小数输出
#include <cstdio>
using namespace std;

int main()
{
    int atk, def;
    double mult;
    scanf("%d %lf %d", &atk, &mult, &def);
    double damage = atk * mult - def;   // int 与 double 混合运算，结果是 double
    printf("%.2f\n", damage);          // 保留两位小数
    return 0;
}
