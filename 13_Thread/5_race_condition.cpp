// Race Condition : 경쟁 상태
/*
    race condition은 쓰레드들이 같은 자원을 공유할 때 발생하는 문제이다.

    CPU는 연산을 수행하기 위해 register에 데이터를 기록한 다음 연산을 수행한다.
    이 레지스터의 크기는 매우 작은데 64bits 컴퓨터는 register의 크기가 8 byte뿐이다.
    그리고 이 register의 개수도 많지 않은데 일반적인 연산에 사용되는 범용 register는 16개뿐이다.
    (32bits 컴퓨터는 8개뿐이었다.)

    쓰레드는 메모리를 공유하지만 Register는 공유하지 않는다.
    각 쓰레드 별로 고유한 register를 가지고 있다.

    즉, 모든 데이터들이 메모리에 저장되어 있고 연산할 때 마다 메모리(RAM)에서 register(CPU)로 값을 가져온 뒤,
    빠르게 연산을 수행하고 다시 메모리에 가져다 놓는다.
    ex) counter += 1 의 실제 작동방법(Assembly Code), 어셈블리 코드는 기계어와 1대 1 대응이다. (CPU가 한줄 한줄 시행한다.)

        // rax, rbp 모두 CPU의 Register이다. mov는 대입 명령어로 [rbp - 8] 값의 rax에 대입된다.
        // []는 역참조로 rbp - 8을 주소값이라 했을때 해당 주소에 있는 값을 읽어라는 의미이고 qword는 8byte를 의미한다.
        // 즉 C++ 포인터에 * 연산과 같이 rbp - 8 주소에서 8byte 만큼 값을 읽어오라는 의미이다.
        mov rax, qword ptr [rbp - 8]    // rax = *(int**)(rbp - 8)와 같다. rbp - 8에는 counter의 주소값이 있다.
        mov ecx, dword ptr [rax]        // ecx = *(int*)(rax); rax에는 result의 주소값이 있다. 결국 ecx에는 result의 현재 값이 들어간다.
        add ecx, 1                      // ecx에 1을 더한다.
        mov dword ptr [rax], ecx        // result 현재값에 1을 더한 값을 다시 result에 넣어준다.

    multi-thread는 언제 context switching이 일어날 지 모른다.
    쓰레드는 register를 공유하지 않으므로 쓰레드1과 쓰레드2의 rax, ecx는 다르다는 의미이다.
    만약 counter의 값이 0일때 쓰레드1에서 두번쨰 줄의 Assembly코드까지 작동하다 context switch가 일어나 쓰레드2가 실행된다면
    register가 공유되지 않으므로 쓰레드1과 쓰레드2 모두 ecx값이 0이되고 1이 각각 더해진다고 하지만 결과는 1이 한번만 더해진
    1이 된다.

    쓰레드가 어떻게 실행되지는 운영체제에서 결정되기 때문에 결국 race condition 문제가 있는 프로그램은 실행 결과가 원하는 값이
    않나오고 무작위로 결과가 나온다는 것이다.

    결국 제대로 프로그램을 안만들면 디버깅이 매우 어려워진다.

    이 문제를 해결하기 위해 mutex를 사용한다.

*/
// 서로 다른 쓰레드들이 counter 변수의 값을 1씩 증가시키는 프로그램이다.
#include <iostream>
#include <thread>
#include <vector>

// 1씩 더해서 총 40000이 되어야 되지만 정확히 더한값이 나오지 않느다.
void worker(int &counter)
{
    for (int i = 0; i < 10000; i++)
    {
        counter += 1;
    }
}

int main()
{
    int counter = 0;

    std::vector<std::thread> workers;
    for (int i = 0; i < 4; i++)
    {
        // 레퍼런스로 전달하려면 ref 함수로 감싸야 한다 (지난 강좌 bind 함수 참조)
        workers.push_back(std::thread(worker, std::ref(counter)));
    }

    for (int i = 0; i < 4; i++)
    {
        workers[i].join();
    }

    std::cout << "Counter 최종 값 : " << counter << std::endl;
}