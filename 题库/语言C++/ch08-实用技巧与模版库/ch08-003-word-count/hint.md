方法一（推荐）：用 `stringstream` ——先 `getline(cin, line)` 读整行，再把它塞进 `stringstream ss(line)`，然后 `while (ss >> word) cnt++`。`>>` 自动跳过多余空格，一次读一个单词，天然处理"多个空格"。
方法二：手动扫描，遇到"非空格且前一个是空格"就是一个新单词的开始。
