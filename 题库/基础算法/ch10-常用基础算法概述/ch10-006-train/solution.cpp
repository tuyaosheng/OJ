#include <iostream>
using namespace std;

int a[10005];

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) cin >> a[i];
    long long cnt = 0;
    // 数逆序对：i<j 且 a[i]>a[j]
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (a[i] > a[j]) cnt++;
    cout << cnt << endl;
    return 0;
}
