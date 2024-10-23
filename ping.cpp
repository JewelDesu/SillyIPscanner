#include "ping.hpp"

void count_open_addr(int start, int end)
{
        for(int i=start;i<end;i++)
    {
        string com ("ping -c1 -s1 -w1 " + ip + to_string(i));
        int bing = system(com.c_str());
        if (bing == 0)
            {
                lock_guard<mutex> guard(vec_mtx);
                ipaddrs.push_back(ip + to_string(i));

            }
    }
}

int get_line_count()
{
    ifstream arp("/proc/net/arp");

    int n=0;
    string line;

    while(getline(arp,line))
        n++;
    arp.close();
    return n;
}

void print_addr(vector<string>& ipaddrs, vector<string>& macaddrs, struct adresses* addr){

    int l = ipaddrs.size();
    int k = get_line_count();
    for (int i=0;i<l;i++)
    {
        cout << ipaddrs[i] << " " << macaddrs[i] << endl;
    }

    cout<<l<<" "<<k<<" "<<get_line_count()<<endl;
} 

void sorting_adresses (struct adresses* addr)
{
    int n=ipaddrs.size();

    for(int i=0;i<n;i++)
    {
        int l=macaddrs.size();
        for(int j=0;j<get_line_count();j++)
        {
            if(ipaddrs[i] == addr[j].ipaddr)
            {
                //lock_guard<mutex> guard(vec_mtx);
                macaddrs.push_back(addr[j].macaddr);
            }
        }
        if(l != i+1)
        {
            macaddrs.push_back("LOCAL HOST");
        }
    }
    print_addr(ipaddrs,macaddrs,addr);
}

void ping_active_adresses()
{
    int l=ipaddrs.size();
    for(int i=0;i<l;i++)
    {
        string com ("ping -c1 -s1 -w2 " + ipaddrs[i]);
        system(com.c_str());
    }
}  

void get_mac_adresses ()
{
    int g = get_line_count();
    adresses* addr = new adresses[g];
    ifstream arp("/proc/net/arp");
    //int t=1;
    string add,hw,flag,mac,mask,device;
    arp.ignore(500,'\n');

        for(int i=0;i<g;i++)
        {
            arp>>add;
            arp>>hw;
            arp>>flag;
            arp>>mac;
            arp>>mask;
            arp>>device;
            addr[i].ipaddr = add;
            addr[i].macaddr = mac;  
        }

    
    arp.close();
    cout<<" "<<g<<endl;
   // print_addr(ipaddrs,macaddrs,addr);
    sorting_adresses(addr);

}

void thread_handler(int start, int end)
{
    int max_threads = thread::hardware_concurrency();
    thread thread_list[max_threads];
    int interval_size = (end - start + 1)/max_threads;
    int thread_num;

    for (thread_num = 0; thread_num < max_threads; thread_num++){
        int right_bound = start + interval_size;
        thread_list[thread_num] = thread(count_open_addr, start, right_bound);
        start = right_bound + 1;
    }
    for (thread_num = 0; thread_num < max_threads; thread_num++){
        thread_list[thread_num].join();
    }

    ping_active_adresses();
    
    
}

int main() {
    int start = 1;
    int end = 254;

    thread_handler(start,end);
    get_mac_adresses();
    
    return 0;
}
