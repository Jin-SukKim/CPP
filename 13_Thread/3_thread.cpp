// Thread Creating
/*
    C++ 11전엔 표준 쓰레드가 없어 윈도우는 CreateThread, 리눅스는 pthread_create를 써야됬지만
    표준 쓰레드가 생겨 thread 라이브러리를 활용해 쉽게 만들 수 있게 됬다.
    (리눅스에서 컴파일 할 경우 옵션에 -pthread를 추가해야 된다.)

    여러 쓰레드들이 CPU 코어에서 어떻게 할당되고 또 언제 Context switching될지는 운영체제에서 컨트롤된다.

    thread 사용법

        thread t1(인자) 로 쓰레드를 생성할 수 있다.

        join 함수는 해당하는 쓰레드가 실행을 종료하면 return하는 함수이다.
        따라서 해당 쓰레드가 종료되지 않으면 return되지 않는다.
        join 함수의 쓰레드 종료 순서는 중요하지 않다.

        하지만 join을 하지 않으면 프로그램이 종료된 후 쓰레드의 소멸자가 호출되는 예외가 나온다.

        C++ 표준에선 join이나 detach 되지 않은 쓰레드들의 소멸자가 호출된다면 예외를 발생시키도록 되어있다.

        detach 함수는 해당 쓰레드를 실행시킨 후, 잊어버리는 것이다.
        대신 쓰레드가 알아서 백그라운드에서 돌아가게 된다.

        기본적으로 프로세스가 종료될 때 해당 프로세스의 모든 쓰레드들은 종료 여부와 상관없이 자동으로 종료된다.
        하지만 쓰레드를 detach한다면 프로세스는 쓰레드가 종료되는 것을 기다리지 않는다.

        즉 코드가 쓰레드의 종료를 기다리지 않고 끝까지 실행된 뒤 main함수가 끝나지만 쓰레드는 아직 끝나지 않았으므로
        백그라운드에서 쓰레드가 종료될 때 까지 실행된다.    

        쓰레드는 return값이 없어서 만약 어떤 결과를 저장하고 싶다면 포인터의 형태로 전달하면 된다.
*/
#include <iostream>
#include <thread>
using std::thread;

void func1()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "쓰레드 1 작동중! \n";
    }
}

void func2()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "쓰레드 2 작동중! \n";
    }
}

void func3()
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "쓰레드 3 작동중! \n";
    }
}
int main()
{
    thread t1(func1);
    thread t2(func2);
    thread t3(func3);

    t1.join();
    t2.join();
    t3.join();

    thread t1(func1);
    thread t2(func2);
    thread t3(func3);

    t1.detach();
    t2.detach();
    t3.detach();

    std::cout << "메인 함수 종료 \n";
}