#include "ObjLoader.h"

// disclaimer: edges have to be splitted along the seams!
objectData ObjLoader::loadObjFileV2(std::string objFileName)
{
	// https://stackoverflow.com/questions/8421170/read-floats-from-a-txt-file
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

	// new approach: (VVVVTTTTNNNN) batch format

	std::vector<float> vVector(vList.size() * 3);
	std::vector<float> vtVector(vtList.size() * 2);
	std::vector<float> vnVector(vList.size() * 3);
	std::vector<int> iVector(fList.size());

	int loopCounter = 0;
	for (vector3i const& i : fList)
	{
		vector3i currnetVTNindexes = i;
		//std::cout << currnetVTNindexes.x << "/" << currnetVTNindexes.y << "/" << currnetVTNindexes.z << std::endl;
		int indexToPut = currnetVTNindexes.x - 1;  // this will index the vectors
		//std::cout << indexToPut << std::endl;
		int indexOfV = currnetVTNindexes.x;  // put everything here
		int indexOfT = currnetVTNindexes.y;  // N to put
		int indexOfN = currnetVTNindexes.z;  // T to put

		iVector[loopCounter] = indexToPut;
		loopCounter += 1;

		// get elements that will be put to correct place
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

		vVector[indexToPut * 3] = v.x;
		vVector[(indexToPut * 3) + 1] = v.y;
		vVector[(indexToPut * 3) + 2] = v.z;

		vtVector[indexToPut * 2] = vt.x;
		vtVector[(indexToPut * 2) + 1] = vt.y;

		vnVector[indexToPut * 3] = vn.x;
		vnVector[(indexToPut * 3) + 1] = vn.y;
		vnVector[(indexToPut * 3) + 2] = vn.z;

	}

	objectData allData;
	allData.vertices = vVector;
	allData.uvs = vtVector;
	allData.normals = vnVector;
	allData.indices = iVector;

	return allData;

	/*
	// concatenate vectors into one
	std::vector<float> objetDataVector(vVector.size() + vtVector.size() + vnVector.size());
	int globalCount = 0;

	for (int i = 0; i < vVector.size(); i++)
	{
		objetDataVector[globalCount] = vVector[i];
		globalCount += 1;
	}

	for (int i = 0; i < vtVector.size(); i++)
	{
		objetDataVector[globalCount] = vtVector[i];
		globalCount += 1;
	}

	for (int i = 0; i < vnVector.size(); i++)
	{
		objetDataVector[globalCount] = vnVector[i];
		globalCount += 1;
	}

	std::cout << "objetDataVector: " << std::endl;
	for (int i = 0; i < objetDataVector.size(); i++)
	{
		std::cout << objetDataVector[i] << std::endl;
	}
	std::cout << "end of objetDataVector" << std::endl;
	*/
}


objectData ObjLoader::advancedObjLoader(std::string objFileName)
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

	objectData allData;
	allData.vertices = vVector;
	allData.uvs = vtVector;
	allData.normals = vnVector;
	allData.indices = iVector;

	std::cout << "Obj file has been loaded succesfully: " << objFileName << std::endl;

	return allData;
}


/*
// just here for reference, do not use
objectData loadObjFile(std::string objFileName)
{
	// https://stackoverflow.com/questions/8421170/read-floats-from-a-txt-file
	std::list<float> vList;  // 3 vertex coordinates, does NOT need to be rearranges
	//std::list<float> vtList;  // 2 texture coordinates, needs to be rearranged
	std::list<vector2f> vtList;  // 2 texture coordinates, needs to be rearranged
	std::list<float> vnList;  // 3 normal coordinates, needs to be rearranged

	std::list<int> vIndList;
	std::list<int> vtIndList;  // rearrange texture coordinates using this list
	std::list<int> vnIndList;  // rearrange normal coordinates using this list

	std::ifstream ifs(objFileName, std::ifstream::in);
	std::string line;

	while (std::getline(ifs, line))
	{
		std::istringstream in(line);
		std::string type;
		in >> type;

		if (type == "v")  // put vertex coordinates into list
		{
			float x, y, z;
			in >> x >> y >> z;
			vList.push_back(x);
			vList.push_back(y);
			vList.push_back(z);
		}
		else if (type == "vt")  // put texture coordinates into list
		{
			//float x, y;
			//in >> x >> y;
			//vtList.push_back(x);
			//vtList.push_back(y);
			vector2f vt;
			in >> vt.x >> vt.y;
			vtList.push_back(vt);
		}
		else if (type == "vn")  // put normal coordinates into list
		{
			float x, y, z;
			in >> x >> y >> z;
			vnList.push_back(x);
			vnList.push_back(y);
			vnList.push_back(z);
		}
		else if (type == "f")  // process face
		{

			//std::string f1;
			//std::string f2;
			//std::string f3;
			//in >> f1 >> f2 >> f3;
			//std::stringstream ssf1(f1);
			//std::stringstream ssf2(f2);
			//std::stringstream ssf3(f3);

			//std::string f;
			//while (std::getline(ssf1, f, '/'))
			//{
				//std::cout << f << std::endl;
			//}
			//f.clear();
			//while (std::getline(ssf2, f, '/'))
			//{
				//std::cout << f << std::endl;
			//}
			//f.clear();
			//while (std::getline(ssf3, f, '/'))
			//{
				//std::cout << f << std::endl;
			//}


			// new - this code assumes that the face have all the 3 components (v,vt,vn)
			std::string f;
			while (in >> f)  // f looks like this: 2/1/1
			{
				std::stringstream ssf(f);
				std::string tempf;
				if (std::getline(ssf, tempf, '/'))  // index of vertex coorinates
				{
					int vIndex = std::stoi(tempf);
					vIndList.push_back(vIndex);
				}

				tempf.clear();
				if (std::getline(ssf, tempf, '/'))  // index of texture coordinates
				{
					int vtIndex = std::stoi(tempf);
					vtIndList.push_back(vtIndex);
				}

				tempf.clear();
				if (std::getline(ssf, tempf, '/'))  // index of normal coordinates
				{
					int vnIndex = std::stoi(tempf);
					vnIndList.push_back(vnIndex);
				}

			}
		}

	} // end of while, lists are filled

	int vNumber = vList.size();  // number of vertex coordinates = number of vertexes * 3
	int vtNumber = vtList.size();
	int vnNumber = vnList.size();
	int indNumber = vIndList.size();
	int numberOfVertexes = vList.size() / 3;

	std::cout << "vNumber: " << vNumber << std::endl << "vtNumber: " << vtNumber << std::endl << "vnNumber: " << vnNumber << std::endl;

	// fill up vVector
	// http://www.cplusplus.com/forum/general/22957/
	std::vector<float> vVector(vNumber);
	int vVectorCounter = 0;
	std::list<float>::const_iterator vListIt;
	for (vListIt = vList.begin(); vListIt != vList.end(); ++vListIt)
	{
		vVector[vVectorCounter] = *vListIt;
		vVectorCounter = vVectorCounter + 1;
	}

	std::vector<int> indVector(indNumber);
	int indVectorCounter = 0;
	std::list<int>::const_iterator vIndListIt;
	for (vIndListIt = vIndList.begin(); vIndListIt != vIndList.end(); ++vIndListIt)
	{
		indVector[indVectorCounter] = *vIndListIt - 1;
		indVectorCounter = indVectorCounter + 1;
	}

	// fill up vtVector_ (containes rearranged texture coordinates)
	int vtIndNumber = vtIndList.size();
	std::vector<float> vtVector_(numberOfVertexes * 2);  // rearranged vector

	std::list<int>::const_iterator vtIndListIt;  // iterate throught texture coordinate indexes vtIndList
	int counter1 = 0;
	for (vtIndListIt = vtIndList.begin(); vtIndListIt != vtIndList.end(); ++vtIndListIt)
	{
		// get index from vVector where to push the texture coordinates
		int& indexToPush = indVector[counter1];  // starts from 1
		int finalIndexToPush = indexToPush * 2;

		// get index of texture coordinates to put into that place
		int indexToElement = *vtIndListIt - 1;  // starts from 0
		auto itToElement = std::next(vtList.begin(), indexToElement);
		vector2f elementToPush = *itToElement;

		// put texture coordinates into the vector
		vtVector_[finalIndexToPush] = elementToPush.x;
		vtVector_[finalIndexToPush+1] = elementToPush.y;

		counter1 = counter1 + 1;
	}

	objectData loadedObject;
	loadedObject.vVector = vVector;
	loadedObject.vtVector = vtVector_;
	loadedObject.indVector = indVector;

	// new approach: store interleaved vertex attributes in one vector


	return loadedObject;
}
*/
