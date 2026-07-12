#include <iostream>
#include <cstring>
using namespace std;

char s[100005];

int main()
{
    cin >> s;
    int len = strlen(s);
    char *p = s;                     // 指向开头
    char *q = s + len - 1;           // 指向结尾
    while (p < q)                    // 两端向中间靠拢
    {
        char t = *p; *p = *q; *q = t;  // 交换首尾字符
        p++;
        q--;
    }
    cout << s << endl;
    return 0;
}
