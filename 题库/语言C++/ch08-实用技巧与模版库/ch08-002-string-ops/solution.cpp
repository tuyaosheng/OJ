#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main()
{
    string s1, s2;
    cin >> s1 >> s2;
    reverse(s2.begin(), s2.end());   // 原地反转 s2
    cout << s1 + s2 << endl;         // string 直接用 + 拼接
    return 0;
}
