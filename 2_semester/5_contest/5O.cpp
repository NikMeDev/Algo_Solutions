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
  if(num >= 0) {
    return num + 0.5;
  }
  return num - 0.5;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t size_a;
  std::cin >> size_a;
  ++size_a;
  std::vector<std::complex<double>> polynomial_a(size_a);
  for (size_t i = 0; i < size_a; i++) {
    std::cin >> polynomial_a[i];
  }
  std::reverse(polynomial_a.begin(), polynomial_a.end());
  size_t size_b;
  std::cin >> size_b;
  ++size_b;
  std::vector<std::complex<double>> polynomial_b(size_b);
  for (size_t i = 0; i < size_b; i++) {
    std::cin >> polynomial_b[i];
  }
  std::reverse(polynomial_b.begin(), polynomial_b.end());
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
  std::cout << size_a + size_b - 2 << ' ';
  for (size_t i = 0; i < size_a + size_b - 1; i++) {
    std::cout << Round(std::real(polynomial_c[size_a + size_b - 2 - i]))
              << ' ';
  }
  return 0;
}