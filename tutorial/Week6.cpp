// requires C++17 or later
// or, you can use a boost::any instead
#include <iostream>
#include <functional>
#include <any>
#include <map>
#include <string>


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
                std::cout << "insideFoo:\t" << instance->member<int>("insideFoo")++ <<"\n";
            };
            
            Function = std::bind(UnboundFunction, this);
        }
        
        void Copy(const Foo& other) {
            // first, our member variables
            members = other.members;
            
            UnboundFunction = other.UnboundFunction;
            
            Function = std::bind(other.UnboundFunction, this);
        }
    protected:
        std::map<std::string, std::any> members;

        template<typename T>
        T& member(std::string name) {
            return std::any_cast<T&>(members[name]);
        }        
        
        // Unbound Virtual Functions
        std::function<void(Foo*)> UnboundFunction;

        template <typename type>
        static void Override(VirtualFunction<Foo, type>& Func, std::function<type> f) {
            Func = f;
        }
        

        template<typename t1, typename t2>
        void Bind(VirtualFunction<Foo,t1>& Func,
                       std::function<t2> f) {
            Override<void()>(Func,std::bind(f, this));                                               
        
                           
        }

        
    public:
        VirtualFunction<Foo, void()> Function;

        Foo() {
            members["insideFoo"] = 0;
            InitFunctions();
        }
        
        
        Foo(int i) {
            members["insideFoo"] = i;
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
            UnboundFunction = [](Foo* instance) {  
                 std::cout << "Hello from Bar! " << static_cast<Bar*>(instance)->member<int>("insideBar") << "\n";
             };
             
             Bind(Function, UnboundFunction);
             
        }
        
    public:

        Bar() : Foo() {
            members["insideBar"] = 100; // some data
            
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
