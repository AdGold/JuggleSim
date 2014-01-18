#include "patternSettings.h"

namespace AnimationComponents {

PatternSettings::PatternSettings()
{
	currentFile = siteswap = "";
	speed = 400;
	ballSize = 10;
	stack = reverse = false;
	gravity = 980;
	throwsPerSec = 5;
	dwell= 0.25;
	background = Colour(255,255,255);
}

Colour readColour(xml_node col)
{
	Colour c;
	c.r = stoi(col.child("red").child_value());
	c.b = stoi(col.child("blue").child_value());
	c.g = stoi(col.child("green").child_value());
	return c;
}

xml_node writeColour(Colour col, xml_node parent)
{
	xml_node c = parent.append_child();
	xml_node red = c.append_child();
	xml_node blue= c.append_child();
	xml_node green = c.append_child();
	red.set_name("red");
	blue.set_name("blue");
	green.set_name("green");
	red.append_child(node_pcdata).set_value(to_string(col.r).c_str());
	blue.append_child(node_pcdata).set_value(to_string(col.b).c_str());
	green.append_child(node_pcdata).set_value(to_string(col.g).c_str());
	return c;
}

bool PatternSettings::open(string file)
{
	currentFile = file;
	xml_document doc;
	xml_parse_result result = doc.load_file(file.c_str());
	if (!result) return false;
	xml_node pattern = doc.child("pattern");
	setSiteswap(pattern.child("siteswap").child_value());
	setSpeed(stoi(pattern.child("speed").child_value()));
	setBallSize(stoi(pattern.child("ballsize").child_value()));
	setIsStack(stoi(pattern.child("stack").child_value()));
	setIsReverse(stoi(pattern.child("reverse").child_value()));
	setGravityInternal(stoi(pattern.child("gravity").child_value()));
	setThrowsPerSecond(stoi(pattern.child("throwspersec").child_value()));
	setDwellTime(stod(pattern.child("dwell").child_value()));
	setBackgroundColour(readColour(pattern.child("background")));
	ballColours.clear();
	for (auto &col : pattern.child("ballcolours"))
	{
		ballColours.push_back(readColour(col));
	}
}

template<class T>
xml_node add(xml_node parent, string name, T val)
{
	xml_node node = parent.append_child();
	node.set_name(name.c_str());
	node.append_child(node_pcdata).set_value(to_string(val).c_str());
	return node;
}

void PatternSettings::save()
{
	xml_document doc;
	xml_node pattern = doc.append_child();
	pattern.set_name("pattern");
	xml_node ss = pattern.append_child();
	ss.set_name("siteswap");
	ss.append_child(node_pcdata).set_value(getSiteswap().c_str());
	add(pattern, "speed", getSpeed());
	add(pattern, "ballsize", getBallSize());
	add(pattern, "stack", isStack());
	add(pattern, "reverse", isReverse());
	add(pattern, "gravity", getGravityInternal());
	add(pattern, "throwspersec", getThrowsPerSecond());
	add(pattern, "dwell", getDwellTime());
	xml_node background = writeColour(getBackgroundColour(), pattern);
	background.set_name("background");
	xml_node ballcolours = pattern.append_child();
	ballcolours.set_name("ballcolours");
	for (auto &c : getBallColours())
	{
		xml_node col = writeColour(c, ballcolours);
		col.set_name("colour");
	}
	doc.save_file(currentFile.c_str());
}

}
