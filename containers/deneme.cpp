#include <iostream>

template <typename Iterator>
class reverse_iterator {
public:
    reverse_iterator(Iterator iter) : mIterator(iter) { }

    // Birinci operatör tanımı
    template <typename U> 
    reverse_iterator& operator=(const reverse_iterator<U>& other) {
        mIterator = other.base();
        return *this;
    }

    // İkinci operatör tanımı
    template <typename U> 
    reverse_iterator<Iterator>& operator=(const reverse_iterator<U>& other) {
        mIterator = other.base();
        return *this;
    }

    Iterator base() const {
        return mIterator;
    }

private:
    Iterator mIterator;
};

int main() {
    reverse_iterator<int> ri1(42);
    reverse_iterator<double> ri2(3.14);

    // İlk operatör kullanımı
    ri1 = ri2;
    std::cout << "ri1.base() after first assignment: " << ri1.base() << std::endl;

    // İkinci operatör kullanımı
    reverse_iterator<int> ri3(10);
    ri3 = ri2;
    std::cout << "ri3.base() after second assignment: " << ri3.base() << std::endl;

    return 0;
}