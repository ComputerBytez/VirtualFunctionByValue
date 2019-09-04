/* Copyright 2019
 * Nathan Black
 *
 * This class is a class to simulate a virtual function,
 * but allow it to be passed by value and not by referrence.
 */
#include <functional>

namespace ComputerBytez {

// This class is a helper class that may be useful for something else, but is only being used here in 
// the ComputerBytez namespace, so we'll keep it in this file
template <class Type, class Friend>
class FriendAssign : public Type {
    private:
        using Type::operator=;
    public:
        using Type::Type;
    
    friend Friend;  
};

// A VirtualFunction is actually basically a rehashing of FriendAssign, just with
// more syntax sugar.
// VirtualFunction(s) are basically just a subset of FriendAssigns
template <class Base, typename type>
using VirtualFunction = FriendAssign<std::function<type>, Base>;

}; // end namespace ComputerBytez
