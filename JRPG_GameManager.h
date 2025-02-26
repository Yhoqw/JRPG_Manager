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

//Variables Arrays and constants (Dont mind the names these are taken from a list of popular JRPGs)
string first_names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro", "Luca ", "Raven", "Skyler", "Noa", "Sage", "Shin", "Jin"};
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi", " Lion-Heart" };

string Team_name1[] = {"Midgar", "Zanarkand", "Palmacosta", "Alcamoth", "Tortuga", "Shevat", "Gilito", "Lindblum"};
string Team_name2[] = {" Crimson Blades", " Sentinels", " Vanguard", " Covenant", " Syndicate", " Stormbringers", " Knights", " Abysswalkers", " Pact"};

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3, TOTAL_MATCHES = 6; //I could use #define for this instead

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
		
		void add_Win() { wins++; };
		void add_Loss() { losses++; };		
		
		Team()
		{	
			name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];	
			get_OVR();
		}
};

class Schedule{
	private:
		int Day = 0;
		int matchups[6][2] = {	{0, 1} ,{2, 3}, {0, 2}, {1, 3}, {0, 3}, {1, 2}  };
		Team* teams;
	
	public:
		void reset() {Day = 0;}	// Reset for a new season
		bool battles_Remaining() {return Day < 6;} // Check if the season is ongoing
		void Matchup();
		
		Schedule(Team* teamsPtr) : teams(teamsPtr) 
		{
			Day = 0;
		}
};

class GameManager {
    private:	
        Team NPC_Teams[NUMBER_OF_TEAMS];
        Schedule season;
        
    public:
    	void Run_Game();
    	void Management_Mode();
    	
    	GameManager() : season(NPC_Teams) {}
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

void Schedule :: Matchup(){
	
    if (!battles_Remaining()) 
	{
        cout << "Season is over! No more battles" << endl;
        return;
    }
	
    int teamA = matchups[Day][0];
    int teamB = matchups[Day][1];

    cout << "\nDay " << (Day+1) << ": " << teams[teamA].get_Name() << " vs " << teams[teamB].get_Name() << "\n" << endl;
    Sleep(1000); 

	BattleManager::Battle_Simulation(teams[teamA], teams[teamB]);
	
    Day++;	
}

void GameManager :: Run_Game(){
	
	while ( season.battles_Remaining() ) //Dont use system cls right now for debugging purposes 
	{	
		season.Matchup();
		Management_Mode();
	}	
}

void GameManager :: Management_Mode(){
	
	int input, loop = 1; 
	enum Actions{SIM_DAY = 1};
	
	do{
		cout << "\nPress 1.To Simulate to next day\n" << endl;
		cin >> input;
		
		switch(input)
		{				
			//Exit case
			case SIM_DAY:
			{
				loop = 0;
				break;
			}
			
			default:
				break;
		}
	} while (loop != 0);	
}
