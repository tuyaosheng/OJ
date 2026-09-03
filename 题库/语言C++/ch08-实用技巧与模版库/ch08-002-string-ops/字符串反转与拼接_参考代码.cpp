#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout
//   <algorithm> —— reverse
//   <string>    —— string

int main()
{
    string s1, s2;
    cin >> s1 >> s2;
    reverse(s2.begin(), s2.end());   // 原地反转 s2
    cout << s1 + s2 << "\n";         // string 直接用 + 拼接
    return 0;
}
