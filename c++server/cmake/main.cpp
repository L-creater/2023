#include "gun.h"
#include "solider.h"

void test(){
    Solider sanduo("a");
    sanduo.addGun(new Gun("AK"));
    sanduo.addBullerToGun(20);
    sanduo.fire();
}

int main(){
    std::cout<< "this is a test string" <<std::endl;
    test();
    return 0;
}