用**小根堆**（优先队列）：`priority_queue<int, vector<int>, greater<int>>`，每次弹出两个最小的堆合并，把合并结果（花费）累加进答案，再把新堆压回优先队列，重复到只剩一堆。
`greater<int>` 让优先队列变成"最小的先出"。总花费用 long long。这是"贪心 + 优先队列"的招牌题。
