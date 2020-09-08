#include "ObjReader.h"


ObjectData ObjReader::readObjFile(std::string objFileName)
{
	std::list<vector3f> vList;  // 3 vertex coordinates, does NOT need to be rearranges
	std::list<vector2f> vtList;  // 2 texture coordinates, needs to be rearranged
	std::list<vector3f> vnList;  // 3 normal coordinates, needs to be rearranged

	std::list<vector3i> fList;

	std::ifstream ifs(objFileName, std::ifstream::in);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::istringstream in(line);
		std::string type;
		in >> type;

		if (type == "v")  // put vertex coordinates into list
		{
			vector3f v;
			in >> v.x >> v.y >> v.z;
			vList.push_back(v);
		}
		else if (type == "vt")  // put texture coordinates into list
		{
			vector2f vt;
			in >> vt.x >> vt.y;
			vtList.push_back(vt);
		}
		else if (type == "vn")  // put normal coordinates into list
		{
			vector3f vn;
			in >> vn.x >> vn.y >> vn.z;
			vnList.push_back(vn);
		}
		else if (type == "f")  // process face
		{
			// new - this code assumes that the face have all the 3 components (v,vt,vn)
			std::string f;
			vector3i currentVTNindexes;
			while (in >> f)  // f looks like this: 2/1/1
			{
				std::stringstream ssf(f);
				std::string tempf;
				if (std::getline(ssf, tempf, '/'))  // index of vertex coorinates
				{
					currentVTNindexes.x = std::stoi(tempf);
				}

				tempf.clear();
				if (std::getline(ssf, tempf, '/'))  // index of texture coordinates
				{
					currentVTNindexes.y = std::stoi(tempf);
				}

				tempf.clear();
				if (std::getline(ssf, tempf, '/'))  // index of normal coordinates
				{
					currentVTNindexes.z = std::stoi(tempf);
				}
				fList.push_back(currentVTNindexes);
			}
		}

	} // end of while, lists are filled

	ifs.close();

	// new approach: (VVVVTTTTNNNN) batch format

	int numberOfFaces = fList.size() / 3;
	//std::cout << "THIS numberOfFaces: " << numberOfFaces << std::endl;
	int numberOfElements = fList.size();
	//std::cout << "THIS numberOfElements: " << numberOfElements << std::endl;

	std::vector<float> vVector(numberOfElements * 3);
	std::vector<float> vtVector(numberOfElements * 2);
	std::vector<float> vnVector(numberOfElements * 3);
	std::vector<int> iVector(numberOfElements);

	int loopCounter = 0;
	for (vector3i const& i : fList)  // iterate through faces
	{
		vector3i currnetVTNindexes = i;
		// indexes from 1 !!!
		int indexOfV = currnetVTNindexes.x;  // v
		int indexOfT = currnetVTNindexes.y;  // vt
		int indexOfN = currnetVTNindexes.z;  // vn

		// get elements at the indexes
		std::list<vector3f>::iterator itToV = vList.begin();
		std::advance(itToV, indexOfV - 1);
		vector3f v = *itToV;
		//std::cout << v.x << " " << v.y << " " << v.z << std::endl;

		std::list<vector2f>::iterator itToT = vtList.begin();
		std::advance(itToT, indexOfT - 1);
		vector2f vt = *itToT;
		//std::cout << vt.x << " " << vt.y << std::endl;

		std::list<vector3f>::iterator itToN = vnList.begin();
		std::advance(itToN, indexOfN - 1);
		vector3f vn = *itToN;
		//std::cout << vn.x << " " << vn.y << " " << vn.z << std::endl;

		// fill up vectors
		vVector[loopCounter * 3] = v.x;
		vVector[(loopCounter * 3) + 1] = v.y;
		vVector[(loopCounter * 3) + 2] = v.z;

		vtVector[loopCounter * 2] = vt.x;
		vtVector[(loopCounter * 2) + 1] = vt.y;

		vnVector[loopCounter * 3] = vn.x;
		vnVector[(loopCounter * 3) + 1] = vn.y;
		vnVector[(loopCounter * 3) + 2] = vn.z;

		iVector[loopCounter] = loopCounter;
		loopCounter += 1;
	}

	ObjectData allData;
	allData.vertices = vVector;
	allData.uvs = vtVector;
	allData.normals = vnVector;
	allData.indices = iVector;

	std::cout << "Obj file has been loaded succesfully: " << objFileName << std::endl;

	return allData;
}
