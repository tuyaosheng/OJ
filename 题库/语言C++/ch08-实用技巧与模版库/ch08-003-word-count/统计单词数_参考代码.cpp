#include <bits/stdc++.h>
using namespace std;
// 不用万能头时，需要单独引入：
//   <iostream>  —— cin / cout / getline
//   <sstream>   —— stringstream
//   <string>    —— string

int main()
{
    string line;
    getline(cin, line);              // 读入一整行（含空格）
    stringstream ss(line);           // 把这行接到字符串流上
    string word;
    int cnt = 0;
    while (ss >> word) cnt++;        // >> 自动跳过多余空格，一次读一个词
    cout << cnt << "\n";
    return 0;
}
