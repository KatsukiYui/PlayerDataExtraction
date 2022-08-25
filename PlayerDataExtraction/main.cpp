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
    ivec2 position;
    
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
                output.position.x = std::stof(temp);
                break;
            }
            case 4: //team int
            {
                output.position.y = (int)(std::stof(temp));
                break;
            }
            case 5: //ShirtNumber int
            {
                output.speed = (std::stof(temp));
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

vec2 calculateVelocity(ivec2 _pos, ivec2 _prevPos, float _speed) 
{
    //Normalized displacement vector = Vector2.Normal(framePos - prevFramePos)
    // velocity vector = above * speed
    vec2 position{ _pos.x / 100.0f, _pos.y / 100.0f };
    vec2 previousPosition{ _prevPos.x / 100.0f, _prevPos.y / 100.0f };
    float speed = _speed / 100.0f;
    vec2 displacement =  vec2::subtract(position, previousPosition);

    float magnitude = sqrt(displacement.x * displacement.x + displacement.y * displacement.y);


    // normalize vector
    vec2 normalizedDisplacement = vec2::divide(displacement, magnitude);

    //float test = sqrt(normalizedDisplacement.x * normalizedDisplacement.x + normalizedDisplacement.y * normalizedDisplacement.y);
    vec2 velocity = vec2::multiply(normalizedDisplacement, speed);

    return velocity;
}

void formatAndSaveVelocity(std::string fileName, std::map<int, std::vector<PlayerData>>& dataFrameMap, int firstFrame)
{
    std::ofstream os(fileName, std::ofstream::binary);
    bool first = true;

    if (os)
    {
        ivec2 previousPos{ 0,0 };

        for (auto it = dataFrameMap.begin(); it != dataFrameMap.end(); ++it)
        {
            os << "FRAME " << it->first << ": " << std::endl;

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
                    velocity = calculateVelocity(p.position, previousPos, p.speed);

                }

                os << "    {" << std::endl
                    << "        ID: " << p.Id << std::endl
                    << "        SHIRT NUMBER: " << p.shirtNumber << std::endl
                    << "        TEAM: " << p.teamId << std::endl
                    << "        POSITION: " << p.position.x << ", " << p.position.y << std::endl
                    << "        SPEED: " << (p.speed / 100.0f) << " m/s" << std::endl
                    << "        VELOCITY: " << velocity.x << ", " << velocity.y << " m/s" << std::endl
                    << "    }" << std::endl;

                previousPos = p.position;
            }

            os << "------------------------------------" << std::endl << std::endl;

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