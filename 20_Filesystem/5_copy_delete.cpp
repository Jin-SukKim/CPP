// 파일의 복사와 삭제
/*
    기존의 ofstream으로 파일 간의 복사를 할 수 있다.
    (파일 전체 내용을 읽은 후 다른 파일로 출력하는 방식)

    하지만 디렉토리를 복사하거나 디렉토리 안의 모든 파일들을 복사하는 작업 등은 불가능는데
    filesystem의 copy 를 사용해 이 작업들을 수행할 수 있따.

    copy 함수에 복사할 대상과 복사할 위치를 차례대로 인자로 전달하면 된다.

    std::filesystem::copy(from, to, option);
    from과 to는 std::filesystem::path이고 각 경로는 복사할 대상과 복사할 위치이다.
    option은 여러가지 존재한다 (std::filesystem::copy_options::recursive 등등)
    recursive 옵션은 복사할 대상에 있는 모든 디렉토리와 파일들을 복사하고
    recursive 옵션은 안준다면 해당 디렉토리에 존재하는 파일들만이 복사되고
    그 안의 다른 디렉토리들은 복사되지 않는다.

    만약 복사할 대상이 이미 존재한다면 예외를 throw한다.

    filesystem은 이미 존재하는 파일에 대해 선택지를 3가지 제공한다.

        1. skip_existing : 이미 존재하는 파일은 무시 (예외 안던지고)
        2. overwrite_existing : 이미 존재하는 파일은 덮어 씌운다.
        3. update_existing : 이미 존재하는 파일이 더 오래되었을 경우 덮어 씌운다.

  파일 삭제는 remove를 통해서 경로만 전달하면 수행 가능하다.
  디렉토리 역시 삭제 가능하다.
  단 해당 디렉토리는 반드시 빈 디렉토리이여야 한다.
  만약 빈 디렉토리가 아니라면 remove_all 함수를 사용해야된다.

  만약 디렉토리 안에 확장자가 .txt와 같이 특정 파일들만 지우고 싶다면
  directory_iterator를 사용하면된다.
  ex)
    for (const auto &entry : fs::directory_iterator("./a"))
    {
      const std::string ext = entry.path().extension();
      if (ext == ".txt")
      {
        fs::remove(entry.path());
      }
    }
    와 같이 코드를 작성하면 되겠지만 문제가 있다.
    remove 함수로 파일을 삭제할 때 마다 해당 디렉토리의 구조가 변경된다.
    그런데 directory_iterator는 디렉토리 구조가 바뀔 떄 마다 무효화된다.

    따라서 remove 후에 entry 반복자는 사용할 수 없게 되는 것이다.
    ++entry가  다음 디렉토리를 가르키는 것을 보장할 수 없다.

    결국 파일을 삭제할 때 마다 반복자를 초기화 해줘야 된다.
    (아래 코드에 작성되어있다.)
*/

/*
    a 디렉토리 안에 파일 a.txt와 b 디렉토리가 존재하고
    b 디렉토리 안에 b.txt가 존재하고
    c 디렉토리는 비어있다고 가정한다.

    a의 모든 내용을 c에 복사하기
    경로에 있는 파일 삭제하기
*/
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main()
{
  fs::path from("./a");
  fs::path to("./c");

  // 파일 복사
  fs::copy(from, to, fs::copy_options::recursive);

  // 파일 삭제
  fs::path p("./a/b.txt");
  std::cout << "Does " << p << " exist? [" << std::boolalpha
            << std::filesystem::exists(p) << "]" << std::endl;
  fs::remove(p);
  std::cout << "Does " << p << " exist? [" << std::boolalpha
            << std::filesystem::exists(p) << "]" << std::endl;

  // 디렉토리 삭제하기
  std::error_code err;
  fs::remove(p, err); // 실패
  std::cout << err.message() << std::endl;

  fs::remove_all(p); // 성공!

  // 특정 패턴을 가진 파일들만 삭제하기
  fs::path p2("./a");
  
  while (true)
  {
    bool is_modified = false;
    for (const auto &entry : fs::directory_iterator("./a"))
    {
      const std::string ext = entry.path().extension();
      if (ext == ".txt")
      {
        fs::remove(entry.path());
        is_modified = true;
        break;
      }
    }

    if (!is_modified)
    {
      break;
    }
  }
}