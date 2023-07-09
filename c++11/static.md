# 静态成员变量

对象的内存中包含了成员变量，不同的对象占用不同的内存（已在《[C++对象的内存模型](http://c.biancheng.net/view/vip_2218.html)》中提到），这使得不同对象的成员变量相互独立，它们的值不受其他对象的影响。例如有两个相同类型的对象 a、b，它们都有一个成员变量 m_name，那么修改 a.m_name 的值不会影响 b.m_name 的值。

可是有时候我们希望在多个对象之间共享数据，对象 a 改变了某份数据后对象 b 可以检测到。共享数据的典型使用场景是计数，以前面的 Student 类为例，如果我们想知道班级中共有多少名学生，就可以设置一份共享的变量，每次创建对象时让该变量加 1。

**在[C++](http://c.biancheng.net/cplus/)中，我们可以使用静态成员变量来实现多个对象共享数据的目标。静态成员变量是一种特殊的成员变量，它被关键字`static`修饰**，例如：

```c++
class Student{
public:
    Student(char *name, int age, float score);
    void show();
public:
    static int m_total;  //静态成员变量
private:
    char *m_name;
    int m_age;
    float m_score;
};
```

这段代码声明了一个静态成员变量 m_total，用来统计学生的人数。

static 成员变量属于类，不属于某个具体的对象，即使创建多个对象，也只为 m_total 分配一份内存，所有对象使用的都是这份内存中的数据。当某个对象修改了 m_total，也会影响到其他对象。

static 成员变量必须在类声明的外部初始化，具体形式为：

type class::name = value;

type 是变量的类型，class 是类名，name 是变量名，value 是初始值。将上面的 m_total 初始化：

int Student::m_total = 0;

静态成员变量在初始化时不能再加 static，但必须要有数据类型。被 private、protected、public 修饰的静态成员变量都可以用这种方式初始化。

**注意：static 成员变量的内存既不是在声明类时分配，也不是在创建对象时分配，而是在（类外）初始化时分配。反过来说，没有在类外初始化的 static 成员变量不能使用。**

static 成员变量既可以通过对象来访问，也可以通过类来访问。请看下面的例子：

```c++
//通过类来访问 static 成员变量
Student::m_total = 10;
//通过对象来访问 static 成员变量
Student stu("小明", 15, 92.5f);
stu.m_total = 20;
//通过对象指针来访问 static 成员变量
Student *pstu = new Student("李华", 16, 96);
pstu -> m_total = 20;
```

**注意：static 成员变量不占用对象的内存，而是在所有对象之外开辟内存，即使不创建对象也可以访问。具体来说，static 成员变量和普通的 static 变量类似，都在内存分区中的全局数据区分配内存，不了解的读者请阅读《[C语言内存精讲](http://c.biancheng.net/c/140/)》专题。**

```c++
#include <iostream>
using namespace std;

class Student{
public:
    Student(char *name, int age, float score);
    void show();
private:
    static int m_total;  //静态成员变量
private:
    char *m_name;
    int m_age;
    float m_score;
};

//初始化静态成员变量
int Student::m_total = 0;

Student::Student(char *name, int age, float score): m_name(name), m_age(age), m_score(score){
    m_total++;  //操作静态成员变量
}
void Student::show(){
    cout<<m_name<<"的年龄是"<<m_age<<"，成绩是"<<m_score<<"（当前共有"<<m_total<<"名学生）"<<endl;
}

int main(){
    //创建匿名对象
    (new Student("小明", 15, 90)) -> show();
    (new Student("李磊", 16, 80)) -> show();
    (new Student("张华", 16, 99)) -> show();
    (new Student("王康", 14, 60)) -> show();

    return 0;
}
```

```

本例中将 m_total 声明为静态成员变量，每次创建对象时，会调用构造函数使 m_total 的值加 1。

之所以使用匿名对象，是因为每次创建对象后只会使用它的 show() 函数，不再进行其他操作。不过使用匿名对象无法回收内存，会导致内存泄露，在中大型程序中不建议使用。
```

## 几点说明 

1) 一个类中可以有一个或多个静态成员变量，所有的对象都共享这些静态成员变量，都可以引用它。



2) static 成员变量和普通 static 变量一样，都在内存分区中的全局数据区分配内存，到程序结束时才释放。这就意味着，static 成员变量不随对象的创建而分配内存，也不随对象的销毁而释放内存。而普通成员变量在对象创建时分配内存，在对象销毁时释放内存。



3) 静态成员变量必须初始化，而且只能在类体外进行。例如：

int Student::m_total = 10;

初始化时可以赋初值，也可以不赋值。如果不赋值，那么会被默认初始化为 0。全局数据区的变量都有默认的初始值 0，而动态数据区（堆区、栈区）变量的默认值是不确定的，一般认为是垃圾值。

4) 静态成员变量既可以通过对象名访问，也可以通过类名访问，但要遵循 private、protected 和 public 关键字的访问权限限制。当通过对象名访问时，对于不同的对象，访问的是同一份内存。

# 静态成员函数

- 在类中，static 除了可以声明[静态成员变量](http://c.biancheng.net/view/2227.html)，还可以声明静态成员函数。普通成员函数可以访问所有成员（包括成员变量和成员函数），静态成员函数只能访问静态成员。
- **编译器在编译一个普通成员函数时，会隐式地增加一个形参 this，并把当前对象的地址赋值给 this，所以普通成员函数只能在创建对象后通过对象来调用，因为它需要当前对象的地址。而静态成员函数可以通过类来直接调用，编译器不会为它增加形参 this，它不需要当前对象的地址，所以不管有没有创建对象，都可以调用静态成员函数。**

- 普通成员变量占用对象的内存，静态成员函数没有 this [指针](http://c.biancheng.net/c/80/)，不知道指向哪个对象，无法访问对象的成员变量，也就是说静态成员函数不能访问普通成员变量，只能访问静态成员变量。
- 普通成员函数必须通过对象才能调用，而静态成员函数没有 this 指针，无法在函数体内部访问某个对象，所以不能调用普通成员函数，只能调用静态成员函数。

**静态成员函数与普通成员函数的根本区别在于：普通成员函数有 this 指针，可以访问类中的任意成员；而静态成员函数没有 this 指针，只能访问静态成员（包括静态成员变量和静态成员函数）。**

```c++
#include <iostream>
using namespace std;

class Student{
public:
    Student(char *name, int age, float score);
    void show();
public:  //声明静态成员函数
    static int getTotal();
    static float getPoints();
private:
    static int m_total;  //总人数
    static float m_points;  //总成绩
private:
    char *m_name;
    int m_age;
    float m_score;
};

int Student::m_total = 0;
float Student::m_points = 0.0;

Student::Student(char *name, int age, float score): m_name(name), m_age(age), m_score(score){
    m_total++;
    m_points += score;
}
void Student::show(){
    cout<<m_name<<"的年龄是"<<m_age<<"，成绩是"<<m_score<<endl;
}
//定义静态成员函数
int Student::getTotal(){
    return m_total;
}
float Student::getPoints(){
    return m_points;
}

int main(){
    (new Student("小明", 15, 90.6)) -> show();
    (new Student("李磊", 16, 80.5)) -> show();
    (new Student("张华", 16, 99.0)) -> show();
    (new Student("王康", 14, 60.8)) -> show();

    int total = Student::getTotal();
    float points = Student::getPoints();
    cout<<"当前共有"<<total<<"名学生，总成绩是"<<points<<"，平均分是"<<points/total<<endl;

    return 0;
}
```

```
总人数 m_total 和总成绩 m_points 由各个对象累加得到，必须声明为 static 才能共享；getTotal()、getPoints() 分别用来获取总人数和总成绩，为了访问 static 成员变量，我们将这两个函数也声明为 static。

在C++中，静态成员函数的主要目的是访问静态成员。getTotal()、getPoints() 当然也可以声明为普通成员函数，但是它们都只对静态成员进行操作，加上 static 语义更加明确。

和静态成员变量类似，静态成员函数在声明时要加 static，在定义时不能加 static。静态成员函数可以通过类来调用（一般都是这样做），也可以通过对象来调用，上例仅仅演示了如何通过类来调用。
```

