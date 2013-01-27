#ifndef THROW
#define THROW

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

namespace SiteswapComponents{

class Throw
{
public:
	Throw(){}
	Throw(int th, bool cross, int side);
	Throw(const Throw &t);
	vector<int> getLHThrows() const{return LHThrows;}
	vector<int> getRHThrows() const{return RHThrows;}
	vector<bool> getLHCrossing() const{return LHCrossing;}
	vector<bool> getRHCrossing() const{return RHCrossing;}
	void setLHThrows(vector<int> LHT){LHThrows = LHT;}
	void setRHThrows(vector<int> RHT){RHThrows = RHT;}
	void setLHCrossing(vector<bool> LHC){LHCrossing = LHC;}
	void setRHCrossing(vector<bool> RHC){RHCrossing = RHC;}
	void flip();
	void add(int hand, int ss, bool cross);
	string toSiteswap(bool lower) const;
	string stringRep() const;
    bool hasMultiplex() const;
    bool hasZeroX() const;
    bool hasSynch() const;
    bool isEmpty() const;
    int sum() const;
    int max() const;
    const Throw &operator=(const Throw &th);
private:
	vector<int> LHThrows, RHThrows;
	vector<bool> LHCrossing, RHCrossing;
	string throwsToSiteswap(vector<int> th, vector<bool> x, bool lower) const;
    string throwToSiteswap(int th, bool x, bool lower) const;
    string arrayToSiteswap(vector<int> arr, bool lower) const;
    string intToSiteswap(int s, bool lower) const;
    string intToString(int n) const;
};

};

#endif //THROW
