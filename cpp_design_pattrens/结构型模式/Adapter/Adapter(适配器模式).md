# Adapter(适配器模式) 

## 适配器模式适合应用场景

 当你希望使用某个类， 但是其接口与其他代码不兼容时， 可以使用适配器类。

 适配器模式允许你创建一个中间层类， 其可作为代码与遗留类、 第三方类或提供怪异接口的类之间的转换器。

 如果您需要复用这样一些类， 他们处于同一个继承体系， 并且他们又有了额外的一些共同的方法， 但是这些共同的方法不是所有在这一继承体系中的子类所具有的共性。

 你可以扩展每个子类， 将缺少的功能添加到新的子类中。 但是， 你必须在所有新子类中重复添加这些代码， 这样会使得代码有[坏味道](https://refactoringguru.cn/smells/duplicate-code)。

将缺失功能添加到一个适配器类中是一种优雅得多的解决方案。 然后你可以将缺少功能的对象封装在适配器中， 从而动态地获取所需功能。 如要这一点正常运作， 目标类必须要有通用接口， 适配器的成员变量应当遵循该通用接口。 这种方式同[装饰](https://refactoringguru.cn/design-patterns/decorator)模式非常相似。

##  实现方式

1. 确保至少有两个类的接口不兼容：
   - 一个无法修改 （通常是第三方、 遗留系统或者存在众多已有依赖的类） 的功能性*服务*类。
   - 一个或多个将受益于使用服务类的*客户端*类。
2. 声明客户端接口， 描述客户端如何与服务交互。
3. 创建遵循客户端接口的适配器类。 所有方法暂时都为空。
4. 在适配器类中添加一个成员变量用于保存对于服务对象的引用。 通常情况下会通过构造函数对该成员变量进行初始化， 但有时在调用其方法时将该变量传递给适配器会更方便。
5. 依次实现适配器类客户端接口的所有方法。 适配器会将实际工作委派给服务对象， 自身只负责接口或数据格式的转换。
6. 客户端必须通过客户端接口使用适配器。 这样一来， 你就可以在不影响客户端代码的情况下修改或扩展适配器。

##  与其他模式的关系

- [桥接模式](https://refactoringguru.cn/design-patterns/bridge)通常会于开发前期进行设计， 使你能够将程序的各个部分独立开来以便开发。 另一方面， [适配器模式](https://refactoringguru.cn/design-patterns/adapter)通常在已有程序中使用， 让相互不兼容的类能很好地合作。
- [适配器](https://refactoringguru.cn/design-patterns/adapter)可以对已有对象的接口进行修改， [装饰模式](https://refactoringguru.cn/design-patterns/decorator)则能在不改变对象接口的前提下强化对象功能。 此外， *装饰*还支持递归组合， *适配器*则无法实现。
- [适配器](https://refactoringguru.cn/design-patterns/adapter)能为被封装对象提供不同的接口， [代理模式](https://refactoringguru.cn/design-patterns/proxy)能为对象提供相同的接口， [装饰](https://refactoringguru.cn/design-patterns/decorator)则能为对象提供加强的接口。
- [外观模式](https://refactoringguru.cn/design-patterns/facade)为现有对象定义了一个新接口， [适配器](https://refactoringguru.cn/design-patterns/adapter)则会试图运用已有的接口。 *适配器*通常只封装一个对象， *外观*通常会作用于整个对象子系统上。
- [桥接](https://refactoringguru.cn/design-patterns/bridge)、 [状态模式](https://refactoringguru.cn/design-patterns/state)和[策略模式](https://refactoringguru.cn/design-patterns/strategy) （在某种程度上包括[适配器](https://refactoringguru.cn/design-patterns/adapter)） 模式的接口非常相似。 实际上， 它们都基于[组合模式](https://refactoringguru.cn/design-patterns/composite)——即将工作委派给其他对象， 不过也各自解决了不同的问题。 模式并不只是以特定方式组织代码的配方， 你还可以使用它们来和其他开发者讨论模式所解决的问题。