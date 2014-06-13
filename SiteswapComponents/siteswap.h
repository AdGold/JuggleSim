#ifndef SITESWAP
#define SITESWAP

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

#include "state.h"
#include "throws.h"
#include "transitions.h"
#include "templateSS.h"

#include <emscripten/bind.h>

namespace SiteswapComponents{

class Siteswap
{
public:
    Siteswap(){lower=true;synchGround=false;}
    Siteswap(string pattern, bool isStack){Siteswap();setSiteswap(pattern, isStack);}
	bool setSiteswap(string _pattern, bool isStack);
	string getSiteswap() const { return siteswap.toSiteswap(lower); }
	string getEntry() const { return entry.toSiteswap(lower); }
	string getExit() const { return exit.toSiteswap(lower); }
	string getDoubleSiteswap() const { return doubleSiteswap.toSiteswap(lower); }
    string getStackNotation() const { return stackNotation.toSiteswap(lower); }
	string getState() const { return state.toString(); }
	string getComment() const { return comment; }
	string getErrMsg() const { return errMsg; }
	int getBalls() const { return balls; }
	int getPeriod() const { return period; }
	int getMaxThrow() const { return maxThrow; }
	int getExcitation() const { return excitation; }
	int getDifficulty() const { return difficulty; }
	//vector<string> getThrows() { return siteswap.getThrows(lower); }
	bool isValid() const { return valid; }
	bool isGround() const { return ground; }
	bool isPureSynch() const { return pureSynch; }
	bool isPureAsynch() const { return pureAsynch; }
	bool isMultiplex() const { return multiplex; }
	bool isVanila() const { return vanila; }
	bool useSynchGround() const { return synchGround; }
	void useSynchGround(bool synchGround) { this->synchGround = synchGround; }
	bool useLower() const { return lower; }
	void useLower(bool lower) { this->lower = lower; }

	string flipped() const;
    string synchEquivelent() const { return synchEquivelent(false); }
    string synchEquivelent(bool hand) const;

    bool loadTemplates() { bool loaded; TemplateSS::load("templates.xml", loaded); return loaded; }
private:
	Throws siteswap, entry, exit, doubleSiteswap, stackNotation;
	State state;
	string comment, errMsg;
	bool valid, ground, pureSynch, pureAsynch, multiplex, vanila;
	bool synchGround, lower;
	int balls, period, maxThrow, excitation, difficulty;
	bool isCrossingThrow(int ss, bool x) const { return (ss % 2)^x; }
	void reset();
    void init_excitation();
    void init_difficulty();
    void init_other();
    void init_transitions();
    void init_stack();
    string expand(string _pattern);
    bool checkvalid(string pattern);
    vector<int> stackToSiteswap(vector<int> stack);
    vector<int> siteswapToStack(vector<int> ss);
};

string putInBestRotation(string pattern);

};

using namespace SiteswapComponents;

EMSCRIPTEN_BINDINGS(siteswap_test) {
    emscripten::class_<Siteswap>("Siteswap")
        .constructor<>()
        .function("setSiteswap", &Siteswap::setSiteswap)
        .function("getSiteswap", &Siteswap::getSiteswap)
        .function("getEntry", &Siteswap::getEntry)
        .function("getExit", &Siteswap::getExit)
        .function("getDoubleSiteswap", &Siteswap::getDoubleSiteswap)
        .function("getStackNotation", &Siteswap::getStackNotation)
        .function("getState", &Siteswap::getState)
        .function("getComment", &Siteswap::getComment)
        .function("getErrMsg", &Siteswap::getErrMsg)
        .function("getBalls", &Siteswap::getBalls)
        .function("getPeriod", &Siteswap::getPeriod)
        .function("getMaxThrow", &Siteswap::getMaxThrow)
        .function("getExcitation", &Siteswap::getExcitation)
        .function("getDifficulty", &Siteswap::getDifficulty)
        .function("isValid", &Siteswap::isValid)
        .function("isGround", &Siteswap::isGround)
        .function("isPureSynch", &Siteswap::isPureSynch)
        .function("isPureAsynch", &Siteswap::isPureAsynch)
        .function("isMultiplex", &Siteswap::isMultiplex)
        .function("isVanila", &Siteswap::isVanila)
	/*bool useSynchGround() const { return synchGround; }
	void useSynchGround(bool synchGround) { this->synchGround = synchGround; }
	bool useLower() const { return lower; }
	void useLower(bool lower) { this->lower = lower; }
*/
        .function("flipped", &Siteswap::flipped)
        .function("synchEquivelent", (string(Siteswap::*)(bool)const)&Siteswap::synchEquivelent)
        .function("loadTemplates", &Siteswap::loadTemplates)
        .property("useSynchGround", (bool(Siteswap::*)()const)&Siteswap::useSynchGround, (void(Siteswap::*)(bool))&Siteswap::useSynchGround)
        .property("useLower", (bool(Siteswap::*)()const)&Siteswap::useLower, (void(Siteswap::*)(bool))&Siteswap::useLower)
        ;
}

#endif //SITESWAP
