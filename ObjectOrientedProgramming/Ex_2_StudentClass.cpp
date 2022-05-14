#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

using std::cout;

class StudentData {
 private:
  const char* Name_;
  int Grade_;
  double GradePoint_;

  const char* SetName_(const char* name);
  const char* SetName_(std::string name_str);
  int SetGrade_(const int grade);
  double SetGradePoint_(const double gradepoint);

 public:
  StudentData(const char* name, const int grade, const double gradepoint)
      : Name_(SetName_(name)),
        Grade_(SetGrade_(grade)),
        GradePoint_(SetGradePoint_(gradepoint)){};
  StudentData(std::string name_str, const int grade, const double gradepoint)
      : Name_(SetName_(name_str)),
        Grade_(SetGrade_(grade)),
        GradePoint_(SetGradePoint_(gradepoint)){};

  std::string GetName() const;
  int GetGrade() const;
  double GetGradePoint() const;
};

const char* StudentData::SetName_(const char* name) {
  int i = 1;
  while (i) {
    if (isspace(name[i - 1])) {
      break;
      // return name;
    } else if (name[i - 1] == '\0') {
      throw std::invalid_argument(
          "Full name must contain atleast first and second name.");
    };
    i++;
  };
  return name;
};

const char* StudentData::SetName_(std::string name_str) {
  int str_len = name_str.size();
  /*Self Node: the array has been created in the heap memory if it is
   in the stack, then the after exicuting the function SetName_ in constructor
   the memory delete the data in that memory in stack. So, ulmately no
   data will be pointed by the private member char *Name_
   */
  char* name = new char[str_len];
  for (int i = 0; i < str_len + 1; i++) {
    name[i] = name_str[i];
  }
  name[str_len + 1] = '\0';
  int i = 1;
  while (i) {
    if (isspace(name[i - 1])) {
      break;
      // return name;
    } else if (name[i - 1] == '\0') {
      throw std::invalid_argument(
          "Full name must contain atleast first and second name.");
    };
    i++;
  };

  return name;
};

int StudentData::SetGrade_(const int grade) {
  if (grade > 12 || grade < 0)
    throw std::invalid_argument("Grade must be inbetween 0 and 12.");
  else
    return grade;
}

double StudentData::SetGradePoint_(const double gradepoint) {
  if (gradepoint > 4.0 || gradepoint < 0)
    throw std::invalid_argument("Grade Point must be inbetween 0 and 12.");
  else
    return gradepoint;
}

std::string StudentData::GetName() const {
  std::string namexx(StudentData::Name_);
  return namexx;
};

int StudentData::GetGrade() const { return Grade_; };
double StudentData::GetGradePoint() const { return GradePoint_; };

int main() {
  std::string name = "Rubel Mozumder";
  int grade = 10;
  double gradepoint = 3.5;
  try {
    StudentData student(name, grade, gradepoint);
    assert(student.GetGrade() == grade);
    assert(student.GetGradePoint() == gradepoint);
    assert(student.GetName() == name);
  } catch (std::invalid_argument& msg) {
    cout << msg.what() << "\n";
  };
}
