# Builder

**生成器模式**是一种创建型设计模式， 使你能够分步骤创建复杂对象。 该模式允许你使用相同的创建代码生成不同类型和形式的对象。

![image-20230325195447384](/home/liu/.config/Typora/typora-user-images/image-20230325195447384.png)

 

##  生成器模式适合应用场景

 使用生成器模式可避免 “重叠构造函数 （telescoping constructor）” 的出现。

 假设你的构造函数中有十个可选参数， 那么调用该函数会非常不方便； 因此， 你需要重载这个构造函数， 新建几个只有较少参数的简化版。 但这些构造函数仍需调用主构造函数， 传递一些默认数值来替代省略掉的参数。



生成器模式让你可以分步骤生成对象， 而且允许你仅使用必须的步骤。 应用该模式后， 你再也不需要将几十个参数塞进构造函数里了。



 **当你希望使用代码创建不同形式的产品** **（****例如石头或木头房屋****）** **时****，** **可使用生成器模式**.

 如果你需要创建的各种形式的产品， 它们的制造过程相似且仅有细节上的差异， 此时可使用生成器模式。

基本生成器接口中定义了所有可能的制造步骤， 具体生成器将实现这些步骤来制造特定形式的产品。 同时， 主管类将负责管理制造步骤的顺序。


 **使用生成器构造[组合](https://refactoringguru.cn/design-patterns/composite)树或其他复杂对象**。

 生成器模式让你能分步骤构造产品。 你可以延迟执行某些步骤而不会影响最终产品。 你甚至可以递归调用这些步骤， 这在创建对象树时非常方便。

生成器在执行制造步骤时， 不能对外发布未完成的产品。 这可以避免客户端代码获取到不完整结果对象的情况。



## 实现方法

1. 清晰地定义通用步骤， 确保它们可以制造所有形式的产品。 否则你将无法进一步实施该模式。

2. 在基本生成器接口中声明这些步骤。

3. 为每个形式的产品创建具体生成器类， 并实现其构造步骤。

   不要忘记实现获取构造结果对象的方法。 你不能在生成器接口中声明该方法， 因为不同生成器构造的产品可能没有公共接口， 因此你就不知道该方法返回的对象类型。 但是， 如果所有产品都位于单一类层次中， 你就可以安全地在基本接口中添加获取生成对象的方法。

4. 考虑创建主管类。 它可以使用同一生成器对象来封装多种构造产品的方式。

5. 客户端代码会同时创建生成器和主管对象。 构造开始前， 客户端必须将生成器对象传递给主管对象。 通常情况下， 客户端只需调用主管类构造函数一次即可。 主管类使用生成器对象完成后续所有制造任务。 还有另一种方式， 那就是客户端可以将生成器对象直接传递给主管类的制造方法。

6. 只有在所有产品都遵循相同接口的情况下， 构造结果可以直接通过主管类获取。 否则， 客户端应当通过生成器获取构造结果。



## 与其他模式的关系

- 在许多设计工作的初期都会使用[工厂方法模式](https://refactoringguru.cn/design-patterns/factory-method) （较为简单， 而且可以更方便地通过子类进行定制）， 随后演化为使用[抽象工厂模式](https://refactoringguru.cn/design-patterns/abstract-factory)、 [原型模式](https://refactoringguru.cn/design-patterns/prototype)或[生成器模式](https://refactoringguru.cn/design-patterns/builder) （更灵活但更加复杂）。
- [生成器](https://refactoringguru.cn/design-patterns/builder)重点关注如何分步生成复杂对象。 [抽象工厂](https://refactoringguru.cn/design-patterns/abstract-factory)专门用于生产一系列相关对象。 *抽象工厂*会马上返回产品， *生成器*则允许你在获取产品前执行一些额外构造步骤。
- 你可以在创建复杂[组合模式](https://refactoringguru.cn/design-patterns/composite)树时使用[生成器](https://refactoringguru.cn/design-patterns/builder)， 因为这可使其构造步骤以递归的方式运行。
- 你可以结合使用[生成器](https://refactoringguru.cn/design-patterns/builder)和[桥接模式](https://refactoringguru.cn/design-patterns/bridge)： *主管*类负责抽象工作， 各种不同的*生成器*负责*实现*工作。
- [抽象工厂](https://refactoringguru.cn/design-patterns/abstract-factory)、 [生成器](https://refactoringguru.cn/design-patterns/builder)和[原型](https://refactoringguru.cn/design-patterns/prototype)都可以用[单例模式](https://refactoringguru.cn/design-patterns/singleton)来实现。 