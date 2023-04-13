#include <iostream>

class Product{
public:
    virtual ~Product(){}
    virtual std::string operation() const = 0;
};

class ConcreteProduct1 : public Product{
public:
    std::string operation() const override{
        return {"Result of the ConcreteProduct1"};
    }
};

class ConcreteProduct2 : public Product{
public:
    std::string operation() const override{
        return {"Result of the ConcreteProduct2"};
    }
};

class Creator{
     /**
     * Note that the Creator may also provide some default implementation of the
     * factory method.
     */
public:
    virtual ~Creator(){};
    virtual Product* FactoryMethod() const = 0;
      /**
   * Also note that, despite its name, the Creator's primary responsibility is
   * not creating products. Usually, it contains some core business logic that
   * relies on Product objects, returned by the factory method. Subclasses can
   * indirectly change that business logic by overriding the factory method and
   * returning a different type of product from it.
   */
  std::string SomeOperation() const {
   // Call the factory method to create a Product object.
    Product* product = this->FactoryMethod();
    // Now, use the product.
    std::string result =  "Creator: The same creator's code has just worked with " + product->operation();
    delete product;
    return result;
  }
};
class ConcreteCreator1 : public Creator{
public:
    Product* FactoryMethod() const override{
        return new ConcreteProduct1();
    }
};
class ConcreteCreator2 : public Creator{
public:
    Product* FactoryMethod() const override{
        return new ConcreteProduct2();
    }
};

/**
 * The client code works with an instance of a concrete creator, albeit through
 * its base interface. As long as the client keeps working with the creator via
 * the base interface, you can pass it any creator's subclass.
 */
void ClientCode(const Creator& creator) {
  // ...
  std::cout << "Client: I'm not aware of the creator's class, but it still works.\n"
            << creator.SomeOperation() << std::endl;
  // ...
}

/**
 * The Application picks a creator's type depending on the configuration or
 * environment.
 */

int main(){
    std::cout << "App: Launched with the ConcreteCreator1.\n";
    Creator* creator = new ConcreteCreator1();
    ClientCode(*creator);
    std::cout<<std::endl;
    std::cout << "App: Launched with the ConcreteCreator2.\n";
    Creator* creator2 = new ConcreteCreator2();
    ClientCode(*creator2);

    delete creator;
    delete creator2;
    return 0;
}