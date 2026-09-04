// 本题用到 vector、queue、string、map、cin/cout，若不使用万能头，
// 需要 #include <cstdio> <vector> <queue> <string> <map> <iostream>
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2005;
vector<int> g[MAXN];
int dist_[MAXN];

bool oneDiff(const string& a, const string& b) {
    int diff = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) diff++;
        if (diff > 1) return false;
    }
    return diff == 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<string> words(n);
    map<string, int> idx;
    for (int i = 0; i < n; i++) {
        cin >> words[i];
        idx[words[i]] = i;
    }
    string beginWord, endWord;
    cin >> beginWord >> endWord;

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (oneDiff(words[i], words[j])) {
                g[i].push_back(j);
                g[j].push_back(i);
            }

    int s = idx[beginWord], t = idx[endWord];
    for (int i = 0; i < n; i++) dist_[i] = -1;
    queue<int> q;
    q.push(s);
    dist_[s] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (dist_[v] == -1) {
                dist_[v] = dist_[u] + 1;
                q.push(v);
            }
        }
    }
    cout << dist_[t] << '\n';
    return 0;
}
