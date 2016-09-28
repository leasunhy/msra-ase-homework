#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <exception>

using namespace std;

const int MAXN = 50000000;

void print_usage(int argc, char **argv);
vector<long long> get_primes(int maxn);

int main(int argc, char **argv)
{
    if (argc != 2) {
        print_usage(argc, argv);
        return -1;
    }
    int n = std::strtol(argv[1], nullptr, 10);
    if (n <= 0)
        throw runtime_error("Invalid argument");

    vector<long long> primes = get_primes(MAXN);

    cout << "The " << n << "th prime number is: " << primes[n - 1] << endl;
    return 0;
}

vector<long long> get_primes(int maxn)
{
    // sieve
    vector<bool> mask(maxn, true);
    vector<long long> primes { 2, 3 };
    for (long long j = 3 * 3; j < MAXN; j += 6)
        mask[j] = false;

    for (long long i = 5; i < MAXN; i += 2) {
        if (mask[i]) {
            primes.push_back(i);
            for (long long j = i * i; j > 0 && j < MAXN; j += i + i)
                mask[j] = false;
        }
    }
    return primes;
}

void print_usage(int /* argc */, char **argv)
{
    cout << "Usage: " << argv[0] << " <n>" << endl
         << "\tn: the index of the prime number to find." << endl;
}

