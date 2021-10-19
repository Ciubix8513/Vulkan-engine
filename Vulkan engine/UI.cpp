/*#include "UI.h"

void UI::Init(Vulkan* v)
{
	m_vulkanPtr = v;
    
}

void Text::Init(Vulkan* v, char* file)
{
	m_vulkanPtr = v;
    //Get font data
	LoadFontData(file);
    
}

void Text::LoadFontData(char* file)
{    
        std::ifstream fin;
        char tmp;
        int numChars = 95;

        //Get number of chars
        fin.open(file);
        if (fin.fail())
            throw std::runtime_error("Could not open file");
        fin.unsetf(std::ios_base::skipws);
        numChars = std::count(std::istream_iterator<char>(fin), std::istream_iterator<char>(), '\n');
        numChars++;
        fin.close();

        fin.open(file);
        if (fin.fail())
            throw std::runtime_error("Could not open file");


        m_Font = new Font[numChars];

        //Read file data
        for (int i = 0; i < numChars; i++)
        {
            fin.get(tmp);
            while (tmp != ' ')
            {
                fin.get(tmp);
            }
            fin.get(tmp);
            while (tmp != ' ')
            {
                fin.get(tmp);
            }

            fin >> m_Font[i].left;
            fin >> m_Font[i].right;
            fin >> m_Font[i].size;
        }


        fin.close();
}
*/