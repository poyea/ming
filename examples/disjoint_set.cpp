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
  std::vector<ming::DisjointSet<Object>::node_ptr> vec{
      dsu.insert(1), dsu.insert(2, 1.5, 2.5), dsu.insert(3, 1.5, 2.5)};

  dsu.merge(vec[0], vec[1]);
  vec.emplace_back(dsu.insert(4, 1.5, 2.5));
  dsu.merge(vec[2], vec[3]);

  for (std::size_t s = 0; s < vec.size(); ++s) {
    std::cout << "Parent of element " << s << " is "
              << dsu.find(vec[s])->get_object().data << '\n';
  }

  for (std::size_t s = 0; s < vec.size(); ++s) {
    for (std::size_t z = s + 1; z < vec.size(); ++z) {
      std::cout << "Element " << s << " and " << z
                << (dsu.are_same_set(vec[s], vec[z]) ? " are " : " are not ")
                << "in same set\n";
    }
  }
}