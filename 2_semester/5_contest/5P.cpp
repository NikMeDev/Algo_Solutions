#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <complex>

using namespace std::complex_literals;
constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209;

void FFT(std::vector<std::complex<double>>& polynomial) {
  size_t n = polynomial.size();
  if (n == 1) {
    return;
  }
  std::vector<std::complex<double>> first_half(n / 2);
  std::vector<std::complex<double>> second_half(n / 2);
  for (size_t i = 0; i < n / 2; i++) {
    first_half[i] = polynomial[2 * i];
    second_half[i] = polynomial[2 * i + 1];
  }
  FFT(first_half);
  FFT(second_half);
  std::complex<double> nth_root = std::exp(2i * PI / (double)n);
  std::complex<double> omega = 1;
  for (size_t i = 0; i < n / 2; i++) {
    polynomial[i] = first_half[i] + omega * second_half[i];
    polynomial[i + n / 2] = first_half[i] - omega * second_half[i];
    omega *= nth_root;
  }
}

void IFFT(std::vector<std::complex<double>>& polynomial) {
  size_t n = polynomial.size();
  if (n == 1) {
    return;
  }
  std::vector<std::complex<double>> first_half(n / 2);
  std::vector<std::complex<double>> second_half(n / 2);
  for (size_t i = 0; i < n / 2; i++) {
    first_half[i] = polynomial[2 * i];
    second_half[i] = polynomial[2 * i + 1];
  }
  IFFT(first_half);
  IFFT(second_half);
  std::complex<double> nth_root = std::exp(-2i * PI / (double)n);
  std::complex<double> omega = 1;
  for (size_t i = 0; i < n / 2; i++) {
    polynomial[i] = (first_half[i] + omega * second_half[i]) / (double)2;
    polynomial[i + n / 2] =
        (first_half[i] - omega * second_half[i]) / (double)2;
    omega *= nth_root;
  }
}

int64_t Round(double num) {
  if (num >= 0) {
    return num + 0.5;
  }
  return num - 0.5;
}

std::vector<int64_t> Multiply(std::vector<std::complex<double>> polynomial_a,
                              std::vector<std::complex<double>> polynomial_b) {
  size_t size_a = polynomial_a.size();
  size_t size_b = polynomial_b.size();
  size_t power_of_two = 1;
  while (power_of_two < size_a + size_b) {
    power_of_two *= 2;
  }
  while (polynomial_a.size() < power_of_two) {
    polynomial_a.emplace_back(0);
  }
  while (polynomial_b.size() < power_of_two) {
    polynomial_b.emplace_back(0);
  }
  FFT(polynomial_a);
  FFT(polynomial_b);
  std::vector<std::complex<double>> polynomial_c(power_of_two);
  for (size_t i = 0; i < power_of_two; i++) {
    polynomial_c[i] = polynomial_a[i] * polynomial_b[i];
  }
  IFFT(polynomial_c);
  std::vector<int64_t> result(size_a + size_b - 1);
  for (size_t i = 0; i < size_a + size_b - 1; i++) {
    result[i] = Round(std::real(polynomial_c[i]));
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string string;
  std::cin >> string;
  std::vector<std::complex<double>> poly(string.length());
  for (size_t i = 0; i < string.length(); i++) {
    poly[i] = string[i] - '0';
  }
  std::vector<int64_t> pairs = Multiply(poly, poly);
  size_t result = 0;
  for (size_t i = 0; i < string.size(); i++) {
    if (string[i] == '1') {
      result += pairs[2 * i] - 1;
    }
  }
  std::cout << result / 2;
  return 0;
}