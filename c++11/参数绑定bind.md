## std::bind 和std::function

其定义在头文件functional中.

```c
//调用bind的一般形式为
auto newCallable = bind(callable, arg_list);
```

其中，newCallable本身是一个可调用对象，arg_list是一个逗号分割的参数列表，对应给定的callable的参数，，，当我们调用newCallable时，newCallable会调用callable并传递给它arg_list中的参数。

arg_list中的参数可能包含形如_n的名字，其中n是一个整数，这些参数是占位符，表示newCallable中的参数，他们占据了传递给newCallable的参数的位置，数值n表示生成的可调用对象中参数的位置：

_1 newCallable第一个参数,_2为newCallable的第二个参数

## 使用placeholders名字

名字 _n都定义在名为placeholders的命名空间中，而这个空间本身定义在std命名空间， 为了使用两个命名空间都要写上

```c
using std::placeholders::_1;
```

## bind参数

可以使用bind绑定给定可调用对象的中的参数或重新安排其顺序。

```c
auto g = bind(f, a, b, _2, c, _1);
g(_1,_2);----映射为f(a,b,-2,c,-1);
g(x,y);		--f(a, b, y, c, x);
```

## 绑定引用参数

​	默认情况下，bind 的占位符参数被拷贝到bind返回的可调用对象中。

bind不能拷贝ostream，必须使用标准库ref函数。

```
for_each(words.begin(),words.end(),bind(print, ref(os), _1, ' '));
```

函数ref返回一个对象，包含给定的引用，此对象是可以拷贝的。

标准可还有一个函数 cref ，生成一个const引用类

```

```

![image-20230315193158799](/home/liu/python/image-2023315.png)

