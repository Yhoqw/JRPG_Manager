#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>

//TODO:
// Replace with Vectors (Team Member arrays, NPC Team array)
// Store Name vectors in Json file

using namespace std;
//Project By Yazdan Ali Khan (2024665), Hammad Shahid (2024389)

//#define DEBUG  // Comment this line to disable debug messages

//Windows .h 
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int color) 
{	SetConsoleTextAttribute(hConsole, color);	}

void PlaySoundEffect(const string &effect) 
{
	// Parameters take frequency and duration
	if (effect == "attack") Beep(800, 400); 	
    else if (effect == "miss") Beep(300, 200);
    else if (effect == "buff") Beep(700, 200);
}

//Variables Arrays and constants (Dont mind the names these are taken from a list of popular JRPGs)
inline vector<string> First_Names = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro", "Luca ", "Raven", "Skyler", "Noa", "Sage", "Shin", "Jin"};
inline vector<string> Last_Names = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi", " Lion-Heart" };

inline vector<string> Team_Name1 = {"Midgard", "Zanarkand", "Palmacosta", "Alcamoth", "Tortuga", "Shevat", "Gilito", "Lindblum", "Asgard"};
inline vector<string> Team_Name2 = {" Blades", " Sentinels", " Vanguard", " Covenant", " Syndicate", " Stormbringers", " Knights", " Abysswalkers", " Pact"};

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3, TOTAL_MATCHES = 11; //I could use #define for this instead lol
const int MAX_ATK = 20, MAX_SPD = 10, MAX_DEF = 10, MAX_HP = 15; //Max stats for the entities

//Prototypes
class Entity;
class PLayer;
class Team;
class PlayerTeam;
class BattleManager;
class Schedule;
class GameManager;
class ConsoleManager;	

//CLASSES

	//Entitys
class Entity{															//Base class for all NPCs
	protected:
		string Name;	
		bool Is_Alive;
		int HP, ATK, SPD, DEF;
		int Cur_HP = HP, Cur_ATK = ATK, Cur_SPD = SPD, Cur_DEF = DEF;
		
	public:
		void* Opposing_TeamPtr = nullptr;
		
		virtual void Display_Stats(int color);	//11 for entity, 10 for player
		void Generate_Character();
		void Reset_Stats();
		void Attack_Check(Entity &Target);
			
		//Getters
		bool get_Is_Alive() const; 
		
		string get_Name() const	{	return(Name);	}			//Maybe I should have a wrapper class for getters and setters?
		int get_HP() const		{	return(HP);		}	
		int get_ATK() const		{	return(ATK);	}		
		int get_SPD() const		{	return(SPD);	}
		int get_DEF() const		{	return(DEF);	}

		
		int get_Cur_HP() const			{	return(Cur_HP);		}	
		int get_Cur_ATK() const			{	return(Cur_ATK);	}			
		int get_Cur_SPD() const			{	return(Cur_SPD);	}
		int get_Cur_DEF() const			{	return(Cur_DEF);	}
		
		//Setters
		void set_HP(int new_HP);
		void set_ATK(int new_ATK) 	  {Cur_ATK = new_ATK;}
		void set_SPD(int new_SPD) 	  {Cur_SPD = new_SPD;}
		void set_DEF(int new_DEF)     {Cur_DEF = new_DEF;}

		virtual void Choose_Target(Team &enemy_Team);       //Overloaded Choose Target because it kept messing up
		virtual void Choose_Target(PlayerTeam &enemy_Team); 
		virtual void Actions();
		
		Entity() : HP(MAX_HP), ATK(0), SPD(0), DEF(0), Is_Alive(true) 
		{ 
			Generate_Character(); 
			Reset_Stats(); 
		}
};

class Player : public Entity {										
    public:
    	void Change_Stat(int &stat, int amount, const string statName);
    	
    	void Choose_Target(Team &enemy_Team) override;		//Overridden methods
    	void Actions() override;

		void set_Player_Name();
    	
        Player()											
		{
            Generate_Character();
            Reset_Stats();
        }
};

	//Teams
class Team{																//Base Team class contains NPCS(Entities)
	protected:
		string name;
		short ID, wins = 0, losses = 0;
		int OVR_ATK;
		bool Is_Team_Alive;
	
	public:	
		Entity Members[PLAYERS_PER_TEAM];
		
		void Run_For_All_Members(void (Entity::*method)()); 
		void Display_Members();
		
		virtual void Display_stats();
										//Getters
		string get_Name() const { return(name);	};
		int get_OVR();
		int get_Wins() const    { return(wins);	};
		int get_Losses()  const { return(losses);	};
										//Setters
		void add_Win()  { wins++; };
		void add_Loss() { losses++; };	
		void reset_Stats();	

		bool Has_Members_Alive();
		
		Team()						
		{	
			name = 	Team_Name1[rand() % Team_Name1.size()] + Team_Name2[rand() % Team_Name2.size()];
			get_OVR();
		}
};

class PlayerTeam : public Team{ 										//Team Class contains PCs
	public:
		Player Members[PLAYERS_PER_TEAM];	
		
		void Display_Members();
		void Display_stats() override;

		void set_Member_Name();
		void set_Team_Name();
		bool Has_Members_Alive();
		
        PlayerTeam() : Team()				
		{
			for (int i = 0; i < PLAYERS_PER_TEAM; i++) 
			{
				Members[i] = Player(); // Explicitly initialize each Player
			}
		}
};

	//The rest of the Classes
class BattleManager{													//All Combat functions are contained here
	public: 
		static void Battle_Simulation(Team &team1, Team &team2);
		static void PC_Battle(PlayerTeam &PC_Team, Team &Opposing_Team); 
		static void Declare_Winner(Team &winner, Team &loser);
		static void Display_Round_Summary(PlayerTeam &PC_Team, Team &Opposing_Team); 
};

class Schedule{															//Sets the Matchups based on day contained in GameManager class
	private:
		int Day = 0;					//Maybe Ill add a function that automaically decides the matchups based on the number of teams? 
		vector<pair<int, int>> matchups = {	{0,0}, {4, 3}, {1, 0} ,{2, 3}, {4, 0}, {0, 2}, {1, 3}, {4, 1}, {0, 3}, {1, 2}, {4, 2}}; //0 is team at index 0, 1 at index 1... 4 is for player team
		Team* teams;
		PlayerTeam* playerTeam;
	
	public:
		void reset() {Day = 0;}	// Reset for a new season
		void Matchup();			// sets either PC Battles or Sim Battles based on the matchups array
		void Print_Schedule();	//Prints matchups

		int get_Days() {   return Day;	}	
		
    Schedule(Team* teamsPtr, PlayerTeam* playerPtr) : teams(teamsPtr), playerTeam(playerPtr) {	reset();	}
};	

class GameManager {														//Essentially the Main Game class
    private:	
        Team NPC_Teams[NUMBER_OF_TEAMS];	
        PlayerTeam PC_Team;
        Schedule season;

    public:
    	void Run_For_All_Teams(void (Team::*method)()); 
    	void Display_Standings();
    	void Display_Roster();
    	
    	void Run_Game();
    	void Management_Mode(); 
    	void Final();
    	void Trade();
    	void Reset_TeamStats(); 
		void Swap_Entities(int teamA_id, int playerA_id, int teamB_id, int playerB_id);
		void Select_Team_For_Trade(int Teamid);
    	
    	GameManager() : season(NPC_Teams, &PC_Team) {} //This constructor functionally only exists to pass Team Ptrs to schedule
};

class ConsoleManager {													//Handles UI and Static Methods
	public:
        static void PrintTitle();
		static void PrintMenu();
		static void ClearScreen();
		static void PrintManagementMenu();
};

//Utility Function which exists to run two different function pointers which run a method for every object in an array
template <typename T, typename Method>
void Run_For_All(T* Array, int size, Method method) 
{
    for (int i = 0; i < size; i++) 
	{
        (Array[i].*method)();
    }
}