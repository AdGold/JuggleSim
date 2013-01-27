#include "state.h"

namespace SiteswapComponents{

void State::setElements(Throws doubleSiteswap, int balls, int maxThrow)
{
    excitation = -1;
    asynch = doubleSiteswap.countNum([](const Throw &th){return th.hasSynch();}) == 0;
    elements.clear();
    Throws dsiteswap;
    for (int i = 0; i < balls; i++)
    {
        dsiteswap.addRange(doubleSiteswap);
    }
    for (int i = 0; i < maxThrow; i++)
    {
        elements.push_back(StateEl());
    }
    for (int i = 0; i < dsiteswap.size(); i++)
    {
        vector<int> RHT = dsiteswap[i].getRHThrows();
        vector<bool> RHC = dsiteswap[i].getRHCrossing();
        for (int j = 0; j < RHT.size(); j++)
        {
            if (RHT[j] + i >= dsiteswap.size())
            {
                int k = 1;
                if (isCrossingThrow(RHT[j], RHC[j])) k = 0;
                int place = (RHT[j] + i - dsiteswap.size());
                elements[place][k]++;
            }
        }
        vector<int> LHT = dsiteswap[i].getLHThrows();
        vector<bool> LHC = dsiteswap[i].getLHCrossing();
        for (int j = 0; j < LHT.size(); j++)
        {
            if (LHT[j] + i >= dsiteswap.size())
            {
                int k = 0;
                if (isCrossingThrow(LHT[j], LHC[j])) k = 1;
                int place = (LHT[j] + i - dsiteswap.size());
                elements[place][k]++;
            }
        }
    }
    for (int i = elements.size() - 1; i >= 0; i--)
    {
        if (elements[i][0] == 0 && elements[i][1] == 0)
        {
            elements.pop_back();
        }
        else break;
    }
}

void State::setElements(vector<StateEl> pattern)
{
    excitation = -1;
    elements.clear();
    for (StateEl e : pattern) { elements.push_back(e); }
}

string State::toString() const
{
    string result = "";
    if (elements.size() == 0)return result;
    if (asynch)
    {
        StateEl el = elements[0];
        int side = 0;
        if (el[0] == 0) side = 1;
        for (StateEl e : elements)
        {
            result += e.toString(side);
            side = 1 - side;
        }
        return result;
    }
    else
    {
        for (StateEl e : elements) { result += e.toString(2); }
        return result;
    }
}

string State::stringRep() const
{
    string result = "";
    for (StateEl st : elements)result += st.stringRep();
    return result;
}

const bool State::operator==(const State &st) const
{
    if (size() != st.size())return false;
    for (int i = 0; i < size(); i++)
    {
        if ((*this)[i] != st[i])return false;
    }
    return true;
}

State getGround(int balls, int side, bool synch)
{
    State ground;
    if (synch)
    {
        for (int i = 0; i < balls / 2; i++)
        {
            ground.add(StateEl(1,1));
            ground.add(StateEl());
        }
        if (balls % 2 != 0) ground.add(StateEl(1-side,side));
    }
    else
    {
        for (int i = 0; i < balls; i++,side=1-side) ground.add(StateEl(1-side,side));
    }
    return ground;
}

};