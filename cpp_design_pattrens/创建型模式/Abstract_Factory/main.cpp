#include <iostream>

/*
 *产品系列中的每个不同产品都应该有一个基本接口。
 *全部产品的变体必须实现此接口。
 */
class AbstractProductA
{
public:
    virtual ~AbstractProductA(){};
    virtual std::string UsefulFunctionA() const = 0;
};

/**
 * Concrete Products are created by corresponding Concrete Factories.
 */

class ConcreteProductA1 : public AbstractProductA
{
public:
    std::string UsefulFunctionA() const override
    {
        return "The result of the product A1";
    }
};

class ConcreteProductA2 : public AbstractProductA
{
    std::string UsefulFunctionA() const override
    {
        return "The result of the product A2";
    }
};

/*
这是另一个产品的基本界面。所有产品都可以交互
*彼此之间，但只有在产品之间才有可能进行适当的交互
*相同的具体变体
*
*/

class AbstractProductB
{
public:
    virtual ~AbstractProductB(){};
    virtual std::string UsefulFunctionB() const = 0;
    /*
    ...但它也可以与 ProductA 协作。
    *抽象工厂确保它创建的所有产品都是
    *相同的变体，因此兼容。
    */
    virtual std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const = 0;
};

class ConcreteProductB1 : public AbstractProductB
{
public:
    std::string UsefulFunctionB() const override
    {
        return "The result of the product B1";
    }
    std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override
    {
        const std::string result = collaborator.UsefulFunctionA();
        return "The result of the B1 collaborating with ( " + result + " )";
    }
};

class ConcreteProductB2 : public AbstractProductB
{
public:
    std::string UsefulFunctionB() const override
    {
        return "the result of the product B2";
    }
    /**
     * The variant, Product B2, is only able to work correctly with the variant,
     * Product A2. Nevertheless, it accepts any instance of AbstractProductA as an
     * argument.
     */

    std::string AnotherUsefulFunctionB(const AbstractProductA &collaborator) const override
    {
        const std::string result = collaborator.UsefulFunctionA();
        return "The result of the B2 collaborating with ( " + result + " )";
    }
};

/**
 * The Abstract Factory interface declares a set of methods that return
 * different abstract products. These products are called a family and are
 * related by a high-level theme or concept. Products of one family are usually
 * able to collaborate among themselves. A family of products may have several
 * variants, but the products of one variant are incompatible with products of
 * another.
 */

class AbstractFactory
{
public:
    virtual AbstractProductA *CreatProductA() const = 0;
    virtual AbstractProductB *CreatProductB() const = 0;
};

/**
 * Concrete Factories produce a family of products that belong to a single
 * variant. The factory guarantees that resulting products are compatible. Note
 * that signatures of the Concrete Factory's methods return an abstract product,
 * while inside the method a concrete product is instantiated.
 */

class ConcreteFactory1 : public AbstractFactory
{
public:
    AbstractProductA *CreatProductA() const override
    {
        return new ConcreteProductA1();
    }
    AbstractProductB *CreatProductB() const override
    {
        return new ConcreteProductB1();
    }
};

class ConcreteFactory2 : public AbstractFactory
{
public:
    AbstractProductA *CreatProductA() const override
    {
        return new ConcreteProductA2();
    }
    AbstractProductB *CreatProductB() const override
    {
        return new ConcreteProductB2();
    }
};

void ClientCode(const AbstractFactory &factory)
{
    const AbstractProductA *product_a = factory.CreatProductA();
    const AbstractProductB *product_b = factory.CreatProductB();
    std::cout << product_b->UsefulFunctionB() << std::endl;
    std::cout << product_b->AnotherUsefulFunctionB(*product_a) << "\n";
    delete product_a;
    delete product_b;
}

int main()
{
    std::cout << "Client: Testing client code with the first factory type:\n";
    ConcreteFactory1 *f1 = new ConcreteFactory1();
    ClientCode(*f1);
    delete f1;
    std::cout << std::endl;
    std::cout << "Client: Testing the same client code with the second factory type:\n";
    ConcreteFactory2 *f2 = new ConcreteFactory2();
    ClientCode(*f2);
    delete f2;
    return 0;
}