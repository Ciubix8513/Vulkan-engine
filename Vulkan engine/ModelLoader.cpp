#include "ModelLoader.h"

ModelLoader::Mesh ModelLoader::LoadOBJ(std::string path)
{
    //TODO Mesure how long my code takes to run
   // auto BeginTime = std::chrono::system_clock::now();

    //Open the file
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Could not open the file");
    std::vector<Vector3> Positions;
    std::vector<Vector2> UVs;
    std::vector<Vector3> Normals;
    std::vector<Vector3int> Vertecies;
    std::string word;
    Mesh mesh;
    mesh.atr = 0;

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
            //Upon reaching the first f line enter a cycle 
            while (f.good())
            {              
                if (mesh.atr == ALL)
                {
                    std::vector<Vector3int> tmpV;                    
                    while (f.good())
                    {
                        //Vertex format is Position/Texture/Normal
                        f >> word;
                        if (word.find('/') == std::string::npos)
                            break;
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
                                    v.x = atoi(index.c_str()) - 1;
                                if (atr == 1)
                                    v.y = atoi(index.c_str()) - 1;
                                index = "";
                                atr++;
                            }
                        }
                        v.z = atoi(index.c_str()) - 1;
                        tmpV.push_back(v);
                    }
                    if (tmpV.size() > 3) 
                    {
                        Vector3int* v  = Triangulate(tmpV.data(),tmpV.size());
                        for (size_t i = 0; i < (tmpV.size() -2) *3; i++)
                            Vertecies.push_back(v[i]);                        
                    }
                    else                    
                        for (size_t i = 0; i < tmpV.size(); i++) 
                            Vertecies.push_back(tmpV[i]); 
                }
                else if (mesh.atr == (POSITION | NORMAL))
                {
                    //TODO implement POSITION NORMAL Vertex format 
                    //Vertex format is Position//Normal
                    f >> word;
                }
            }
        }
    }
    //Close file 
    f.close(); 
    /*
    int TableSize =  std::max<size_t>({ Positions.size(),UVs.size(),Normals.size() });
    auto HashTable = new  std::optional<Vector3int>[TableSize];
    std::vector<unsigned int> indecies;
    int a123 = 0;
    int a1234 = 0;
    for (size_t i = 0; i < Vertecies.size(); i++)
    {
        int a = Hash(Vertecies[i], TableSize);
        if (!HashTable[a].has_value())
        {
            HashTable[a] = Vertecies[i];
            a123++;
        }else if (!(HashTable[a].value() == Vertecies[i]))
        {
            a1234++;
            std::cout << ' ';
        }
        indecies.push_back(a);
    }
    //Fill index array, I sure hope this works
    mesh.Indecies = new unsigned int[indecies.size()];
    memcpy(mesh.Indecies, indecies.data(), indecies.size()*sizeof(unsigned int));
    mesh.numIndecies = indecies.size();
    //Fill vertex array
    mesh.Vertices = new Vertex[TableSize];
    for (size_t i = 0; i < TableSize; i++)
        if(mesh.atr == ALL)
        mesh.Vertices[i] = Vertex(Positions[HashTable[i].value().x], UVs[HashTable[i].value().y], Normals[HashTable[i].value().z]);
        else
            mesh.Vertices[i] = Vertex(Positions[HashTable[i].value().x],Vector2::Zero(), Normals[HashTable[i].value().z]);
    mesh.numVertecies = TableSize;
    delete[] HashTable;
    */
    /*
    //INCREDIBLY slow solution
    std::vector<Vector3int> Vectors;
    std::vector<uint32_t> Indecies;
    //TODO find other way to do this
    //Make an array of unique values. Probably very inefficient, but I see no other way
    for (size_t i = 0; i < Vertecies.size(); i++)
    {
        uint32_t index = 0;
        if (FindValue<Vector3int>(Vectors.data(), Vectors.size(), Vertecies[i], index))
            Indecies.push_back(index);
        else
        {
            Vectors.push_back(Vertecies[i]);
            Indecies.push_back(Vectors.size()  - 1 );
        }
    }
    //By this point we have only unique vertices, creating mesh
    //Fill indecies
    //TODO put indecex array creation into a separate thread

    mesh.Indecies = new uint32_t[Indecies.size()];
    mesh.numIndecies = Indecies.size();
    memcpy(mesh.Indecies, Indecies.data(), Indecies.size() * 4);
    

    //Create vertices
    mesh.Vertices = new Vertex[Vectors.size()];
    mesh.numVertecies = Vectors.size();
    for (size_t i = 0; i < Vectors.size(); i++)
        if (mesh.atr == ALL)
            mesh.Vertices[i] = Vertex(Positions[Vectors[i].x], UVs[Vectors[i].y], Normals[Vectors[i].z]);
        else
            mesh.Vertices[i] = Vertex(Positions[Vectors[i].x], Vector2::Zero(), Normals[Vectors[i].z]);
            
    */
    //Fast? solution (I sure hope it works)
    std::unordered_map<Vector3int, uint32_t> uniqueVertices;
    std::vector<Vector3int > Vert;
    std::vector<uint32_t> Indecies;
    for (size_t i = 0; i < Vertecies.size(); i++)
    {
        if (uniqueVertices.count(Vertecies[i]) == 0)
        {
            uniqueVertices[Vertecies[i]] = Vert.size();
            Vert.push_back(Vertecies[i]);
        }
        Indecies.push_back(uniqueVertices[Vertecies[i]]);
    }

    mesh.Indecies = new uint32_t[Indecies.size()];
    mesh.numIndecies = Indecies.size();
    memcpy(mesh.Indecies, Indecies.data(), Indecies.size() * 4);


   
    mesh.Vertices = new Vertex[Vert.size()];
    mesh.numVertecies = Vert.size();
    for (size_t i = 0; i < Vert.size(); i++)
        if (mesh.atr == ALL)
            mesh.Vertices[i] = Vertex(Positions[Vert[i].x], UVs[Vert[i].y], Normals[Vert[i].z]);
        else
            mesh.Vertices[i] = Vertex(Positions[Vert[i].x], Vector2::Zero(), Normals[Vert[i].z]);

   
    return mesh;
}

//Should work
Vector3int* ModelLoader::Triangulate(Vector3int* vertices, uint32_t size)
{
    Vector3int* output = new Vector3int[(size - 2) * 3];
    size_t iter = 0;
    for (size_t i = 2; i < size; i++)
    {
        output[iter] = vertices[0];
        iter++;
        output[iter] = vertices[i-1];
        iter++;
        output[iter] = vertices[i];
        iter++;
    }

    return output;
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
