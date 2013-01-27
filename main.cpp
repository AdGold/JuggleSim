#include <iostream>
using namespace std;

#include "SiteswapComponents/siteswap.h"
using namespace SiteswapComponents;

int main()
{
	Siteswap ss;
	bool loaded;
	TemplateSS::load("templates.xml", loaded);
	if (!loaded)
	{
		cout<<"templates could not be loaded, either templates.xml does not exist or it is not in correct XML format."<<endl;
		return 0;
	}

	// ss.useSynchGround(true);
	while (1)
	{
		cout<<"Enter SS: ";
		char inp[99];
		cin.getline(inp, 99);
		string str=inp;
		ss.setSiteswap(str, false);
		string err = ss.getErrMsg();
		if (err.length()){cout<<"Invalid: "<<err<<endl;continue;}
		cout<<"SS: "<<ss.getSiteswap()<<endl;

		
		//// = unimplemented
		//// cout<<"Difficulty: "<<ss.getDifficulty()<<endl;
		// = working
		// cout<<"Stack equivelent: "<<ss.getStackNotation()<<endl;
		// cout<<"Entry; "<<ss.getEntry()<<endl;
		// cout<<"Exit: "<<ss.getExit()<<endl;
		// cout<<"Ground: "<<ss.isGround()<<endl;
		// cout<<"Flipped: "<<ss.flipped()<<endl;
		// cout<<"Synch equivs: RH forward: "<<ss.synchEquivelent(true)<<", LH forward: "<<ss.synchEquivelent(false)<<endl;
		// cout<<"SS: "<<ss.getSiteswap()<<endl;
		// cout<<"DoubleSiteswap: "<<ss.getDoubleSiteswap()<<endl;
		// cout<<"State: "<<ss.getState()<<endl;
		// cout<<"Comment: "<<ss.getComment()<<endl;
		// cout<<"Balls: "<<ss.getBalls()<<endl;
		// cout<<"Period: "<<ss.getPeriod()<<endl;
		// cout<<"MaxThrow: "<<ss.getBMaxThrow()<<endl;
		// cout<<"Excitation: "<<ss.getExcitation()<<endl;
		// cout<<"Valid: "<<ss.isValid()<<endl;
		// cout<<"PureSynch: "<<ss.isPureSynch()<<endl;
		// cout<<"PureAsynch: "<<ss.isPureAsynch()<<endl;
		// cout<<"Multiplex: "<<ss.isMultiplex()<<endl;
		// cout<<"Vanilla: "<<ss.isVanila()<<endl;
		// cout<<"UseSynchGround: "<<ss.useSynchGround()<<endl;
		// cout<<"UseLower: "<<ss.useLower()<<endl;
		// cout<<"Best rot: "<<putInBestRotation(str)<<endl;
	}
	return 0;
}
