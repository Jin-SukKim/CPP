// Thread(쓰레드)
/*
    CPU 코어에서 돌아가는 프로그램 단위를 Thread라고 한다.
    즉 CPU 코어 하나에서는 한 번에 한개의 쓰레드 명령을 실행시키게 된다.

    한 개의 프로세스는 최소 한 개 이상의 Thread로 이루어져 있으며 여러 개의 Thread로 구성될 수 있다.

    Thread와 Process의 가장 큰 차이점은 프로세스들은 서로 메모리를 공유하지 않는다.
    즉 프로세스 1과 2가 있다면 서로의 메모리에 접근할 수 없지만 한 프로세스안의 쓰레드들은 서로 같은 메모리를 공유한다.
    프로세스 1에 쓰레드 1, 2, 3이 있다면 쓰레드 1 2 3은 서로 같은 메모리를 공유해 같은 변수의 값에 접근할 수 있게 된다.

    최신 CPU들은 하나의 코어가 아닌 여러개의 코어를 탑재해서 나온다.
    따라서 싱글 코어 CPU는 아무리 Multi-thread 프로그램이라 할지라도 결국에는 한 번에 한 쓰레드만
    실행할 수 있었지만(스위칭) 멀티 코어 CPU는 여러개의 코어에서 각기 다른 쓰레드들이 동시에 실행될 수 있따.

    Multi-Thread 사용하는 이유

    1. 병렬 가능한(Parallelizable) 작업들
        ex) 만약 1부터 1000000까지 수를 더하는 프로그램이 필요한 경우
            하나의 for 문으로 돌리면 매우 오랜 시간이 걸린다.
            하지만 만약 Thread를 여러개 만들어 숫자를 여러 범위로 나눠서 계산 후 합쳐준다면 훨씬 빠를것이다.
            1~1000, 1001~2000 ... 등 10개 threads를 만들어 각기 범위대로 더한후 결과를 합쳐주는 식이다.
            만약 숫자를 더할때 1초가 걸린다면 thread하나로 for문을 돌리면 10000000초가 걸리지만
            10개로 나누다면 1000초가 걸리고 10개로 나온 값을 더하는 10초를 포함해 1010초만 걸리는 것이다.
        
        물론  모든 프로그램이 가능한 것은 아니다. 피보나치 수열의 경우 계산하기 위해 이전에 계산된 값이 필요하므로
        이러한 병렬 계산이 불가능하다.
        이렇게 어떠한 연산(A)을 수행하기 위해 다른 연산(B)의 결과가 필요한 상황을 A가 B에 의존(dependent)한다라고 한다.

    2. 대기시간이 긴 작업들
        ex) 인터넷에서 웹사이트를 긁어오는 웹 클롤링같은 경우
            download하기 위해 url에 접속해야된다. 하지만 CPU의 처리속도보다 인터넷의 속도는 매우 느리다.
            흔히 ping이란 서버에 요청을 보내 돌아오는 신호의 시간을 의미하는데 보통 한국은 30ms, 해외의 경우 150~300ms가 걸린다.
            보통 CPU는 1초에 10^9번 연산을 할 수 있다. 
            즉 이 서버의 응답을 기다리는 0.15초 동안 1.5 * 10^8번 연산할 수 있는 시간을 버리게 되는 것이다. (매우 비효율적)

            하지만 멀티 쓰레드를 이용해 context switching을 해 요청을 보내고 기다리는 동안 다른 요청을 보내느 형식으로
            프로그램이 작동한다면 매우 효율적이게 CPU를 활용할 수 있게된다.

    Thread를 생성하고
    mutex를 이용해 race condition을 방지하고
    condition variable을 통해 Producer - Consumer 패턴을 구현하고
    Atomic 객체를 통해 원자적 연산을 수행하고
    memory_order을 통해 컴파일러가 어떤 식으로 명령어를 재배치할지 설정한다.
        

*/