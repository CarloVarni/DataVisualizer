

#include <MultiData.hpp>
#include <type_traits>
#include <iostream>

int main() {

  std::vector<std::string> label{"uno", "due", "tre"};
  std::vector<
    std::variant< int, double, float, bool>
    >
    data{2, 4.1, 123};
  
  MultiData A(label, data, 1.2);
  std::cout << A << std::endl;

}
