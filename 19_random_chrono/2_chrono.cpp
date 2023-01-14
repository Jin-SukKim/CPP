// 시간관련 라이브러리 chrono
/*
    chrono 라이브러리는 크게 3가지 요소들로 이루어져 있다.

        1. 현재 시간을 알려주는 시계 - 예를 들어서 system_clock
        2. 특정 시간을 나타내는 time_stamp
        3. 시간의 간격을 나타내는 duration

    chrono에서 제공하는 clock들
        1. std::system_clock
            - 현재 컴퓨터 상의 시간
        2. std::high_resolution_clock
            - 좀더 정밀한 시간 계산이 필요한 경우(프로그램 성능 측정 등)

        객체 이름이 시계이지만 실제 시계처럼 몇시 몇분을 알려주는 것이 아니라
        지정된 시간으로 부터 몇번의 틱(tick)이 발생했는지 알려주는 time_stamp 객체를 return한다.
        ex)
            std::system_clock의 경우 1970년 1월 1일부터 현재까지 발생한 tick의 횟수를 return한다.

        time_stamp 객체는 clock의 시작점과 현재 시간의 duration을 보관하는 객체이다.(흔히 Unix time이라 한다.)

        tick은 시계의 초침이라고 생각하면 되는데 컴퓨터도 내부에 시계가 있어 특정 진동수로 똑딱거린다.

        각 시계마다 정밀도가 다르기에 각 clock에서 얻어지는 tick값이 조금씩 다르다.
        ex)
            std::system_clock이 1초에 1tick이라면
            std::high_resolution_clock은 0.00000001 초 마다 1 tick이다.

    chrono 라이브러리의 경우 다른 라이브러리와는 다르게 std::chrono에 객체들이 정의되어 있다.
    따라서 high_resolution_clock을 사용하기 위해 std::chrono::high_resolution_clock이라고 정의해야된다.

    만약 std::chrono를 항상 쓰는게 번거롭다면 namespace ch = std::chrono라는 별칭을 지어줘도 된다.

    clock에는 현재의 time_point를 return하는 static 함수인 now가 정의되어 있다.
    now()를 호출하면 clock에 맞는 time_point 객체를 return한다.
    ex)
        std::chrono::time_point<ch::high_resolution_clock>를 return한다.

    time_point가 템플릿인 이유는 clock마다 1초에 발생하는 tick의 횟수가 다르기 때문에
    실제 시간으로 변환시 어떤 clock을 사용했는지 알아야 하기 때문이다.

    아래 코드에서 시간을 측정하기 위해 난수 생성 시작 시간과 끝난 시간을 받았다.

    시간을 계산하기 위해 auto diff = end - start; 를 해줬다.
    두 time_stamp를 빼게 된다면 duration 객체를 return한다.
    (C++ 이전에는 duration 객체의 템플릿 인자를 전달해야 됬지만 이젠 그냥 auto를 사용하면 된다.)

    이 duration에는 count라는 멤버 함수가 정의되어 있는데, 이것은 해당 시간 차이 동안
    몇 번의 tick이 발생했는지 알려준다.
    하지만 tick이 발생한 횟수가 아닌 좀 더 의미있는 정보인 실제 시간을 알려면 duration_cast를 사용하면 된다
    ex)
        ch::duration_cast<ch::microseconds>(diff).count()

    duration_cast는 임의의 duration 객체를 받아서 원하는 duration으로 캐스팅할 수 있다.
    std::chrono::microseconds는 chrono에 미리 정의되어 있는 duration 객체들 중 하나로
    1초에 10^6번 tick을 하게 한다. 즉 microseconds로 캐스팅한 후 return하는 count값은
    해당 duration이 몇 microseconds인지를 나타낸다.
    (microseconds 말고도 anoseconds, milliseconds, seconds, minutes, hours 등이 있다.)

    chrono 라이브러리 상에서 날짜를 간단하게 다룰 수 있도록 도와주는 클래스가 없다.
    예를 들어 현재 시간을 출력하고 싶다면 C의 함수들에 의존해야 된다.
    ex) 
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        system_clock에서 현재의 time_point를 얻어온 이후 날짜 출력을 위해 time_t로 변환해야된다.
        chrono에서 제공하는 static 함수인 to_time_t 를 이용하면 된다.

        std::put_time(std::localtime(&t), "%F %T %z")
        현재 시간을 std::tm 객체로 변환하기 위해 std::localtime에 time_t를 전달한 후
        std::put_time을 이용해 원하는 형태의 문자열로 구성할 수 있게 한다.
        %F %T %z는 strftime에서 사용하는 인자와 동일하다.
*/
// 난수 생성 속도를 측정하는 시간 프로그램
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
namespace ch = std::chrono;

int main()
{
    // 현재 시간을 출력하는 코드
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::cout << "현재 시간은 : " << std::put_time(std::localtime(&t), "%F %T %z")
              << '\n';
              
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, 1000);

    for (int total = 1; total <= 1000000; total *= 10)
    {
        std::vector<int> random_numbers;
        random_numbers.reserve(total);

        std::chrono::time_point<std::chrono::high_resolution_clock> start =
            std::chrono::high_resolution_clock::now();

        for (int i = 0; i < total; i++)
        {
            random_numbers.push_back(dist(gen));
        }

        std::chrono::time_point<std::chrono::high_resolution_clock> end =
            std::chrono::high_resolution_clock::now();

        // C++ 17 이전
        // std::chrono::duration diff = end - start;

        // C++ 17 이후
        auto diff = end - start;

        // tick 발생 횟수
        std::cout << std::setw(7) << total
                  << "개 난수 생성 시 틱 횟수 : " << diff.count() << std::endl;

        // 실제 걸린 시간
        std::cout << std::setw(7) << total << "개 난수 생성 시 걸리는 시간: "
                  << ch::duration_cast<ch::microseconds>(diff).count() << "us"
                  << std::endl;
    }
}