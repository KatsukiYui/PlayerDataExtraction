#include <iostream>
#include <fstream>
#include <string>
#include <vector>


struct PlayerData
{
    float x = 0;
    float y = 0;
    float z = 0;
    
    float speed = 0;

    int team = -1;

    int ShirtNumber = -1;
};


int main()
{
    std::ifstream is("tracking.txt", std::ifstream::binary);
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char* buffer = new char[length];

        //std::cout << "Reading " << length << " characters... ";

        // read data as a block:
        //is.read(buffer, length);
        //std::cout << buffer << std::endl;
        const char* delim = ";";
        
        is.getline(buffer, length);
        //std::cout << buffer << std::endl;

        //convert frame to string
        std::string frame(buffer);

        bool start = false;

        std::string playerData = "";
        //iterating over the frame's characters
        for (std::string::size_type i = 0; i < frame.size(); i++) {
            
            
            /*
            if (frame[i] == ':')
            {

                std::cout << " COLON ";
            }
            else 
            { 
            std::cout << frame[i];
            }
            */

            if (frame[i] == ':')
            {
                
                if (!start)
                {
                    start = true;
                }
                else
                {
                    i = frame.size();
                }
            }
            else if (start)
            {
                playerData += frame[i];
            }

        }

        std::cout << playerData << std::endl;


        std::vector<PlayerData> pData; 




        /*
        if (is)
            std::cout << "all characters read successfully.";
        else
            std::cout << "error: only " << is.gcount() << " could be read";
            */
        is.close();

        // ...buffer contains the entire file...

        delete[] buffer;
    }


	return 0;
}