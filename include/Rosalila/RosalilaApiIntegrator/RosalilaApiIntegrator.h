#ifndef ROSALILA_API_INTEGRATOR_H
#define ROSALILA_API_INTEGRATOR_H

#include <iostream>

#include "../Rosalila.h"

#ifdef STEAM
#include "Steam/SteamIntegration.h"
#endif

using namespace std;

class ROSALILA_DLL RosalilaApiIntegrator
{
public:
    void init();

    bool isUsingApi();

    void unlockAchievement(string achievement_name);

    void setScore(string leaderboard_name, double score);
    void storeLeaderboardAttachment(string leaderboard_name, char* attachment, int attachment_size);
	#ifdef STEAM
    Leaderboard* getLeaderboard(string leaderboard_name);
	
    void downloadEntryAttachment(LeaderboardEntry* leaderboard_entry);
	#endif
	
	void findLeaderboard(string leaderboard_name);

    void setStat(string stat_name, double stat);
    double getStat(string stat_name);

    void storeData(string key, string data);
    string getData(string key);

    void updateCallbacks();
    string getState();

    void setCurrentControllerActionSet(string action_set_name);
    bool isControllerActionDown(string action_name);
    bool isUsingSteamController();
    void showControllerBindingPanel();
	
};


#endif
