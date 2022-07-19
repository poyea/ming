#include <ming/disjoint_set.hpp>

#include <iostream>
#include <vector>

struct Object {
  Object(int d) : data(d), flt(0), dbl(0) {}
  Object(int d, float f, double b) : data(d), flt(f), dbl(b) {}
  int data;
  float flt;
  double dbl;
};

int main(void) {
  ming::DisjointSet<Object> dsu;

  std::vector<ming::DisjointSet<Object>::Iterator> vec;

  auto it1 = dsu.insert(1), it2 = dsu.insert(2, 1.5, 2.5),
       it3 = dsu.insert(3, 1.5, 2.5);
  vec.push_back(it1);
  vec.push_back(it2);
  vec.push_back(it3);

  std::cout << (vec[0])->get()->get_object().data << '\n';
  std::cout << (vec[1])->get()->get_object().data << '\n';
  std::cout << (vec[2])->get()->get_object().data << '\n';

  dsu.merge(vec[0], vec[1]);

  auto it4 = dsu.insert(4, 1.5, 2.5);
  vec.push_back(it4);
  dsu.merge(vec[2], vec[3]);

  for (std::size_t s = 0; s < vec.size(); ++s) {
    std::cout << "Parent of element " << (s + 1) << " is "
              << (dsu.find(vec[s]))->get()->get_object().data << '\n';
  }

  for (std::size_t s = 0; s < vec.size(); ++s) {
    for (std::size_t z = s + 1; z < vec.size(); ++z) {
      std::cout << "Element " << s << " and " << z
                << (dsu.are_same_set(vec[s], vec[z]) ? " are " : " are not ")
                << "in same set\n";
    }
  }

  return 0;
}