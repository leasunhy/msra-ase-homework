#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <random>
#include <chrono>
#include <limits>
#include <functional>
#include <sstream>

using namespace std;

using PrimeType = unsigned long long;

const int TEST_COUNT = 15;
const string HELPER_FILENAME = "10000.dat";

void print_usage(int argc, char **argv);
bool is_prime(PrimeType n, std::function<int()>& rand_int);
string print_number(PrimeType n, int section_len = 3);
template <typename T> T read_data(istream& in);

int main(int argc, char ** argv)
{
    if (argc != 2) {
        print_usage(argc, argv);
        return -1;
    }
    unsigned n = std::strtoul(argv[1], nullptr, 10);
    if (n <= 0)
        throw runtime_error("Invalid argument");

    int bin_len;
    vector<unsigned int> lens;
    {
        ifstream fin(HELPER_FILENAME, std::ios::binary);
        bin_len = read_data<int>(fin);
        auto vector_len = read_data<size_t>(fin);
        lens.resize(vector_len);
        fin.read(reinterpret_cast<char*>(lens.data()), vector_len * sizeof(unsigned int));
        fin.close();
    }

    PrimeType sum = 0, bin_index = 0, prev_sum = 0;
    while (sum < n) {
        prev_sum = sum;
        sum += lens[bin_index];
        bin_index += 1;
    }

    // the range of current bin is [lower, upper)
    PrimeType upper = bin_index * bin_len;
    PrimeType lower = upper - bin_len;

    // deals with the case where upper or lower is even
    if (!(upper & 1))
        upper -= 1;
    if (!(lower & 1))
        lower += 1;
    int bin_prime_counter = n - prev_sum;

    std::default_random_engine generator(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<PrimeType> distribution(
        0, std::numeric_limits<PrimeType>::max());
    std::function<int()> rand_int = std::bind(distribution, generator);

    for (PrimeType i = lower; i < upper; i += 2) {
        if (is_prime(i, rand_int))
            bin_prime_counter -= 1;
        if (bin_prime_counter == 0) {
            cout << print_number(i) << endl;
            return 0;
        }
    }

    return 0;
}

template <typename T>
T read_data(istream& in)
{
    T res;
    in.read(reinterpret_cast<char*>(&res), sizeof(T));
    return res;
}

string print_number(PrimeType n, int section_len)
{
    stringstream ss;
    ss << n;
    string ori = ss.str();
    size_t first = ori.length() % section_len;
    string result = ori.substr(0, first);
    for (size_t i = first; i < ori.length(); i += section_len) {
        result += ' ';
        result += ori.substr(i, section_len);
    }
    return result;
}

// returns (base ^ exp % modulo)
PrimeType power(PrimeType base, unsigned int exp, PrimeType modulo)
{
    PrimeType res = 1;
    base %= modulo;
    while (exp) {
        if (exp & 1)
            res = res * base % modulo;
        exp >>= 1;
        base = base * base % modulo;
    }
    return res;
}

// tests whether `n` is a prime number.
//   d is an odd number satisfying d * (2 ^ r) == n - 1.
//   rand_int is a function which returns a random integer.
bool miller_test(PrimeType d, PrimeType n, std::function<int()>& rand_int)
{
    PrimeType a = 2 + rand_int() % (n - 4);
    PrimeType x = power(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

// a routine for checking whether n is a prime number, using Miller Test.
bool is_prime(PrimeType n, std::function<int()>& rand_int)
{
    if (n <= 4)
        return n == 2 || n == 3;

    PrimeType d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < TEST_COUNT; ++i) {
        if (!miller_test(d, n, rand_int))
            return false;
    }

    return true;
}

void print_usage(int /* argc */, char **argv)
{
    cout << "Usage: " << argv[0] << " <n>" << endl
         << "\tn: the index of the prime number to find." << endl;
}

