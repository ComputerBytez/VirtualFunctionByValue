// Note: This file will not compile
// This is simply the starting code for week 2
#include <iostream>
#include <functional>

template <class Type, class Friend>
class FriendAssign : public Type {
    private:
        using Type::operator=;
    public:
        using Type::Type;
    
    friend Friend;  
};

template <class Base, typename type>
using VirtualFunction = FriendAssign<std::function<type>, Base>;

class Foo {
    private:
        void InitFunctions() {
            Function = []() {
                std::cout << "Hello from Foo!\n";
            };
        }
    public:
        VirtualFunction<Foo, void()> Function;

        Foo() {
            InitFunctions();
        }
};

class Bar : public Foo {
    private:
        void InitFunctions() {
            Function = []() {
                std::cout << "Hello from Bar!\n";
            };
        }
    public:

        Bar() : Foo() {
            InitFunctions();
        }
};

int main() {
    Foo myFoo;
    Bar myBar;

    myFoo.Function();
    myBar.Function();

    myFoo = myBar;

    myFoo.Function();
    

    // this fails to compile, which is good
    // Let's comment this out
    //myFoo.Function = []() {
    //    std::cout << "Hello from main!\n";
    //};

    return 0;
}
