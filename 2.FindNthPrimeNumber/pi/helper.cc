#include <iostream>
#include <vector>

using namespace std;

template <typename T>
ostream& write_value(ostream& out, const T& value)
{
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
    return out;
}

int main(int argc, char ** argv)
{
    if (argc != 2)
        return -1;
    int BIN = strtol(argv[1], nullptr, 10);
    if (BIN <= 0)
        return -1;
    std::ios::sync_with_stdio(false);
    unsigned int n;
    unsigned int count = 0;
    unsigned long long upper = BIN;
    vector<unsigned int> counts;
    while (cin >> n) {
        if (n >= upper) {
            counts.push_back(count);
            //cout.write(reinterpret_cast<char*>(&count), sizeof(count));
            count = 0;
            upper += BIN;
            cerr << "Finished: [" << upper - BIN << ", " << upper << "]" << endl;
        }
        count += 1;
    }
    if (count)
        counts.push_back(count);
        //cout.write(reinterpret_cast<char*>(&count), sizeof(count));
    unsigned int len = counts.size();
    int bin_len = BIN;
    cout.write(reinterpret_cast<char*>(&bin_len), sizeof(BIN));
    cout.write(reinterpret_cast<char*>(&len), sizeof(len));
    cout.write(reinterpret_cast<char*>(counts.data()), len * sizeof(count));
    return 0;
}

