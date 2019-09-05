#include <iostream>
#include <functional>


// Our VirtualFunction class (and FriendAssign):

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

// Good. Now a test case of inheritance:

class Foo {
    private:
        void InitFunctions() {
            Function = []() {
                std::cout << "Hello from Foo!\n";
            };
        }
    protected:
        template <typename type>
        static void Override(VirtualFunction<Foo, type>& Func, std::function<type> f) {
            Func = f;
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
            Override<void()>(Function, []() { // Note: We have to specify the type of function here 
                 std::cout << "Hello from Bar!\n"; // because template deduction doesn't play well
             });                                  // with lambdas. See https://stackoverflow.com/questions/51436339/template-argument-deduction-substitution-failed-with-lambda-as-function-pointer
        }
    public:

        Bar() : Foo() {
            InitFunctions();
        }
};

//Finally, a test program:

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
