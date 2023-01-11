

class Array
{
  friend Int;

  const int dim; // 몇 차원 배열인지
  int *size;     // size[0] * size[1] * ... * size[dim - 1] 짜리 배열이다.

  struct Address
  {
    int level;
    // 맨 마지막 레벨(dim - 1 레벨) 은 데이터 배열을 가리키고, 그 위 상위
    // 레벨에서는 다음 Address 배열을 가리킨다.
    void *next;
  };

  Address *top; // 제일 위에 존재하는 시작점

public:

  void delete_address(Address *current);
  void initialize_address(Address *current);
  void copy_address(Address *dst, Address *src);

  Array(int dim, int *array_size) : dim(dim)
  {
    size = new int[dim];
    for (int i = 0; i < dim; i++)
      size[i] = array_size[i];

    top = new Address;
    top->level = 0;

    initialize_address(top);
  }

  Array(const Array &arr) : dim(arr.dim)
  {
    size = new int[dim];
    for (int i = 0; i < dim; i++)
      size[i] = arr.size[i];

    top = new Address;
    top->level = 0;

    initialize_address(top);

    // 내용물 복사
    copy_address(top, arr.top);
  }

  Int Array::operator[](const int index) {
                return Int(index, 1, static_cast<void*>(top), this);
  }
  
  ~Array()
  {
    delete_address(top);
    delete[] size;
  }

  
};