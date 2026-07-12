// 汉诺塔：把 n 个盘子从 A 柱移到 C 柱，大盘不能压小盘。
//
// 递归结构（三步）—— 要把 n 个盘子从 from 移到 to（借助 via）：
//   ① 把上面的 n-1 个，从 from 移到 via（借助 to）   <- 交给递归，别管细节
//   ② 把最大的第 n 号，从 from 直接移到 to           <- 此时 to 是空的，一步到位
//   ③ 把那 n-1 个，从 via 移到 to（借助 from）       <- 交给递归
//
// 精髓：【信任递归】。只想清楚一层怎么做，剩下的交给它自己。
// 总步数：T(n) = 2*T(n-1) + 1，T(0)=0 => T(n) = 2^n - 1。
#include <iostream>
using namespace std;

int step = 0;       // 步数计数器

// 把 n 个盘子，从 from 柱，借助 via 柱，移动到 to 柱
void hanoi(int n, char from, char via, char to)
{
    if (n == 0) return;                 // 边界：没盘子要移了

    // ① 上面 n-1 个：from -> via。注意"借助"的柱子变成了 to
    hanoi(n - 1, from, to, via);

    // ② 最大的第 n 号盘子：from -> to（此时 to 上是空的，可以直接放）
    step++;
    cout << "第" << step << "步：将第" << n
         << "号盘子从" << from << "柱移动到" << to << "柱\n";

    // ③ 那 n-1 个：via -> to。注意"借助"的柱子变成了 from
    hanoi(n - 1, via, from, to);
}

int main()
{
    ios::sync_with_stdio(false);        // n=15 有 3 万多行输出，关同步加速
    cin.tie(nullptr);

    int n;
    cin >> n;

    // 先输出总步数 2^n - 1（用位运算 1<<n 算 2^n）
    cout << ((1 << n) - 1) << "\n";

    hanoi(n, 'A', 'B', 'C');            // 从 A 移到 C，借助 B

    return 0;
}
