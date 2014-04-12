#include <iostream>
#include <vector>

template <typename C>
void afficheContainer(const C& container, const int n){
  typename C::iterator it();
  int c(0);
  for(it = container.begin(); it != container.end(); it++){
      if(c%n == 0){
          std::cout<<it;
      }
      c++;
  }
  std::cout<<std::endl;
}

int main()
{
    std::vector<int> v(3);
    int i(0);

    for(i=0;i<10;i++){
        v.push_back(i);
    }
    afficheContainer(v);
    std::cout << "Hello World!" << std::endl;
    return 0;
}

