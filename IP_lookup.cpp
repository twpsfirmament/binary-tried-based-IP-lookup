#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<cstring>
using namespace std;
struct node
{
    node *nxt[2];
    string next_hop_addr ="";
    node(){
        memset(nxt,0,sizeof(nxt));
    }
};
node* root = new node();
long long num_node = 1;
const vector<string> spilt(const string &str,const char &delimiter){
    vector<string> result;
    stringstream ss(str);
    string token;
    while(getline(ss,token,delimiter))
    {
        result.push_back(token);
    }
    return result;
}
const vector<string> spiltIP(string ip)
{
    vector<string> spiltwithslash = spilt(ip, '/');
    vector<string> spiltwithdot = spilt(spiltwithslash[0],'.');
    vector<string> res;
    for(auto& s :spiltwithdot)
    {
        res.push_back(s);
    }
    res.push_back(spiltwithslash[1]);
    return res;
}
unsigned int str2int(string first8bits,string second8bits,string third8bits,string last8bits)
{
    unsigned int fir = stoi(first8bits);
    unsigned int sec = stoi(second8bits);
    unsigned int thi = stoi(third8bits);
    unsigned int las = stoi(last8bits);
    unsigned int res = 0;
    res = (fir<<24) + (sec<<16) + (thi<<8) + las;
    return res;
}
void insert(unsigned int IP32bits,int prefixlen,string nxt_hop)
{
    node* cur = root;
    if(prefixlen != 0)
    {
        for(int i = 31;i >= 32-prefixlen;i--)
        {
            if(cur->nxt[(IP32bits>>i)&1] == nullptr)
            {
                cur->nxt[(IP32bits>>i)&1] = new node();//node hasn't exist,create it
                num_node++;
            }
        cur = cur->nxt[(IP32bits>>i)&1];//next step
        }
    }
    cur->next_hop_addr = nxt_hop;//store info in the last node;
}
string lookup(unsigned int lookupIP32bits)
{
    node* cur = root;
    string current_string = root->next_hop_addr;
    for (int i = 31; i >= 0; i--)
    {
        if(cur->nxt[(lookupIP32bits>>i)&1])
        {
            cur = cur->nxt[(lookupIP32bits>>i)&1];//get the ith bit which is next path
            if(!cur->next_hop_addr.empty())
                current_string = cur->next_hop_addr;
        }
        else 
            break;//null means lookup is end,return longest prefix info
    }
    return current_string;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    std::ifstream ifs("prefix.txt");
    if(!ifs.is_open())
    {
        cout << "Failed to open file. \n";
        return 1;
    }
    string p,n;
    ifs >> p;
    getline(ifs,n);
    string pre,nxt_hop;
    while (ifs>> pre >> nxt_hop)
    {
        vector<string>s = spiltIP(pre);
        unsigned int IP32bits = str2int(s[0],s[1],s[2],s[3]);
        insert(IP32bits,stoi(s[4]),nxt_hop);
    }//buildfinished
    ifs.close();
    string lookupip;
    while(cin >> lookupip)
    {
        vector<string> lookupIPstr = spiltIP(lookupip);
        unsigned int lookupIP32bits = str2int(lookupIPstr[0],lookupIPstr[1],lookupIPstr[2],lookupIPstr[3]);
        cout << lookup(lookupIP32bits) << "\n";
    }
    cout <<"The memory size is: "<<sizeof(node) * num_node << "\n";
    return 0;
}