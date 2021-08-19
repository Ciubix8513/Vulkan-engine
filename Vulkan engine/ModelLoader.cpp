#include "ModelLoader.h"

ModelLoader::Mesh ModelLoader::LoadOBJ(std::string path)
{
    //Open the file
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Could not open the file");
    std::vector<Vector3> Positions;
    std::vector<Vector2> UVs;
    std::vector<Vector3> Normals;
    std::vector <unsigned int> Indecies;
    std::string word;
    Mesh mesh;

    bool firstF = true;
    //Read data
    while (f.good())
    {
        f >> word;
        //Get position data
        if (word == "v")
        {
            Vector3 tmp;
            f >> word;
            tmp.x = atof(word.c_str());
            f >> word;
            tmp.y = atof(word.c_str());
            f >> word;
            tmp.z = atof(word.c_str());
            Positions.push_back(tmp);
        }
        else if (word == "vt")
        {
            Vector2 tmp;
            f >> word;
            tmp.x = atof(word.c_str());
            f >> word;
            tmp.y = atof(word.c_str());
            UVs.push_back(tmp);
        }
        else if (word == "vn")
        {
            Vector3 tmp;
            f >> word;
            tmp.x = atof(word.c_str());
            f >> word;
            tmp.y = atof(word.c_str());
            f >> word;
            tmp.z = atof(word.c_str());
            Normals.push_back(tmp);
        }
        else if (word == "f")
        {
            if (firstF)
            {
                //Run this once
                firstF = false;                
                //Create vertex array, asigm
                mesh.numVertecies = Positions.size();
                mesh.Vertices = new Vertex[Positions.size()];
            }
            std::cout << ' ';
        }
    }






    return Mesh();
}
