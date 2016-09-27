#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <omp.h>


class X
{
  int value;


public:
  X(int n)
    : value(n)
  { }

  X()
  {
    value = rand();
  }

// Fragment klasy zalany kawą
  static long long int count_less;
  static long long int count_eq;

  static void reset()
  {
	  X::count_less = 0;
	  X::count_eq = 0;
  }

  friend bool operator < (const X &p, const X &l)
  {
    #pragma omp critical
    {
      X::count_less++;
    }

	  return l.value < p.value;
  }

  void operator =  (const X &o)
  {
    #pragma omp critical
    {
      X::count_eq++;
    }

	  value = o.value;
  }

};

long long int X::count_less = 0;
long long int X::count_eq = 0;

int main()
{
  srand(time(0));
#pragma omp parallel
printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
  for (int N = 10; N <=  100000000; N *= 10)
  {
    X::reset();
    double q = N*log(N)/log(2.0);
    std::vector<X> v(N);
    std::sort(v.begin(), v.end());
    std::cout.precision(2);
    std::cout << std::fixed;
    std::cout << "N = "
              << std::setw(9) << N << ", op<: "
              << std::setw(10) << X::count_less << " = "
              << std::setw(4)  << X::count_less/q << "*N*log2(N);  op=: "
              << std::setw(10) << X::count_eq << " = "
              << std::setw(4)  << X::count_eq/q << "*N*log2(N)\n";
  }
}
