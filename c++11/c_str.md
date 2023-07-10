string的函数，c_str 是获取这个字符串的**首**指针。

```bash
string str = "1234";
char* pStr = str.c_str(); //pStr就是指向“1234”字串的首地址'1'
```