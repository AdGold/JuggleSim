#ifndef STATE
#define STATE

#include <vector>
#include <string>
using namespace std;

#include "stateEl.h"
#include "throws.h"

namespace SiteswapComponents{

class State
{
public:
    State() { }
    State(const State &state) { setElements(state.getElements()); }
    vector<StateEl> getElements() const { return elements; }
    void setElements(Throws doubleSiteswap, int balls, int maxThrow);
    void setElements(vector<StateEl> pattern);
    string toString() const;
    string stringRep() const;
    void coutStringRep() const {for (StateEl st : elements){st.coutStringRep();}cout<<endl;}
    StateEl& operator[] (int x) { return elements[x]; }
    const StateEl& operator[] (int x) const { return elements[x]; }
    void removeAt(int index) { elements.erase(elements.begin()+index); }
    void add(StateEl th) { elements.push_back(th); }
    void clear() { elements.clear(); }
    void addRange(vector<StateEl> list) { for (StateEl th : list) { add(th); } }
    void addRange(State moreThrows) { addRange(moreThrows.getElements()); }
    int size() const { return elements.size(); }
    void insert(int index, StateEl th) { elements.insert(elements.begin()+index, th); }

    const bool operator==(const State &st) const;
    const bool operator!=(const State &st) const { return !((*this)==st); }
private:
    bool asynch;
    int excitation;
    vector<StateEl> elements;
    bool isCrossingThrow(int ss, bool x) const { return (ss % 2)^x; }
};

State getGround(int balls, int side, bool synch);

};

#endif //STATE