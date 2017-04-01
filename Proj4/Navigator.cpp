#include "provided.h"
#include "MyMap.h"
#include <queue>
#include <string>
#include <vector>
using namespace std;
class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	struct Node1
	{
		string coord;
		double dist;
		Node1(string Co, double dis)
		{
			coord = Co;
			dist = dis;
		}
	};
	struct NodeComp
	{
		bool operator()(const Node1& a, const Node1& b)
		{
			return a.dist > b.dist;
		}
	};
	MapLoader m_mapLoader;
	AttractionMapper m_attMapper;
	SegmentMapper m_segMapper;
	string lowerString(string str) const;
	NavResult dijkstra(GeoCoord start, GeoCoord end, string startName, string endName, vector<NavSegment>& directions) const;
	GeoCoord stringToGeoCoord(string s) const;
	bool checkForDestination(string des, StreetSegment street) const;
	void determineDirection(GeoSegment geoSeg, string& direction) const;
	void updateShortestDist(string v, MyMap<string, int>& vstdNds, MyMap<string, double>& shrtdst, priority_queue<Node1, vector<Node1>, 
		NodeComp>& distance, string current, GeoCoord currentGeo, double currentShortestPath, MyMap<string, string>& myPath, GeoCoord des) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

string NavigatorImpl::lowerString(string str) const
{
	std::string ss = str;
	for (int i = 0; i < str.length(); i++)
		ss[i] = tolower(ss[i]);
	return ss;
}
bool NavigatorImpl::loadMapData(string mapFile)
{
	bool bj = m_mapLoader.load(mapFile);
	if (!bj) return false;
	m_attMapper.init(m_mapLoader);
	m_segMapper.init(m_mapLoader);
	//cerr << "finish loading data" << endl; //testing 
	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	//check the validity of starting and ending locations
	GeoCoord startPoint, endPoint;
	bool bj1, bj2;
	bj1 = m_attMapper.getGeoCoord(start, startPoint);
	if (!bj1) return NAV_BAD_SOURCE;
	bj2 = m_attMapper.getGeoCoord(end, endPoint);
	if (!bj2) return NAV_BAD_DESTINATION;
	//Algorithm to find the shortest route
	return dijkstra(startPoint, endPoint, start, end, directions);
}

void NavigatorImpl::updateShortestDist(string v, MyMap<string, int>& vstdNds, MyMap<string, double>& shrtdst, priority_queue<Node1, 
	vector<Node1>, NodeComp>& distance, string current, GeoCoord currentGeo, double currentShortestPath, MyMap<string, string>& myPath, GeoCoord des) const
{
	int* visited = vstdNds.find(v); 
	if (visited == nullptr) //not in the closed list
	{
		double distanceBtwCoord = distanceEarthMiles(currentGeo, stringToGeoCoord(v));
		double* dist = shrtdst.find(v); //using g-score as a measure
		if (dist == nullptr) //not in the open list
		{
			shrtdst.associate(v, distanceBtwCoord + currentShortestPath);
			distance.push(Node1(v, distanceBtwCoord + currentShortestPath + distanceEarthMiles(stringToGeoCoord(v), des)));
			myPath.associate(v, current);
		}
		else if (*dist > distanceBtwCoord + currentShortestPath) //in the open list, comparing g-score
		{
			shrtdst.associate(v, distanceBtwCoord + currentShortestPath);
			distance.push(Node1(v, distanceBtwCoord + currentShortestPath + distanceEarthMiles(stringToGeoCoord(v), des)));
			myPath.associate(v, current);
		}
	}
}

NavResult NavigatorImpl::dijkstra(GeoCoord start, GeoCoord end, string startName, string endName, vector<NavSegment> &directions) const
{
	MyMap<string, int> visitedNodes;//closed list, data structure to keep visited nodes
	priority_queue<Node1, vector<Node1>, NodeComp> distance;//open list(f), data structure combined is a priority queue TODO:Check for NodeComp
	MyMap<string, double> shortestDist; //g-score, data structure to keep the shortest distance
	MyMap<string, string> myPath;	//parent, data structure to keep track of the path
	shortestDist.associate(start.latitudeText + ',' + start.longitudeText, 0);
	distance.push(Node1(start.latitudeText + ',' + start.longitudeText, 0)); //initialize the starting point
	while (visitedNodes.find(end.latitudeText + ',' + end.longitudeText) == nullptr)
	{
		//check if all nodes that have shortest distance has been added to visited nodes. aka whether there is no route
		if (distance.empty()) return NAV_NO_ROUTE;
		string current = distance.top().coord; //lowest f-score
		distance.pop();
		while (visitedNodes.find(current) != nullptr) //possible replication, ensure unprocessed
		{
			current = distance.top().coord;
			distance.pop();
		}
		//mark current as processed
		visitedNodes.associate(current, 0);
		//loop through all connected & unprocessed nodes from current
		GeoCoord currentGeo = stringToGeoCoord(current);
		double currentShortestPath = *shortestDist.find(current); //g-score
		vector<StreetSegment> connectedSeg = m_segMapper.getSegments(currentGeo);
		vector<StreetSegment>::iterator it2;
		it2 = connectedSeg.begin();
		while (it2 != connectedSeg.end()) 
		{
			string vMid = "";
			bool bj = checkForDestination(endName, (*it2)); //first check whether the current segment has our destination 
			if (bj) vMid = end.latitudeText + ',' + end.longitudeText;
			//compare two different path, and update the shortestDistance
			string vStart = (*it2).segment.start.latitudeText + ',' + (*it2).segment.start.longitudeText;
			string vEnd = (*it2).segment.end.latitudeText + ',' + (*it2).segment.end.longitudeText;
			updateShortestDist(vStart, visitedNodes, shortestDist, distance, current, currentGeo, currentShortestPath,  myPath, end);
			updateShortestDist(vEnd, visitedNodes, shortestDist, distance, current, currentGeo, currentShortestPath,  myPath, end);
			if (vMid != "") updateShortestDist(vMid, visitedNodes, shortestDist, distance, current, currentGeo, currentShortestPath, myPath, end);
			it2++;
		}
	}
	//find the path
	string endPoint = end.latitudeText + ',' + end.longitudeText;
	string startPoint = *myPath.find(endPoint);
	vector<NavSegment> subDir;
	while (endPoint != start.latitudeText + ',' + start.longitudeText)
	{
		//determine the NavSegment properties
		NavSegment dir;
		GeoCoord strtGeo = stringToGeoCoord(startPoint), endGeo = stringToGeoCoord(endPoint);
		string strtName;
		if (subDir.size() == 0) strtName = "";
		else strtName = subDir[subDir.size() - 1].m_streetName;
		// get the street connected with the endPoint
		vector<StreetSegment> curStrt = m_segMapper.getSegments(endGeo);
		//for each streetSegment, check whether it is the path (whether it have same coord)
		vector<StreetSegment>::iterator it = curStrt.begin();
		while (it != curStrt.end())
		{
			if ((*it).segment.end.latitudeText + ',' + (*it).segment.end.longitudeText == startPoint
				|| (*it).segment.start.latitudeText + ',' + (*it).segment.start.longitudeText == startPoint)
				break;
			else if (startPoint == start.latitudeText + ',' + start.longitudeText)
			{
				bool bj = checkForDestination(startName, (*it));
				if (bj) break; //if we reach the starting point
			}
			it++;
		}
		if (strtName != "" && strtName != (*it).streetName) // need to make a turn
		{
			NavSegment dirTurn;
			dirTurn.m_command = NavSegment::NavCommand::TURN;
			double angle = angleBetween2Lines(GeoSegment(strtGeo, endGeo), subDir[subDir.size() - 1].m_geoSegment);
			if (angle >= 180) dirTurn.m_direction = "right"; 
			else dirTurn.m_direction = "left";
			dirTurn.m_streetName = strtName;
			subDir.push_back(dirTurn);
		}
		dir.m_command = NavSegment::NavCommand::PROCEED;
		dir.m_streetName = (*it).streetName;
		dir.m_distance = distanceEarthMiles(strtGeo, endGeo);
		dir.m_geoSegment = GeoSegment(strtGeo, endGeo);
		determineDirection(dir.m_geoSegment, dir.m_direction);
		subDir.push_back(dir);
		//update, and proceed to the next move 
		endPoint = startPoint;
		if (endPoint != start.latitudeText + ',' + start.longitudeText) 
		startPoint = *myPath.find(endPoint);
	}
	//now reverse subDir
	vector<NavSegment>::iterator it;
	it = subDir.end() - 1;
	while (it != subDir.begin())
	{
		directions.push_back(*it);
		it--;
	}
	directions.push_back(*it);
	return NAV_SUCCESS;
}

void NavigatorImpl::determineDirection(GeoSegment geoSeg, string& direction) const
{
	double angle = angleOfLine(geoSeg);
	if (angle <= 22.5) direction = "east";
	if (angle > 22.5 && angle <= 67.5) direction = "northeast";
	if (angle > 67.5 && angle <= 112.5) direction = "north";
	if (angle > 112.5 && angle <= 157.5) direction = "northwest";
	if (angle > 157.5 && angle <= 202.5) direction = "west";
	if (angle > 202.5 && angle <= 247.5) direction = "southwest";
	if (angle > 247.5 && angle <= 292.5) direction = "south";
	if (angle > 292.5 && angle <= 337.5) direction = "southeast";
	if (angle > 337.5 && angle <= 360) direction = "east";
}
bool NavigatorImpl::checkForDestination(string des, StreetSegment street) const
{
	string s2 = lowerString(des);
	vector<Attraction>::iterator it = street.attractions.begin();
	while (it != street.attractions.end())
	{
		string s1 = lowerString((*it).name);
		if (s1 == s2) return true;
		it++;
	}
	return false;
}
GeoCoord NavigatorImpl::stringToGeoCoord(string s) const 
{
	int j = 0;
	for (j = 0; j < s.length(); j++)
		if (s[j] == ',') break;
	GeoCoord currentGeo(s.substr(0, j), s.substr(j + 1, s.length() - j - 1));
	return currentGeo;
}
//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
