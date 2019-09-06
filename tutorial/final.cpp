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

template <class B>
class VirtualFunctionalClass {
    public:
        // some typedefs for easier use
        // VirtualFunctionalClass is very hard to say
        // Why not make something that is easier to say?
        using Base = VirtualFunctionalClass<B>;
    
        // Why use a VirtualFunction that requires us to pass the Base everywhere when we can
        // make our own that doesn't?
        template<typename T>
        using VirtualFunction=VirtualFunction<Base, T>;

        template <typename type>
        static void Override(VirtualFunction<type>& Func, std::function<type> f) {
            Func = f;
        }    
    
        template<typename t1, typename t2>
        void Bind(VirtualFunction<t1>& Func,
                       std::function<t2> f) {
            Override<void()>(Func,std::bind(f, this));
        }

        std::map<std::string, std::any> members;

        template<typename T>
        T& member(std::string name) {
            return std::any_cast<T&>(members[name]);
        }        

    
};

class Foo : protected VirtualFunctionalClass<Foo> {
    private:
        void InitFunctions() {
            UnboundFunction = [](Base* instance) {
                std::cout << "insideFoo:\t" << instance->member<int>("insideFoo")++ <<"\n";
            };
            
            Bind(Function, UnboundFunction);
        }
        
        void Copy(const Foo& other) {
            // first, our member variables
            members = other.members;
            
            UnboundFunction = other.UnboundFunction;
            
            Bind(Function, other.UnboundFunction);
        }
    protected:
        // Unbound Virtual Functions
        std::function<void(Base*)> UnboundFunction;

        
    public:
        VirtualFunction<void()> Function;

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
            UnboundFunction = [](Base* instance) {  
                 std::cout << "Hello from Bar! " << instance->member<int>("insideBar") << "\n";
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
