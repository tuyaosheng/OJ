经典线性法（Kadane）：维护"以当前元素结尾的最大子段和" cur。
每来一个 a[i]：`cur = max(a[i], cur + a[i])`——要么接在前面后面，要么自己另起一段；再用 cur 更新全局答案 ans。
初值：ans 设为第一个元素（保证至少选一个，处理全负数据）。ans、cur 用 long long。
