// filesystem 라이브러리
/*
    C++ 17에 도입된 라이브러리이다.
    filesystem 라이브러리는 파일 데이터의 입출력을 담당하는 fstream과는 다르다.
    fstream은 파일의 데이터 입출력 담당하지만 파일에 관한 정보(파일 이름, 위치 등)을 수정할 수는 없다.
    반면 filesystem은 파일에 관한 정보(파일 메타데이타)에 대한 접근을 도와주는 역할을 수행하고
    파일 자체를 읽는 일은 수행하지 않는다.

    어떤 파일을 찾고 싶다면 filesystem을 이용해 찾고, 찾은 파일을 읽고 싶다면 fstream을 사용하면 된다.

    filesystem을 이용해 할 수 있는 일
        1. 원하는 경로에 폴더를 추가
        2. 파일을 삭제
        3. 파일의 정보 확인(파일 생성 시간, 권한 등등)
        등등 을 할 수 있다.

    filesystem 라이브러리의 경우 모든 클래스와 함수들이 std::filesystem의 공간에 정의되어 있다.
    ex) path 클래스 사용하기
        std::filesystem::path() 

    보통 namespace fs = std::filesystem을 추가해 간단하게 사용한다.
*/

// 파일을 찾는 프로그램
#include <filesystem>
#include <iostream>

int main()
{
    std::filesystem::path p("./some_file");

    std::cout << "Does " << p << " exist? [" << std::boolalpha
              << std::filesystem::exists(p) << "]" << std::endl;
    std::cout << "Is " << p << " file? [" << std::filesystem::is_regular_file(p)
              << "]" << std::endl;
    std::cout << "Is " << p << " directory? [" << std::filesystem::is_directory(p)
              << "]" << std::endl;
}