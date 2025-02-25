#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//Windows h (all windows h stuff will be mentioned with a (.h) comment
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int color) 
{	SetConsoleTextAttribute(hConsole, color);	}

//Variables Arrays and constants
string first_names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro", "Luca ", "Raven", "Skyler", "Noa", "Sage", "Shin", "Jin"};
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi", " Lion-Heart" };

string Team_name1[] = {"Midgar", "Zanarkand", "Palmacosta", "Alcamoth", "Tortuga"};
string Team_name2[] = {" Crimson Blades", " Sentinels", " Vanguard", " Covenant", " Syndicate"};

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3; //I could use #define for this instead

//create seperate functions for player actions?

//CLASSES
class Entity{
	protected:
		string name;	
		int HP = 50, ATK, SPD, DEF, ID, Team_ID = 1;
		int Cur_HP = HP, Cur_ATK = ATK, Cur_SPD = SPD, Cur_DEF = DEF;
		
	public:
		
		void Display_Stats();	
		void generate_character();
		void Reset_Stats();
		
		int get_HP();
		int get_ATK();
		int get_SPD();
		int get_DEF();
		
		//Constructor (Ideally the reading file info will be done in the constructor)
		Entity()
		{	
			generate_character();	
			Reset_Stats();
		}
};
 
class Team{
	private:
		string name;
		short ID, wins = 0, losses = 0;
		int OVR_ATK;
		bool player_Controlled = false;
	
	public:	
		Entity Members[PLAYERS_PER_TEAM];
		
		void Display_stats();
		
		string get_Name();
		int get_OVR();
		int get_Wins();
		int get_Losses();
		
		Team()
		{	
			name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];	
			get_OVR();
		}
};

class BattleManager{
	public: 
		static void Battle_Simulation();
		static void Battle_Selector();
};

class GameManager {
    private:
    	int Day = 0;
        Team NPC_Teams[NUMBER_OF_TEAMS];
        
    public:
    	void Run_Game();
    	void Schedule();
    	int get_Day();
};

class ConsoleManager {
    public:
        static void PrintTitle();
};

//METHODS

//Entity Methods
void Entity :: Display_Stats(){
	
	SetColor(11); // Light cyan for stats
	cout << name << " |HP: " << Cur_HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << " |Team: " << Team_ID << endl;
	SetColor(7); //Reset to white		
};

void Entity :: generate_character(){

	//Generates a random name 		
	name = first_names[rand() % (sizeof(first_names) / sizeof(first_names[0]))] + last_names[rand() % (sizeof(last_names) / sizeof(last_names[0]))] ;
	
	ATK = (rand() % 20) + 1;
	SPD = (rand() % 10);
	DEF = (rand() % 10);
	
};

void Entity :: Reset_Stats(){
	
	Cur_HP = HP;
	Cur_ATK = ATK;
	Cur_SPD = SPD;
	Cur_DEF = DEF;
}

int Entity :: get_HP(){
	return(HP);
};

int Entity :: get_ATK(){
	return(ATK);
};

int Entity :: get_SPD(){
	return(SPD);
};

int Entity :: get_DEF(){
	return(DEF);
};

string Team :: get_Name(){
	return(name);
}

int Team :: get_OVR(){
	
	OVR_ATK = 0; // Reset before summing
	
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
	{	OVR_ATK += Members[i].get_ATK();	}
		  
	return (OVR_ATK /= PLAYERS_PER_TEAM);
}

void Team :: Display_stats(){

	SetColor(11);
	cout << "Team: " << name << " wins: " << wins << " losses: " << losses << endl;
	cout << endl;
	SetColor(7); 	
	
}

void BattleManager :: Battle_Simulation(){ 			 
	
}

void BattleManager :: Battle_Selector(){
	cout << "WIP" << endl;
}

void GameManager :: Schedule(){

    int matchups[6][2] = 
	{
        {0, 1} ,{2, 3}, {0, 2}, {1, 3}, {0, 3}, {1, 2}  
    };
	
    int teamA = matchups[Day][0];
    int teamB = matchups[Day][1];

    cout << "Day " << (Day+1) << ": " << NPC_Teams[teamA].get_Name() << " vs " << NPC_Teams[teamB].get_Name() << endl;
    Sleep(1000); 

	BattleManager::Battle_Selector();
	
    Day++;	
	
}

void GameManager :: Run_Game(){
	
	while (Day != 7)
	{	
		Schedule();	
	}	
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

//MAIN
int main(){
	
	GameManager GM;
	srand(static_cast<unsigned int>(time(0)));	//for seeding
		
	ConsoleManager::PrintTitle(); 
	cout << "Welcome to RPG manager! This is a blend of sport sims and JRPGs. This program currently generates 4 Teams they will play each in simulations of JRPG style \ncombat by the end of the season, 2 teams will play a final to pick a winner" << endl;
	
	GM.Run_Game();
	
	return(0);	
};