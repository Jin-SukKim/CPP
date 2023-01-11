#include "Matrix_overloading_ex.hpp"

// 재귀함수를 사용하는 이유는 for loop를 사용해 N 차원 배열을 만드려면 User가 몇차원 배열을 만들지 모르기 때문에
// for loop가 몇번 사용될 지 모르기 때문에 사용한다.
// address 를 초기화 하는 함수이다. 재귀 호출로 구성되어 있다. (이 함수는 DFS 알고리즘과 동일하다.)
void Array::initialize_address(Address *current)
{
  if (!current)
    return;
  if (current->level == dim - 1)
  { // 두 번째 질문 (종료 조건)
    current->next = new int[size[current->level]];
    return;
  }
  current->next = new Address[size[current->level]];
  for (int i = 0; i != size[current->level]; i++)
  { // 다음 단계로 넘어가는 과정
    // type casting은 4가지 종류가 존재한다. (static_cast, const_cast, dynamic_cast, reinterpret_cast)
    // static_cast : 우리가 흔히 생각하는, 언어적 차원에서 지원하는 일반적인 타입 변환. (원하는 캐스팅 종류)<바꾸려는 타입>(무엇을 바꿀 것인가?)
    (static_cast<Address *>(current->next) + i)->level = current->level + 1;

    initialize_address(static_cast<Address *>(current->next) + i);
  }
}

void Array::delete_address(Address *current)
{
  if (!current)
    return;
  for (int i = 0; current->level < dim - 1 && i < size[current->level]; i++)
  {
    delete_address(static_cast<Address *>(current->next) + i);
  }

  if (current->level == dim - 1)
  {
    delete[] static_cast<int *>(current->next);
    return;
  }

  delete[] static_cast<Address *>(current->next);
}

void Array::copy_address(Address *dst, Address *src) {
  if (dst->level == dim - 1)
   {
     for (int i = 0; i < size[dst->level]; ++i)
       static_cast<int *>(dst->next)[i] = static_cast<int *>(src->next)[i];
     return;
   }
   for (int i = 0; i != size[dst->level]; i++)
   {
     Address *new_dst = static_cast<Address *>(dst->next) + i;
     Address *new_src = static_cast<Address *>(src->next) + i;
     copy_address(new_dst, new_src);
   }
}
