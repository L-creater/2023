# extern  只有变量的声明需要

```c
//声明
extern int a;
int f();
struct c;
//定义
int a;
int f(){
    return 0;
}
struct c{};
```

