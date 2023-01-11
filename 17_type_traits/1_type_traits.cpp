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

    위와 같은 템플릿 코드도 템플릿 메타 함수들을 안다면 해석할 수 있다.
*/