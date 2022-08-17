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

    int shirtNumber = -1;
};

std::vector<std::string> splitString(std::string& input, char separator)
{
    std::vector<std::string> output;
    std::string temp = "";

    for (std::string::size_type i = 0; i < input.size(); i++)
    {      
        if (input[i] == separator)
        {
            output.push_back(temp);
            temp = "";
        }
        else
        {
            temp += input[i];
        }
    }

    return output;
}

PlayerData convert(std::string& input)
{
    PlayerData output;
    std::string temp = "";
    int component = 0;

    for (std::string::size_type i = 0; i < input.size(); i++)
    {
        if (input[i] == ',')
        {
            switch (component)
            {
            case 0: //x
            {
                output.x = std::stof(temp);
                break;
            }
            case 1: //y
            {
                output.y = std::stof(temp);
                break;
            }
            case 2: //z
            {
                output.z = std::stof(temp);
                break;
            }
            case 3: //speed
            {
                output.speed = std::stof(temp);
                break;
            }
            case 4: //team int
            {
                output.team = (int)(std::stof(temp));
                break;
            }
            case 5: //ShirtNumber int
            {
                output.shirtNumber = (int)(std::stof(temp));
                break;
            }
            }

            component++;
            temp = "";
        }
        else
        {
            temp += input[i];
        }
    }

    return output;
}

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
        
        is.getline(buffer, length);
        //std::cout << buffer << std::endl;

        //convert frame to string
        std::string frame(buffer);

        bool start = false;

        std::string playerData = "";
        //iterating over the frame's characters
        for (std::string::size_type i = 0; i < frame.size(); i++) 
        {
            
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
        
        std::vector<std::string> playerData2 = splitString(playerData, ';');

        std::vector<PlayerData> playerDataVector;

        for (int i = 0; i < playerData2.size(); i++)
        {
            std::cout << playerData2[i] << std::endl;

            PlayerData temp;

            temp = convert(playerData2[i]);

            playerDataVector.push_back(temp);
        }

        std::cout << "Number of players: " << playerDataVector.size() << std::endl;

        for (PlayerData p : playerDataVector)
        {
            std::cout << "PLAYER: " << p.shirtNumber << " FROM TEAM: " << p.team <<
                " is at " << p.x << "," << p.y << "," << p.z << " moving at the speed of: " <<
                p.speed << std::endl;
        }

        is.close();

        // ...buffer contains the entire file...
        delete[] buffer;
    }


	return 0;
}