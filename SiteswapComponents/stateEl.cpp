#include "stateEl.h"

namespace SiteswapComponents{

int& StateEl::operator[] (int x)
{
	if (x > 1)throw out_of_range ("Index out of range, must be 0 (for LH) or 1 (for RH)");
	return (x)?RH:LH;
}

string StateEl::toString(int n) const
{
	if (n==2) return '('+intToString(LH)+','+intToString(RH)+')';
	return intToString((n==1)?RH:LH) ;
}

const bool StateEl::operator==(const StateEl &other) const
{
	return RH == other.getRH() && LH == other.getLH();
}

string StateEl::intToString(int n) const
{
   ostringstream ss;
   ss << n;
   return ss.str();
}

};