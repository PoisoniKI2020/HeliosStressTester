#include "stdc++.h"
#include <direct.h>
#include <boost/process.hpp>
#include <windows.h>
#include <chrono>
#include <random>
#include <pplwin.h>
using namespace std;

namespace bp = boost::process;

#ifdef __GNUC__
#define gen_random mt19937 rnd(time(nullptr));
#define gtln getline(iss, token, delimiter)
#elif _MSC_VER
#define gen_random mt19937_64 rnd(time(nullptr));
#define gtln std::getline(iss, token, delimiter)
#endif


string tested = "";
string testing = "";
string tests_path = "";

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
std::string strip(const std::string& s)
{
    // Найти первую позицию символа, не являющегося пробельным
    size_t first = s.find_first_not_of(" \t\n\r");

    // Найти последнюю позицию символа, не являющегося пробельным
    size_t last = s.find_last_not_of(" \t\n\r");

    // Если строка пустая, то вернуть пустую строку
    if (first == std::string::npos || last == std::string::npos)
        return "";

    // Вернуть подстроку, содержащую символы между первой и последней позициями
    return s.substr(first, last - first + 1);
}


bool is_number(const string& str) {
    try {
        stoi(str);
    }
    catch (const invalid_argument&) {
        return false;
    }
    return true;
}

/*
std::string Run_Program(const std::string& input, const std::string& name) {
    // Записываем входные данные во временный файл
    std::ofstream input_file("input.txt");
    input_file << input;
    input_file.close();

    // Создаем объект процесса
    bp::ipstream out;
    bp::opstream in;
    bp::child process(name, bp::std_in < in, bp::std_out > out);

    // Отправляем входные данные в стандартный ввод процесса
    in << input << std::endl;
    in.close();

    // Ждем не более 10 секунд и проверяем, закончил ли процесс работу
    auto start_time = std::chrono::steady_clock::now();
    while (process.running()) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        if (elapsed_time > std::chrono::seconds(5)) {
            process.terminate(); // Останавливаем процесс
            return "Process terminated due to timeout";
        }
    }

    // Читаем выходные данные из процесса
    std::string output((std::istreambuf_iterator<char>(out)),
                        std::istreambuf_iterator<char>());

    // Удаляем временные файлы
    std::remove("input.txt");

    return output;
}
*/


std::string Run_Program(const std::string& input, const std::string& name) {
    // Записываем входные данные во временный файл
    std::ofstream input_file("input.txt");
    input_file << input;
    input_file.close();

    // Создаем объект процесса
    bp::ipstream out;
    bp::opstream in;
    bp::child process(name, bp::std_in < in, bp::std_out > out);

    // Отправляем входные данные в стандартный ввод процесса
    in << input << std::endl;
    in.close();

    // Ждем не более 10 секунд и проверяем, закончил ли процесс работу
    auto start_time = std::chrono::steady_clock::now();
    while (process.running()) {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        if (elapsed_time > std::chrono::seconds(3)) {
            process.terminate(); // Останавливаем процесс
            return "Process terminated due to timeout";
        }
    }

    // Читаем выходные данные из процесса
    std::string output((std::istreambuf_iterator<char>(out)),
        std::istreambuf_iterator<char>());

    // Удаляем временные файлы
    std::remove("input.txt");

    return output;
}

struct Command {
    string command;
    int l, r, size, count, vtype, type, eol = 0;
    string size_var = "", name;
    Command(string& tmp) {
        vector<string> args = split(tmp, ';');
        for (auto x : args) {
            cout << x << " ";
        }
        cout << "\n";
        //cout << args[0] << "\n";
        name = args[1];
        if (args[0] == "count") {
            count = stoi(args[1]);
            type = 0;
        }
        if (args[0] == "v") {
            type = 1;
            if (args[2] == "int") {
                vtype = 0;
                l = stoi(args[3]);
                r = stoi(args[4]);
            }
            else if (args[2] == "vector_int") {
                vtype = 1;
                if (is_number(args[3])) size = stoi(args[3]);
                else size_var = args[3];
                l = stoi(args[4]);
                r = stoi(args[5]);
            }
            else if (args[2] == "permutation") {
                vtype = 2;
                if (is_number(args[3])) size = stoi(args[3]);
                else size_var = args[3];
            }
            else if (args[2] == "graph") {
                vtype = 3;
                if (is_number(args[3])) size = stoi(args[3]);
                else size_var = args[3];
            }
            eol = (args.back() == "1");
        }
        if (args[0] == "tested") {
            name = args[1];
            type = 2;
        }
        if (args[0] == "testing") {
            name = args[1];
            type = 3;
        }
        if (args[0] == "save_tests") {
            name = args[1];
            type = 4;
        }
    }
};

int mx(int a, int b) {
    return max(a, b);
}

vector<Command> script;

int test_count = 0;

mt19937_64 rnd(time(nullptr));

int generate_integer(int l, int r) {
    int res = rnd() % (r - l + 1);
    res += l;
    return res;
}

vector<pair<int, int>> generate_graph(int n, int m) {
    vector<pair<int, int>> edges;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, n - 1);

    // Добавляем ребра до тех пор, пока не наберем нужное количество
    while (edges.size() < m) {
        int u = dist(gen);
        int v = dist(gen);
        if (u != v) {
            edges.emplace_back(min(u, v), max(u, v));
        }
    }

    // Удаляем дубликаты ребер
    sort(edges.begin(), edges.end());
    edges.erase(unique(edges.begin(), edges.end()), edges.end());

    return edges;
}

void write_to_file(string file_path, string data) {
    ofstream fileStream;
    fileStream.open(file_path, ios::out | ios::app);
    if (fileStream.is_open()) {
        fileStream << data << endl;
        fileStream.close();
    }
}

string Run_Script(int id) {
    map<string, int> integers;
    string input = "";
    for (auto cmd : script) {
        //cout << cmd.vtype << "\n";
        if (cmd.vtype == 0) {
            integers[cmd.name] = generate_integer(cmd.l, cmd.r);
            input += to_string(integers[cmd.name]);
        }
        else if (cmd.vtype == 1) {
            if (cmd.size_var != "") {
                for (int i = 0; i < integers[cmd.size_var]; i++) {
                    input += to_string((generate_integer(cmd.l, cmd.r))) + " ";
                }
            }
            else {
                for (int i = 0; i < cmd.size; i++) {
                    input += to_string((generate_integer(cmd.l, cmd.r))) + " ";
                }
            }
        }
        else if (cmd.vtype == 2) {
            if (cmd.size_var != "") {
                vector<int> perm(integers[cmd.size_var]);
                for (int i = 0; i < integers[cmd.size_var]; i++) {
                    perm[i] = i + 1;
                }
                random_shuffle(perm.begin(), perm.end());
                for (auto x : perm) input += to_string(x) + " ";
            }
            else {
                vector<int> perm(cmd.size);
                for (int i = 0; i < cmd.size; i++) {
                    perm[i] = i + 1;
                }
                random_shuffle(perm.begin(), perm.end());
                for (auto x : perm) input += to_string(x) + " ";
            }
        }
        if (cmd.eol) input += "\n";
        else input += " ";
    }
    //cout << "Hello\n";
    if (tests_path != "") {
        remove((tests_path + to_string(id) + ".input").c_str());
        write_to_file(tests_path + to_string(id) + ".input", input);
    }
    if (tested != "" && testing != "") {
        string rans = strip(Run_Program(input, testing));
        if (rans == "Process terminated due to timeout") return rans;
        string tans = strip(Run_Program(input, tested));
        if (tans == "Process terminated due to timeout") return tans;
        if (tests_path != "") {
            remove((tests_path + to_string(id) + "_1" + ".output").c_str());
            write_to_file(tests_path + to_string(id) + "_1" + ".output", rans);
            remove((tests_path + to_string(id) + "_2" + ".output").c_str());
            write_to_file(tests_path + to_string(id) + "_2" + ".output", tans);
        }
        
        if (rans != tans) {
            return (strip(input) + "\n" + rans + "\n" + tans + "\n");
        }
        //cout << rans << "\n";
        return "OK\n";
        //cout << input << "\n";
    }
    else {
        return input;
    }
}

int main(int argc, char* argv[]) {
    /*
    main.exe count:10 v:n:int:0:100:1 v:a:vector_int:n:0:100:1
    main.exe count:10 v:n:int:1:5:0 v:k:int:0:2:0 v:a:vector_int:n:0:2:1
    */
    //cout << "OK\n";
    for (int id = 1; id < argc; id++) {
        //cout << id << "\n";
        string tmp(argv[id]);
        Command c(tmp);

        if (c.type == 0) {
            test_count = c.count;
        }
        else if (c.type == 1) {
            script.push_back(c);
        }
        else if (c.type == 2) {
            tested += c.name;
        }
        else if (c.type == 3) {
            testing += c.name;
        }
        else if (c.type == 4) {
            if (c.name == "1") {
                tests_path = "Tests\\";
                _mkdir(tests_path.c_str());
            }
            else tests_path = c.name;
        }
    }
    
    int gen_count = script.size();
    for (int test = 0; test < test_count; test++) {
        cout << Run_Script(test) << "\n";
    }
    return 0;
}
//"-TestCount=100"
