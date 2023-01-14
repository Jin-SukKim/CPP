/*
    filesystem을 이용해 여러가지 디렉토리 작업들을 할 수 있다.
        1. 해당 디렉토리 안에 있는 파일/폴더들 살펴보기
            - directory_iterator
        2. 해당 디렉토리 안에 폴더 생성하기 (파일 생성은 ofstream 으로 할 수 있다)
            - create_directory
        3. 해당 디렉토리 안에 파일/폴더 복사하기
            - std::filesystem::copy
        4. 해당 디렉토리 안에 파일/폴더 삭제하기
            - std::filesystem::remove
        등등

    디렉토리 안에 있는 모든 파일들은 순회하고 싶다면 directory_iterator을 사용하면 된다.

    fs::directory_iterator itr(fs::current_path() / "a"); 와 같이 정의를 한다.
    directory_iterator는 반복자 자체를 스스로 정의해야되고
    반복자 생성시 탐색할 경로를 전달해줘야 된다.

    path 클래스에는 operator/ 가 정의되어 있어 fs::current_path() / "a"와 같이
    현재 경로에 /a 등을 편리하게 추가할 수 있다.

    반복자의 끝은 filesystem에 정의되어 있는 end 함수로 얻을 수 있다.

    각 반복자들은 디렉토리에 정의되어 있는 개개의 파일을 나타내는 directory_entry를 가리키고 있다.
    directory_entry에는 파일의 이름이나 크기, 경로 등 여러가지 정보가 저장되어 있다.

    directory_iterator의 단점은 해당 디렉토리 안에 다른 디렉토리가 있는 경우
    그 안까지는 살펴보지 않는다는 점이다.

    만약 디렉토리 안에 있는 다른 서브 디렉토리들 안까지 모두 살펴보려면
    recursive_directory_iterator를 사용하면 된다.

    fs::recursive_directory_iterator(path)
*/
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
    // 디렉토리 반복자
    fs::directory_iterator itr(fs::current_path() / "a");

    /*
    while (itr != fs::end(itr))
    {
        const fs::directory_entry &entry = *itr;

        // 파일의 경로 알아내기
        std::cout << entry.path() << std::endl;
        itr++;
    }
    */

    // range for 문을 사용해 더 간단히 표현하기
    for (const fs::directory_entry &entry :
         fs::directory_iterator(fs::current_path() / "a"))
    {
        std::cout << entry.path() << std::endl;
    }

    // 해당 디렉토리 내부 다른 디렉토리들 까지 다 살펴보기
    for (const fs::directory_entry &entry :
         fs::recursive_directory_iterator(fs::current_path() / "a"))
    {
        std::cout << entry.path() << std::endl;
    }
}