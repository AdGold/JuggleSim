#ifndef TEMPLATE_SS
#define TEMPLATE_SS

#include <vector>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <sstream>
#include <iostream>
using namespace std;

#include "../pugixml/pugixml.hpp"
using namespace pugi;

#define PBI pair<bool,int>

namespace SiteswapComponents{

namespace TemplateSS
{
	void load(string filename, bool &loaded);
	bool isTemplate(string pattern);
	string expandTemplate(string pattern, bool lower, string &err);
	string expand(string _pattern, int balls, bool lower, string &err);

	string intToString(int n);
	string intToSiteswap(int s, bool lower);

	queue<PBI > infixToPostfix(string infix, string &err, int N, int I);
	int evalPostfix(queue<PBI > q, string &err);
	int evaluate(string expr, int n, int i, string &err);
};

};

#endif //TEMPLATE_SS