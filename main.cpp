#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#ifdef __cpp_lib_print
#include <print>
#else
#include <iostream>
#endif

struct Teacher {
  std::string name;
  std::string lastname;
};

struct Student {
  std::string name;
  std::string lastname;
};

struct Classroom {
  std::vector<Teacher> teachers;
  std::vector<Student> students;
};

// Отсортировать людей в классе по именам
std::string sorted_classroom_list(Classroom &classroom) {
  std::vector<std::string> names;
  names.reserve(classroom.teachers.size() + classroom.students.size());
  for (const auto &t : classroom.teachers)
    names.push_back(t.name + " " + t.lastname);
  for (const auto &s : classroom.students)
    names.push_back(s.name + " " + s.lastname);

  // UTF-8 байты кириллицы упорядочены так же, как буквы в алфавите
  std::sort(names.begin(), names.end());

  std::string result;
  for (size_t i = 0; i < names.size(); ++i) {
    if (i) result += '\n';
    result += names[i];
  }
  return result;
}

Classroom parse_classroom_from_file(const std::string &path);

int main(int argc, char **argv) {
  if (argc < 2) {
#ifdef __cpp_lib_print
    std::println("usage: {} <path_to_classroom_file>", argv[0]);
#else
    std::cout << "usage: " << argv[0] << " <path_to_classroom_file>" << std::endl;
#endif
    return 0;
  }

  std::string path{argv[1]};
  auto classroom = parse_classroom_from_file(path);
  
#ifdef __cpp_lib_print
  std::println("{}", sorted_classroom_list(classroom));
#else
  std::cout << sorted_classroom_list(classroom) << std::endl;
#endif
}

Classroom parse_classroom_from_file(const std::string &path) {
  std::ifstream file{path};
  if (!file.is_open()) throw std::runtime_error("file not found");
  
  std::string role, name, lastname;
  Classroom classroom;
  
  while (file >> role >> name >> lastname) {
    if (role == "Учитель")
      classroom.teachers.push_back(Teacher{name, lastname});
    if (role == "Ученик")
      classroom.students.push_back(Student{name, lastname});
  }

  if (!file.eof())
    throw std::runtime_error("file reading error");

  return classroom;
}
