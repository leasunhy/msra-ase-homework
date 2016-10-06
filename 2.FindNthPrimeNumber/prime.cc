#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <exception>
#include <sstream>

using std::vector;
using std::cout;
using std::endl;
using std::runtime_error;
using std::string;
using std::stringstream;

const int MAXN = 20000000;

using PrimeType = long long;

void print_usage(int argc, char **argv);
vector<PrimeType> get_primes(int maxn);
string print_number(PrimeType n, int section_len = 3);

int main(int argc, char **argv)
{
    if (argc != 2) {
        print_usage(argc, argv);
        return -1;
    }
    int n = std::strtol(argv[1], nullptr, 10);
    if (n <= 0)
        throw runtime_error("Invalid argument");

    vector<PrimeType> primes = get_primes(MAXN);

    cout << print_number(primes[n - 1]) << endl;
    return 0;
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

vector<PrimeType> get_primes(int maxn)
{
    // sieve
    vector<bool> mask((maxn >> 1) + 1, true);
    vector<PrimeType> primes { 2 };

    for (PrimeType i = 3; i < MAXN; i += 2) {
        if (mask[i >> 1]) {
            primes.push_back(i);
            PrimeType step = i + i;
            for (PrimeType j = i * i; j > 0 && j < MAXN; j += step)
                mask[j >> 1] = false;
        }
    }
    return primes;
}

void print_usage(int /* argc */, char **argv)
{
    cout << "Usage: " << argv[0] << " <n>" << endl
         << "\tn: the index of the prime number to find." << endl;
}

