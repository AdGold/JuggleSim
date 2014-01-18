#include "siteswap.h"

namespace SiteswapComponents{

bool Siteswap::setSiteswap(string _pattern, bool isStack)
{
    reset();
    if (_pattern.length() == 0) return false;
    int index = _pattern.find('#');
    if (index != string::npos)
    {
        if (index + 1 < _pattern.length()) comment = _pattern.substr(index + 1);
        else comment = "";
        while (index - 1 > 0 && (_pattern[index - 1] == ' ' ||_pattern[index - 1] == '\t')) index--;
        _pattern = _pattern.substr(0, index);
    }
    else comment = "";
    if (TemplateSS::isTemplate(_pattern))
    {
        _pattern = TemplateSS::expandTemplate(_pattern, lower, errMsg);
        if (errMsg.size()) return valid = false;
        isStack = false;
    }
    string pattern = expand(_pattern);
    if (!(errMsg = siteswap.setThrows(pattern)).empty()) return false;
    
    if (isStack)
    {
        pureAsynch = siteswap.countNum([](Throw &s){return s.hasSynch();}) == 0;
        multiplex = siteswap.countNum([](Throw &s){return s.hasMultiplex();}) != 0;
        if (!pureAsynch || multiplex)
        {
            errMsg = "Stack notation must be vanila.";
            return false;
        }
        int side = (bool)siteswap[0].getRHThrows().size();
        vector<int> ss, stack;
        for (int i = 0; i < siteswap.size(); i++, side=1-side)
        {
            stack.push_back(((side)?siteswap[i].getRHThrows():siteswap[i].getLHThrows())[0]);
        }
        ss = stackToSiteswap(stack);
        side = (bool)siteswap[0].getRHThrows().size();
        siteswap.clear();
        for (int i = 0; i < ss.size(); i++, side=1-side)
        {
            siteswap.add(Throw(ss[i], false, side));
            stackNotation.add(Throw(stack[i], false, side));
        }
    }
    valid = checkvalid(pattern);
    if (valid)
    {
        init_other();
        state.setElements(doubleSiteswap, balls, maxThrow);
        init_excitation();
        init_transitions();
        if (pureAsynch && !isStack && !multiplex) init_stack();
    }
    return valid;
}

string Siteswap::flipped() const
{
    Throws flippedSiteswap(siteswap);
    flippedSiteswap.flip();
    return flippedSiteswap.toSiteswap(lower);
}
string Siteswap::synchEquivelent(bool hand) const
{
    if (doubleSiteswap.size() == 0) return "";
    bool flipAtEnd = false;
    if (!doubleSiteswap[0].hasSynch())
    {
        if ((doubleSiteswap[0].getRHThrows().size() > 0 && hand)
            || (doubleSiteswap[0].getLHThrows().size() > 0 && !hand))
        {
            hand = !hand;
            flipAtEnd = true;
        }
    }
    int RHDelta = -1;
    int LHDelta = 1;
    if (hand)
    {
        RHDelta = 1;
        LHDelta = -1;
    }
    Throws synchEquiv;
    for (int i = 0; i < doubleSiteswap.size(); i += 2)
    {
        Throw th;
        vector<int> RHThrow, LHThrow;
        vector<bool> RHCrossing, LHCrossing;
        for (int m = 0; m < 2; m++)
        {
            vector<int> RH = doubleSiteswap[i + m].getRHThrows();
            vector<bool> RHC = doubleSiteswap[i + m].getRHCrossing();
            for (int k = 0; k < RH.size(); k++)
            {
                bool crossing = isCrossingThrow(RH[k], RHC[k]);
                if (crossing)
                {
                    RHThrow.push_back(RH[k] + RHDelta);
                    RHCrossing.push_back(!RHC[k]);
                }
                else
                {
                    RHThrow.push_back(RH[k]);
                    RHCrossing.push_back(RHC[k]);
                }
            }
        }
        for (int m = 0; m < 2; m++)
        {
            vector<int> LH = doubleSiteswap[i + m].getLHThrows();
            vector<bool> LHC = doubleSiteswap[i + m].getLHCrossing();
            for (int k = 0; k < LH.size(); k++)
            {
                bool crossing = isCrossingThrow(LH[k], LHC[k]);
                if (crossing)
                {
                    LHThrow.push_back(LH[k] + LHDelta);
                    LHCrossing.push_back(!LHC[k]);
                }
                else
                {
                    LHThrow.push_back(LH[k]);
                    LHCrossing.push_back(LHC[k]);
                }
            }
        }
        Throw add;
        add.setRHThrows(RHThrow);
        add.setRHCrossing(RHCrossing);
        add.setLHThrows(LHThrow);
        add.setLHCrossing(LHCrossing);
        synchEquiv.add(add);
        synchEquiv.add(Throw());
    }
    if (!hand)
    {
        //shift LH throws two beats
        Throw temp = synchEquiv[synchEquiv.size() - 2];
        vector<int> iTemp = temp.getLHThrows();
        vector<bool> bTemp = temp.getLHCrossing();
        for (int i = synchEquiv.size() - 4; i >= 0; i -= 2)
        {
            synchEquiv[i + 2].setLHThrows(synchEquiv[i].getLHThrows());
            synchEquiv[i + 2].setLHCrossing(synchEquiv[i].getLHCrossing());
        }
        synchEquiv[0].setLHThrows(iTemp);
        synchEquiv[0].setLHCrossing(bTemp);
    }
    if (flipAtEnd) synchEquiv.flip();
    string str = synchEquiv.toSiteswap(lower);
    return str;
}

void Siteswap::reset()
{
    siteswap.clear();
    doubleSiteswap.clear();
    state.clear();
    entry.clear();
    exit.clear();
    stackNotation.clear();
    comment = "";errMsg = "";
    ground = valid = pureSynch = pureAsynch = multiplex = false;
    maxThrow = difficulty = balls = period = 0;
    excitation = -999;
}
void Siteswap::init_excitation()
{
    if (pureAsynch)
    {
        int total = 0;
        int g = (int)((balls + 1) * balls / 2.0);
        for (int i = 0; i < state.size(); i++)
        {
            total += state[i][0] * (i + 1);
            total += state[i][1] * (i + 1);
        }
        excitation = total - g;
    }
    else if (pureSynch)
    {
        int total = 0;
        int g = 0;
        for (int i = 0; i < balls; i += 2) g += (i + 1) * 2;
        if (balls % 2 == 1) g += balls + 1;
        for (int i = 0; i < state.size(); i++)
        {
            total += state[i][0] * (i + 1);
            total += state[i][1] * (i + 1);
        }
        excitation = total - g;
    }
    else
    {
        int total = 0;
        int g = 0;
        for (int i = 0; i < balls; i++) g += (i + 1) * 2;
        for (int i = 0; i < state.size(); i++)
        {
            total += state[i][0] * (i + 1);
            total += state[i][1] * (i + 1);
        }
        excitation = total - g;
    }
    ground = (excitation == 0);
}
void Siteswap::init_difficulty()
{
    difficulty = 1;
}
void Siteswap::init_other()
{
    if (siteswap.size() % 2 != 0) pureSynch = false;
    else
    {
        pureSynch = siteswap.countNum([](Throw &s){return s.hasSynch();}) == siteswap.size() / 2;
    }
    pureAsynch = siteswap.countNum([](Throw &s){return s.hasSynch();}) == 0;
    multiplex = siteswap.countNum([](Throw &s){return s.hasMultiplex();}) != 0;
    vanila = pureAsynch && !multiplex;
    maxThrow = siteswap.max();
}

void Siteswap::init_transitions()
{
    State ground0 = getGround(balls, 0, synchGround);
    State ground1 = getGround(balls, 1, synchGround);
    Throws entry1 = Transitions::getTransition(ground0, state);
    Throws entry2 = Transitions::getTransition(ground1, state);
    Throws exit1 = Transitions::getTransition(state, ground0);
    Throws exit2 = Transitions::getTransition(state, ground1);
    int enl1 = entry1.size(), exl1 = exit1.size(), enl2 = entry2.size(), exl2 = exit2.size();
    int enSl1 = entry1.toSiteswap(lower).length(), exSl1 = exit1.toSiteswap(lower).length();
    int enSl2 = entry2.toSiteswap(lower).length(), exSl2 = exit2.toSiteswap(lower).length();
    int enChoice = 0, exChoice = 0;
    if (synchGround)
    {
        if (enl1 + exl1 < enl2 + exl2);
        else if (enl1 + exl1 > enl2 + exl2) enChoice = exChoice = 1;
        else if (enSl1 + exSl1 < enSl2 + exSl2);
        else if (enSl1 + exSl1 > enSl2 + exSl2) enChoice = exChoice = 1;
    }
    else
    {
        if (enl1 < enl2) ;
        else if (enl1 > enl2) enChoice = 1;
        else if (enSl1 < enSl2) ;
        else if (enSl1 > enSl2) enChoice = 1;
        if (exl1 < exl2) ;
        else if (exl1 > exl2) exChoice = 1;
        else if (exSl1 < exSl2) ;
        else if (exSl1 > exSl2) exChoice = 1;
    }
    entry.addRange(((enChoice==0)?entry1:entry2).getThrows());
    exit.addRange(((exChoice==0)?exit1:exit2).getThrows());
    if (!synchGround)
    {
        if (entry.toSiteswap(lower)[0]=='R')entry.flip();
        if (exit.toSiteswap(lower)[0]=='R')exit.flip();
    }
}

void Siteswap::init_stack()
{
    int side = (bool)siteswap[0].getRHThrows().size();
    vector<int> ss, stack;
    for (int i = 0; i < siteswap.size(); i++, side=1-side)
    {
        ss.push_back(((side)?siteswap[i].getRHThrows():siteswap[i].getLHThrows())[0]);
    }
    stack = siteswapToStack(ss);
    side = (bool)siteswap[0].getRHThrows().size();
    for (int th:stack)
    {
        stackNotation.add(Throw(th, false, side));
        side=1-side;
    }
}

vector<int> Siteswap::siteswapToStack(vector<int> ss)
{
    int n = ss[0], p = ss.size();
    vector<int> stack;
    for (int i = 0; i < p; i++, n = ss[i])
    {
        for (int j = 1; j < ss[i]; j++)
        {
            if (j+ss[(i+j)%p] < ss[i]) n--;
        }
        stack.push_back(n);
    }
    return stack;
}

vector<int> Siteswap::stackToSiteswap(vector<int> stack)
{
    int place,p = stack.size();
    vector<int> ss(stack);
    for (int i = 0; i < p; i++)
    {
        place = stack[i];
        for (int j = 1; place > 0; j++)
        {
            if (stack[(i+j)%p] < place)ss[i]++;
            else place--;
        }
    }
    return ss;
}

string Siteswap::expand(string _pattern)
{
    string pattern = "";
    string loop = "";
    bool looping = false;
    for (int i = 0; i < _pattern.length(); i++)
    {
        if (_pattern[i] == '|')
        {
            looping = true;
        }
        else if (_pattern[i] == '^' && looping)
        {
            int index = _pattern.find('|', i);
            if (index == string::npos)
            {
                errMsg = "EOF while pasing '|...^n|'.";
                return "";
            }
            string times = _pattern.substr(i + 1, (index - 1) - i);
            int nTimes = 0;
            nTimes = strtol(times.c_str(), NULL, 10);
            if (nTimes == 0) { errMsg = "Number not valid."; return ""; }
            if (nTimes > 500)
            {
                errMsg = "Max repeats is 500.";
                return "";
            }
            for (int j = 0; j < nTimes; j++)
            {
                pattern += loop;
            }
            loop = "";
            looping = false;
            i = index;
            if (i + 1 == _pattern.length()) break;
            continue;
        }
        else
        {
            if (looping) loop += _pattern[i];
            else pattern += _pattern[i];
        }
    }
    if (looping) { errMsg = "EOF while pasing '|...^n|'."; return ""; }
    return pattern;
}
bool Siteswap::checkvalid(string pattern)
{
    period = siteswap.size();
    int len = siteswap.size();
    period = len;
    int total2 = siteswap.sum();
    if (len == 0) return false;
    if (total2 % len != 0)
    {
        errMsg = "Invalid pattern average.";
        return false;
    }
    balls = total2 / len;

    doubleSiteswap.clear();
    doubleSiteswap.addRange(siteswap);
    if (len % 2 == 1)
    {
        Throws second(siteswap);
        second.flip();
        doubleSiteswap.addRange(second);
        len *= 2;
    }
    vector<Throw> validate;
    vector<int> throws, catches;
    vector<pair<int,int>> array;
    for (int i = 0; i < len; i++)
    {
        vector<int> LHThrows = doubleSiteswap[i].getLHThrows();
        vector<bool> LHCross = doubleSiteswap[i].getLHCrossing();
        int LH = LHThrows.size();
        for (int j = 0; j < LH; j++)
        {
            throws.push_back((LHThrows[j] + i) % len);
            bool crossing = LHCross[j];
            catches.push_back((isCrossingThrow(LHThrows[j], LHCross[j])) ? 1 : 0);
        }
        vector<int> RHThrows = doubleSiteswap[i].getRHThrows();
        vector<bool> RHCross = doubleSiteswap[i].getRHCrossing();
        int RH = RHThrows.size();
        for (int j = 0; j < RH; j++)
        {
            throws.push_back((RHThrows[j] + i) % len);
            bool crossing = RHCross[j];
            catches.push_back((isCrossingThrow(RHThrows[j], RHCross[j])) ? 0 : 1);
        }
        array.push_back({ LH, RH });
    }
    //	MessageBox.Show("Throws:  "+listToString(throws)+"\r\nCatches: "+listToString(catches));
    for (int i = 0; i < throws.size(); i++)
    {
        int n = throws[i];
        int j = catches[i];
        //array[n][j]--; //-- added on next line
        if (--((j)?array[n].second:array[n].first) < 0)
        {
            errMsg = "Collision in siteswap, did you mean one of these?";
            return false;
        }
    }
    int v = siteswap.countNum([](Throw &n){return n.hasZeroX();});
    if (v > 0) { errMsg = "Has a '0x'."; return false; }
    return true;
}

string putInBestRotation(string pattern)
{
	Siteswap patternClass;
    patternClass.setSiteswap(pattern, false);
    vector<string> rotations(pattern.length());
    for (int i = 1; i < pattern.length(); i++)
    {
        rotations[i - 1] = pattern.substr(i) + pattern.substr(0, i);
    }
    Siteswap test;
    int s = patternClass.getExcitation();
    for (int i = 1; i < pattern.length(); i++)
    {
        if (test.setSiteswap(rotations[i - 1], false))
        {
            if (test.getExcitation() < s)
            {
                pattern = rotations[i - 1];
                s = test.getExcitation();
            }
            else if (test.getExcitation() == s)
            {
                //check which starts on a higher number
                string ss1 = pattern;
                string ss2 = rotations[i - 1];
                for (int j = 0; j < ss1.length(); j++)
                {
                    if (ss2[j] > ss1[j])
                    {
                        pattern = rotations[i - 1];
                        s = test.getExcitation();
                        break;
                    }
                    else if (ss1[j] > ss2[j])
                    {
                        break;
                    }
                }
            }
        }
    }
    return pattern;
}

};
