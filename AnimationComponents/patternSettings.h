#ifndef PATTERNSETTINGS
#define PATTERNSETTINGS

#include <string>
#include <vector>
using namespace std;
#include "../SiteswapComponents/siteswap.h"
using namespace SiteswapComponents;
#include "../pugixml/pugixml.hpp"

namespace AnimationComponents{

class Colour
{
public:
	int r,g,b;
	Colour():r(0),b(0),g(0){}
	Colour(int R, int B, int G):r(R),b(B),g(G){}
};

class PatternSettings
{
private:
	Colour background;
	vector<Colour> ballColours;
	int speed, ballSize, throwsPerSec;
	double dwell, gravity;
	bool stack, reverse;
	string currentFile, siteswap;
public:
	PatternSettings();
	bool open(string file);
	void save();
	string getSiteswap(){return siteswap;}
	void setSiteswap(string ss){siteswap = ss;}
	int getSpeed(){return speed;}
	void setSpeed(int sp){speed = sp;}
	int getBallSize(){return ballSize;}
	void setBallSize(int bs){ballSize=bs;}
	int getGravityInternal(){return gravity;}
	void setGravityInternal(int g){gravity = g;}
	double getGravity(){return (double)gravity/-100;}
	void setGravity(double g){gravity = g*-100;}
	int getThrowsPerSecond(){return throwsPerSec;}
	void setThrowsPerSecond(int tps){throwsPerSec = tps;}
	double getBeatLength(){return  1.0/throwsPerSec;}
	void setBeatLength(double bl){throwsPerSec = 1.0/bl;}
	double getDwellTime(){return dwell;}
	void setDwellTime(double dt){dwell = dt;}
	Colour getBackgroundColour(){return background;}
	void setBackgroundColour(Colour c){background = c;}
	bool isSiteswap(){return !stack;}
	void setIsSiteswap(bool ss){stack = !ss;}
	bool isStack(){return stack;}
	void setIsStack(bool st){stack = st;}
	bool isReverse(){return reverse;}
	void setIsReverse(bool r){reverse = r;}
	double realSpeed(){return (double)((reverse)?-1:1)*speed/30000.0;}
	vector<Colour> getBallColours(){return ballColours;}
	void setBallColours(vector<Colour> vc){ballColours=vc;}
	string getCurrentFile(){return currentFile;}
	void setCurrentFile(string f){currentFile = f;}
};

};

#endif //PATTERNSETTINGS
