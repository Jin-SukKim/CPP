/*
    C++ 표준 라이브러리의 알고리즘(algorithm) 라이브러리는 컨테이너에 반복자들을 가지고 여러가지 작업을 쉽게 수행할 수 있도록 도와주는 라이브러리이다.
    이 작업들은 정렬과 검색같은 단순 작업 말고도 여러 조건을 만족하면 이루어지는 작업등 복잡한 작업도 포함된다.

    이때 함수들은 크게 두가지 형태로 되어있다.
    
    1)  알고리즘을 수행할 반복자의 시작점과 끝점 바로 뒤를 받는다.
        template <typename Iter>
        void do_something(Iter begin, Iter end);

    2)  똑같이 반복자의 시작점과 끝점 바로 뒤를 받지만 '특정한 조건' 을 추가 인자로 받게 된다. 
        특정한 조건은 서술자(Predicate)라 불리며 Pred에는 보통 bool을 return하는 함수 객체(functor)을 전달한다.
        template <typename Iter, typename Pred>
        void do_something(Iter begin, Iter end, Pred pred)
*/