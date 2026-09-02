样例 1：100 × 1.50 − 30 = 120.00。

三个坑：

1. 倍率是小数，存它要用 `double`；
2. `atk * mult` 是"整数 × 浮点"，结果自动是浮点，不用额外转换；
3. **保留两位小数**：`printf("%.2f\n", x)`，或 `cout << fixed << setprecision(2) << x << endl;`（需要 `#include <iomanip>`）。
