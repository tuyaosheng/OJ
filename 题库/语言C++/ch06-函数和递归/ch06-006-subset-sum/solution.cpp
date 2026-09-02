#include <iostream>
using namespace std;

int n, a[25];
long long m;
bool found = false;

// 考虑第 i 个元素，当前已选元素之和为 sum
void dfs(int i, long long sum)
{
    if (found) return;               // 已找到答案，剪枝提前退出
    if (i > n)
    {
        if (sum == m) found = true;  // 到底了，检查是否凑出 m
        return;
    }
    dfs(i + 1, sum + a[i]);          // 选第 i 个
    dfs(i + 1, sum);                 // 不选第 i 个
}

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i++) cin >> a[i];
    dfs(1, 0);
    cout << (found ? "YES" : "NO") << endl;
    return 0;
}
