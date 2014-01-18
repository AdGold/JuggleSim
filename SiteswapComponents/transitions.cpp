#include "transitions.h"

namespace SiteswapComponents{

namespace Transitions
{
	Ops getAB(State s1, State s2)
	{
		while (s2.size() < s1.size()) s2.add(StateEl());
		int shift;
		bool shifted = true;
		for (shift = 0; shift < s1.size() && shifted; shift++)
		{
			shifted = false;
			for (int i = shift; i < s1.size(); i++)
			{
				if (s2[i-shift].getLH() < s1[i].getLH() || s2[i-shift].getRH() < s1[i].getRH())
				{
					shifted = true;
					break;
				}
			}
		}
		if (!shifted)shift--;
		vector<pair<int,int> > a,b;
		for (int i = 0; i < shift; i++)
		{
			for (int j = 0; j < s1[i].getLH(); j++)a.push_back(pair<int,int>(i,0));
			for (int j = 0; j < s1[i].getRH(); j++)a.push_back(pair<int,int>(i,1));
		}
		for (int i = 0; i < s2.size(); i++)
		{
			if (i+shift >= s1.size())
			{
				for (int j = 0; j < s2[i].getLH(); j++)b.push_back(pair<int,int>(i,0));
				for (int j = 0; j < s2[i].getRH(); j++)b.push_back(pair<int,int>(i,1));
			}
			else
			{
				for (int j = 0; j < s2[i].getLH()-s1[i+shift].getLH(); j++)b.push_back(pair<int,int>(i,0));
				for (int j = 0; j < s2[i].getRH()-s1[i+shift].getRH(); j++)b.push_back(pair<int,int>(i,1));
			}
		}
		return Ops(a,b,shift);
	}

	Throws getThrows(Ops ops)
	{
		Throws throws;
		auto a = ops.a, b = ops.b;
		int place = 0, shift = ops.shift;
		for (int i = 0; i < shift; i++)
		{
			throws.add(Throw());
			if (a[place].first == i)
			{
				for (; a[place].first == i && place < a.size(); place++)
				{
					int ss = b[place].first-a[place].first+shift;
					bool cross = (ss%2)^(a[place].second!=b[place].second);
					throws[i].add(a[place].second, ss, cross);
				}
			}
			else
			{
				if (throws[i-1].isEmpty())
				{
					throws[i].add(0,0,false);
					throws[i].add(1,0,false);
				}
				else if (!throws[i-1].hasSynch())
				{
					if (throws[i-1].getRHThrows().size())throws[i].add(0,0,false);
					else if (throws[i-1].getLHThrows().size())throws[i].add(1,0,false);
					else cout<<"problem"<<endl;
				}
			}
		}
		return throws;
	}

	Throws getTransition(State s1, State s2)
	{
		Ops ops = getAB(s1,s2);
		return getThrows(ops);
	}
}

};
