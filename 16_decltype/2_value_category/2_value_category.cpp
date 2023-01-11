// value category
/*
    C++의 모든 식(expression)에는 두 가지 정보가 항상 따라다닌다.
        1. Type
        2. Value Category

    Value Catergory는 총 5가지의 값 카테고리가 존재한다.
    lvalue와 rvalue도 포함되어 있다.

    C++에서 어떤 식의 value category는 두 가지로 분류된다.

    1. 정체를 알 수 있는가? 
        - 정체를 안다는 것은 해당 식이 어떤 다른 식과 같은 것인지 아닌지를 구분할 수 있다는 것이다.
        - 일반적인 변수라면 주소값을 취해 구분할 수 있고, 함수의 경우는 이름을 확인하면 알 수 있다.

    2. 이동시킬 수 있는가?
        - 해당 식을 다른 곳으로 안전하게 이동할 수 있는지의 여부를 묻는다.
        - 해당 식을 받는 이동 생성자, 이동 대입 연산자 등을 사용할 수 있어야 된다.

    value category table

                                      
                            이동 시킬 수 있다         이동 시킬 수 없다 
    정체를 알 수 있다     xvalue(eXpiring value)           lvalue

    정체를 알 수 없다      prvalue(pure rvalue)           쓸모 없음!

    그리고 정체를 알 수 있는 모든 식들을 glvalue(generalized lvalue)라고 하고 이동 시킬 수 있는 모든 식들을 rvalue라고 한다.
    C++에서 실체도 없고 이동도 못 시키는 것들은 언어 상 아무런 의미가 없기에 따로 부르는 명칭은 존재하지 않는다.
*/
