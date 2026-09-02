#include <iostream>
#include <string>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int male = 0, female = 0;      // 身份证男/女人数
    long long qqSum = 0;           // QQ 号总和
    int qqCnt = 0;                 // QQ 号个数
    for (int i = 0; i < n; i++)
    {
        char type;
        cin >> type;
        if (type == 'I')
        {
            string id;
            cin >> id;
            int gender = id[16] - '0';   // 第 17 位（下标 16）表示性别
            if (gender % 2 == 1) male++;  // 奇数为男
            else                 female++;// 偶数为女
        }
        else                          // type == 'Q'
        {
            long long qq;
            cin >> qq;
            qqSum += qq;
            qqCnt++;
        }
    }
    cout << male << " " << female << " " << qqSum / qqCnt << endl;
    return 0;
}
