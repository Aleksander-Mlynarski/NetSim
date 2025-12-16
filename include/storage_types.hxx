#include <package.hxx>

#include "types.hxx"

enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
public:

    virtual ~IPackageStockpile() = default; //virtual destructor
    virtual void push(Package&& package) = 0; //r-value reference – moves object instead of copying it
    virtual bool empty() const = 0; //query (added const)
    virtual size_type size() const = 0;//query (added const)

    //iterators:
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

};
