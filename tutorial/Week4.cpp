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
            UnboundFunction = [](Foo* instance) {
                std::cout << "insideFoo:\t" << instance->insideFoo++ <<"\n";
            };
            
            Function = std::bind(UnboundFunction, this);
        }
        void Copy(const Foo& other) {
            // first, our member variable
            insideFoo = other.insideFoo;
            
            UnboundFunction = other.UnboundFunction;
            
            Function = std::bind(other.UnboundFunction, this);
        }
    protected:
        // in case we want to mess with it from inside Bar
        int insideFoo;
        
        // Unbound Virtual Functions
        std::function<void(Foo*)> UnboundFunction;

        template <typename type>
        static void Override(VirtualFunction<Foo, type>& Func, std::function<type> f) {
            Func = f;
        }
    public:
        VirtualFunction<Foo, void()> Function;

        Foo() {
            insideFoo = 0;
            InitFunctions();
        }
        
        
        Foo(int i) {
            insideFoo = i;
            InitFunctions();
        }

        // copy construction and assignment
        Foo(const Foo& other) {
            Copy(other);
        }
        
        Foo& operator=(Foo& other) {
            Copy(other);
            return *this;
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

int main() {
    Foo myFoo;
    Foo myFoo2(5);
    Bar myBar;
    
    std::cout << "Foo1\n";
    myFoo.Function();
    std::cout << "Foo2\n";
    myFoo2.Function();

    // Copy by value
    myFoo = myFoo2;
    
    std::cout << "Foo1\n";
    myFoo.Function();

    std::cout << "Foo2\n";
    myFoo2.Function();

    std::cout << "Foo1\n";
    myFoo.Function();

    std::cout << "Foo2\n";
    myFoo2.Function();

    myFoo2.Function();
    myFoo2.Function();
    myFoo2.Function();

    myFoo = myBar;
    
    std::cout << "Bar1\n";
    myBar.Function();
    
    std::cout << "Foo1\n";
    myFoo.Function();

    return 0;
}
