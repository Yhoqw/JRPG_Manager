#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>
#include "JRPG_GameManager.h"

using namespace std;

//Windows h (all windows h stuff will be mentioned with a (.h) comment
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int color) 
{	SetConsoleTextAttribute(hConsole, color);	}

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3, TOTAL_MATCHES = 6; //I could use #define for this instead

//CLASSES

class BattleManager{
	public: 
		static void Battle_Simulation(Team &team1, Team &team2);
		static void Declare_Winner(Team &winner, Team &loser);
		
};

class ConsoleManager {
    public:
        static void PrintTitle();
};


//METHODS

void BattleManager::Declare_Winner(Team &winner, Team &loser){
	
    cout << "\n" << winner.get_Name() << " wins!\n" << endl;
    winner.add_Win();
    loser.add_Loss();
}

void BattleManager::Battle_Simulation(Team &team1, Team &team2) {
	
    team1.get_OVR(); 
    team2.get_OVR();
    
    int victory_Threshold = 50 + team1.get_OVR() - team2.get_OVR();
    int team1_Victory = rand() % 100;

    if (team1_Victory > victory_Threshold) 
	{	Declare_Winner(team1, team2);	} 
	else
	{	Declare_Winner(team2, team1);	}
}

void ConsoleManager :: PrintTitle(){
	
	SetColor(6); //Yellow
	cout<<"::::::::::: :::::::::  :::::::::       ::::    ::::      :::     ::::    :::     :::      ::::::::  :::::::::: :::::::::"<<endl;
	cout<<"    :+:     :+:    :+: :+:    :+:      +:+:+: :+:+:+   :+: :+:   :+:+:   :+:   :+: :+:   :+:    :+: :+:        :+:    :+:"<<endl;
	cout<<"    +:+     +:+    +:+ +:+    +:+      +:+ +:+:+ +:+  +:+   +:+  :+:+:+  +:+  +:+   +:+  +:+        +:+        +:+    +:+ "<<endl;
	cout<<"    +#+     +#++:++#:  +#++:++#+       +#+  +:+  +#+ +#++:++#++: +#+ +:+ +#+ +#++:++#++: :#:        +#++:++#   +#++:++#:"<<endl;
	cout<<"    +#+     +#+    +#+ +#+             +#+       +#+ +#+     +#+ +#+  +#+#+# +#+     +#+ +#+   +#+# +#+        +#+    +#+"<<endl;
	cout<<"#+# #+#     #+#    #+# #+#             #+#       #+# #+#     #+# #+#   #+#+# #+#     #+# #+#    #+# #+#        #+#    #+#"<<endl;
	cout<<" #####      ###    ### ###             ###       ### ###     ### ###    #### ###     ###  ########  ########## ###    ### "<<endl;
	SetColor(7); //Reset to white
	cout<<"__________________________________________________________________________________________________________________________"<<endl;		
}