#ifndef STATE_EL
#define STATE_EL

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

namespace SiteswapComponents{

class StateEl
{
public:
	StateEl(){LH=RH=0;}
	StateEl(int l, int r) { LH = l; RH = r; }
    StateEl(const StateEl &e) { LH = e.getLH(); RH = e.getRH(); }
    int getRH() const {return RH; }
    int getLH() const {return LH; }
    int& operator[] (int x);
    string toString(int n) const;
    string stringRep() const {return toString(2);}
    void coutStringRep() const {cout<<"("<<LH<<", "<<RH<<")";}
    const bool operator==(const StateEl &th) const;
    const bool operator!=(const StateEl &th) const { return !((*this) == th); }
private:
    string intToString(int n) const;
    int LH, RH;
};

};

#endif //STATE_EL