#include "throw.h"

namespace SiteswapComponents{

Throw::Throw(const Throw &t)
{
	setLHThrows(t.getLHThrows());
	setRHThrows(t.getRHThrows());
	setLHCrossing(t.getLHCrossing());
	setRHCrossing(t.getRHCrossing());
}

Throw::Throw(int th, bool cross, int side)
{
    ((side)?RHThrows:LHThrows).push_back(th);
    ((side)?RHCrossing:LHCrossing).push_back(cross);
}

void Throw::flip()
{
	vector<int> itmp = RHThrows;
	RHThrows = LHThrows;
	LHThrows = itmp;
	vector<bool> btmp = RHCrossing;
	RHCrossing = LHCrossing;
	LHCrossing = btmp;
}

void Throw::add(int hand, int ss, bool cross)
{
    ((hand)?RHThrows:LHThrows).push_back(ss);
    ((hand)?RHCrossing:LHCrossing).push_back(cross);
}

string Throw::toSiteswap(bool lower) const
{
    bool left = LHThrows.size() > 0;
    bool right = RHThrows.size() > 0;
    string l = throwsToSiteswap(LHThrows, LHCrossing, lower);
    string r = throwsToSiteswap(RHThrows, RHCrossing, lower);
    if (left && right) return "(" + l + ", " + r + ")";
    else return l + r;
}

string Throw::stringRep() const
{
    return "([" + arrayToSiteswap(LHThrows, true) + "], [" + arrayToSiteswap(RHThrows, true) + "])";
}

string Throw::throwsToSiteswap(vector<int> th, vector<bool> x, bool lower) const
{
    int c = th.size();
    string result = "";
    for (int i = 0; i < c; i++) { result += throwToSiteswap(th[i], x[i], lower); }
    return (c > 1) ? "[" + result + "]" : result;
}

string Throw::throwToSiteswap(int th, bool x, bool lower) const
{
    return intToSiteswap(th, lower) + ((x) ? "x" : "");
}

string Throw::arrayToSiteswap(vector<int> arr, bool lower) const
{
    string result = "";
    for (int n : arr) { result += intToSiteswap(n, lower); }
    return result;
}

string Throw::intToSiteswap(int s, bool lower) const
{
    string x = "";
    if (s < 10) return intToString(s);
    else if (s < 36 && lower && s != 33) return x+(char)(s + (int)'a' - 10);
    else return "{" + intToString(s) + "}";
}

string Throw::intToString(int n) const
{
   ostringstream ss;
   ss << n;
   return ss.str();
}

bool Throw::hasMultiplex() const
{
    return RHThrows.size() > 1 || LHThrows.size() > 1;
}
bool Throw::hasZeroX() const
{
    for (int i = 0; i < RHThrows.size(); i++)
    {
        if (RHThrows[i] == 0 && RHCrossing[i] == true) return true;
    }
    for (int i = 0; i < LHThrows.size(); i++)
    {
        if (LHThrows[i] == 0 && LHCrossing[i] == true) return true;
    }
    return false;
}
bool Throw::hasSynch() const
{
    return RHThrows.size() > 0 && LHThrows.size() > 0;
}
bool Throw::isEmpty() const
{
    return LHThrows.size() == 0 && RHThrows.size() == 0;
}

int Throw::sum() const
{
    int total = 0;
    for (int n : RHThrows) total += n;
    for (int n : LHThrows) total += n;
    return total;
}

int Throw::max() const
{
    int max = 0;
    for (int n : RHThrows) { if (n > max)max = n; }
    for (int n : LHThrows) { if (n > max)max = n; }
    return max;
}

const Throw &Throw::operator=(const Throw &th)
{
    LHThrows = th.getLHThrows();
    RHThrows = th.getRHThrows();
    LHCrossing = th.getLHCrossing();
    RHCrossing = th.getRHCrossing();
    return (*this);
}

};
