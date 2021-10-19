#include "ModelLoader.h"

a::ModelLoader::Mesh* a::ModelLoader::LoadModel(std::string path)
{
    std::FILE* file;
    //TODO FIX THIS!!!
   
    if (fopen_s(&file, path.c_str(), "r") !=0)
        throw std::runtime_error("Could not open file");
    
    MDLheader header;
    fread((char*)&header, 1,sizeof(header),file);
    if ((std::string)header.signature != "_LUNAR_MODEL_FORMAT_")
        throw std::runtime_error("Wrong file signature");
    Mesh *mesh = new Mesh;
    mesh->atr = header.Attributes;
    mesh->numIndecies = header.NumIndecies;
    mesh->numVerticies = header.NumVertices;
    mesh->Vertices = new Vertex[mesh->numVerticies];
    mesh->Indecies = new unsigned int[mesh->numIndecies];   
    
    fread(mesh->Vertices, sizeof(Vertex), mesh->numVerticies,file);   
   unsigned char* a = new unsigned char[8];
   
    
    fread(a, 8, 1, file);
   
    fread(mesh->Indecies, 4U,  mesh->numIndecies,file);
   
    fclose(file);
    return mesh;
}

void a::ModelLoader::SaveModel(Mesh* mesh, std::string path)
{
    //File structure:
    //Signature 21 bytes
    //Mesh properties 9 bytes
    //Vertecies data
    //  Padding PADDING 8 bytes
    //Indecies data

    std::ofstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Could not create file");
    //Create header
    MDLheader header{};
    memcpy(header.signature,"_LUNAR_MODEL_FORMAT_",21 );//It's my file type and I get to choose the file signature
    header.Attributes = mesh->atr;
    header.NumVertices = mesh->numVerticies;
    header.NumIndecies = mesh->numIndecies;
    
    file.write((char*)&header, sizeof(header));
    file.write((char*)mesh->Vertices, sizeof(Vertex) * mesh->numVerticies);
    file.write((char*)"PADDING", 8);
    file.write((char*)mesh->Indecies, 4U * mesh->numIndecies);
    file.close();
    return;

    
}

a::ModelLoader::Mesh* a::ModelLoader::ConvertOBJ(std::string path)
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
    Mesh* mesh = new Mesh;
    mesh->atr = 0;

    //Read data
    while (f.good())
    {
        f >> word;
        //Get position data
        if (word == "v")
        {
            mesh->atr |= POSITION;
            Vector3 tmp;
            f >> word;
            tmp.x = (float)atof(word.c_str());
            f >> word;
            tmp.y = (float)atof(word.c_str());
            f >> word;
            tmp.z = (float)atof(word.c_str());
            Positions.push_back(tmp);
        }
        else if (word == "vt")
        {
            mesh->atr |= UV;
            Vector2 tmp;
            f >> word;
            tmp.x = (float)atof(word.c_str());
            f >> word;
            tmp.y = (float)atof(word.c_str());
            UVs.push_back(tmp);
        }
        else if (word == "vn")
        {
            mesh->atr |= NORMAL;
            Vector3 tmp;
            f >> word;
            tmp.x = (float)atof(word.c_str());
            f >> word;
            tmp.y = (float)atof(word.c_str());
            f >> word;
            tmp.z = (float)atof(word.c_str());
            Normals.push_back(tmp);
        }
        else if (word == "f")
        {
            //Upon reaching the first f line enter a cycle 
            while (f.good())
            {              
                if (mesh->atr == ALL)
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
                else if (mesh->atr == (POSITION | NORMAL))
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
    //Fast? solution (I sure hope it works)
    std::unordered_map<Vector3int, size_t> uniqueVertices;
    std::vector<Vector3int > Vert;
    std::vector<uint32_t> Indecies;
    for (unsigned int i = 0; i < Vertecies.size(); i++)
    {
        if (uniqueVertices.count(Vertecies[i]) == 0)
        {
            uniqueVertices[Vertecies[i]] = Vert.size();
            Vert.push_back(Vertecies[i]);
        }
        Indecies.push_back(uniqueVertices[Vertecies[i]]);
    }

    mesh->Indecies = new uint32_t[Indecies.size()];
    mesh->numIndecies = Indecies.size();
    memcpy(mesh->Indecies, Indecies.data(), Indecies.size() * 4);


   
    mesh->Vertices = new Vertex[Vert.size()];
    mesh->numVerticies = Vert.size();
    for (size_t i = 0; i < Vert.size(); i++)
        if (mesh->atr == ALL)
            mesh->Vertices[i] = Vertex(Positions[Vert[i].x], UVs[Vert[i].y], Normals[Vert[i].z]);
        else
            mesh->Vertices[i] = Vertex(Positions[Vert[i].x], Vector2::Zero(), Normals[Vert[i].z]);

   
    return mesh;
}


//Should work
Vector3int* a::ModelLoader::Triangulate(Vector3int* vertices, uint32_t size)
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

a::ModelLoader::Vertex::Vertex(Vector3 pos, Vector2 uv, Vector3 normal)
{
    Position = pos;
    UV = uv;
    Normal = normal;
}

a::ModelLoader::Vertex::Vertex()
{
    Position = Vector3::Zero();
    UV = Vector2::Zero();
    Normal = Vector3::Zero();
}

a::ModelLoader::Mesh::Mesh()
{
    Vertices = 0;
    Indecies = 0;
    numVerticies = 0;
    numIndecies = 0;
    atr = 0;
}

//ModelLoader::Mesh::~Mesh()
//{
//    delete Vertices;
//    delete Indecies;
//}
