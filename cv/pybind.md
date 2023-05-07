# pybind

[使用指南](https://blog.csdn.net/zhuikefeng/article/details/107224507)

[官方文档](https://pybind11.readthedocs.io/en/stable/index.html)

[github](https://github.com/pybind/pybind11)





## 配置环境

1. ```bash
   pip install pybind11
   
   #我是安装到conda的一个环境包内---newbase
   ##pip后文件会添加到
   export PYBIND_INCLUDE_PATH=/home/ly/anaconda3/envs/newbase/lib/python3.10/site-packages/pybind11/include
   
   #可以将其添加在 ~/.bashrc
   ```

2. 测试环境

   ```c++
   //测试代码
   #include <pybind11/pybind11.h>
   
   int add(int i, int j) {
       return i + j;
   }
   
   PYBIND11_MODULE(example, m) {
       m.doc() = "pybind11 example plugin"; // optional module docstring
   
       m.def("add", &add, "A function that adds two numbers");
   }
   
   //该宏创建一个函数，当从 Python 中发出语句PYBIND11_MODULE()时将调用该函数 。
   //import 模块名称--(example) 作为第一个宏参数给出（不应在引号中）。
   //第二个参数 ( m) 定义了一个变量类型py::module_，
   //它是创建绑定的主要接口。该方法module_::def() 生成将函数公开给 Python 的绑定代码add()。
   
   //实际上，实现和绑定代码通常位于不同的文件中。
   ```
   
   
   
   ```bash
   #编译文件
   #编译前记着---创建pybind11头文件路径的环境变量以供编译时使用。
   c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) example.cpp -o example$(python3-config --extension-suffix)
   ```
   
   ![image-20230420153934456](/home/ly/.config/Typora/typora-user-images/image-20230420153934456.png)

**最重要的就是这个python模块绑定**

1. example：模型名，切记不需要引号
2. m：可以理解成模块对象吧
3. m.doc()：help说明
4. m.def：用来注册函数和python打通界限



[先看下这个例子](https://iamsorush.com/posts/pybind11-robot/)

```c++
PYBIND11_MODULE(moduleName, m)
{
  // define all classes
  py::class_<NameOfClass, ClassInterface>(m, "NameOfClass")
        .def(py::init<type1, type2, type3>()) // constructor
        .def("aFunction", &NameOfClass::aFunction)
        .def_readwrite("aPublicMember", &NameOfClass::aPublicMember);

  // define all standalone functions
  m.def("StandAloneFunction", &StandAloneFunction);
}
```

- **moduleName** : Python 中模块的名称
- **m** : pybind11 句柄，按原样接受
- **NameOfClass**：要暴露给 Python 的类的名称
- **ClassInterface**：类的接口或父类。如果不需要在 Python 中捕获多态性，则没有必要。
- **type1, typ2, ...** : 类构造函数的参数类型
- **aFunction** : 类的函数成员的名称
- **aPublicMember** : 类的公共成员

我们还可以声明不属于任何类的函数：

```c++
PYBIND11_MODULE(moduleName, m)
{
  // define all classes
  // ...

  // define all standalone functions
  m.def("StandAloneFunction", &StandAloneFunction);
}
```



## 面向对象code

### bind_struct

​	

```c++
#include<iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

PYBIND11_MODULE(struct_, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
}
```

```bash
c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) struct_.cpp -o struct_$(python3-config --extension-suffix)

```

- `class_` creates bindings for a C++ *class* or *struct*-style data structure.   --绑定类或类似struct的数据结构

- `init()` is a convenience function that **takes the types of a constructor’s parameters as template arguments and wraps the corresponding constructor** (see the [Custom constructors](https://pybind11.readthedocs.io/en/latest/advanced/classes.html#custom-constructors) section for details).  ---将构造函数的参数类型作为模板参数并wraps相应的构造函数,

  ```python
  >>> import struct_
  >>> p = struct_.Pet("lyt")
  >>> print(p)
  <struct_.Pet object at 0x7f95aa7ee4f0>
  >>> p.getName()
  'lyt'
  >>> p.setName("wxb")
  >>> p.getName()
  'wxb'
  ```

  

### [Keyword arguments](https://pybind11.readthedocs.io/en/latest/basics.html#keyword-args)

### [Default arguments](https://pybind11.readthedocs.io/en/latest/basics.html#default-args)





### 绑定lambda函数

​		Note how `print(p)` produced a rather useless summary of our data structure in the example above:

​		--对我们的数据，生成了一个相当于无用summary(概括)

```python
>>> print(p)
<example.Pet object at 0x10cd98060>
```

为了解决此问题，我们绑定一个 utility function，返回我们可以认识的内容，the special method slot named `__repr__`

不幸的是，Pet 数据结构中没有合适的功能，如果我们不必更改它就好了。这可以通过绑定 Lambda 函数轻松实现：

```c++
py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName)
    .def("__repr__",
        [](const Pet &a) {
            return "<strcut_.Pet named '" + a.name + "'>";
        }
    );
```

**Both stateless [1](https://pybind11.readthedocs.io/en/latest/classes.html#f1) and stateful lambda closures are supported by pybind11. With the above change, the same Python code now produces the following output:**????

Stateless closures are those with an empty pair of brackets `[]` as the capture object.

无状态闭包是那些以一对空括号 [] 作为捕获对象的闭包。

```
>>> print(p)
<example.Pet named 'Molly'>
```

### Instance and static fields

1. class_::def_readwrite() ---directly expose the `name` field --直接暴露名称字段  --- 相当于直接修改变量
2. class_::def_readonly() ---exists for `const` fields.--const字段

#### struct

```c++

PYBIND11_MODULE(struct_, m) {   
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def_readwrite("name", &Pet::name) ///
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("__repr__",
        [](const Pet &a) {
            return "<example.Pet named '" + a.name + "'>";
        }
        );
}
```



```python
>>> import struct_
>>> p = struct_.Pet("lyt")
>>> p.name
'lyt'
>>> p.name = "ch"
>>> p.name
'ch'
```

#### class 

1. `class_::def_property()`

   1. 只读变量可以将第三个参数设为nullptr，只写变量可以将第二个参数设为nullptr。

2. `class_::def_property_readonly()` ---for read-only data

   可用于在 Python 中提供类似字段的接口，该接口将透明地调用 setter 和 getter 函数：

```c++
#include<iostream>
#include <pybind11/pybind11.h>

namespace py = pybind11;

class Pet {
public:
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }
private:
    std::string name;
};


PYBIND11_MODULE(class_, m) {   
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
         //只读变量可以将第三个参数设为nullptr，只写变量可以将第二个参数设为nullptr。
        .def_property("name", &Pet::getName, &Pet::setName) //-&Pet::getName, &Pet::setName---//
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("__repr__",
        [](const Pet &a) {
            return "<example.Pet named '" + a.getName() + "'>";
        }
        );
}
```

```python
>>> import class_
>>> p = class_.Pet("lyt")
>>> p.name
'lyt'
>>> p.name="wxb"
>>> p.name 
'wxb'

```

```bash
c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) class_.cpp -o class_$(python3-config --extension-suffix)
```

提供了类似的函数`class_::def_readwrite_static()`、 `class_::def_readonly_static()` `class_::def_property_static()`和来绑定静态变量和属性。另请参阅文档高级部分中有关[静态属性的](https://pybind11.readthedocs.io/en/latest/advanced/classes.html#static-properties)`class_::def_property_readonly_static()`部分 。

### [Dynamic attributes](https://pybind11.readthedocs.io/en/latest/classes.html#dynamic-attributes)

python端为类绑定动态属性

用之前的代码实现时，

```python
p = example.Pet()
p.name = "Charly"  # OK, attribute defined in C++
p.age = 2  # fail
```

C++类要支持动态属性，必须将py::dynamic_attr这个标记添加到py::class_构造函数中:

**Now everything works as expected:**

```python
PYBIND11_MODULE(class_, m) {   
    py::class_<Pet>(m, "Pet", py::dynamic_attr())
        .def(py::init<const std::string &>())
        .def_property("name", &Pet::getName, &Pet::setName)
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("__repr__",
        [](const Pet &a) {
            return "<example.Pet named '" + a.getName() + "'>";
        }
        );
}
```

```python
>>> p = class_.Pet("ch")
>>> print(p)
<example.Pet named 'ch'>
>>> p.name
'ch'
>>> p.age = 18
>>> p.__dict__
{'age': 18}
```



### Inheritance and automatic downcasting

假设现在示例由两个具有继承关系的数据结构组成： ---非多态  --下面会有多态

```c++
struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

struct Dog : Pet {
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};
```

有两种不同的方式来指示与 pybind11 的层次关系：

1. 第一种方式将第一种将父类作为子类额外的模板参数。：  ---fastdeploy 用的就是这种,推荐这种

   ```c++
   py::class_<Pet>(m, "Pet")
      .def(py::init<const std::string &>())
      .def_readwrite("name", &Pet::name);
   
   // Method 1: template parameter:
   py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog")  //此处是在<>内
       .def(py::init<const std::string &>())
       .def("bark", &Dog::bark);
   ```

   

2. 我们也可以给之前绑定的 Pet class_ 对象取一个名字，在绑定 Dog 类的时候引用它：

   ```c++
   py::class_<Pet> pet(m, "Pet");
   pet.def(py::init<const std::string &>())
      .def_readwrite("name", &Pet::name);
   
   // Method 2: pass parent class_ object:
   py::class_<Dog>(m, "Dog", pet /* <- specify Python parent type */)  //-此处是在（）内--//
       .def(py::init<const std::string &>())
       .def("bark", &Dog::bark);
   ```

   

   ```bash
   c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) Inheritance_struct.cpp -o Inheritance_struct$(python3-config --extension-suffix)
   ```

   ```python
   >>> import Inheritance_struct
   >>> p =Inheritance_struct.Dog("asd")
   >>> p.name
   'asd'
   >>> p.bark()
   'woof!'
   ```

   完整代码

   ```c++
   #include<iostream>
   #include <pybind11/pybind11.h>
   struct Pet {
       Pet(const std::string &name) : name(name) { }
       std::string name;
   };
   struct Dog : Pet {
       Dog(const std::string &name) : Pet(name) { }
       std::string bark() const { return "woof!"; }
   };
   
   PYBIND11_MODULE(Inheritance_struct, m) {   
           pybind11::class_<Pet>(m, "Pet")
           .def(pybind11::init<const std::string &>())
           .def_readwrite("name", &Pet::name);
   
           // Method 1: template parameter:
           pybind11::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog")  //此处是在<>内
               .def(pybind11::init<const std::string &>())
               .def("bark", &Dog::bark);
   }
   ```

但是由于子类的bark是非虚函数，并且父类中没有任何虚函数，不能通过父类指针调用子类的bark函数。

```c++
//返回指向派生实例的基指针
// Return a base pointer to a derived instance
m.def("pet_store", []() { return std::unique_ptr<Pet>(new Dog("Molly")); });		
```

```python
>>> p = example.pet_store()
>>> type(p)  # `Dog` instance behind `Pet` pointer
Pet          # no pointer downcasting for regular non-polymorphic types
>>> p.bark()
AttributeError: 'Pet' object has no attribute 'bark'
```

- 该函数返回一个 Dog 实例，但由于它是基指针后面的非多态类型，Python 只能看到 Pet。
- 在 C++ 中，只有至少有一个虚函数的类型才被认为是多态的，pybind11 会自动识别这一点：

**多态**

```c++
struct PolymorphicPet {
    virtual ~PolymorphicPet() = default;
};

struct PolymorphicDog : PolymorphicPet {
    std::string bark() const { return "woof!"; }
};

// Same binding code
py::class_<PolymorphicPet>(m, "PolymorphicPet");
py::class_<PolymorphicDog, PolymorphicPet>(m, "PolymorphicDog")
    .def(py::init<>())
    .def("bark", &PolymorphicDog::bark);

// Again, return a base pointer to a derived instance
m.def("pet_store2", []() { return std::unique_ptr<PolymorphicPet>(new PolymorphicDog); });
```

```c++
>>> p = Inheritance_struct.pet_store2()
>>> type(p)
<class 'Inheritance_struct.PolymorphicDog'>
>>> p.bark()
'woof!'
```

### 函数重载

```c++
struct Pet {
    Pet(const std::string &name, int age) : name(name), age(age) { }

    void set(int age_) { age = age_; }
    void set(const std::string &name_) { name = name_; }

    std::string name;
    int age;
};
```

如果直接绑定Pet::set将导致错误，因为编译器不知道是哪种方法。可以将它们转换为函数指针来消除歧义。多个函数绑定相同的名称会自动创建一系列重载函数。

```c++
py::class_<Pet>(m, "Pet")
   .def(py::init<const std::string &, int>())
   .def("set", static_cast<void (Pet::*)(int)>(&Pet::set), "Set the pet's age")
   .def("set", static_cast<void (Pet::*)(const std::string &)>(&Pet::set), "Set the pet's name");
```

The overload signatures are also visible in the method’s docstring:

```python
>>> help(overload.Pet)
Help on class Pet in module overload:

class Pet(pybind11_builtins.pybind11_object)
 |  Method resolution order:
 |      Pet
 |      pybind11_builtins.pybind11_object
 |      builtins.object
 |  
 |  Methods defined here:
 |  
 |  __init__(...)
 |      __init__(self: overload.Pet, arg0: str, arg1: int) -> None
 |  
 |  set(...)
 |      set(*args, **kwargs)
 |      Overloaded function.
 |      
 |      1. set(self: overload.Pet, arg0: int) -> None
 |      
 |      Set the pet's age
 |      
 |      2. set(self: overload.Pet, arg0: str) -> None
 |      
 |      Set the pet's name
 |  
 |  ----------------------------------------------------------------------
 |  Static methods inherited from pybind11_builtins.pybind11_object:
 |  
 |  __new__(*args, **kwargs) from pybind11_builtins.pybind11_type
 |      Create and return a new object.  See help(type) for accurate signature.

```

#### c++14 重载

```python
py::class_<Pet>(m, "Pet")
    .def("set", py::overload_cast<int>(&Pet::set), "Set the pet's age")
    .def("set", py::overload_cast<const std::string &>(&Pet::set), "Set the pet's name");
```

- 在这里，py::overload_cast 只需要指定参数类型。推导出返回类型和类。
- 这避免了在原始转换中看到的 void (Pet::*)() 的额外噪声( additional noise)。
- 如果函数基于常量重载，则应使用 py::const_ 标记：

```python
struct Widget {
    int foo(int x, float y);
    int foo(int x, float y) const;
};

py::class_<Widget>(m, "Widget")
   .def("foo_mutable", py::overload_cast<int, float>(&Widget::foo))
   .def("foo_const",   py::overload_cast<int, float>(&Widget::foo, py::const_));
```

如果你更喜欢 py::overload_cast 语法但只有 C++11 兼容的编译器，你可以使用 py::detail::overload_cast_impl 和一组额外的括号：

```c++
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;

py::class_<Pet>(m, "Pet")
    .def("set", overload_cast_<int>()(&Pet::set), "Set the pet's age")
    .def("set", overload_cast_<const std::string &>()(&Pet::set), "Set the pet's name");
```

**note**

**要定义多个重载构造函数，只需使用 .def(py::init<...>()) 语法一个接一个地声明。用于指定关键字和默认参数的现有机制也可以使用。**

### [返回值](https://pybind11.readthedocs.io/en/latest/advanced/functions.html#return-value-policies)

Python 和 C++ 使用根本不同的方式来管理它们管理的对象的内存和生命周期。在为返回非平凡类型的函数创建绑定时，这可能会导致问题。仅通过查看类型信息，还不清楚是 Python 应该负责返回值并最终释放其资源，还是应该在 C++ 端处理。为此，pybind11 提供了几个可以传递给 module_::def() 和 class_::def() 函数的返回值策略注释。默认策略是 return_value_policy::automatic。



## pybind11::class_

```python
//注册结构体
#include <pybind11/pybind11.h>
 
namespace py = pybind11;
 
PYBIND11_MODULE(example, m) {//这几行代码只使用了一个m，说明这几个函数都是绑在一起
        py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
}
```



## .def

#绑定c++**指针参数函数**

​		lambda 函数

```python
#绑定c++指针参数函数

.def("run",
           [](vision::detection::PaddleDetPreprocessor& self,
              std::vector<pybind11::array>& im_list) {
               
           }
```

https://blog.csdn.net/tcy23456/article/details/117659776



## def_readwrite

## def_property_readonly

## init

`init()`是一个方便的函数，它将构造函数的参数类型作为模板参数并包装相应的构造函数

## def_submodule
