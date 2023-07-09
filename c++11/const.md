# const

## 常量成员函数

两个版本同时存在时：

1. const object只能调用const版本
2. non-const object 只能调用non-const版本



- 常量成员函数不改变class内data的值
- non-const  member functions 可以调用 const member functions  ,反之不可



const在指针前--指针指向的内容不能改变，在指针后指针自身无法改变是为常量。

```c++
char greeting[] = "hello"
const char* p = greeting;
char* const p = greeting;
const char* const p = greeting;
```



## interator

```c++
std::vector<int> vec;
const std::vector<int>::interator iter = vec.begin();  // iter 的作用像个 T* const 迭代器不能指向不同的东西，指向的东西的值可以改变。
*iter = 10; //可以
++iter; // 错误
```

```c++
std::vector<int>::iterator cIter = vec.begin();
*cIter = 10; //错误	*cIter是const
++cTter;	//可以 
```



const可以和函数返回值，各参数，函数自身(如果是成员函数)，产生关联。

## 函数返回值为const

```c++

//为什么设置函数返回值为const?? -- 避免输入错误 
class Rational{};
const Rational operator* (const Rational& lhs, const Rational& rhs);

Rational a, b, c;
if(a * b = c)   //其使用该是  ==  打字错误时，回报错 -- 这就是为什么设置函数返回值为const
```

## const成员函数

- 目的---为了确认该成员函数可作用于const对象身上。

  - 使class接口比较容易被理解 -- 得知那个函数可以改动对象内容，那个函数不行
  - 它们使操作const对象成为可能，改善c++程序效率的根本办法是以pass by reference-to-const来传递对象，此技术可行的前提是我们有const成员函数可用来处理取得的const对象。

  