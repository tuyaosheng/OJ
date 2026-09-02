#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s;
    cin >> s;
    int i = 0, j = s.length() - 1;   // 双指针：一头一尾
    bool ok = true;
    while (i < j)
    {
        if (s[i] != s[j])            // 对应位置不相等，不是回文
        {
            ok = false;
            break;
        }
        i++;                         // 两个指针向中间靠拢
        j--;
    }
    if (ok) cout << "yes" << endl;
    else    cout << "no" << endl;
    return 0;
}
