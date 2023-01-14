// Random Number를 쉽게 생성해주는 random 라이브러리
/*
    C 스타일 난수 생성에는 문제가 있다(stdlib.h, time.h 등)

    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>

    int main() {
        srand(time(NULL));

        for (int i = 0; i < 5; i++) {
            printf("난수 : %d \n", rand() % 100);
        }
        return 0;
    }

    이 C 스타일 난수 생성기는 1~99 까지 수를 생성한다.

    엄밀히 말하면 진짜 난수를 생성하는게 아닌 난수처럼 보이는
    의사 난수(pseudo random number)를 생성하는 것이다.

    컴퓨터 상에서 완전히 무작위로 난수를 생성하는 것은 어렵다.

    대신 첫 번째 수만 무작위로 정하고 나머지는 그 수를 기반으로 여러가지 수학적 기법을
    이용해 마치 난수처럼 보이지만 실제로는 무작위로 생성된 것이 아닌 수열을 만들게 된다.

    무작위로 정해진 첫 번째 수를 시드(seed)라고 부르는데, C의 경우 srand를 통해 seed를 설정한다.
    위의 경우 time(NULL)을 이용해 프로그램 실행했던 초를 seed로 설정했다.

    하지만 이 코드는 문제점이 존재한다.
    1. 시드값이 너무 천천히 변한다.
        -   시드값을 현재의 초(sec)로 지정했다. 즉 같은 시간대에 시작된 프로그램의 경우 모두 같은
            난수를 생성하게 된다.

    2. 0 부터 99까지 균등하게 난수를 생성하지 않는다.
        -   rand()가 return하는 값이 0부터 RAND_MAX라는 점이다.
            0부터 RAND_MAX까지의 모든 값들을 같은 확률로 난수 생성하지만
            100으로 나눈 나머지는 꼭 그러라는 법이 없다.
            1의 경우 rand()가 return한 값이 1이거나 101일 때 생성하지만 50은 rand()가 return한 값이 50일때
            생성되므로 1이 뽑힐 확률이 50이 뽑힐 확률보다 2배나 높다.

    3. rand()자체도 뛰어나지 않다.
        -   C의 rand()함수는  선형 합동 생성기 (Linear congruential generator) 이라는
            알고리즘을 바탕으로 구현되어 있는데 뛰어난 난수 생성 알고리즘은 아니다.
            몇 시뮬레이션에 사용할 수 없다.

    C++은 random 라이브러리를 사용해 난수를 생성한다.
    C++은 양질의 seed를 생성하기 위해 random_device를 제공한다.

    대부분 OS는 진짜 난수값을 얻을 수 있는 여러가지 방식을 제공하고 있다.
    ex) Linux
            - /dev/random, /dev/urandom 을 통해 난수값을 얻는다.

        수학적 알고리즘이 아닌 컴퓨터가 실행되며 얻는 무작위 요소들(ex: 장치드라이버의 noise 등)을
        기반으로 진정한 난수를 생성한다.

    std::random_device는 OS 단에서 제공하는 진짜 난수를 사용할 수 있다.
    다만 진짜 난수의 경우 컴퓨터가 주변 환경과 무작위적으로 상호작용하며 만들어지기에 속도가 느리다.
    따라서 seed 값을 초기화하는데 사용하고 이후엔 난수 생성 엔진으로 생성하는게 낫다.

    난수 생성 엔진은 여러가지가 존재하는데 아래 코드에선 mt19937을 사용했다.
    std::mt19937 gen(std::random_device)와 같이 할 수도 있고 원하는 정수를 그냥 넣어도 된다.
    std::mt19937은 메르센 트위스터 라는 알고리즘을 사용한다.
    하지만 객체의 크기가 커서(2KB 이상) C의 rand 함수와 같은 알고리즘(선형 합동 알고리즘)을 쓰는
    minstd_rand를 메모리가 부조학 시스템에서 사용할 수도 있다.

    C++에선 난수 생성 엔진을 만들어도 바로 난수를 생성할 수 있는 것이 아니라
    어디에서 수들을 뽑아낼지 알려주는 분포(distribution)을 정의해야 된다.

    이번 코드의 경우 균등한 확률로 정수를 뽑고 싶으므로 uniform distribution(균등 분포) 객체를 정의했다.
    ex)
        std::uniform_int_distribution<int> dis(0, 99);

    생성한 분포 객체에 난수 생성 엔진을 전달해 난수를 생성한다.
    ex)
        dis(gen)

    분포들의 종류는 여러가지가 존재한다.
    (uniform distribution, normal distribution 등등)
*/
#include <iostream>
#include <random>

int main()
{
    // 시드값을 얻기 위한 random_device 생성.
    std::random_device rd;

    // random_device 를 통해 난수 생성 엔진을 초기화 한다.
    std::mt19937 gen(rd());

    // 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
    std::uniform_int_distribution<int> dis(0, 99);

    for (int i = 0; i < 5; i++)
    {
        std::cout << "난수 : " << dis(gen) << std::endl;
    }

    // normal distribution example (정규 분표)
    std::normal_distribution<double> dist(/* 평균 = */ 0, /* 표준 편차 = */ 1);

    std::map<int, int> hist{};
    for (int n = 0; n < 10000; ++n)
    {
        ++hist[std::round(dist(gen))];
    }
    for (auto p : hist)
    {
        std::cout << std::setw(2) << p.first << ' '
                  << std::string(p.second / 100, '*') << " " << p.second << '\n';
    }
}