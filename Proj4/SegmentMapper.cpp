#include "provided.h"
#include <vector>
#include "MyMap.h"
using namespace std;
//80 line -30-40-30-30-30 = -160
class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	//vector --> store multiple StreetSegment
	MyMap<std::string, std::vector<StreetSegment>> m_map; //Cannot compare GeoCoord, so make it string
	void putGeoCoordIntoMap(string coord, StreetSegment streetSeg);
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	size_t numOfSeg = ml.getNumSegments();
	for (int i = 0; i < numOfSeg; i++) //for each Street Segment in MapLoader
	{
		//get the current StreetSegment
		StreetSegment currentSeg;
		bool bj = ml.getSegment(i, currentSeg);
		if (!bj) return;
		//get the start GeoCoord and the end GeoCoord
		string start = currentSeg.segment.start.latitudeText + ',' + currentSeg.segment.start.longitudeText;
		string end = currentSeg.segment.end.latitudeText + ',' + currentSeg.segment.end.longitudeText;
		//put them into the map
		putGeoCoordIntoMap(start, currentSeg);
		putGeoCoordIntoMap(end, currentSeg);
		//check for attraction
		int numOfAtt = currentSeg.attractions.size();
		if (numOfAtt == 0) continue;
		else
			for (int j = 0; j < numOfAtt; j++) //for each attraction
			{
				string geoCoord = currentSeg.attractions[j].geocoordinates.latitudeText + ',' +
					currentSeg.attractions[j].geocoordinates.longitudeText;
				putGeoCoordIntoMap(geoCoord, currentSeg);
			}
	}
	return;
}
void SegmentMapperImpl::putGeoCoordIntoMap(string coord, StreetSegment streetSeg)
{
	vector<StreetSegment>* valuePointer = m_map.find(coord);
	if (valuePointer == nullptr)
	{
		vector<StreetSegment> value;
		value.push_back(streetSeg);
		m_map.associate(coord, value);
	}
	else
	{
		vector<StreetSegment> value = *valuePointer;
		value.push_back(streetSeg);
		m_map.associate(coord, value);
	}
}
vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& geoCoord) const
{
	vector<StreetSegment> segment;
	string coord = geoCoord.latitudeText + ',' + geoCoord.longitudeText;
	if (m_map.find(coord) == nullptr) return segment; //no objects found on this geoCoordinates
	else
	{
		segment = *m_map.find(coord); 
		return segment;
	}
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
