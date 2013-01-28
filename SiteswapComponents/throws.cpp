#include "throws.h"

namespace SiteswapComponents{

string Throws::setThrows(string pattern)
{
    if (!parse(pattern)) return errMsg.str();
    return "";
}

void Throws::setThrows(vector<Throw> pattern)
{
    throws.clear();
    for (Throw th : pattern) { throws.push_back(th); }
}

string Throws::toSiteswap(bool lower) const
{
    string result = "";
    for (string &str : getThrows(lower)) { result += str; }
    string add = "";
    if (throws.size() > 0 && throws[0].getLHThrows().size() == 0 && throws[0].getRHThrows().size() > 0)
        add = "R";
    return add + result;
}
string Throws::stringRep() const
{
    string result = "";
    for (Throw th : throws) { result += th.stringRep(); }
    return result;
}
vector<string> Throws::getThrows(bool lower) const
{
    vector<string> result;
    for (int i = 0; i < throws.size(); i++)
    {
        string add = throws[i].toSiteswap(lower);
        if (throws[i].hasSynch())
        {
            if (i + 1 >= throws.size() || !throws[i + 1].isEmpty())
            {
                add += '!';
            }
        }
        result.push_back(add);
    }
    return result;
}
void Throws::flip()
{
    for (Throw &th : throws) { th.flip(); }
}

int Throws::sum() const
{
    int total = 0;
    for (Throw th : throws) { total += th.sum(); }
    return total;
}
int Throws::max() const
{
    int max = 0;
    for (Throw th : throws)
    {
        int m = th.max();
        if (m > max) max = m;
    }
    return max;
}

int Throws::getNum(string pattern, int &i)
{
    if (isdigit(pattern[i])) { return pattern[i] - '0'; }
    else if (islower(pattern[i])) { return pattern[i] - 'a' + 10; }
    else if (pattern[i] == '{')
    {
        int num = 0;
        int period2 = pattern.length();
        i++;
        while (i < period2 && pattern[i] != '}')
        {
            if (isdigit(pattern[i]))
            {
                num *= 10;
                num += pattern[i] - '0';
            }
            else { errMsg<<"Invalid input character '"<<pattern[i]<<"' in curly bracket."; return -1; }
            i++;
        }
        if (num > 99) {errMsg<<"Highest possible throw is 99."; return -2; }
        if (i < period2) { return num; }
        else { errMsg<<"No closing curly bracket"; return -1; }
    }
    else { errMsg<<"Invalid input character: '"<<pattern[i]<<"'"; return -1; }
}
pair<int,bool> Throws::getSingleThrow(string pattern, int &i)
{
    pair<int,bool> result;
    bool crossing = false;
    int num = getNum(pattern, i);
    if (num < 0)
    {
        i = -1;
        result.first = result.second = -1;
        return result;
    }
    if (i + 1 < pattern.length() && pattern[i + 1] == 'x') { crossing = true; i++; }
    result.first = num;
    result.second = crossing;
    return result;
}
pair<vector<int>,vector<bool> > Throws::getMultiplex(string pattern, int &i)
{
    pair<vector<int>,vector<bool> > result;
    vector<int> nums;
    vector<bool> crosses;
    if (pattern[i] != '[')
    {
        pair<int,bool> th = getSingleThrow(pattern, i);
        if (i < 0)
        {
            //errMsg taken from getSingleThrow
            //i = -1;
            return result;
        }
        nums.push_back(th.first);
        crosses.push_back(th.second);
        result.first = nums;
        result.second = crosses;
        return result;
    }
    int num = -1;
    i++;
    if (i == pattern.length() || pattern[i] == ']')
    {
        errMsg<<"No closing square bracket";
        i = -1;
        return result;
    }
    while (i < pattern.length() && pattern[i] != ']')
    {
        pair<int,bool> th = getSingleThrow(pattern, i);
        if (th.first < 0)
        {
            //errMsg taken from getSingleThrow
            i = -1;
            return result;
        }
        nums.push_back(th.first);
        crosses.push_back(th.second);
        i++;
    }
    if (i == pattern.length())
    {
        errMsg<<"No closing square bracket.";
        i = -1;
        return result;
    }
    result.first = nums;
    result.second = crosses;
    return result;
}
Throw Throws::getThrow(string pattern, int &i, int hand)
{
    Throw t;
    if (i >= pattern.length()) { errMsg<<"EOF while parsing"; i = -1; return t; }
    if (pattern[i] == '(')
    {
        i++;
        Throw left, right;
        if (i >= pattern.length()) { errMsg<<"EOF while parsing"; i = -1; return t; }
        left = getThrow(pattern, i, 0);
        if (i < 0) { /*i = -1;*/ return t; }
        i++;
        if (i >= pattern.length()) { errMsg<<"EOF while parsing"; i = -1; return t; }
        if (pattern[i] != ',') { errMsg<<"No comma"; i = -1; return t; }
        i++;
        if (i >= pattern.length()) { errMsg<<"EOF while parsing"; i = -1; return t; }
        if (pattern[i] == ' ') i++;
        right = getThrow(pattern, i, 1);
        if (i < 0) { /*i = -1;*/ return t; }
        i++;
        if (i >= pattern.length()) { errMsg<<"EOF while parsing"; i = -1; return t; }
        if (pattern[i] != ')') { errMsg<<"No end bracket."; i = -1; return t; }
        t.setRHThrows(right.getRHThrows());
        t.setRHCrossing(right.getRHCrossing());
        t.setLHThrows(left.getLHThrows());
        t.setLHCrossing(left.getLHCrossing());
        return t;
    }
    else
    {
        pair<vector<int>,vector<bool> > multiplex = getMultiplex(pattern, i);
        if (i < 0)
        {
            //errMsg taken from getMultiplex
            //i = -1;
            return t;
        }
        if (hand == 0)
        {
            t.setLHCrossing(multiplex.second);
            t.setLHThrows(multiplex.first);
        }
        else
        {
            t.setRHCrossing(multiplex.second);
            t.setRHThrows(multiplex.first);
        }
        return t;
    }
}
bool Throws::parse(string pattern)
{
    throws.clear();
    errMsg.str("");
    errMsg.clear();
    int hand = 0;
    for (int i = 0; i < pattern.length(); i++)
    {
        if (pattern[i] == 'R') { hand = 1; i++; }
        else if (pattern[i] == 'L') { hand = 0; i++; }
        if (i == pattern.length())
        {
            errMsg<<"No number after hand identifier.";
            return false;
        }
        Throw add;
        bool blank = false, repeat = false;
        int i2 = i;
        add = getThrow(pattern, i, hand);
        if (pattern[i2] == '(')
        {
            if (i + 1 < pattern.length() && pattern[i + 1] == '!')
                i++;
            else blank = true;
            if (i + 1 < pattern.length()  && pattern[i + 1] == '*')
            {
                i++;
                repeat = true;
            }
            hand = 0;
        }
        if (i < 0) { return false; }
        throws.push_back(add);
        if (blank)
        {
            Throw blankAdd;
            throws.push_back(blankAdd);
        }
        if (repeat)
        {
            Throws repeatAdd;
            repeatAdd.setThrows(throws);
            repeatAdd.flip();
            for (int i = 0; i < repeatAdd.size(); i++) throws.push_back(repeatAdd[i]);
        }
        hand = 1 - hand;
    }
    return true;
}

};
