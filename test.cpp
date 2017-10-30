#include <memory>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class A{};

template <class T>
class AImpl : public A{
public:
    T obj;
    AImpl(T _obj):obj(_obj){}
    ~AImpl(){
        cout << "Deleting " << obj << endl;
    }
};

int main(int argc, char** argv)
{
    AImpl <string>* a1 = new AImpl <string> ("string1234");
    AImpl <int>* a2 = new AImpl <int> (1234);
    AImpl <double>* a3 = new AImpl <double> (1.234);
    vector <shared_ptr<A>> As;
    As.push_back(shared_ptr<A>(a1));
    As.push_back(shared_ptr<A>(a2));
    As.push_back(shared_ptr<A>(a3));
}
