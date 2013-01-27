#ifndef TRANSITIONS
#define TRANSITIONS

#include "state.h"
#include "throws.h"
#include <vector>
#include <iostream>
using namespace std;

namespace SiteswapComponents{

namespace Transitions
{
	struct Ops
	{
		Ops(){};
		Ops(vector<pair<int,int> > A, vector<pair<int,int> > B, int S){a=A;b=B;shift=S;}
		vector<pair<int,int> > a,b;
		int shift;
	};
	Ops getAB(State s1, State s2);
	Throws getThrows(Ops ops);
	Throws getTransition(State s1, State s2);
}

};

#endif //TRANSITIONS