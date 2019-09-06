/* Copyright 2019
 * Nathan Black
 *
 * This class is a boiler plate class to help with Virtual Functions By Value
 */
#include "VirtualFunction"
// Note: Requires C++17 or later, or a boost::any
#include <any>
#include <map>
#include <string>

namespace ComputerBytez {

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


}; // end namespace ComputerBytez
