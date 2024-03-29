//############################################################################
//  Templates
//############################################################################

#include <iostream>
#include <span>
#include <string>
#include <vector>


using namespace std;

// function template can infer the datatype from parameter
template <typename T> T square(T x) { return x * x; }

// template <typename T> void print_container(T container) {
//     typename T::iterator it;
//     std::cout << "{ ";
//     for (it = container.begin(); it != container.end(); it++) {
//         std::cout << *it << ", ";
//     }
//     std::cout << " }" << std::endl;
// }

template <typename T> void print_container(T container) {
    std::cout << "{ ";
    for (auto it : container) {
        std::cout << it << ", ";
    }
    std::cout << " }" << std::endl;
}

// datatype must be given
template <typename T> class BoVector {
    T arr[1000];
    int size;

public:
    BoVector() : size(0) {}

    void push(T x) {
        arr[size] = x;
        size++;
    }

    T get(int i) const { return arr[i]; }

    T getSize() const { return size; }

    void print() const {
        for (int i = 0; i < size; ++i) {
            cout << arr[i] << " ";
        }
    }
};

template <typename T> BoVector<T> operator*(const BoVector<T>& rhs1, const BoVector<T>& rhs2) {
    BoVector<T> ret;

    for (int i; i < rhs1.getSize(); ++i) {
        ret.push(rhs1.get(i) * rhs2.get(i));
    }

    return ret;
}

int main(int argc, char* argv[]) {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print_container(vec);

    BoVector<int> bv;
    bv.push(1);
    bv.push(5);
    bv.push(2);
    bv.push(4);
    bv.push(3);
    bv.print();
    cout << "size: " << bv.getSize() << endl;
    cout << "print squared bv: ";
    bv = square(bv);
    bv.print();

    return 0;
}
