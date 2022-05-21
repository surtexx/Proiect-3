#ifndef POO_IOBASE_H
#define POO_IOBASE_H

#include <iostream>
#include <vector>
using namespace std;

class IoBase {
private:
public:
    // metoda citire
    virtual istream &read(istream &is) { // nu este nevoie de obiectul citit, pt ca avem this
        return is; // vom vedea ca in interfete deseori nu avem functionalitati
    }

    // metoda scriere
    virtual ostream &write(ostream &os) const { // nu este nevoie de obiectul citit, pt ca avem this
        return os; // vom vedea ca in interfete deseori nu avem functionalitati
    }

    // ...
    friend ostream &operator<<(ostream &os, const IoBase &base) {
        return base.write(os);
    }

    friend istream &operator>>(istream &is, IoBase &base) {
        return base.read(is);
    }
};

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
    unsigned int nr;
    cout << "\nNumar: ";
    is >> nr;
    for (unsigned i = 0; i < nr; i++)
    {
        T x;
        is >> x;
        v.push_back(x);
    }
    return is;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T> v) {
    os << "\n";
    for (unsigned i = 0; i < v.size()-1; i++)
        os <<"    - "<<v[i] << "\n";
    os << "    - " << v[v.size() - 1];
    return os;
}

#endif //POO_IOBASE_H
