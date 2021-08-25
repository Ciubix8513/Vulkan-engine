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
    std::vector<Vector3int> Vertecies;
   // std::vector <unsigned int> Indecies;
    std::string word;
    Mesh mesh;
    mesh.atr = 0;

    bool firstF = true;
    //Read data
    while (f.good())
    {
        f >> word;
        //Get position data
        if (word == "v")
        {
            mesh.atr |= POSITION;
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
            mesh.atr |= UV;
            Vector2 tmp;
            f >> word;
            tmp.x = atof(word.c_str());
            f >> word;
            tmp.y = atof(word.c_str());
            UVs.push_back(tmp);
        }
        else if (word == "vn")
        {
            mesh.atr |= NORMAL;
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
            if(mesh.atr == ALL)
            {                               
                //Process vertecies will break if mesh is not triangulated
                for (size_t j = 0; j < 3; j++)  
                {
                    //Vertex format is Position/Texture/Normal
                    f >> word;
                    std::string index;
                    Vector3int v;
                    int atr = 0;
                    for (size_t i = 0; i < word.size(); i++)
                    {
                        if (word[i] != '/')
                            index += word[i];
                        else
                        {
                            if (atr == 0)
                               v.x  = atoi(index.c_str())-1;
                            if (atr == 1)
                                v.y = atoi(index.c_str())-1;
                            index = "";
                            atr++;
                        }
                    }
                    v.z = atoi(index.c_str())-1;
                    Vertecies.push_back(v);
                }

            }
            else if(mesh.atr ==( POSITION | NORMAL))
            {
                //Vertex format is Position//Normal
                f >> word;
            }
           


        }   
        

    }
    //Close file 
    f.close();
    
    int TableSize = std::max<size_t>({ Positions.size(),UVs.size(),Normals.size() });
    Vector3int* HashTable = new Vector3int[TableSize];
    std::vector<unsigned int> indecies;
    for (size_t i = 0; i < Vertecies.size(); i++)
    {
        int a = Hash(Vertecies[i], TableSize);
        HashTable[a] =Vertecies[i];
        indecies.push_back(a);
    }
    //Fill index array, I sure hope this works
    mesh.Indecies = new unsigned int[indecies.size()];
    memcpy(mesh.Indecies, indecies.data(), indecies.size());
    mesh.numIndecies = indecies.size();
    //Fill vertex array
    mesh.Vertices = new Vertex[TableSize];
    for (size_t i = 0; i < TableSize; i++)    
        if(mesh.atr == ALL)
        mesh.Vertices[i] = Vertex(Positions[HashTable[i].x], UVs[HashTable[i].y], Normals[HashTable[i].z]);
        else
            mesh.Vertices[i] = Vertex(Positions[HashTable[i].x],Vector2::Zero(), Normals[HashTable[i].z]);
    mesh.numVertecies = TableSize;    
    return mesh;
}

//Taken from "Optimized Spatial Hashing for Collision Detection of Deformable Objects" Computer Graphics Laboratory ETH Zurich 2003
int ModelLoader::Hash(Vector3int data, int tableSize)
{
    int p1, p2, p3;
    
    p1 = 73856093;
    p2 = 19349663;
    p3 = 83492791;
    
    return ((data.x * p1) ^(data.y * p2)^(data.z *p3))%tableSize;
}

ModelLoader::Vertex::Vertex(Vector3 pos, Vector2 uv, Vector3 normal)
{
    Position = pos;
    UV = uv;
    Normal = normal;
}

ModelLoader::Vertex::Vertex()
{
    Position = Vector3::Zero();
    UV = Vector2::Zero();
    Normal = Vector3::Zero();
}
