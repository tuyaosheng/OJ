先写求最大公约数的函数 `int gcd(int a, int b)`（辗转相除法：`while (b) { r = a%b; a = b; b = r; }`）。
LCM = x × y / gcd(x, y)。x、y 最大 10^6，乘积达 10^12，会超 int——要用 long long，且**先除后乘**或把乘积用 long long 存：`(long long)x / gcd(x,y) * y`。
