#include "provided.h"
#include "MyMap.h"

#include <string>
using namespace std;
//70 lines
class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<std::string, GeoCoord> m_map;
	int m_size;
	std::string lowerTheString(std::string s) const;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}
AttractionMapperImpl::~AttractionMapperImpl()
{
	//TODO
}

std::string AttractionMapperImpl::lowerTheString(std::string str) const
{
	std::string ss = str;
	for (int i = 0; i < str.length(); i++)
		ss[i] = tolower(ss[i]);
	return ss;
}

void AttractionMapperImpl::init(const MapLoader &ml)
{
	size_t numOfSeg = ml.getNumSegments();
	for (int i = 0; i < numOfSeg; i++) //for each Street Segment in MapLoader
	{
		//bool bbb = true; //testing
		//if (i % 1000 == 0)
			//bbb = true; //testing
		StreetSegment currentSeg;
		bool bj = ml.getSegment(i, currentSeg);
		if (!bj) return;
		if (currentSeg.attractions.size() == 0) continue;
		else //There is attraction
		{
			int numOfAtt = currentSeg.attractions.size();
			for (int j = 0; j < numOfAtt; j++) //for each attraction
			{
				string s = lowerTheString(currentSeg.attractions[j].name);
				string name = s;
				GeoCoord geoCoord = currentSeg.attractions[j].geocoordinates;
				m_map.associate(name, geoCoord);
			}
		}
	}
}


bool AttractionMapperImpl::getGeoCoord(std::string attraction, GeoCoord &gc) const
{
	string s = lowerTheString(attraction); //case insensitive

	if (m_map.find(s) == nullptr) return false; //cannot find the attraction
	else
	{
		gc = *m_map.find(s); 
		return true;
	}
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
