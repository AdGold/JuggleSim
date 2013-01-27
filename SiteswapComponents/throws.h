#ifndef THROWS
#define THROWS

#include <vector>
#include <string>
#include <locale>
#include <iostream>//remove

using namespace std;

#include "throw.h"

namespace SiteswapComponents{

class Throws
{
public:
    Throws() { }
    Throws(const Throws &throws) { setThrows(throws.getThrows()); }
    vector<Throw> getThrows() const { return throws; }
    string setThrows(string pattern);
    void setThrows(vector<Throw> pattern);
    string toSiteswap(bool lower) const;
    string stringRep() const;
    vector<string> getThrows(bool lower) const;
    const Throw &operator[] (int x) const { return throws[x]; }
    Throw &operator[] (int x) { return throws[x]; }
    void flip();

    void removeAt(int index) { throws.erase(throws.begin()+index); }
    void add(Throw th) { throws.push_back(th); }
    void clear() { throws.clear(); }
    void addRange(vector<Throw> list) { for (Throw th : list) { add(th); } }
    void addRange(Throws moreThrows) { addRange(moreThrows.getThrows()); }
    int size() const { return throws.size(); }
    void insert(int index, Throw th) { throws.insert(throws.begin()+index, th); }
    template<typename Func>
    int countNum(Func func){int s = 0;for(Throw t:throws){if(func(t))s++;}return s;}
    int sum() const;
    int max() const;

    // Throws operator=(const Throws &other) { setThrows(other.getThrows()); cout<<"/op ="<<endl; }
private:
    vector<Throw> throws;
    ostringstream errMsg;

    int getNum(string pattern, int &i);
    pair<int,bool> getSingleThrow(string pattern, int &i);
    pair<vector<int>,vector<bool> > getMultiplex(string pattern, int &i);
    Throw getThrow(string pattern, int &i, int hand);
    bool parse(string pattern);
};

};

#endif //THROWS
