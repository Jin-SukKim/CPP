// filesystem 의 path 클래스
/*
    path 클래스 객체 선언
    ex)
        std::filesystem::path p("./some_file");

    컴퓨터 상의 모든 파일은 해당 파일 위치를 나타내는 고유의 주소가 있는데 이를 경로(Path)라고 한다.
    경로라 불리는 이유는 컴퓨터가 해당 파일을 참조할 때 가장 맨 처음 디렉토리부터 순차적으로 찾아가기 때문이다.
    ex) a/b/c의 경우
        /a directory를 찾고 그 안의 b 디렉토리 찾고 그 안의 c를 찾아 들어간다.

    이때 경로를 지정하는 방식이 두 가지가 있다.
        1. 절대 경로(Absolute Path)
            -   가장 최상위 디렉토리(root 디렉토리)에서 원하는 파일까지의 경로를 의미한다.
                (root 디렉토리는 윈도우에서는 보통 C:\나 D:\, Linux는 / 가 된다.)
        2. 상대 경로(Relative Path)
            -   현재 프로그램이 실행되고 있는 위치에서 해당 파일을 찾아가는 경로를 의미한다.
                ex) 경로가 a/b라 했을 때 현재 프로그램의 실행 위치에서 a라는 디렉토리를 차고
                    그 안의 b 파일을 찾는 것이다.

        ./some_file은 맨 앞이 / 나 C:\ 등이 아니라 상대 경로이다.
        . 는 현재 디렉토리를 의미하는 문자이다.
        결과적으로 현재 프로그램이 실행되고 있는 위치에 존재하는 some_file을 나타낸다.

    filesystem 라이브러리에서 파일이나 디렉토리를 다루는 모든 함수들은
    파일을 나타내기 위해서 path 객체를 인자로 받는다.

    1. 원하는 경로에 있는 파일/디렉토리의 path 를 정의
    2. 해당 path 로 파일/디렉토리 정보 수집
    순서로 보통 작업을 한다.

    다만 path 클래스는 그냥 경로를 나타낼 뿐 실제 파일을 지칭하는 것은 아니다.
    해당 경로에 파일이 실제로 존재하는지 확인하려면 exists 함수를 사용해야 된다.
    ex)
        std::filesystem::exists(p)

    그리고 해당 위치에 있는게 파일인지 디렉토리인지 확인하려면
    is_regular_file 함수와 is_directory 함수로 확인할 수 있다.

    참고로 is_file이 아닌 is_regular_file인 이유는 Linux에서는 주변 장치(device)나
    소켓(socket)들도 다 파일로 취급하기 떄문이다.

    filesystem 라이브러리에서는 경로를 가지로 여러가지 작업을 할 수 있는 함수들을 제공한다.

        1. std::filesystem::current_path()
            -   프로그램이 실행되는 경로를 return
                모든 상대 경로는 프로그램의 현재 실행 경로를 기반으로 구한다.
        2. std::filesystem::path::relative_path()
            -   path에 저으이된 상대경로
        3. std::filesystem::absolute(p)
            -   path에 정의된 상대경로를 절대경로로 바꾸기
                단점으로 .이나 .. 같은 불필요한 요소들이 포함될 수 있다.
        4. std::filesystem::canonical(p)
            -   해당 파일의 경로를 가장 짧게 나타낼 수 있는 공식적인 절대 경로를 제공
                불필요한 요소를 제외하고 깔끔하게 경로 표현할 수 있다.

        이 함수들은 파일이 존재하지 않으면 예외를 throw 하므로 exists 함수를 이용해
        파일 존재 여부를 확인해야 된다.
        예외를 처리하고 싶지 않다면 마지막 인자로 발생한 오류를 받는 std::error_code 객체를 전달하면 된다.
        filesystem에서 예외를 던지는 함수들의 경우 마지막 인자로 error_code를 받는 오버로딩을 제공한다.

*/

// 파일 위치 찾는 프로그램
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    // 파일의 경로
    std::filesystem::path p("./some_file");

    // exists 함수를 이용해 실제 파일이 존재하는 지 확인한다.
    std::cout << "Does " << p << " exist? [" << std::boolalpha
              << std::filesystem::exists(p) << "]" << std::endl;

    // 해당 위치에 있는게 파일인지 확인하기
    std::cout << "Is " << p << " file? [" << std::filesystem::is_regular_file(p)
              << "]" << std::endl;

    // 해당 위치에 있는게 디렉토리인지 확인하기.
    std::cout << "Is " << p << " directory? [" << std::filesystem::is_directory(p)
              << "]" << std::endl;

    std::cout << "내 현재 경로 : " << fs::current_path() << std::endl;
    std::cout << "상대 경로 : " << p.relative_path() << std::endl;
    std::cout << "절대 경로 : " << fs::absolute(p) << std::endl;
    std::cout << "공식적인 절대 경로 : " << fs::canonical(p) << std::endl;
}