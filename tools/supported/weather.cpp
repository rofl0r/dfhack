// Just show some position data

#include <iostream>
#include <iomanip>
#include <climits>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdio>

#define DFHACK_WANT_MISCUTILS
#define DFHACK_WANT_TILETYPES
#include <DFHack.h>

using namespace DFHack;

void printWeather(DFHack::WeatherType current)
{
    switch (current)
    {
        case CLEAR:
            cout << "The sky is clear." << endl;
            cout << "Options:" << endl;
            cout << "'r' to make it rain." << endl;
            cout << "'s' to make it snow." << endl;
            break;
        case RAINING:
            cout << "It is raining." << endl;
            cout << "Options:" << endl;
            cout << "'c' to clear the sky." << endl;
            cout << "'s' to make it snow." << endl;
            break;
        case SNOWING:
            cout << "It is snowing." << endl;
            cout << "Options:" << endl;
            cout << "'c' to clear the sky." << endl;
            cout << "'r' to make it rain." << endl;
            break;
    }
    cout << "'q' to quit." << endl;
    cout << "anything else to refresh" << endl;
    cout << ">";
}

using namespace DFHack;
int main (int argc, char** argv)
{
    string command = "";
    bool quiet = false;
    bool cmdarg = false;
    for(int i = 1; i < argc; i++)
    {
        string test = argv[i];
        if(test == "-q")
        {
            quiet = true;
        }
        else
        {
            command = test;
            cmdarg = true;
        }
    }

    DFHack::ContextManager DFMgr("Memory.xml");
    DFHack::Context *DF = DFMgr.getSingleContext();

    try
    {
        DF->Attach();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        #ifndef LINUX_BUILD
            cin.ignore();
        #endif
        return 1;
    }

    World *W = DF->getWorld();
    W->Start();
    bool end = false;
    while(!end)
    {
        WeatherType current = (WeatherType) W->ReadCurrentWeather();
        DF->Resume();
        printWeather(current);
        if (command == "") getline(cin, command); // only read from stdin if command hasn't been passed on the console
        DF->Suspend();
        if(command == "c")
        {
            W->SetCurrentWeather(CLEAR);
        }
        else if(command == "r")
        {
            W->SetCurrentWeather(RAINING);
        }
        else if(command == "s")
        {
            W->SetCurrentWeather(SNOWING);
        }
        else if(command == "q")
        {
            end = true;
        }
        command = "";
        if(cmdarg) end = true; // exit the loop when a cmd line arg has been passed.
    }
    #ifndef LINUX_BUILD
        if (!quiet)
        {
            std::cout << "Done. Press any key to continue" << std::endl;
            cin.ignore();
        }
    #endif
    DF->Detach();
    return 0;
}
