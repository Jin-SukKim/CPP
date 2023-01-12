// type_traits 라이브러리
/*
    type_trais 라이브러리는 타입 관련 연산을 위한 템플릿 메타 함수(Template Meta Function)들을 제공해준다.

    template <class _CharT, class _Traits, class _Yp, class _Dp>
    typename enable_if<
        is_same<void, typename __void_t<decltype(
                        (declval<basic_ostream<_CharT, _Traits>&>() << declval<
                            typename unique_ptr<_Yp, _Dp>::pointer>()))>::type>::value,
        basic_ostream<_CharT, _Traits>&>::type
        operator<<(basic_ostream<_CharT, _Traits>& __os,
                unique_ptr<_Yp, _Dp> const& __p) {
        return __os << __p.get();
    }

    이 코드는 표준 라이브러리에 들어가 있는 만큼 최대한 안전해야 할 것이다.
    따라서 템플릿 default 인자로 타입을 체크하는 부분은 9_void_t.cpp에서 한 것과 같이
    함수 return 타입으로 체크하고 있다.

    __void_t<decltype((declval<basic_ostream<_CharT, _Traits>&>() 
                    << declval<typename unique_ptr<_Yp, _Dp>::pointer>()))>::type

    __void_t는 std::void_t와 같은 함수이다.

    위 코드는
    declval<basic_ostream<_CharT, _Traits>&>() << declval<typename unique_ptr<_Yp, _Dp>::pointer>()    
    가 문법적으로 맞는지 확인하고 있다.
    즉 basic_ostream의 operator<<가 unique_ptr의 pointer 타입 객체를 출력할 수 있는지 확인하는 것이다.   
    만일 해당 타입 객체를 출력할 수 있다면 위 __voit_t는 void로 연산될 것이고
    불가능하다면 operator<< 함수는 오버로딩 목록에서 제외될 것이다.

    만약 basic_ostream이 unique_ptr의 pointer 타입을 출력할 수 있다고 한다면

    typename enable_if<
        is_same<void, typename __void_t<decltype(
                        (declval<basic_ostream<_CharT, _Traits>&>() << declval<
                            typename unique_ptr<_Yp, _Dp>::pointer>()))>::type>::value,
        basic_ostream<_CharT, _Traits>&>::type

    이 코드는
    
    typename enable_if<is_same<void, void>::value,
                   basic_ostream<_CharT, _Traits>&>::type
    와 같이 바뀔 것이다.
    is_same은 type_traits에 정의되어 있는 메타 함수로 인자로 전달된 두 타입이 같으면
    value가 true 아니면 value가 false가 되는 메타 함수이다.
    위의 경우는 두 타입이 void로 같기 때문에 is_same<void, void>::value는 true가 된다.

    그럼 위의 코드는
    typename enable_if<true, basic_ostream<_CharT, _Traits>&>::type operator<<(
        basic_ostream<_CharT, _Traits>& __os, unique_ptr<_Yp, _Dp> const& __p) {
        return __os << __p.get();
    }
    가 되고 enable_if 메타 함수에 의해

    basic_ostream<_CharT, _Traits>& operator<<(basic_ostream<_CharT, _Traits>& __os,
                                                unique_ptr<_Yp, _Dp> const& __p) {
        return __os << __p.get();
    }
    가 된다.

    상당히 복잡해 보이는 코드였지만 결국 9_voit_t에서 컨테이너를 사용한 예제와 비슷했다.
    다만 return 값이 basic_ostream<_CharT, Tratis>&였고 is_same과 enable_if를 사용해
    return 타입을 바꿔준 것이다.


    결론

    type_traits에서 메타 함수들을 사용할 수 있다.
    컴파일러는 모든 코드를 치환하는게 아닌 함수의 타입과 템플릿 인자 정의 부분만 살쳐
    SFINAE라는 규칙이 존재한다.
    enable_if를 사용해 원하는 타입만 받는 함수를 작성할 수 있다.
    void_t를 이용해 원하는 타입만 받는 함수를 작성할 수 있다.
*/  