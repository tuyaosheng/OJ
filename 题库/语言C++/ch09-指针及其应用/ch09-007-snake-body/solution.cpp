// 贪吃蛇身世
// 考点：链表节点定义、头插法建表、遍历输出
#include <iostream>
using namespace std;

struct Node {
    int val;        // 这一节记录的食物编号
    Node *next;     // 指向下一节（朝蛇尾方向）
};

int main()
{
    int n;
    cin >> n;
    Node *head = nullptr;           // 空蛇：头指针为空
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        Node *p = new Node;         // 造新节点
        p->val = x;
        p->next = head;             // 新节点接住原来的头
        head = p;                   // 头指针改指新节点（头插完成）
    }
    // 从蛇头到蛇尾遍历输出
    bool first = true;
    for (Node *p = head; p != nullptr; p = p->next) {
        if (!first) cout << " ";
        cout << p->val;
        first = false;
    }
    cout << endl;
    return 0;
}
