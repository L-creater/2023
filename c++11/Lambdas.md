## Lambda

---等同于--未命名的内联函数

```c++
[capture list] (parameter list) -> return type { function body};
//capture list --捕获列表 --传值传引用
//parameter list --参数列表
//return type	--返回类型
//function body --函数体

[capture list] (parameter list) mutable throwSpec -> return type { function body};

//mutable throwSpec -> return type --三个可有可无 --都没有时，(parameter list) 可写可不写 ，三个只要有一个，(parameter list) 必须写

```

```c++
auto I = []{
	std::cout << "hello lambda" <<std::endl;
};
...
I(); 	//print "hello lambda"
```

```c++
//期望改变捕获变量的值---mutable
int id = 0;
auto f = [id]() mutable{	//如果没有mutable  不能++id
	std::cout<<"id:"<<id<<std::endl;
	++id;	//ok
};
id = 42;
f();	//0
f();	//1
f();	//2
std::cout<<id<<std::endl; //42

//上面的等同于
class Functor{
    private:
    	int id;
    public:
        void operator()(){
            std::cout << "id: "<<std::endl;
            ++id;
        }
};


int id = 0;
auto f = [&id] (int param) mutable{
    std::cout<<"id:"<<id<<std::endl;
    ++id;   //ok
    ++param;
    std::cout<<"param: "<<param<<std::endl;
    };
    id =42;
    f(7);	//id=42
    f(7);	//43
    f(7);	//44
    std::cout<<id<<std::endl; //45
//值捕获和引用捕获
//隐式捕获
[=,&];
//= --值捕获
//& --引用捕获
```

1![image-20230305095002805](/home/liu/.config/Typora/typora-user-images/image-20230305095002805.png)

### **指定我们需要的比较大小方式**

```c++

    std::vector<int> vi{5,28,50,83,70,590,245,59,24};
    int x=30;
    int y=100;
    vi.erase(remove_if(vi.begin(),vi.end(), [x,y](int n){return x<n && n<y;}),vi.end());

    for(auto i:vi)
        std::cout<<i<<' ';
    std::cout<<std::endl;
```

```c++
stable_sort(words.begin(),words.end(),[](const string &a, const string &b){return a.size<b.size();});

//查找第一个长度大于等于sz的元素
auto wc = find_if(words.begin(),words.end(),[sz](const string &a) {return a.size() >= sz;});
```

### 指定lambda的返回类型

**默认情况下，如果一个lambda体包含return之外的任何语句，则编译器假定此lambda返回void**

```c++
//将序列中每个负数替换为其绝对值
transform(vi.begin(),vi.end(),vi.begin(),
         [](int i) {return i < 0 ? -i : i;});
//transform接受三个迭代器和一个可调用对象，前两个迭代器表示输入序列，第三个迭代器表示目的位置。算法对输入序列中每个元素调用可调用对象，并将结果写道目的位置。
//本例中，lambda是单一的return语句，返回一个表达式结果，无需指定返回类型--可以跟根据条件运算符推算出来。
```

```c++
//error
transform(vi.begin(),vi.end(),vi.begin(),
         [](int i) {if(i < 0) return  -i; else reurn i;});
//此时，编译器推断这个lambda返回类型为void ,但其实返回了int

//修改--指定返回值类型
transform(vi.begin(),vi.end(),vi.begin(),
         [](int i)->int {if(i < 0) return  -i; else reurn i;});
```
