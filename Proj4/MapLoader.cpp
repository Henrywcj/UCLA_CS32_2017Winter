#include "provided.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	std::vector<StreetSegment> m_street;
};

MapLoaderImpl::MapLoaderImpl()
{
	//TODO: anything??
}
MapLoaderImpl::~MapLoaderImpl()
{
	//TODO: should I use pointer? 
}


bool MapLoaderImpl::load(string mapFile)
{
	//open the file
	ifstream inFile(mapFile);
	//inFile.open(mapFile);
	//check for error 
	if (!inFile) return false;
	//if (inFile.fail()) return false;
	//read the data line by line
	string s;
	int ii = 0;
	GeoCoord start, end, attractGeoCoord;
	GeoSegment seg;
	while (getline(inFile,s))
	{
		StreetSegment newSeg;
		//ii++; cerr << ii << endl; //testing
		//get the name
		newSeg.streetName = s;
		//get the segment
		getline(inFile, s);
		string s1 = "", s2 = "", s3 = "", s4 = "";
		int i, j;
		for (i = 0; i < s.length(); i++)
			if (s[i] == ',') break;
		s1 = s.substr(0, i);
		if (s[i + 1] == ' ') i++;
		for (j = i + 1; j < s.length(); j++)
			if (s[j] == ' ') break;
		s2 = s.substr(i + 1, j - i - 1);
		for (i = j + 1; i < s.length(); i++)
			if (s[i] == ',') break;
		s3 = s.substr(j + 1, i - j - 1);
		if (s[i + 1] == ' ') i++;
		s4 = s.substr(i + 1, s.length() - i);
		start = GeoCoord(s1, s2); end = GeoCoord(s3, s4);
		seg = GeoSegment(start, end);
		newSeg.segment = seg;
		//get the number of attractions
		getline(inFile, s);
		if (stoi(s) == 0) m_street.push_back(newSeg);
		else
		{
			int count = stoi(s);
			//put all attractions into attraction vector
			for (int k = 0; k < count; k++)
			{
				getline(inFile, s);
				for (i = 0; i < s.length(); i++)
					if (s[i] == '|') break;
				s1 = s.substr(0, i);
				for (j = i + 1; j < s.length(); j++)
					if (s[j] == ',') break;
				s2 = s.substr(i + 1, j - i - 1);
				if (s[j + 1] == ' ') j++;
				s3 = s.substr(j + 1, s.length() - j - 1);
				attractGeoCoord = GeoCoord(s2, s3);
				Attraction att;
				att.geocoordinates = attractGeoCoord;
				att.name = s1;
				newSeg.attractions.push_back(att);
			}
			m_street.push_back(newSeg);
		}
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_street.size();
}
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < 0 || segNum >= getNumSegments()) return false;
	else
	{
		seg = m_street[segNum];
		return true;
	}
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
	return m_impl->getSegment(segNum, seg);
}
