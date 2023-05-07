# const

## 常量成员函数

两个版本同时存在时：

1. const object只能调用const版本
2. non-const object 只能调用non-const版本



- 常量成员函数不改变class内data的值
- non-const  member functions 可以调用 const member functions  ,反之不可

![image-20230422105556696](/home/ly/.config/Typora/typora-user-images/image-20230422105556696.png)