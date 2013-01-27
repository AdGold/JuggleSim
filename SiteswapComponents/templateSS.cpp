#include "templateSS.h"

namespace SiteswapComponents{

namespace TemplateSS{

map<string, string> templatesMap[2];
bool fileNotFound;

void load(string filename, bool &loaded)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(filename.c_str());
	if (!result) { loaded = false; fileNotFound=true; return; }
	fileNotFound = false;
	loaded = true;
	xml_node templates = doc.child("templates");
	map<string, string> even, odd;
	for (auto &category : templates)
	{
		bool o = false, e = false;
		if (string(category.name()) == "odd")o = true;
		else if (string(category.name()) == "even")e = true;
		else if (string(category.name()) == "both")e = o = true;
		//else cout<<"unrecognised category "<<category.name()<<endl;

		for (auto &temp : category)
		{
			vector<string> possibleNames;
			string templateDefinition = "";
			for (auto &child : temp)
			{
				if (string(child.name()) == "name") possibleNames.push_back(child.child_value());
				else if (string(child.name()) == "template") templateDefinition = child.child_value();
				//else { cout<<"unrecognised"<<endl; }
			}
			//if (templateDefinition.empty()) cout<<"empty or no template definition for pattern."<<endl;
			//if (possibleNames.empty()) cout<<"No pattern name(s) given."<<endl;
			for (string s: possibleNames)
			{
				if (o)odd[s] = templateDefinition;
				if (e)even[s] = templateDefinition;
			}
		}
	}
	templatesMap[0] = even;
	templatesMap[1] = odd;
}

string expandTemplate(string pattern, bool lower, string &err)
{
	int i = 0, len = pattern.length(), balls = 0;
	string patternName;
	while (isdigit(pattern[i])) balls = balls*10+(pattern[i++]-'0');
	if (pattern[i] == ' ')
	{
		string test = pattern.substr(i+1, 5);
		if (test == "ball " || (test == "balls" && i+6 < len && pattern[i+6] == ' '))
			patternName = (i+6+(test[4]=='s')<len)?pattern.substr(i+6+(test[4]=='s')):"";
		else patternName = "";
	}
	else if (pattern[i] == 'b' && i+1 < len && pattern[i+1] == ' ')
		patternName = (i+2<len)?pattern.substr(i+2):"";
	string templ;
	auto it = templatesMap[balls%2].find(patternName);
	if (it != templatesMap[balls%2].end())
		return expand(it->second, balls, lower, err);
	else if ((it=templatesMap[(balls+1)%2].find(patternName)) != templatesMap[(balls+1)%2].end())
		err = "Pattern " + patternName + " only works for " + 
			((balls%2)?"even":"odd") + " numbers of balls.";
	else
		err = "No pattern named '" + patternName + "'";
	return string("");
}

bool isTemplate(string pattern)
{
	if (fileNotFound) return false;
	int i = -1, len = pattern.length();
	while (++i < len && isdigit(pattern[i])) ;
	if (i >= len)return false;
	if (pattern[i] == ' ')
	{
		string test = pattern.substr(i+1, 5);
		if (test == "ball " || (test == "balls" && i+6 < len && pattern[i+6] == ' '))
			return true;
		return false;
	}
	else if (pattern[i] == 'b' && i+1 < len && pattern[i+1] == ' ')return true;
	return false;
}

string expand(string _pattern, int balls, bool lower, string &err)
{
	vector<pair<string,string> > toReplace, toReplace2;
	string pattern = "";
	for (int i = 0; i < _pattern.length(); i++)
	{
		if (_pattern[i] == '{')
		{
			int end = _pattern.find('}', i);
			if (end == string::npos) return string("No ending '}'");
			string toEval = _pattern.substr(i+1, end-i-1);
			if (toEval.find('i') == string::npos)
			{
				int n = evaluate(toEval, balls, 0, err);
				if (err.size()) return err;
				pattern += intToSiteswap(n, lower);
				i = end;
			}
			else pattern += _pattern[i];
		}
		else pattern += _pattern[i];
	}
	string result = "";
	for (int i = 0; i < pattern.length(); i++)
	{
		if (pattern[i] == '|')
		{
			if (i>=pattern.length())return string("EOF while parsing");
			int repStart = i+1, repLength = 0;
			while (++i < pattern.length() && pattern[i] != '^')repLength++;
			if (i>=pattern.length())return string("EOF while parsing");
			int paramAStart = i+1, paramALength = 0;
			while (++i < pattern.length() && pattern[i] != '|' && pattern[i] != ',')paramALength++;
			if (i>=pattern.length())return string("EOF while parsing");
			int paramBStart = -1, paramBLength = -1;
			if (pattern[i] == ',')
			{
				paramBStart = i+1;
				paramBLength = 0;
				while (++i < pattern.length() && pattern[i] != '|' && pattern[i] != ',')
					paramBLength++;
			}
			string rep = pattern.substr(repStart, repLength);
			int stop,start = 0;
			if (paramBStart != -1)
			{
				start = evaluate(pattern.substr(paramAStart, paramALength), balls, 0, err);
				if (err.size()) return err;
				stop = evaluate(pattern.substr(paramBStart, paramBLength), balls, 0, err);
				if (err.size()) return err;
			}
			else
			{
				stop = evaluate(pattern.substr(paramAStart, paramALength), balls, 0, err);
				if (err.size()) return err;
			}
			for (int j = start; j < stop; j++)
			{
				for (int k = 0; k < rep.length(); k++)
				{
					if (rep[k] == '{')
					{
						int end = rep.find('}', k);
						if (end == string::npos) return string("No ending '}'");
						int n = evaluate(rep.substr(k+1, end-k-1), balls, j, err);
						if (err.size()) return err;
						result += intToSiteswap(n, lower);
						k = end;
					}
					else result += rep[k];
				}
			}
		}
		else result += pattern[i];
	}
	return result;
}

#define PBI pair<bool,int>
#define isOp(c) (c=='+'||c=='-'||c=='/'||c=='*')
#define opPrec(c) ((c=='(')?0:(c=='+'||c=='-')?1:2)
#define opGTE(a,b) (opPrec(a)>=opPrec(b))
#define cToInt(c) (c-'0')

queue<PBI > infixToPostfix(string infix, string &err, int N, int I)
{
	queue<PBI > res;
	stack<int> ops;
	for (int i = 0; i < infix.length(); i++)
	{
		int n;
		if (isdigit(infix[i]))
		{
			n = cToInt(infix[i]);
			while (i+1<infix.length() && isdigit(infix[i+1])) n = n*10 + cToInt(infix[++i]);
			res.push(PBI(false,n));
		}
		else if (isOp(infix[i]))
		{
			while (ops.size() && opGTE(ops.top(), infix[i]))
			{
				res.push(PBI(true,ops.top()));
				ops.pop();
			}
			ops.push(infix[i]);
		}
		else if (infix[i] == '(')ops.push('(');
		else if (infix[i] == ')')
		{
			while (ops.size() && ops.top() !='(')
			{
				res.push(PBI(true,ops.top()));
				ops.pop();
			}
			if (ops.empty()) { err="Unmatched parenthesis"; return queue<PBI >(); }
			else ops.pop();
		}
		else if (infix[i] == ' ' || infix[i] == '\t');
		else if (infix[i] == 'n')res.push(PBI(false,N));
		else if (infix[i] == 'i')res.push(PBI(false,I));
		else { err = "Invalid character"; return queue<PBI >(); }
	}
	while (ops.size()) { res.push(PBI(true,ops.top())); ops.pop(); }
	return res;
}

int evalPostfix(queue<PBI > q, string &err)
{
	stack<int> s;
	while (q.size())
	{
		PBI f = q.front(); q.pop();
		if (f.first)
		{
			if (s.empty()) { err="Invalid expression"; return 0; }
			int a = s.top(); s.pop();
			if (s.empty()) { err="Invalid expression"; return 0; }
			int b = s.top(), n; s.pop();
			switch(f.second)
			{
				case '+':n=b+a;break;
				case '-':n=b-a;break;
				case '*':n=b*a;break;
				case '/':n=b/a;break;
				default:err="Invalid operator";return 0;
			}
			s.push(n);
		}
		else s.push(f.second);
	}
	if (s.size() != 1) { err="Invalid expression"; return 0; }
	return s.top();
}

int evaluate(string expr, int n, int i, string &err)
{
	queue<PBI > q = infixToPostfix(expr, err, n, i);
	if (err.size())return -9999;
	int res = evalPostfix(q, err);
	if (err.size())return -9999;
	return res;
}

string intToString(int n)
{
   ostringstream ss;
   ss << n;
   return ss.str();
}

string intToSiteswap(int s, bool lower)
{
    string x = "";
    if (s < 10) return intToString(s);
    else if (s < 36 && lower && s != 33) return x+(char)(s + (int)'a' - 10);
    else return "{" + intToString(s) + "}";
}

};

};