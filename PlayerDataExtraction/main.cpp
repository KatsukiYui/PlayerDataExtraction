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
    
    float speed = -1.0f;

    int teamId = -1;

    int Id = -1;

    int shirtNumber = -1;
};

struct BallData
{
    vec3 position; //change to meters

    float speed = -1.0f;

    std::string team = "";

    std::string state = "";

    std::string ballEvent = "";
};

struct EventData
{
    std::string team = "";

    std::string type = "";

    std::string subType = "";

    int period = -1;

    int startFrame = -1;

    float startTime = -1.0f;

    int endFrame = -1;

    int fromPlayer = -1;

    int toPlayer = -1;

    std::string bodyPart = "";

    std::string technique = "";

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
            case 0:
            {
                if (temp != "" && temp != " ")
                {
                    output.teamId = std::stof(temp);
                }
                break;
            }
            case 1:
            {
                if (temp != "" && temp != " ")
                {
                    output.Id = std::stof(temp);
                }
                break;
            }
            case 2:
            {
                if (temp != "" && temp != " ")
                {
                    output.shirtNumber = std::stof(temp);
                }
                break;
            }
            case 3:
            {
                if (temp != "" && temp != " ")
                {
                    output.position.x = (float)((std::stof(temp)) / 100.0f);
                }
                break;
            }
            case 4:
            {
                if (temp != "" && temp != " ")
                {
                    output.position.y = (float)((std::stof(temp)) / 100.0f);
                }
                break;
            }
            case 5:
            {
                if (temp != "" && temp != " ")
                {
                    //output.speed = (float)((std::stof(temp)) / 100.0f);
                    output.speed = (float)(std::stof(temp));
                }
                break;
            }
            }

            component++;
            temp = "";
        }
        else
        {
            if (input[i] != ',' && input[i] != '\r')
            {
                temp += input[i];
            }
        }

    }

    return output;
}

BallData convertBall(std::string& input)
{
    input += ","; //adding a coma at the end for the converter fn
    BallData output;
    std::string temp = "";
    int component = 0;

    for (std::string::size_type i = 0; i < input.size(); i++)
    {
        if (input[i] == ',')
        {
            switch (component)
            {
            case 0:
            {
                if (temp != "" && temp != " ")
                {
                    output.position.x = (float)((std::stof(temp)) / 100.0f);
                }
                break;
            }
            case 1:
            {    
                if (temp != "" && temp != " ")
                {
                    output.position.y = (float)((std::stof(temp)) / 100.0f);
                }
                break;
            }
            case 2:
            {
                if (temp != "" && temp != " ")
                {
                    output.position.z = (float)((std::stof(temp)) / 100.0f);
                }
                break;
            }
            case 3:
            {
                if (temp != "" && temp != " ")
                {
                    output.speed = (float)(std::stof(temp));
                }
                break;
            }
            case 4:
            {
                output.team = temp;
                break;
            }
            case 5:
            {
                output.state = temp;
                break;
            }
            case 6:
            {
                output.ballEvent = temp;
                break;
            }
            }


            component++;
            temp = "";

        }
        else
        {
            if (input[i] != ',' && input[i] != '\r')
            {
                temp += input[i];
            }
        }

    }

    if (output.ballEvent == "") { output.ballEvent = "None"; };

    return output;
}

EventData convertEvent(std::string& input)
{
    input += ","; //adding a coma at the end for the converter fn
    EventData output;
    std::string temp = "";
    int component = 0;

    for (std::string::size_type i = 0; i < input.size(); i++)
    {
        if (input[i] == ',')
        {

            switch (component)
            {
            case 0:
            {
                output.team = temp;
                break;
            }
            case 1:
            {    output.type = temp;
            break;
            }
            case 2:
            {
                output.subType = temp;
                break;
            }
            case 3:
            {
                if (temp != "" && temp != " ")
                {
                    output.period = std::stof(temp);
                }
                break;
            }
            case 4:
            {
                if (temp != "" && temp != " ")
                {
                    output.startFrame = std::stof(temp);
                }
                break;
            }
            case 5:
            {
                if (temp != "" && temp != " ")
                {
                    output.startTime = (float)(std::stof(temp));
                }
                break;
            }
            case 6:
            {
                if (temp != "" && temp != " ")
                {
                    output.endFrame = std::stof(temp);
                }
                break;
            }
            //7 end time not used
            case 8:
            {
                if (temp != "" && temp != " ")
                {
                    output.fromPlayer = std::stof(temp);
                }
                break;
            }
            case 9:
            {
                if (temp != "" && temp != " ")
                {
                    output.toPlayer = std::stof(temp);
                }
                break;
            }
            //10, 11, 12, 13 not used
            case 14:
            {
                output.bodyPart = temp;
                break;
            }
            case 15:
            {
                output.technique = temp;
                break;
            }
            }


            component++;
            temp = "";

        }
        else
        {
            if (input[i] != ',' && input[i] != '\r')
            {
                temp += input[i];
            }
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

void formatAndSaveVelocity(std::string fileName, std::map<int, std::vector<PlayerData>>& dataFrameMap, std::map<int, BallData>& ballFrameMap, int firstFrame)
{
    std::ofstream os(fileName, std::ofstream::binary);
    bool first = true;

    float progress = 0;
    float precentage = 0;

    if (os)
    {
        //output start frame number

        os << "{\"" << "startFrame" << "\":" << firstFrame << "," << std::endl
            << "\"" << "frameData" << "\":[" << std::endl;

        for (auto it = dataFrameMap.begin(); it != dataFrameMap.end(); ++it)
        {

            printf("\33[2K\r");

            progress += 1.0f;

            precentage = progress / (float)(dataFrameMap.size()) * 100;

            std::cout << "Writing File " << precentage << "  \\  " << 100;

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
                    << "\t\t\t\"position\": " << "{" << std::endl << "\t\t\t\t\"x\": " << p.position.x << ", " << std::endl << "\t\t\t\t\"y\": " << 0 << ", " << std::endl << "\t\t\t\t\"z\": " << p.position.y << std::endl << "\t\t\t}" << "," << std::endl
                    << "\t\t\t\"speed\": " << p.speed << "," << std::endl
                    << "\t\t\t\"velocity\": " << "{" << std::endl << "\t\t\t\t\"x\": " << velocity.x << ", " << std::endl << "\t\t\t\t\"y\": " << 0 << ", " << std::endl << "\t\t\t\t\"z\": " << velocity.y << std::endl << "\t\t\t}" << std::endl;
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

            os << "\t]," << std::endl;

            BallData b = ballFrameMap[it->first];
           
            os << std::endl
                << "\t\"" << "ballData" << "\":" << std::endl
                << "\t\t{" << std::endl
                << "\t\t\t\"position\": " << "{" << std::endl << "\t\t\t\t\"x\": " << b.position.x << ", " << std::endl << "\t\t\t\t\"y\": " << b.position.y << ", " << std::endl << "\t\t\t\t\"z\": " << b.position.z << std::endl <<"\t\t\t}" << "," << std::endl
                << "\t\t\t\"speed\": " << b.speed << "," << std::endl
                << "\t\t\t\"team\": " << "\"" << b.team << "\"" << "," << std::endl
                << "\t\t\t\"state\": " << "\"" << b.state << "\"" << "," << std::endl
                << "\t\t\t\"ballEvent\": " << "\"" << b.ballEvent << "\"" << std::endl
                << "\t\t}" << std::endl << std::endl;

            if (it != std::prev(dataFrameMap.end()))
            {
                os << "\t}," << std::endl << std::endl;
            }
            else
            {
                os << "\t}" << std::endl << "]}" << std::endl;
            }
            
            first = false;

        }

        os.close();
    }
}

void formatAndSaveEvent(std::string fileName, std::vector<EventData>& eventDataVector)
{

    std::ofstream os(fileName, std::ofstream::binary);

    float progress = 0;
    float precentage = 0;

    if (os)
    {

        os << "{\"" << "eventData" << "\":[" << std::endl;

        for (int x = 0; x < eventDataVector.size(); x++)
        {

            printf("\33[2K\r");

            progress += 1.0f;

            precentage = progress / (float)(eventDataVector.size()) * 100;

            std::cout << "Writing File " << precentage << "  \\  " << 100;

            EventData e = eventDataVector[x];

            os << "\t{" << std::endl
                << "\t\t\"team\": " << "\"" << e.team << "\"" << "," << std::endl
                << "\t\t\"type\": " << "\"" << e.type << "\"" << "," << std::endl
                << "\t\t\"subtype\": " << "\"" << e.subType << "\"" << "," << std::endl
                << "\t\t\"period\": " << e.period << "," << std::endl
                << "\t\t\"startFrame\": " << e.startFrame << "," << std::endl
                << "\t\t\"startTime\": " << e.startTime << "," << std::endl
                << "\t\t\"endFrame\": " << e.endFrame << "," << std::endl
                << "\t\t\"fromPlayer\": " << e.fromPlayer << "," << std::endl
                << "\t\t\"toPlayer\": " << e.toPlayer << "," << std::endl
                << "\t\t\"bodyPart\": " << "\"" << e.bodyPart << "\"" << "," << std::endl
                << "\t\t\"technique\": " << "\"" << e.technique << "\"" << std::endl;

            if (x != (eventDataVector.size() - 1))
            {
                os << "\t}," << std::endl;
            }
            else
            {
                os << "\t}" << std::endl;
            }
        }

        os << "\t]}" << std::endl;

    }

    os.close();
}

enum FileType{None, Tracking, Event};

int main()
{

    FileType fileType = None;

    while (fileType == None)
    {
        std::cout << "Please Enter File Type\n\t1] Tracking File\n\t2] Event File" << std::endl;
        std::string temp;
        std::cin >> temp;

        if (temp == "1" || temp == "2")
        {
            fileType = FileType(std::stof(temp));
        }
        else
        {
            std::cout << "Invalid Option. Please Enter 1 or 2" << std::endl;
        }
    }

    std::string fileName;
    std::cout << "Enter File Name: " << std::endl;
    std::cin >> fileName;
    fileName += ".txt";

    std::ifstream is(fileName, std::ifstream::binary);

    if (is) 
    {
        // get length of file:
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        char* buffer = new char[length];

        if (fileType == Tracking)
        {

            std::map<int, std::vector<PlayerData>> dataFrameMap;
            std::map<int, BallData> ballFrameMap;

            float progress = 0;
            float precentage = 0;

            int firstFrameNumber = 0;
            bool firstFrame = true;

            while (!is.eof())
            {

                //player data in each frame
                std::vector<PlayerData> playerDataVector;

                is.getline(buffer, length);


                //convert frame to string
                std::string frame(buffer);

                printf("\33[2K\r");

                progress += frame.size();

                precentage = progress / length * 100;

                std::cout << "Reading File " << precentage << "  \\  " << 100;

                if (frame.size() > 0)
                {
                    bool readingPlayer = false; //start reading player data
                    bool readingBall = false;

                    std::string playerDataString = "";
                    std::string ballDataString = "";
                    std::string frameNumber = "";

                    //iterating over the frame's characters
                    for (std::string::size_type i = 0; i < frame.size(); i++)
                    {

                        if (frame[i] == ':')
                        {
                            if (!readingPlayer)
                            {
                                readingBall = false;
                                readingPlayer = true;

                            }
                            else if (!readingBall)
                            {
                                //read ball data
                                readingPlayer = false;
                                readingBall = true;
                            }
                        }
                        else if (readingPlayer)
                        {
                            playerDataString += frame[i];
                        }
                        else if (readingBall && frame[i] != ';')
                        {
                            ballDataString += frame[i];
                        }
                        else if (!readingPlayer && !readingBall)
                        {
                            frameNumber += frame[i];
                        }

                    }

                    if (firstFrame)
                    {
                        firstFrameNumber = std::stof(frameNumber);
                        firstFrame = false;
                    }

                    std::vector<std::string> playerData = splitString(playerDataString, ';');

                    for (int i = 0; i < playerData.size(); i++)
                    {
                        //std::cout << playerData2[i] << std::endl;

                        PlayerData temp;

                        temp = convert(playerData[i]);

                        playerDataVector.push_back(temp);
                    }

                    BallData ballData = convertBall(ballDataString);
                    //save data onto the frame map
                    dataFrameMap.emplace(int(std::stof(frameNumber)), playerDataVector);
                    ballFrameMap.emplace(int(std::stof(frameNumber)), ballData);

                }

            }

            std::cout << std::endl;

            //formatAndSave("output.txt", dataFrameMap);
            formatAndSaveVelocity("tracking_output.txt", dataFrameMap, ballFrameMap, firstFrameNumber);
        }

        else if (fileType == Event)
        {
            std::vector<EventData> eventDataVector;

            float progress = 0;
            float precentage = 0;

            while (!is.eof())
            {

                is.getline(buffer, length);

                //convert event to string
                std::string eventString(buffer);

                printf("\33[2K\r");

                progress += eventString.size();

                precentage = progress / length * 100;

                std::cout << "Reading File" << precentage << "  \\  " << 100;

                if (eventString.size() > 0)
                {

                    EventData eventData = convertEvent(eventString);

                    //save data onto the frame map
                    eventDataVector.push_back(eventData);

                }

            }

            std::cout << std::endl;

            //formatAndSave("output.txt", dataFrameMap);
            formatAndSaveEvent("event_output.txt", eventDataVector);
        }

        is.close();

        // ...buffer contains the entire file...
        delete[] buffer;

        std::cout << std::endl << "Finished Exporting" << std::endl;
    }
    else
    {
        std::cout << "Failed to Read File. Please Restart." << std::endl;
    }

	return 0;
}