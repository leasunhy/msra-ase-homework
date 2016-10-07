#include <cassert>
#include <iostream>
#include <unordered_map>
#include <string>
#include <iterator>
#include <algorithm>
#include <utility>
#include <regex>
#include <thread>
#include <mutex>
#include <queue>
#include <boost/filesystem.hpp>

using std::string;
using std::pair;
using std::cerr;
using std::endl;
using std::cout;

namespace fs = boost::filesystem;

std::queue<fs::path> filenames;
std::mutex filenames_lock;

std::unordered_map<string, int> total_occurences;
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

void worker_func(int id)
{
#ifdef DEBUG
    cerr << "Thread #" << id << ": started." << endl;
#endif  // DEBUG

    std::regex re("\\w+");
    std::unordered_map<string, int> occurences;
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
        string::const_iterator beginiter = content.begin(), enditer = content.end();
        std::smatch what;
        auto flags = std::regex_constants::match_default;
        while (regex_search(beginiter, enditer, what, re, flags)) {
            beginiter = what[0].second;
            occurences[what[0]] += 1;
        }
    }
    total_occurences_lock.lock();
    for (auto& p : occurences)
        total_occurences[p.first] += p.second;
    total_occurences_lock.unlock();

#ifdef DEBUG
    cerr << "Thread #" << id << ": ready to be joined." << endl;
#endif  // DEBUG
}

