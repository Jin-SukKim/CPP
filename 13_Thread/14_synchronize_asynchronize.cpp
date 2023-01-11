// 동기 (synchronous) 와 비동기 (asynchronous) 실행
/*
    한번에 하나씩 순차적으로 실행되는 작업들을 동기적(sunchronous)으로 실행된다고 한다.
    한 작업이 끝나기 전까지 다음 작업으로 이동하지 않는다.

    반대로 프로그램의 실행이 한 갈래가 아닌 여러갈래로 갈라져서 동시에 진행되는 것을 비동기(asunchronous) 실행이라고 한다.
    쓰레드를 이용해 프로그램을 비동기적으로 만들수도 있고 다른 도구들(std::promise, std::future)을 사용할 수도 있다.

    결국 비동기적으로 실행하고 싶은 일은 어떠한 데이터를 다른 쓰레드를 통해 처리해 받는것이다.
    예)
        string txt = read("a.txt");                  // 5ms
        string result = do_something_with_txt(txt);  // 5ms

        do_other_computation();  // 5ms 걸림 (CPU 로 연산을 수행함)

        이 코드는 동기적인 코드이다.

        read와 do_something_with_txt는 메모리(하드디스크)에서 작동되고 do_other_computation()은 cpu연산을 수행한다.
        하지만 이 코드는 순차적으로 실행되기 때문에 read로 읽어오는 시간동안 CPU는 놀게된다.

        만약 read 함수가 cpu를 사용하면 문제될게 없지만 read는 cpu를 사용하지 않고 그저 하드 디스크에서 읽어오기만
        하기 때문에 CPU를 낭비하고 있는 것과 같다.
    
    쓰레드를 이용하면 비동기 실행을 할 수 있다.

    아래 예제는 thread t를 생서한 후 새로운 쓰레드에서 file_read 함수가 작동한다.
    이때 read 함수가 작동하는 데 CPU는 하드 디스크에서 데이터를 기다리지 않고,
    바로 main 함수로 다시 넘어와서 do_other_computation을 수행하게 된다.

    read하는 동안 do_other_computation을 수행하고 t.join을 수행하며 다시 file_read 함수로 넘어 올텐데
    txt의 내용이 도착해 있을 것이므로 바로 do_something_with_txt가 실행된다.

    즉 CPU를 놀리지 않을 수 있다.
*/

void file_read(string *result)
{
    string txt = read("a.txt"); // (1)
    *result = do_something_with_txt(txt);
}
int main()
{
    string result;
    thread t(file_read, &result);
    do_other_computation(); // (2)

    t.join();
}