```c++
template< class InputIt, class OutputIt >
OutputIt copy( InputIt first, InputIt last, OutputIt d_first );
1 //fist [IN]: 要拷贝元素的首地址
2 //last [IN]:要拷贝元素的最后一个元素的下一个地址
3 //x [OUT] : 拷贝的目的地的首地址
```

**copy只负责复制，不负责申请空间，所以复制前必须有足够的空间**

```c++
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
 
int main()
{
    std::vector<int> from_vector(10);
    std::iota(from_vector.begin(), from_vector.end(), 0);
 
    std::vector<int> to_vector;
    std::copy(from_vector.begin(), from_vector.end(),
              std::back_inserter(to_vector));
// or, alternatively,
//  std::vector<int> to_vector(from_vector.size());
//  std::copy(from_vector.begin(), from_vector.end(), to_vector.begin());
// either way is equivalent to
//  std::vector<int> to_vector = from_vector;
 
    std::cout << "to_vector contains: ";
 
    std::copy(to_vector.begin(), to_vector.end(),
              std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
 
    std::cout << "odd numbers in to_vector are: ";
 
    std::copy_if(to_vector.begin(), to_vector.end(),
                 std::ostream_iterator<int>(std::cout, " "),
                 [](int x) { return x % 2 != 0; });
    std::cout << '\n';
 
    std::cout << "to_vector contains these multiples of 3: ";
 
    to_vector.clear();
    std::copy_if(from_vector.begin(), from_vector.end(),
                 std::back_inserter(to_vector),
                 [](int x) { return x % 3 == 0; });
 
    for (int x : to_vector)
        std::cout << x << ' ';
    std::cout << '\n';
}
```

