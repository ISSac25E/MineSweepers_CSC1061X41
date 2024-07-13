#include <iostream>
#include <stdint.h>

using namespace std;

template <uint16_t M_Size>
class foo
{
public:
  foo()
  {
    cout << "foo: " << M_Size << endl;
  }
  uint8_t arr[M_Size];
};

int main()
{
  foo<4> ff;

  cout << sizeof(ff) << endl;

  foo<128> ff1;

  cout << sizeof(ff1) << endl;
}
