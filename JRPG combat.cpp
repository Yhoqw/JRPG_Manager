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

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3, TOTAL_MATCHES = 7; //I could use #define for this instead

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
		void reset_Stats();	
		
		Team()
		{	
			name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];	
			get_OVR();
		}
};

class BattleManager{
	public: 
		static void Battle_Simulation(Team &team1, Team &team2);
		static void Declare_Winner(Team &winner, Team &loser);
		
};

class Schedule{
	private:
		int Day = 0;
		int matchups[7][2] = {	{0,0}, {1, 0} ,{2, 3}, {0, 2}, {1, 3}, {0, 3}, {1, 2}  };
		Team* teams;
	
	public:
		void reset() {Day = 0;}	// Reset for a new season
		void Matchup();
		int get_Days();
		
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
    	void Display_Standings();
    	void Final();
    	void Reset_TeamStats();
    	
    	void Run_For_All_Teams(void (Team::*func)()); //Function pointer which runs a for loop cycling through teams
    	
    	GameManager() : season(NPC_Teams) {}
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

int Team :: get_Wins(){
	return(wins);
}

int Team :: get_Losses(){
	return(losses);
}

void Team :: Display_stats(){

	SetColor(11);
	cout << "Team: " << name << " wins: " << wins << " losses: " << losses << endl;
	cout << endl;
	SetColor(7); 	
	
}

void Team :: reset_Stats(){
	wins = 0;
	losses = 0;
}

void BattleManager::Declare_Winner(Team &winner, Team &loser){
	
	SetColor(10); // Green 
    cout << "\n" << winner.get_Name() << " win!\n" << endl;
    SetColor(7); //Reset to white	
    
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

 int Schedule :: get_Days(){
	return Day;
}

void Schedule :: Matchup(){
	
	cout << "Matchup is working" << endl; //Debugging
	
    if (Day >= TOTAL_MATCHES ) //stop after playing all games in the season
	{
        cout << "Season is over! No more battles" << endl;
        return;
    }
    
    if (Day == 0)
    {
    	Day++;
    	return;
	}
	
    int teamA = matchups[Day][0];
    int teamB = matchups[Day][1];

    cout << "\nDay " << (Day) << ": " << teams[teamA].get_Name() << " vs " << teams[teamB].get_Name() << "\n" << endl;
    Sleep(1000); 

	BattleManager::Battle_Simulation(teams[teamA], teams[teamB]);
	
    Day++;	
}

void GameManager :: Run_For_All_Teams(void (Team::*method)()) {
	
    for (int i = 0; i < NUMBER_OF_TEAMS; i++) 
	{
        (NPC_Teams[i].*method)();  // Call the function pointer on each team
    }
}

void GameManager :: Final(){
	
	cout << "Finals!" << endl;
	
	//Code for the final logic
	int Finalist1_index = 0;
	int Finalist2_index = 1;
	
	for (int i = 0; i < NUMBER_OF_TEAMS; i++) //To set the finalists 
	{
		if ( NPC_Teams[i].get_Wins() > NPC_Teams[Finalist1_index].get_Wins() )
			{	Finalist1_index = i;	}
		
		else if(NPC_Teams[i].get_Wins() > NPC_Teams[Finalist2_index].get_Wins() && Finalist1_index != i)
			{	Finalist2_index = i;	}
	} 
	
    cout << NPC_Teams[Finalist1_index].get_Name() << " vs " << NPC_Teams[Finalist2_index].get_Name() << "\n" << endl;
    Sleep(1000); 	
	
	BattleManager::Battle_Simulation(NPC_Teams[Finalist1_index], NPC_Teams[Finalist2_index]);
	
}

void GameManager :: Run_Game(){
	
	cout << "Run Game is working" << endl;//Debug
	
	while ( season.get_Days() < TOTAL_MATCHES ) //Dont use system cls right now for debugging purposes 
	{	
		season.Matchup();
		Management_Mode();
	}	
	
	Display_Standings();
	Final();
}

void GameManager :: Management_Mode(){
	
	int input, loop = 1; 
	enum Actions{SIM_DAY = 1, STANDINGS};
		
	do{
		cout << "\nPress 1.To Simulate to next day 2.To Show Standings\n" << endl;
		cin >> input;
		
		switch(input)
		{				
			//Exit case
			case SIM_DAY:
			{
				loop = 0;
				break;
			}
			
			case STANDINGS:
			{
				Display_Standings();
			}
				
			default:
				break;
		}
	} while (loop != 0);	
}

void GameManager :: Display_Standings(){
	
	Run_For_All_Teams(&Team::Display_stats);
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

