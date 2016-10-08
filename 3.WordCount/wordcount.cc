#include <cassert>
#include <cctype>
#include <iostream>
#include <unordered_map>
#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <thread>
#include <mutex>
#include <queue>
#include <boost/filesystem.hpp>

// XXX(leasunhy): deprecate use of regex in favor of hand-crafted tokenizer.
#if 0
// on windows, boost.regex is much faster than the STL one.
//    However on Linux, the STL one is much faster.
#ifdef WIN32
#include <boost/regex.hpp>
namespace re = boost;
#else
#include <regex>
namespace re = std;
#endif  // WIN32
#endif

using std::string;
using std::pair;
using std::cerr;
using std::endl;
using std::cout;

namespace fs = boost::filesystem;

using occurences_t = std::unordered_map<string, int>;

std::queue<fs::path> filenames;
std::mutex filenames_lock;

occurences_t total_occurences;
std::mutex total_occurences_lock;

void worker_func(int id);

int main(int argc, char * argv[])
{
    std::ios::sync_with_stdio(false);
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << ": <root> <N> [NThread]" << endl
            << "  <root>   \t" << "Root directory to be scanned." << endl
            << "  <N>      \t" << "Number of hot words to be listed." << endl
            << "  <NThread>\t" << "Number of threads to run. Optional." << endl;
        return -1;
    }

    fs::path root_dir(argv[1]);
    if (!fs::exists(root_dir)) {
        cerr << "Path specified does not exist." << endl;
        return -2;
    } else if (!fs::is_directory(root_dir)) {
        cerr << "Path specified is not a directory." << endl;
        return -2;
    }

    int N = strtol(argv[2], nullptr, 10);
    if (N <= 0) {
        cerr << "Argument N given is not valid." << endl;
        return -3;
    }

    int thread_num = std::thread::hardware_concurrency();
    if (argc >= 4) {
        thread_num = strtol(argv[3], nullptr, 10);
        if (thread_num < 1) {
            cerr << "Argument NThread given is not valid." << endl;
            return -4;
        }
    }

    for (auto& filename : fs::recursive_directory_iterator(root_dir)) {
        if (fs::is_regular_file(filename))
            filenames.push(filename.path());
    }

    std::vector<std::thread> workers;
    for (int i = 0; i < thread_num; ++i)
        workers.emplace_back(worker_func, i);

    for (auto& worker : workers)
        if (worker.joinable())
            worker.join();

    std::vector<pair<string, int>> result(total_occurences.begin(),
                                          total_occurences.end());
    sort(result.begin(), result.end(), [](auto& p1, auto& p2) {
        return p1.second > p2.second;
    });

    for (int i = 0; i < N; ++i)
        cout << result[i].first << " " << result[i].second << endl;

    return 0;
}

bool isw(char c)
{
    return std::isalnum(c) || c == '_' || c == '\''; 
}

string tolower_inplace(string&& str)
{
    for (char & c : str)
        if (c >= 'A' && c <= 'Z')
            c += 'a' - 'A';
    return str;
}

void count_occurences(string& content, occurences_t& occurences)
{
    // XXX(leasunhy): deprecate regex approach.
    //static re::regex expression("\\w[\\w_']*", re::regex_constants::optimize);

    //string::const_iterator beginiter = content.begin(), enditer = content.end();
    //re::smatch what;
    //auto flags = re::regex_constants::match_default;
    //while (re::regex_search(beginiter, enditer, what, expression, flags)) {
    //    beginiter = what[0].second;
    //    occurences[what[0]] += 1;
    //}

    for (size_t i = 0; i < content.length(); ++i) {
        // a word must start from an alphabetica: \w[\w_']+
        if (std::isalpha(content[i])) {
            // search forward
            size_t j = i + 1;
            while (isw(content[j]))
                ++j;
            occurences[tolower_inplace(content.substr(i, j - i))] += 1;
            i = j - 1;
        }
    }
}

void worker_func(int id)
{
#ifdef DEBUG
    cerr << "Thread #" << id << ": started." << endl;
#endif  // DEBUG

    occurences_t occurences;
    while (true) {
        filenames_lock.lock();
        if (filenames.empty()) {
            filenames_lock.unlock();
            break;
        }
        fs::path filename = filenames.front();
        filenames.pop();
        filenames_lock.unlock();

#ifdef DEBUG
        cerr << "Thread #" << id << ": processing "
             << filename << "..." << endl;
#endif  // DEBUG

        fs::ifstream fin(filename);
        // TODO(leasunhy): here we read all the content of the file,
        //                 since we know that the files are small.
        //                 Improve it.
        string content(std::istreambuf_iterator<char>(fin), {});
        
        count_occurences(content, occurences);
    }
    total_occurences_lock.lock();
    for (auto& p : occurences)
        total_occurences[p.first] += p.second;
    total_occurences_lock.unlock();

#ifdef DEBUG
    cerr << "Thread #" << id << ": ready to be joined." << endl;
#endif  // DEBUG
}

