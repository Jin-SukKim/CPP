#include <iostream>
#include "Matrix_overloading_ex.hpp"

class Int {
  void* data;

  int level;
  Array* array;

 public:
  Int(int index, int _level = 0, void* _data = NULL, Array* _array = NULL)
      : level(_level), data(_data), array(_array) {
    if (_level < 1 || index >= array->size[_level - 1]) {
      data = NULL;
      return;
    }
    if (level == array->dim) {
      // 이제 data 에 우리의 int 자료형을 저장하도록 해야 한다.
      data = static_cast<void*>((
          static_cast<int*>(static_cast<Array::Address*>(data)->next) + index));
    } else {
      // 그렇지 않을 경우 data 에 그냥 다음 addr 을 넣어준다.
      data = static_cast<void*>(static_cast<Array::Address*>(
                                    static_cast<Array::Address*>(data)->next) +
                                index);
    }
  };

  Int(const Int& i) : data(i.data), level(i.level), array(i.array) {}

  operator int() {
    if (data) return *static_cast<int*>(data);
    return 0;
  }
  Int& operator=(const int& a) {
    if (data) *static_cast<int*>(data) = a;
    return *this;
  }

  Int operator[](const int index) {
    if (!data) return 0;
    return Int(index, level + 1, data, array);
  }
};

Int Array::operator[](const int index) {
  return Int(index, 1, static_cast<void*>(top), this);
}

int main() {
  int size[] = {2, 3, 4};
  Array arr(3, size);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 4; k++) {
        arr[i][j][k] = (i + 1) * (j + 1) * (k + 1);
      }
    }
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 4; k++) {
        std::cout << i << " " << j << " " << k << " " << arr[i][j][k]
                  << std::endl;
      }
    }
  }
}