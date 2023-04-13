#include <iostream>
#include <unordered_map>

/** *享元设计模式 * * 意图：让您通过共享将更多对象装入可用的 RAM 量 * 多个对象之间状态的公共部分，
 * 而不是保留所有的 * 每个对象中的数据。 */

struct SharedState{
    std::string brand_;
    std::string model_;
    std::string color_;
    SharedState(const std::string &brand, const std::string &model, const std::string &color): brand_(brand), model_(model), color_(color){

    }
    friend std::ostream &operator<<(std::ostream &os, const SharedState &ss){
        return os <<"[" << ss.brand_ <<" , "<<ss.color_<<" , "<<ss.model_<<"]";
    }
};

struct UniqueState
{
    std::string owner_;
    std::string plates_;

    UniqueState(const std::string &owner, const std::string &plates): owner_(owner), plates_(plates){

    }
    friend std::ostream &operator<<(std::ostream &os, const UniqueState &us)
    {
        return os << "[ " << us.owner_ << " , " << us.plates_ << " ]";
    }

};

/** *享元存储状态的公共部分（也称为内在 * state) 属于多个真实的商业实体。享元接受 * 其余的状态（外部状态，每个实体都是唯一的）通过它的 * 方法参数。 */

class FlyWeight
{
private:
    SharedState *shared_state_;
public:
    FlyWeight(const SharedState *shared_state) : shared_state_(new SharedState(*shared_state)){}
    FlyWeight(const FlyWeight &other) : shared_state_(new SharedState(*other.shared_state_)){

    }
    ~FlyWeight(){
        delete shared_state_;
    }

    SharedState *shared_state() const{
        return shared_state_;
    }  

    void Operation(const UniqueState &unique_state) const{
        std::cout << "Flyweight: Displaying shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n"; 
    }
};

/** * 享元工厂创建和管理享元对象。它确保 * 正确共享享元。当客户请求享元时， * 工厂要么返回一个现有的实例，要么创建一个新的实例，如果它 * 尚不存在。 */

class FlyWeightFactory
{
private:
    std::unordered_map<std::string, FlyWeight> flyweights_;
    /** * 返回给定状态的享元字符串哈希。 */
    std::string GetKey(const SharedState &ss) const{
        return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
    }
public:
    FlyWeightFactory(std::initializer_list<SharedState> shared_states)
    {
        for(const SharedState &ss : shared_states){
            this->flyweights_.insert(std::make_pair<std::string,FlyWeight>(this->GetKey(ss),FlyWeight(&ss)));
        }
    }
    ~FlyWeightFactory(){}
    /** * 返回具有给定状态的现有享元或创建新享元。 */

    FlyWeight GetFlyweight(const SharedState &shared_state)
    {
        std::string key = this->GetKey(shared_state);
        if (this->flyweights_.find(key) == this->flyweights_.end())
        {
            std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
            this->flyweights_.insert(std::make_pair(key, FlyWeight(&shared_state)));
        }
        else
        {
            std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
        }
        return this->flyweights_.at(key);
    }
    void ListFlyweights() const
    {
        size_t count = this->flyweights_.size();
        std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
        for (std::pair<std::string, FlyWeight> pair : this->flyweights_)
        {
            std::cout << pair.first << "\n";
        }
    }
};

// ...
void AddCarToPoliceDatabase(
    FlyWeightFactory &ff, const std::string &plates, const std::string &owner,
    const std::string &brand, const std::string &model, const std::string &color)
{
    std::cout << "\nClient: Adding a car to database.\n";
    const FlyWeight &flyweight = ff.GetFlyweight({brand, model, color});
    // The client code either stores or calculates extrinsic state and passes it
    // to the flyweight's methods.
    flyweight.Operation({owner, plates});
}


int main()
{
    FlyWeightFactory *factory = new FlyWeightFactory({{"Chevrolet", "Camaro2018", "pink"}, {"Mercedes Benz", "C300", "black"}, {"Mercedes Benz", "C500", "red"}, {"BMW", "M5", "red"}, {"BMW", "X6", "white"}});
    factory->ListFlyweights();

    AddCarToPoliceDatabase(*factory,
                            "CL234IR",
                            "James Doe",
                            "BMW",
                            "M5",
                            "red");

    AddCarToPoliceDatabase(*factory,
                            "CL234IR",
                            "James Doe",
                            "BMW",
                            "X1",
                            "red");
    factory->ListFlyweights();
    delete factory;

    return 0;
}