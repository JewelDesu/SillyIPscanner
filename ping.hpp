#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <string>
#include <fstream>
//#include <unistd.h>

#include <unordered_map>
#include <thread>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::unordered_map;
using std::string;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::vector;
using std::to_string;
using std::ifstream;
using std::getline;

static vector<string> ipaddrs;
static vector<string> macaddrs;
static string ip ("192.168.0.");
static mutex vec_mtx;

struct adresses{
    string ipaddr;
    string macaddr;
};

int main_ping();
