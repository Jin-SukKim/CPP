// iterator(반복자) - 컨테이너에 원소에 접근할 수 있는 포인터와 같은 객체
/*
    반복자는 컨테이너에 iterator 멤버 타입으로 정의되어 있습니다.
    vector 의 경우 반복자를 얻기 위해서는 begin() 함수와 end() 함수를 사용할 수 있는데 이는 다음과 같은 위치를 리턴합니다.
        - begin() : 첫번째 원소
        - end()   : 마지막 원소 한칸 뒤 주소

    end()가 마지막 원소를 가르키는 것이 아니라서 이것을 통해 빈 벡터를 표현할 수 있다.
    begin() == end() 라면 원소가 없는 빈 벡터를 의미한다. (end()가 마지막 원소라면 빈 벡터를 표현할 수 없다.)

    *  알고리즘 라이브러리의 경우 대부분이 반복자를 인자로 받아서 알고리즘을 수행한다. *
*/

// 반복자 사용 예시
#include <iostream>
#include <vector>

template <typename T>
void print_vector(std::vector<T> &vec)
{
    // 전체 벡터를 출력하기
    std::cout << "[ ";
    // iterator가 std::vector<T>의 의존 타입이기 떄문에 typename을 붙여준다.
    for (typename std::vector<T>::iterator itr = vec.begin(); itr != vec.end(); ++itr)
    {
        std::cout << *itr << " ";
    }
    std::cout << "]";
}

int main()
{
    std::vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    vec.push_back(40);

    // 전체 벡터를 출력하기
    for (std::vector<int>::iterator itr = vec.begin(); itr != vec.end(); ++itr)
    {
        // 반복자는 포인터처럼 사용한다.
        // 포인터를 오버로딩해서 포인터처럼 동작하게 만든것이다. (*itr은 원소의 reference를 return)
        std::cout << *itr << std::endl;
    }

    // int arr[4] = {10, 20, 30, 40}
    // *(arr + 2) == arr[2] == 30;
    // *(itr + 2) == vec[2] == 30;

    // 포인터와 같이 3번째 원소를 '+'를 이용해서 가르키게 한다.
    std::vector<int>::iterator itr = vec.begin() + 2;
    std::cout << "3 번째 원소 :: " << *itr << std::endl;

    // 반복자를 이용해 insert()와 erase() 함수도 사용할수도 있다.
    std::cout << "처음 벡터 상태" << std::endl;
    print_vector(vec);
    std::cout << "----------------------------" << std::endl;

    // vec[2] 앞에 15 추가
    vec.insert(vec.begin() + 2, 15);
    print_vector(vec);

    std::cout << "----------------------------" << std::endl;
    // vec[3] 제거
    vec.erase(vec.begin() + 3);
    print_vector(vec);

    // vector에서 반복자로 erase나 insert사용시 주의할 점
    // 컨테이너에 원소를 추가하거나 제거하게 되면 기존에 사용하였던 모든 반복자들을 사용할 수 없게됩니다.
    /*
    std::vector<int>::iterator itr = vec.begin();
    std::vector<int>::iterator end_itr = vec.end();

    for (; itr != end_itr; ++itr) {
      if (*itr == 20) {
          // itr는 더이상 유효한 반복자가 아니게된다 또한 end_itr 역시 무효화된다.
        vec.erase(itr);
      }
    }
    */

    std::vector<int> vec2;
    vec2.push_back(10);
    vec2.push_back(20);
    vec2.push_back(30);
    vec2.push_back(40);

    // 오류 고치는 방법 하지만 매번 맨처음으로 돌아가야되서 매우 비효율적이다.
    std::vector<int>::iterator itr = vec2.begin();

    for (; itr != vec2.end(); ++itr)
    {
        if (*itr == 20)
        {
            vec2.erase(itr);
            itr = vec2.begin();
        }
    }

    std::vector<int> vec3;
    vec3.push_back(10);
    vec3.push_back(20);
    vec3.push_back(30);
    vec3.push_back(40);
    vec3.push_back(20);

    // 반복자를 사용하지 않고 erase 함수만 바로 만들어서 전달이 효율적
    // 하지만 권장되는 방법이 아니다.
    // 그 이유는 원소에 접근하는 방식은 반복자를 사용하는 것으로 통일하였는데,
    // 위 방법은 이를 모두 깨버리고 그냥 기존의 배열 처럼 정수형 변수 i 로 원소에 접근하는 것이기 때문이다.
    for (std::vector<int>::size_type i = 0; i != vec3.size(); i++)
    {
        if (vec3[i] == 20)
        {
            vec3.erase(vec3.begin() + i);
            i--;
        }
    }

    std::cout << "값이 20 인 원소를 지운다!" << std::endl;
    print_vector(vec3);

    // const interator
    /*
        vector 에서 지원하는 반복자로 const_iterator 가 있습니다. const 포인터와 같다.
        즉, const_iterator 의 경우 가리키고 있는 원소의 값을 바꿀 수 없다.
    */
    std::vector<int> vec4;
    vec4.push_back(10);
    vec4.push_back(20);
    vec4.push_back(30);
    vec4.push_back(40);

    std::cout << "초기 vec 상태" << std::endl;
    print_vector(vec4);

    // itr 은 vec4[2] 를 가리킨다.
    std::vector<int>::iterator itr = vec4.begin() + 2;

    // vec4[2] 의 값을 50으로 바꾼다.
    *itr = 50;

    std::cout << "---------------" << std::endl;
    print_vector(vec4);

    // const 반복자는 cbegin() 과 cend() 함수를 이용하여 얻을 수 있다. - 값을 변경하지 않고 참조만 하는 경우 유용하다.
    std::vector<int>::const_iterator citr = vec4.cbegin() + 2;

    // 상수 반복자가 가리키는 값은 바꿀수 없다. 불가능!
    // *citr = 30;

    // reverse iterator - 역반복자
    /*
        vector에서 지원하는 반복자로 뒤에서부터 거꾸로 간다.
        역반복자의 rend()의 경우 맨 앞 원소의 바로 앞 주소를 가르킨다.
        값이 증가하면 앞쪽 원소로 간다.
        rbegin()은 맨 마지막 원소를 가르킨다.
    */
    std::cout << "초기 vec 상태" << std::endl;
    print_vector(vec4);

    std::cout << "역으로 vec 출력하기!" << std::endl;
    // itr 은 vec4[2] 를 가리킨다.
    // 타입은 const_reverse_iterator 타입이고, crbegin(), crend() 로 얻을 수 있다.
    std::vector<int>::reverse_iterator r_iter = vec4.rbegin();
    for (; r_iter != vec4.rend(); r_iter++)
    {
        std::cout << *r_iter << std::endl;
    }

    // 역반복자의 사용은 중요하다.
    // 끝에서 부터 출력하기 - 오류발생
    /*
        vector의 index를 담당하는 타입이 부호 없는 정수이기 떄문에 오류가 발생한다.
        즉 i가 0일때 i--를 하면 -1이 아닌 해당 타입에서 가장 큰 정수가 된다.
        결국 for문이 무한루프에 들어간다.

        이 문제를 해결하기 위해서는 부호 있는 정수로 선언해야 하는데,
        vector 의 index 타입과 일치하지 않아서 타입 캐스팅을 해야 한다는 문제가 발생한다.

        따라서 역으로 원소를 참조하고 싶다면, 역반복자를 사용한다.
    */
    for (std::vector<int>::size_type i = vec.size() - 1; i >= 0; i--)
    {
        std::cout << vec[i] << std::endl;
    }
}
