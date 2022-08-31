#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

/*
struct vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};
*/

enum teamId
{
    Unassigned = -1,
    Away = 0,
    Home = 1,
    Officials
};

struct ivec2
{
    int x = 0;
    int y = 0;
};

struct vec2
{
    float x = 0.0f;
    float y = 0.0f;

    static vec2 subtract(vec2 a, vec2 b)
    {
        vec2 rtn{ a.x - b.x, a.y - b.y };
        return rtn;
    };

    static vec2 multiply(vec2 a, float b)
    {
        vec2 rtn{ a.x * b, a.y * b };
        return rtn;
    };

    static vec2 divide(vec2 a, float b)
    {
        vec2 rtn{ a.x / b, a.y / b };
        return rtn;
    };
};

struct PlayerData
{
    vec2 position;
    
    float speed = 0.0f; //cm/sec atm

    int teamId = -1;

    int Id = -1;

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
                output.teamId = std::stof(temp);
                break;
            }
            case 1: //y
            {
                output.Id = std::stof(temp);
                break;
            }
            case 2: //z
            {
                output.shirtNumber = std::stof(temp);
                break;
            }
            case 3: //speed
            {
                output.position.x = (float)((std::stof(temp)) / 100.0f);
                break;
            }
            case 4: //team int
            {
                output.position.y = (float)((std::stof(temp)) / 100.0f);
                break;
            }
            case 5: //ShirtNumber int
            {
                //output.speed = (float)((std::stof(temp)) / 100.0f);
                output.speed = (float)(std::stof(temp));
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
                    << "        ID: " << p.Id << std::endl
                    << "        SHIRT NUMBER: " << p.shirtNumber << std::endl
                    << "        TEAM: " << p.teamId << std::endl
                    << "        POSITION: " << p.position.x << ", " << p.position.y << std::endl
                    << "        SPEED: " << p.speed << std::endl
                    << "    }" << std::endl;
            }

            os << "------------------------------------" << std::endl << std::endl;

        }
        
        os.close();
    }
}

vec2 calculateVelocity(vec2 _pos, vec2 _prevPos, float _speed) 
{
    //Normalized displacement vector = Vector2.Normal(framePos - prevFramePos)
    // velocity vector = above * speed
    vec2 position = _pos;
    vec2 previousPosition = _prevPos;
    float speed = _speed;
    vec2 displacement =  vec2::subtract(position, previousPosition);

    float magnitude = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);

    vec2 velocity{ 0, 0 };

    if (magnitude > 0)
    {
        // normalize vector
        vec2 normalizedDisplacement = vec2::divide(displacement, magnitude);

        //float test = sqrt(normalizedDisplacement.x * normalizedDisplacement.x + normalizedDisplacement.y * normalizedDisplacement.y);
        velocity = vec2::multiply(normalizedDisplacement, speed);
    }

    return velocity;
}

void formatAndSaveVelocity(std::string fileName, std::map<int, std::vector<PlayerData>>& dataFrameMap, int firstFrame)
{
    std::ofstream os(fileName, std::ofstream::binary);
    bool first = true;

    if (os)
    {

        os << "{\"" << "frameData" << "\":[" << std::endl;

        for (auto it = dataFrameMap.begin(); it != dataFrameMap.end(); ++it)
        {
            //os << "{\"" << it->first << "\":[" << std::endl;

            os << "\t{\"" << "playerData" << "\":[" << std::endl;

            //for (auto& p : dataFrameMap[it->first])
            for (int x = 0; x <  dataFrameMap[it->first].size(); x++)
            {
                PlayerData p = dataFrameMap[it->first].at(x);
                vec2 velocity{ 0.0f, 0.0f };

                if (first)
                {
                    //first frame use current and next pos and first speed
                    //auto next = std::next(it); //get the next frame
                    //ivec2 nextPos = next->second.at(x).position;
                    //float nextSpeed = next->second.at(x).speed;

                    PlayerData nextP = dataFrameMap.find(firstFrame + 1)->second[x];

                    velocity = calculateVelocity(nextP.position, p.position, p.speed);
                }
                else
                {
                    PlayerData previousP = dataFrameMap.find((it->first) - 1)->second[x];

                    velocity = calculateVelocity(p.position, previousP.position, p.speed);
                }

                //vec2 nomralisedVector = vec2::divide(velocity, sqrt((velocity.x * velocity.x) + (velocity.y * velocity.y)));

                os << "\t\t{" << std::endl
                    << "\t\t\t\"id\": " << p.Id << "," << std::endl
                    << "\t\t\t\"shirtNumber\": " << p.shirtNumber << "," << std::endl
                    << "\t\t\t\"team\": " << p.teamId << "," << std::endl
                    << "\t\t\t\"position\": " << "[" << p.position.x << ", " << p.position.y << "]" << "," << std::endl
                    << "\t\t\t\"speed\": " << p.speed << "," << std::endl
                    << "\t\t\t\"velocity\": " << "[" << velocity.x << ", " << velocity.y << "]" << std::endl;
                    //<< "        NORM-VELOCITY: " << nomralisedVector.x << ", " << nomralisedVector.y << " m/s" << std::endl
                    //<< "        NORM-MAGNITUDE: " << sqrt((nomralisedVector.x * nomralisedVector.x) + (nomralisedVector.y * nomralisedVector.y)) << std::endl

                    if (x != (dataFrameMap[it->first].size() - 1))
                    {
                        os << "\t\t}," << std::endl;
                    }
                    else
                    {
                        os << "\t\t}" << std::endl;
                    }       

            }

            if (it != std::prev(dataFrameMap.end()))
            {
                os << "\t]}," << std::endl << std::endl;
            }
            else
            {
                os << "]}" << std::endl << "]}" << std::endl;
            }
            

            first = false;

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
        int firstFrameNumber = 0;
        bool firstFrame = true;

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
                //std::cout << frameNumber << std::endl;

                if (firstFrame)
                {
                    firstFrameNumber = std::stof(frameNumber);
                    firstFrame = false;
                    std::cout << firstFrameNumber << std::endl;
                }

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


        //formatAndSave("output.txt", dataFrameMap);
        formatAndSaveVelocity("output.txt", dataFrameMap, firstFrameNumber);

    }


	return 0;
}