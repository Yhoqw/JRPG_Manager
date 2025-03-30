#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

#define DEBUG  // Comment this line to disable debug messages

//Windows h (all windows h stuff will be mentioned with a (.h) comment
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetColor(int color) 
{	SetConsoleTextAttribute(hConsole, color);	}

//Variables Arrays and constants (Dont mind the names these are taken from a list of popular JRPGs)
string first_names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro", "Luca ", "Raven", "Skyler", "Noa", "Sage", "Shin", "Jin"};
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi", " Lion-Heart" };

string Team_name1[] = {"Midgard", "Zanarkand", "Palmacosta", "Alcamoth", "Tortuga", "Shevat", "Gilito", "Lindblum", "Asgard"};
string Team_name2[] = {" Blades", " Sentinels", " Vanguard", " Covenant", " Syndicate", " Stormbringers", " Knights", " Abysswalkers", " Pact"};

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3, TOTAL_MATCHES = 11; //I could use #define for this instead

//create seperate functions for player actions?

//Prototypes
class Entity;
class PLayer;
class Team;
class PlayerTeam;

//CLASSES
class Entity{															//Base class for all NPCs
	protected:
		string name;	
		int HP = 50, ATK, SPD, DEF;
		int Cur_HP = HP, Cur_ATK = ATK, Cur_SPD = SPD, Cur_DEF = DEF;
		
	public:
		Team *Opposing_TeamPtr = NULL;
		
		void Display_Stats();	
		void generate_character();
		void Reset_Stats();
		void Attack_Check(Entity &Target);
													//getters
		string get_Name();
		int get_HP();
		int get_ATK();
		int get_SPD();
		int get_DEF();
													//setters
		void set_HP(int new_HP) {Cur_HP = new_HP;}
		void set_ATK(int new_ATK) {Cur_ATK = new_ATK;}
		void set_SPD(int new_SPD) {Cur_SPD = new_SPD;}
		void set_DEF(int new_DEF) {Cur_DEF = new_DEF;}
		
		virtual void Actions();
		
		//Constructor (Ideally the reading file info will be done in the constructor)
		Entity()
		{	
			generate_character();	
			Reset_Stats();
		}
};

class Player : public Entity {											//PC class
    public:
    	void Modify_Stat(int &stat, int amount, const string statName);
    	void Choose_Target(Team &enemy_Team);
    	
    	void Actions() override;
    	
        Player()
		{
            generate_character();
            Reset_Stats();
        }
};
 
class Team{																//Base Team class contains NPCS
	protected:
		string name;
		short ID, wins = 0, losses = 0;
		int OVR_ATK;
		bool player_Controlled = false;
	
	public:	
		Entity Members[PLAYERS_PER_TEAM];
		
		void Display_stats();
		void Display_Members();
		
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

class PlayerTeam : public Team{ 										//Team Class contains PCs
	public:
		Player Members[PLAYERS_PER_TEAM];
		
        PlayerTeam() 
		{
            name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];

            // Initialize player members
            for (int i = 0; i < PLAYERS_PER_TEAM; i++) 
			{
                Members[i] = Player(); 
            }
    	}
};

class BattleManager{													//All Combat functions are contained here
	public: 
		static void Battle_Simulation(Team &team1, Team &team2);
		static void PC_Battle(PlayerTeam &PC_Team, Team &Opposing_Team);
		static void Declare_Winner(Team &winner, Team &loser);
		
};

class Schedule{
	private:
		int Day = 0;
		int matchups[7][2] = {	{0,0}, {1, 0} ,{2, 3}, {0, 2}, {1, 3}, {0, 3}, {1, 2}  };
		Team* teams;
		PlayerTeam* playerTeam;
	
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
        PlayerTeam PC_Team;
        Schedule season;
        
    public:
    	void Run_Game();
    	void Management_Mode();
    	
    	void Display_Standings();
    	void Display_Roster();
    	void Final();
    	void Trade();
    	
    	void Reset_TeamStats();
    	void Swap_Entities(int teamA_id, int playerA_id, int teamB_id, int playerB_id);
    	
    	void Run_For_All_Teams(void (Team::*method)()); //Function pointer which runs a for loop cycling through teams
    	
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
	cout << name << " |HP: " << Cur_HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << endl;
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

void Entity :: Attack_Check(Entity &Target){
	
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - Target.get_SPD(); 
    
    SetColor(12); //Red for attack messages  
    cout << name << " Attacks " << Target.get_Name() << endl; 
    Sleep(500); //Small delay for impact
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = Cur_ATK - Target.get_DEF(); //int damage = max(0, ATK - target.DEF); is a proposed solution that could outright circumvent the negatives issue
	
		damage = (damage < 0) ? 0 : damage; //in case damage gives a negative number (Ternaray operator)
		Target.set_HP(Target.get_HP() - damage);
			
        Target.set_HP( (Target.get_HP() < 0) ? 0 : Target.get_HP() ); //Ensure HP does not go below 0
        
		cout << name << "'s attack hits! Damage dealt: " << damage << "\n" << endl;
	}
	
	else
	{	
		SetColor(14); //Yellow for misses
		cout << "The attack missed!" << endl;	
	}
	
	SetColor(7); //Reset color
    Sleep(500);
}

													//GETTERS
string Entity :: get_Name()	{	return(name);	};

int Entity :: get_HP()	{	return(HP);		};

int Entity :: get_ATK()	{	return(ATK);	};

int Entity :: get_SPD()	{	return(SPD);	};

int Entity :: get_DEF()	{	return(DEF);	};
												
void Entity :: Actions(){
	Display_Stats();
}

//Player Methods
void Player :: Modify_Stat(int &stat, int amount, const string statName) {				
	
    stat += amount;
    SetColor(10);
    cout << statName << " UP +" << amount << "\n" << endl;
    SetColor(7);
}

void Player :: Choose_Target(Team &enemy_Team){										//Called in Actions() it selects the target and attck_checks()
	
	#ifdef DEBUG
	cout << "\Player::Attack operational\n" << endl;
	#endif	
	
    int target;
    cout << "Select a target:\n";
    
	enemy_Team.Display_Members();
    	
	cin >> target;
            
    if (target >= 1 && target <= 3) 
	{
        Attack_Check(enemy_Team.Members[target - 1]);
    } 
	
	else 
	{
    	cout << "Invalid target! Turn wasted.\n";
	}
}

void Player :: Actions(){														//Calls all the available actions for the Entity
	
	#ifdef DEBUG
	cout << "\Player::Actions() operational\n" << endl;
	#endif	
	
	Display_Stats();
	int input;
	
	//Define availaible actions
	enum Actions
	{	Attack = 1, SPD_UP, DEF_UP, ATK_UP, Heal	};	
	
	cout << "1.Attack \n2.SPD UP \n3.DEF UP \n4.ATK UP \n5.Heal" << endl;
	cin >> input;	

	switch (input)
	{
		case Attack:
		{	
            #ifdef DEBUG
            cout << "Debug: Attack selected.\n";
            #endif
            if (Opposing_TeamPtr) 
			{
                Choose_Target(*Opposing_TeamPtr);
            } 
			
			else 
			{
                cout << "No opposing team assigned! Turn wasted.\n";
            }
			break;
		}
				
        case SPD_UP:
            Modify_Stat(Cur_SPD, 5, "SPD");
            break;

        case DEF_UP:
            Modify_Stat(Cur_DEF, 5, "DEF");
            break;

        case ATK_UP:
            Modify_Stat(Cur_ATK, 5, "ATK");
            break;

        case Heal:
            Modify_Stat(Cur_HP, 30, "HP");
            break;

		
        default:
		{
			SetColor(14);
            cout << "Invalid choice! Turn Wasted.\n" << endl;
            break;
		}
	}
		
		SetColor(7); 
    	Sleep(500); 
}

//Team Methods
													//GETTERS
string Team :: get_Name(){
	return(name);
}

int Team :: get_OVR(){
	
	OVR_ATK = 0; // Reset before summing
	
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
	{	OVR_ATK += Members[i].get_ATK();	}
		  
	return (OVR_ATK /= PLAYERS_PER_TEAM);
}

int Team :: get_Wins() {	return(wins);	}

int Team :: get_Losses() {	return(losses);	}
													//Other METHODS
void Team :: Display_stats(){

	SetColor(3); //Cyan
	cout << "Team: " << name << " wins: " << wins << " losses: " << losses << endl;
	SetColor(7); //White
	cout << endl;

}

void Team :: Display_Members(){
	
	SetColor(3); //Blue
	cout << name << ": " << endl;
	SetColor(7); //White
	
    for (int i = 0; i < PLAYERS_PER_TEAM; i++) 
	{
        Members[i].Display_Stats(); 
    }
    
    cout << endl;
};

void Team :: reset_Stats(){
	wins = 0;
	losses = 0;
}

//Battle Manager Functions
void BattleManager::Declare_Winner(Team &winner, Team &loser){					//Print Winning Team message
	
	SetColor(10); // Green 
    cout << "\n" << winner.get_Name() << " win!\n" << endl;
    SetColor(7); //Reset to white	
    
    winner.add_Win();
    loser.add_Loss();
}

void BattleManager::PC_Battle(PlayerTeam &PC_Team, Team &Opposing_Team){		//Player Controlled Team Battles
	
	cout << "\n--- BATTLE BEGINS! ---\n";
    Sleep(1000);
	
	//Players Turn 
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)	
	{	
		PC_Team.Members[i].Opposing_TeamPtr = &Opposing_Team;
		PC_Team.Members[i].Actions();
	}
	
	//Enemy Turn
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
	{
		Opposing_Team.Members[i].Opposing_TeamPtr = &PC_Team;
		Opposing_Team.Members[i].Actions();
	}
	
	
}

void BattleManager::Battle_Simulation(Team &team1, Team &team2) {				//Battle Simulations

	#ifdef DEBUG
	cout << "\nBattle_Simulation operational\n" << endl;
	#endif

    team1.get_OVR(); 
    team2.get_OVR();
    
    int victory_Threshold = 50 + team1.get_OVR() - team2.get_OVR();
    int team1_Victory = rand() % 100;

    if (team1_Victory > victory_Threshold) 
	{	Declare_Winner(team1, team2);	} 
	else
	{	Declare_Winner(team2, team1);	}
}

//Schedule Methods
 int Schedule :: get_Days()	{	return Day;	}	

void Schedule :: Matchup(){
	
	#ifdef DEBUG 
	cout << "\nMatchup operational\n" << endl;
	#endif
	
    if (Day >= TOTAL_MATCHES ) //stop after playing all games in the season
	{
        cout << "Season is over! No more battles" << endl;
        return;
    }
    
    else if (Day == 0) 		//Before the season begins
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

//Game Manager Methods
void GameManager :: Run_For_All_Teams(void (Team::*method)()) {
	
	(PC_Team.*method)(); //Call the function for the player controlled team
	
    for (int i = 0; i < NUMBER_OF_TEAMS; i++) 
	{
        (NPC_Teams[i].*method)();  // Call the function pointer on each team
    }
}

void GameManager :: Final(){
	
	#ifdef DEBUG
	cout << "\nFinals operational\n" << endl;
	#endif	
	
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

void GameManager :: Run_Game(){											//Check numbers of days remaining in the season then cycle through basic game loop until final
	
	#ifdef DEBUG //Debugging
	cout << "\nRun Game operational\n" << endl;
	#endif

	while ( season.get_Days() < TOTAL_MATCHES )
	{	
		season.Matchup();
		Management_Mode();
	}	
	
	Display_Standings();
	Final();
}

void GameManager :: Management_Mode(){									//Options in between matches and match simulations
	
	#ifdef DEBUG
	cout << "\nManagement_Mode operational\n" << endl;
	#endif
	
	int input, loop = 1; 
	enum Actions{SIM_DAY = 1, STANDINGS, ROSTERS, TRADE};
		
	do{
		cout << "\nPress 1.To Simulate to next day 2.To Show Standings 3.To Show Rosters 4.To Trade\n" << endl;
		cin >> input;
		
		switch(input)
		{				
			case SIM_DAY:
			{
				loop = 0;
				break;
			}
			
			case STANDINGS:
			{
				Display_Standings();
				break;
			}
				
			case ROSTERS:
			{
				Display_Roster();
				break;
			}
			
			case TRADE:
			{
				Trade();
				break;
			}
			
			default:
				break;
		}
	} while (loop != 0);	
}

void GameManager :: Display_Standings(){
	
	#ifdef DEBUG
	cout << "\nDisplay Standings operational\n" << endl;
	#endif
	
	Run_For_All_Teams(&Team::Display_stats);
}

void GameManager :: Display_Roster(){
	
	#ifdef DEBUG
	cout << "\nDisplay_Roster operational\n" << endl;
	#endif
	
	Run_For_All_Teams(&Team::Display_Members);
}

void GameManager :: Swap_Entities(int teamA_id, int playerA_id, int teamB_id, int playerB_id) {	
	
	#ifdef DEBUG
	cout << "\nSwap_Entities operational\n" << endl;
	#endif
	
    if (teamA_id >= NUMBER_OF_TEAMS || teamB_id >= NUMBER_OF_TEAMS || playerA_id >= PLAYERS_PER_TEAM || playerB_id >= PLAYERS_PER_TEAM) //Error
	{
        cout << "Invalid indices. Swap failed" << endl;
        return;
    }
	
	//Actual Swap
    Entity temp = NPC_Teams[teamA_id].Members[playerA_id];

    NPC_Teams[teamA_id].Members[playerA_id] = NPC_Teams[teamB_id].Members[playerB_id];
    NPC_Teams[teamB_id].Members[playerB_id] = temp;


	SetColor(6); //Yellow
    cout << "Traded " << NPC_Teams[teamA_id].Members[playerA_id].get_Name() << " for " << NPC_Teams[teamB_id].Members[playerB_id].get_Name() << endl;
	SetColor(7); //Reset to white
}

void GameManager :: Trade(){											//Chooses the parameters for swaping entities (Accessed from Management Mode) 
	
	#ifdef DEBUG
	cout << "\nTrade Operational\n" << endl;
	#endif
	
	int teamA_id, playerA_id, teamB_id, playerB_id;
	
	cout << "Choose the team you wnat to trade a player from " << endl;
	cin >> teamA_id;
	NPC_Teams[teamA_id-1].Display_Members();
	
	cout << "Choose the player ID you want to trade " << endl;
	cin >> playerA_id;
	
	cout << "Choose the team you wnat to trade a player to " << endl;
	cin >> teamB_id;
	NPC_Teams[teamB_id-1].Display_Members();
	
	cout << "Choose the player ID you want to trade " << endl;
	cin >> playerB_id;	
	
	Swap_Entities(teamA_id-1, playerA_id-1, teamB_id-1, playerB_id-1);
}

//Console Manager Functions
void ConsoleManager :: PrintTitle(){

	#ifdef DEBUG
	cout << "\nPrintTitle operational\n" << endl;
	#endif

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
	
	#ifdef DEBUG
	cout << "\ndebugging operational\n" << endl;
	#endif
	
	GameManager GM;
	srand(static_cast<unsigned int>(time(0)));	//for seeding
		
	ConsoleManager::PrintTitle(); 
	cout << "Welcome to RPG manager! This is a blend of sport sims and JRPGs. This program currently generates 4 Teams they will play each in simulations of JRPG style \ncombat by the end of the season, 2 teams will play a final to pick a winner" << endl;
	
	GM.Run_Game();
	
	return(0);	
};
