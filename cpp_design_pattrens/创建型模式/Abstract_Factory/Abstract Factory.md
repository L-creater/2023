## 动机(motivation)

- 在软件系统中，经常面临着“一系列相互依赖的对象工作”；同时，由于需求的变化，往往存在更多系列对象的创建工作。
- 如何应对这种变化？如何绕过常规的对象创建方法(new)，提供一种“封装机制”来避免客户程序和这种“多系列具体对象创建工作”的紧耦合。

## 模式定义

提供一个接口，让该接口负责创建一系列”相关或者相互依赖的对象“，无需指定它们具体的类。 ——《设计模式》GoF

## 要点总结

- 如果没有应对”多系列对象创建“的需求变化，则没有必要使用Abstract Factory模式，这时候使用简单的工厂即可。
- ”系列对象“指的是在某一个特定系列的对象之间有相互依赖、或作用的关系。不同系列的对象之间不能相互依赖。
- Abstract Factory模式主要在于应用”新系列“的需求变动。其缺点在与难以应对”新对象“的需求变动。

##  抽象工厂模式适合应用场景

1.  如果代码需要与多个不同系列的相关产品交互， 但是由于无法提前获取相关信息， 或者出于对未来扩展性的考虑， 你不希望代码基于产品的具体类进行构建， 在这种情况下， 你可以使用抽象工厂。

-  抽象工厂为你提供了一个接口， 可用于创建每个系列产品的对象。 只要代码通过该接口创建对象， 那么你就不会生成与应用程序已生成的产品类型不一致的产品。

1.  **如果你有一个基于一组**[抽象方法](https://refactoringguru.cn/design-patterns/factory-method)**的类****，** **且其主要功能因此变得不明确****，** **那么在这种情况下可以考虑使用抽象工厂模式****。**

- ​	 在设计良好的程序中， *每个类仅负责一件事*。 如果一个类与多种类型产品交互， 就可以考虑将工厂方法抽取到独立的工厂类或具备完整功能的抽象工厂类中。

## 实现方式

1. 以不同的产品类型与产品变体为维度绘制矩阵。
2. 为所有产品声明抽象产品接口。 然后让所有具体产品类实现这些接口。
3. 声明抽象工厂接口， 并且在接口中为所有抽象产品提供一组构建方法。
4. 为每种产品变体实现一个具体工厂类。
5. 在应用程序中开发初始化代码。 该代码根据应用程序配置或当前环境， 对特定具体工厂类进行初始化。 然后将该工厂对象传递给所有需要创建产品的类。
6. 找出代码中所有对产品构造函数的直接调用， 将其替换为对工厂对象中相应构建方法的调用。

## 与其他模式的关系

- 在许多设计工作的初期都会使用[工厂方法模式](https://refactoringguru.cn/design-patterns/factory-method) （较为简单， 而且可以更方便地通过子类进行定制）， 随后演化为使用[抽象工厂模式](https://refactoringguru.cn/design-patterns/abstract-factory)、 [原型模式](https://refactoringguru.cn/design-patterns/prototype)或[生成器模式](https://refactoringguru.cn/design-patterns/builder) （更灵活但更加复杂）。
- [生成器](https://refactoringguru.cn/design-patterns/builder)重点关注如何分步生成复杂对象。 [抽象工厂](https://refactoringguru.cn/design-patterns/abstract-factory)专门用于生产一系列相关对象。 *抽象工厂*会马上返回产品， *生成器*则允许你在获取产品前执行一些额外构造步骤。
- [抽象工厂模式](https://refactoringguru.cn/design-patterns/abstract-factory)通常基于一组[工厂方法](https://refactoringguru.cn/design-patterns/factory-method)， 但你也可以使用[原型模式](https://refactoringguru.cn/design-patterns/prototype)来生成这些类的方法。
- 当只需对客户端代码隐藏子系统创建对象的方式时， 你可以使用[抽象工厂](https://refactoringguru.cn/design-patterns/abstract-factory)来代替[外观模式](https://refactoringguru.cn/design-patterns/facade)。
- 你可以将[抽象工厂](https://refactoringguru.cn/design-patterns/abstract-factory)和[桥接模式](https://refactoringguru.cn/design-patterns/bridge)搭配使用。 如果由*桥接*定义的抽象只能与特定实现合作， 这一模式搭配就非常有用。 在这种情况下， *抽象工厂*可以对这些关系进行封装， 并且对客户端代码隐藏其复杂性。
- [抽象工厂](https://refactoringguru.cn/design-patterns/abstract-factory)、 [生成器](https://refactoringguru.cn/design-patterns/builder)和[原型](https://refactoringguru.cn/design-patterns/prototype)都可以用[单例模式](https://refactoringguru.cn/design-patterns/singleton)来实现。