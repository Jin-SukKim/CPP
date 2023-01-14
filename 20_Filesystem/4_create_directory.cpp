/*
    ofstream을 사용하면 파일을 간단하게 생성할 수 있다.
    ex)
        std::ofstream out("a.txt");
        out << "hi";

    하지만 ofstream으로는 디렉토리를 생성할 수 없다.
    ex) b라는 폴더가 없다면 a.txt도 생성할 수 없고 out << 는 실패한다.
        std::ofstream out("./b/a.txt");
        out << "hi";

    filesystem에서 제공하는 create_directory 함수를 사용해 디렉토리를 생성할 수 있다.
    std::filesystem::create_directory() 함수는 주어진 경로를 인자로 받아 디렉토리를 생성한다.
    ex) std::filesystem::create_directory(std::filesystem::path) 
    주의할 점은 생성하는 디렉토리의 부모 디렉토리는 반드시 존재해야 된다.
    존재하지 않는다면 예외를 throw한다.

    부모 디렉토리들까지 한번에 만들고 싶다면 create_directories 함수를 사용하면 된다
    ex) std::filesystem::create_directories(std::filesystem::path) 
*/

// 디렉토리 생성
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    fs::path p("./a/c");
    std::cout << "Does " << p << " exist? [" << std::boolalpha << fs::exists(p)
              << "]" << std::endl;

    // 디렉토리 생성
    fs::create_directory(p);

    std::cout << "Does " << p << " exist? [" << fs::exists(p) << "]" << std::endl;
    std::cout << "Is " << p << " directory? [" << fs::is_directory(p) << "]"
              << std::endl;

    // 부모 디렉토리까지 한꺼번에 생성
    fs::path p2("./c/d/e/f");
    std::cout << "Does " << p2 << " exist? [" << std::boolalpha << fs::exists(p2)
              << "]" << std::endl;

    fs::create_directories(p2);

    std::cout << "Does " << p2 << " exist? [" << fs::exists(p2) << "]" << std::endl;
    std::cout << "Is " << p2 << " directory? [" << fs::is_directory(p2) << "]"
              << std::endl;
}