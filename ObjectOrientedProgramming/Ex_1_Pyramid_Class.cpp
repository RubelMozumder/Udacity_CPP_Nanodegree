#include <cmath>
#include <iostream>

using std::cin;
using std::cout;

class Pyramid {
 private:
  double length_{0};
  double width_{0};
  double height_{0};
  double SetLength_(double length);
  double SetWidth_(double width);
  double SetHeigth_(double heigth);

 public:
  Pyramid(const double length, const double width, const double height)
      : length_(SetLength_(length)),
        width_(SetWidth_(width)),
        height_(SetHeigth_(height)){};
  double GetLength() const { return length_; };
  double GetWidth() const { return width_; };
  double GetHeight() const { return height_; };

  void SetLength(double length);
  void SetWidth(double width);
  void SetHeight(double height);

  double GetVolume() const;
  double GetSurfaceArea() const;
};

void Pyramid::SetLength(const double length) { Pyramid::SetLength_(length); };
void Pyramid::SetWidth(const double width) { Pyramid::SetWidth_(width); };
void Pyramid::SetHeight(const double height) { Pyramid::SetHeigth_(height); };

double Pyramid::GetVolume() const {
  return Pyramid::length_ * Pyramid::width_ * Pyramid::height_ / 3;
};

double Pyramid::GetSurfaceArea() const {
  const double sur1 = Pyramid::length_ * Pyramid::width_;
  const double sur2_4 =
      Pyramid::length_ * (std::sqrt(std::pow(Pyramid::width_ / 2, 2) +
                                    std::pow(Pyramid::height_, 2)));

  const double sur3_5 =
      Pyramid::width_ * (std::sqrt(std::pow(Pyramid::length_ / 2, 2) +
                                   std::pow(Pyramid::height_, 2)));

  return (sur1 + sur2_4 + sur3_5);
};

double Pyramid::SetLength_(double length) {
label:
  try {
    if (length <= 0)
      throw "ZeroOrNegative";
    else
      // Pyramid::length_ = length;
      return length;

  } catch (const char* msg) {
    cout << "\n\n"
         << msg
         << ": Height/Weight/Width is not allowed negative "
            "or "
            "zero."
         << "\n";
    cout << "Try with new integer number "
         << "\n";
    cin >> length;
    goto label;
  }
};

double Pyramid::SetWidth_(double width) {
label:
  try {
    if (width <= 0)
      throw "ZeroOrNegative";
    else
      // Pyramid::width_ = width;
      return width;

  } catch (const char* msg) {
    cout << "\n\n"
         << msg
         << ": Height/Weight/Width is not allowed negative "
            "or "
            "zero."
         << "\n";
    cout << "Try with new integer number "
         << "\n";
    cin >> width;

    goto label;
  }
};

double Pyramid::SetHeigth_(double height) {
label:
  try {
    if (height <= 0)
      throw "ZeroOrNegative";
    else
      // Pyramid::height_ = height;
      return height;

  } catch (const char* msg) {
    cout << "\n\n"
         << msg
         << ": Height/Weight/Width is not allowed negative "
            "or "
            "zero."
         << "\n";
    cout << "Try with new integer number "
         << "\n";
    cin >> height;

    goto label;
  }
};

int main() {
  Pyramid p1(10, 16, 32);
  cout << p1.GetSurfaceArea() << "\n";
  return 0;
}