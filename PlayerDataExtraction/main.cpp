#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct PlayerData
{
    vec3 position;
    
    float speed = 0.0f;

    int team = -1;

    float shirtNumber = -1;
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
    input += ","; //adding a coma at the end for the converter fn
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
                output.position.x = std::stof(temp);
                break;
            }
            case 1: //y
            {
                output.position.y = std::stof(temp);
                break;
            }
            case 2: //z
            {
                output.position.z = std::stof(temp);
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
                output.shirtNumber = (std::stof(temp));
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

void formatAndSave(std::string fileName, std::map<int, std::vector<PlayerData>>& dataFrameMap)
{
    std::ofstream os(fileName, std::ofstream::binary);
    if (os) 
    {
        for (auto& n : dataFrameMap)
        {
            os << "FRAME " << n.first << ": " << std::endl;

            for (auto& p : dataFrameMap[n.first])
            {
                os << "    {" << std::endl
                    << "        PLAYER: " << p.shirtNumber << std::endl
                    << "        TEAM: " << p.team << std::endl
                    << "        POSITION: " << p.position.x << "," << p.position.y << "," << p.position.z << std::endl
                    << "        SPEED: " << p.speed << std::endl
                    << "    }" << std::endl;
            }

            os << "------------------------------------" << std::endl << std::endl;

        }
        
        os.close();
    }


}

int main()
{
    std::ifstream is("tracking.txt", std::ifstream::binary);
    if (is) 
    {
        // get length of file:
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char* buffer = new char[length];

        std::map<int, std::vector<PlayerData>> dataFrameMap;


        while (!is.eof())
        {

            //player data in each frame
            std::vector<PlayerData> playerDataVector;

            is.getline(buffer, length);
            //std::cout << buffer << std::endl;
            //convert frame to string
            std::string frame(buffer);

            if (frame.size() > 0)
            {



                bool start = false;

                std::string playerData = "";
                std::string frameNumber = "";

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
                    else if (!start)
                    {
                        frameNumber += frame[i];
                    }

                }

                //std::cout << playerData << std::endl;
                std::cout << frameNumber << std::endl;


                std::vector<std::string> playerData2 = splitString(playerData, ';');

                for (int i = 0; i < playerData2.size(); i++)
                {
                    //std::cout << playerData2[i] << std::endl;

                    PlayerData temp;

                    temp = convert(playerData2[i]);

                    playerDataVector.push_back(temp);
                }

                //std::cout << "Number of players: " << playerDataVector.size() << std::endl;

                /*
                for (PlayerData p : playerDataVector)
                {
                    std::cout << "PLAYER: " << p.shirtNumber << " FROM TEAM: " << p.team <<
                        " is at " << p.x << "," << p.y << "," << p.z << " moving at the speed of: " <<
                        p.speed << std::endl;
                }

                */

                //save data onto the frame map
                dataFrameMap.emplace(int(std::stof(frameNumber)), playerDataVector);

            }

        }

        is.close();

        // ...buffer contains the entire file...
        delete[] buffer;


        formatAndSave("output.txt", dataFrameMap);

    }


	return 0;
}