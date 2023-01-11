// 대망의 Array 배열
#include <iostream>

namespace MyArray {
    class Array;
    class Int;

    class Array {
    friend Int;

    const int dim;  // 몇 차원 배열 인지
    int* size;  // size[0] * size[1] * ... * size[dim - 1] 짜리 배열이다.

    struct Address {
        int level;
        // 맨 마지막 레벨(dim - 1 레벨) 은 데이터 배열을 가리키고, 그 위 상위
        // 레벨에서는 다음 Address 배열을 가리킨다.
        void* next;
    };

    Address* top;

    public:
        Array(int dim, int* array_size) : dim(dim) {
            size = new int[dim];
            for (int i = 0; i < dim; i++) size[i] = array_size[i];

            top = new Address;
            top->level = 0;

            initialize_address(top);
        }

        Array(const Array& arr) : dim(arr.dim) {
            size = new int[dim];
            for (int i = 0; i < dim; i++) size[i] = arr.size[i];

            top = new Address;
            top->level = 0;

            initialize_address(top);
            // 내용물 복사
            copy_address(top, arr.top);
        }

        void copy_address(Address* dst, Address* src) {
            if (dst->level == dim - 1) {
            for (int i = 0; i < size[dst->level]; ++i)
                static_cast<int*>(dst->next)[i] = static_cast<int*>(src->next)[i];
            return;
            }
            for (int i = 0; i != size[dst->level]; i++) {
            Address* new_dst = static_cast<Address*>(dst->next) + i;
            Address* new_src = static_cast<Address*>(src->next) + i;
            copy_address(new_dst, new_src);
            }
        }

        // address 를 초기화 하는 함수이다. 재귀 호출로 구성되어 있다.
        void initialize_address(Address* current) {
            if (!current) return;
            if (current->level == dim - 1) {
            current->next = new int[size[current->level]];
            return;
            }
            current->next = new Address[size[current->level]];
            for (int i = 0; i != size[current->level]; i++) {
            (static_cast<Address*>(current->next) + i)->level = current->level + 1;
            initialize_address(static_cast<Address*>(current->next) + i);
            }
        }

        void delete_address(Address* current) {
            if (!current) return;
            for (int i = 0; current->level < dim - 1 && i < size[current->level]; i++) {
            delete_address(static_cast<Address*>(current->next) + i);
            }

            if (current->level == dim - 1) {
            delete[] static_cast<int*>(current->next);
            }
            delete[] static_cast<Address*>(current->next);
        }

        Int operator[](const int index);
        
        ~Array() {
            delete_address(top);
            delete[] size;
        }
    };

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
}  // namespace MyArray

int main() {
  int size[] = {2, 3, 4};
  MyArray::Array arr(3, size);

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