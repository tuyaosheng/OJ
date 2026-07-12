用 `char op; cin >> a >> op >> b;` 读入（cin 读 char 自动跳过空格）。
switch 按 op 分四个 case，default 分支处理无效运算符。
特别注意：除法 case 里要**先判断 b 是否为 0**，再做除法——判断顺序不能反。
