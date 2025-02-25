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

void SetColor(int color) //.h
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
	public:
		
		//Entity Class Attributes/Member variables
		string name;	
		int HP = 50, ATK, SPD, DEF, ID, Team_ID = 1;
		int Cur_HP = HP, Cur_ATK = ATK, Cur_SPD = SPD, Cur_DEF = DEF;
		
		bool Turn = true;

		//Entity Class member functions
		void Display_stats();
		void Attack_Check(Entity &target);
		void Read_File();		
		void generate_character();
		void Reset_Stats();
		
		//Constructor (Ideally the reading file info will be done in the constructor)
		Entity()
		{	
			generate_character();	
			Reset_Stats();
		}
};
 
class Player : public Entity {
	public:
		using Entity::Entity;
		
		void Actions(Entity enemys[3]);
};

class Opposition : public Entity {
	public:
		using Entity :: Entity;
		
		//Enemy Sub-Class member functions
		void Action(Player &player);
};

class Team{
	public:
		string name;
		short ID, wins = 0, losses = 0;
		int OVR_ATK;
		bool player_Controlled = false;
		Entity party_members[3];
		
		void calulate_OVR();	
		void Display_stats();
		
		Team()
		{	name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];	}
};

//METHODS

//Entity Methods
void Entity :: Display_stats(){
	
	SetColor(11); // Light cyan for stats
	cout << name << " |HP: " << Cur_HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << " |Team: " << Team_ID << endl;
	SetColor(7); //Reset to white		
};

void Entity :: Attack_Check(Entity &target){
	
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - target.Cur_SPD; 
    
    SetColor(12); //Red for attack messages  
    cout << name << " Attacks " << target.name << endl; 
    Sleep(500); //Small delay for impact
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = Cur_ATK - target.Cur_DEF; //int damage = max(0, ATK - target.DEF); is a proposed solution that could outright circumvent the negatives issue
	
		damage = (damage < 0) ? 0 : damage; //in case damage gives a negative number (Ternaray operator)
		target.Cur_HP -= damage;
			
        target.Cur_HP = (target.Cur_HP < 0) ? 0 : target.Cur_HP; //Ensure HP does not go below 0
        
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

//Player|Opposition methods 
void Player :: Actions(Entity enemys[]) {
	
	Display_stats();
	
	int input;
	
	//Define availaible actions
	enum Actions
	{	Attack = 1, SPD_UP, DEF_UP, ATK_UP, Heal	};

	//Action Selection and output
	if (Turn == true)
	{
		cout << "1.Attack \n2.SPD UP \n3.DEF UP \n4.ATK UP \n5.Heal" << endl;
		cin >> input;	
			
		switch (input)
		{
			case Attack:
			{
				int target; //Make seperate funtions for choosing target?

				cout << "Select who you wish to attack" "\n1." << enemys[0].name << "\n2." << enemys[1].name << "\n3." << enemys[2].name << endl;
				cin >> target;
				
				int targeted = target-1; //Because the IDs apear as 1, 2, 3 for user but 0, 1, 2 in the array index
				
				Attack_Check(enemys[targeted]);
								
				break;
			}
				
			//Make function to up a given stat by a given amount depending on the parameters?
			case SPD_UP:
				Cur_SPD += 5;
				SetColor(10); //.h
				cout << "SPD UP +5\n" << endl;
				break;
				
			case DEF_UP:
				Cur_DEF += 5;
				SetColor(10); //.h
				cout << "DEF UP +5\n" << endl;
				break;
					
			case ATK_UP:
				Cur_ATK += 5;
				SetColor(10); //.h
				cout << "ATK UP +5\n" << endl;
				break;
	
			case Heal:
				Cur_HP += 30;
				SetColor(10); //.h
				cout << "Healed" << name << " +30\n" << endl;
				break;
				
        	default:
				SetColor(14);
        	    cout << "Invalid choice!\n" << endl;
        	    break;				
		}
		
		SetColor(7); //.h
    	Sleep(500); //.h
	}
}

void Opposition :: Action(Player &player) {
	
	Display_stats();
    Attack_Check(player);
}

//Team methods
void Team :: calulate_OVR() {
	
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
	{	OVR_ATK += party_members[i].ATK;	}
		 
	OVR_ATK /= PLAYERS_PER_TEAM; 
}

void Team :: Display_stats(){

	SetColor(11);
	cout << "Team: " << name << " wins: " << wins << " losses: " << losses << endl;
	cout << endl;
	SetColor(7); 	
	
}


//Game (Non-Class functions) :- Maybe I should make a seperate class for handling game flow?

void Battle(Player players[], Opposition enemies[], Team &opposition_team, Team &player_team) //Where the combat happens
{
	
	for (int i = 0; i < PLAYERS_PER_TEAM; i++) //set objects of enemy array as the same as the opposing team
	{
		enemies[i].name = opposition_team.party_members[i].name;
		enemies[i].ATK = opposition_team.party_members[i].ATK;
		enemies[i].SPD = opposition_team.party_members[i].SPD;
		enemies[i].DEF = opposition_team.party_members[i].DEF;
		enemies[i].Team_ID = opposition_team.party_members[i].Team_ID;
	}					
	
	int players_turn = true;
	
	//Battle loop
	cout << "\n--- BATTLE BEGINS! ---\n";
    Sleep(1000);
	
	//Combat Loop
	while ( (enemies[0].Cur_HP > 0 || enemies[1].Cur_HP > 0 || enemies[2].Cur_HP > 0) && (players[0].Cur_HP > 0 || players[1].Cur_HP > 0 || players[2].Cur_HP > 0) ){
		
		//Players Turn 
		for (int i = 0; i < PLAYERS_PER_TEAM; i++)
		{	players[i].Actions(enemies);	}
	
		players_turn = false;

		//Enemy Turn
		for (int i = 0; i < PLAYERS_PER_TEAM; i++)
		{
			if (enemies[i].Cur_HP > 0)
			{	enemies[i].Action(players[i]);	}
		}
		
		players_turn = true;
	}
	
	//Win Loss states
	if (enemies[0].Cur_HP <= 0 && enemies[1].Cur_HP <= 0 && enemies[2].Cur_HP <= 0)  //Win
	{
		SetColor(10); //green for victory	
		cout << "\nPlayers win!\n" << endl; // !There is a bug somewhere in here which doesnt give wins or losses properly
		opposition_team.losses += 1;
		player_team.wins += 1;	
	}

	if (players[0].Cur_HP <= 0 && players[1].Cur_HP <= 0 && players[2].Cur_HP <= 0) //lose
	{	
		SetColor(12); //Red for defeat
		cout << "\nEnemies win!\n" << endl;	
		opposition_team.wins += 1;
		player_team.losses += 1;
	}
	
	SetColor(7);
}

void Battle_Simulation(Team &team1, Team &team2) //CPU Battles
{	
	team1.calulate_OVR();
	team2.calulate_OVR();
	
	int victory_Threshold = 50 + team1.OVR_ATK - team2.OVR_ATK;	
	int team1_Victory = rand() % 100;
		 
	if (team1_Victory > victory_Threshold)
	{
	 	cout << "\nteam 1 wins!\n" << endl;
	 	team2.losses += 1;
		team1.wins += 1;
	}
	 
	else
	{
		cout << "\nteam 2 win!\n" << endl;	
		team2.wins += 1;
		team1.losses += 1;	 	
	}
}

void Battle_Selector(Player players[], Opposition enemies[], Team teams[], int TeamA, int TeamB)
{
    // If the player’s team is involved, run an interactive battle
    if (teams[TeamA].player_Controlled == true) 
	{
        cout << "Player's team is in this match!" << endl;
        Battle(players, enemies, teams[TeamA], teams[TeamB]); 
    }

    else if (teams[TeamB].player_Controlled == true) 
	{
        cout << "Player's team is in this match!" << endl;
        Battle(players, enemies, teams[TeamA], teams[TeamB]); 
    }
	
    else 
	{
        cout << "CPU match simulation..." << endl;
        Battle_Simulation(teams[TeamA], teams[TeamB]);
    }	
    
    for (int i = 0; i < NUMBER_OF_TEAMS; i++) //To reset the cur_stats after every battle
    {
    	for (int j = 0; j < PLAYERS_PER_TEAM ; j++)
    	{	teams[i].party_members[j].Reset_Stats();	}
	}
}

void Schedule(Team teams[], Player players[], Opposition enemies[], short &day, int Player_controlled_ID)
{
	
    // Match schedule using a 2D array
    int matchups[6][2] = 
	{
        {0, 1}, {2, 3}, // Day 1 & 2
        {0, 2}, {1, 3}, // Day 3 & 4
        {0, 3}, {1, 2}  // Day 5 & 6
    };

	//simulate the schedule through this for loop

    int teamA = matchups[day][0];
    int teamB = matchups[day][1];

    cout << "Day " << (day + 1) << ": " << teams[teamA].name << " vs " << teams[teamB].name << endl;
    Sleep(1000); 

	Battle_Selector(players, enemies, teams, teamA, teamB);
        
    day++;
}

void Config(Entity entitites[], Player players[], Opposition enemies[], Team teams[]) //Where all the the managerial stuff happens
{
	
	int Entitity_ID, player_ID;
	int input, loop = 1; 
	
	enum Actions{Trade = 1, Team_Stats, Sim};
	
	//Management Actions
	do{
		cout << "Press 1.To Trade, 2.Check Team Stats 3.To Simulate to next day" << endl;
		cin >> input;
		
		switch(input)
		{
			case Trade:
			{
				for (int i = 0; i < NUMBER_OF_TEAMS; i++) //displays all npc data
				{
					for (int j = 0; j < PLAYERS_PER_TEAM; j++)
					{
						cout << "ID: " << (j + (i * PLAYERS_PER_TEAM)) + 1 << " ";
						teams[i].party_members[j].Display_stats();	
					}		
				}	
	
	
				//player selection 
				cout << "\n" "Enter the ID of the Entity you want to Trade with a player" << endl;
				cin >> Entitity_ID;
			
				for (int i = 0; i < PLAYERS_PER_TEAM; i++) //Displays all npcs in your team
				{
					cout << "ID: " << i + 1 << " ";
					players[i].Display_stats();				
				}
	
				cout << "Enter ID of the player you want to trade" << endl;
				cin >> player_ID;
				
				//players swap (maybe ill make a function for this?)
				players[player_ID - 1].name = entitites[Entitity_ID - 1].name;
				players[player_ID - 1].ATK = entitites[Entitity_ID - 1].ATK;
				players[player_ID - 1].SPD = entitites[Entitity_ID - 1].SPD;
				players[player_ID - 1].DEF = entitites[Entitity_ID - 1].DEF;
				players[player_ID - 1].Team_ID = entitites[Entitity_ID - 1].Team_ID;
				
				break;
			}
		
			case Team_Stats:
			{
				for (int i = 0; i < NUMBER_OF_TEAMS; i++)
				{	teams[i].Display_stats();	}
				
				break;
			}
						
			//Exit case
			case Sim:
			{
				loop = 0;
				break;
			}
			
			default:
				break;
		}
	} while (loop != 0);
}

//A few functions for convenience and to make int main and other funtions more readable
void display_Title(){
	
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
	
	int team_selection, opp_team;
	short day = 0;
	char exit;
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//objects
 	Team teams[NUMBER_OF_TEAMS]; 
	Entity profiles[NUMBER_OF_TEAMS * PLAYERS_PER_TEAM];
	Player players[PLAYERS_PER_TEAM];
	Opposition enemies[PLAYERS_PER_TEAM] = {Opposition(), Opposition(), Opposition()};			
	
	//set the team IDs for each object in the profiles array
	for (int i = 0; i < NUMBER_OF_TEAMS; i++)
	{
		teams[i].ID = i+1;
		
		for (int j = 0; j < PLAYERS_PER_TEAM; j++)
		{	
			profiles[j + ( i * PLAYERS_PER_TEAM )].Team_ID = i + 1;	
			teams[i].party_members[j] = profiles[j + ( i *PLAYERS_PER_TEAM )]; //The party memebers array takes stats from profiles array
		}
	}
		
	//Gameloop
	display_Title(); //Title Screen
	
	cout << "Welcome to RPG manager! This is a blend of sport sims and JRPGs. There are 4 teams. Choose your team (1-4)" << endl;
	cin >> team_selection;
		
	teams[team_selection - 1].player_Controlled = true;
			
	//set players based on the team you select (maybe ill make seperate functions for these longer loops)
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
	{
		players[i].name = teams[team_selection - 1].party_members[i].name;
		players[i].ATK = teams[team_selection - 1].party_members[i].ATK;
		players[i].SPD = teams[team_selection - 1].party_members[i].SPD;
		players[i].DEF = teams[team_selection - 1].party_members[i].DEF;
		players[i].Team_ID = teams[team_selection - 1].party_members[i].Team_ID;
	}
	
	//Actual Running game						
	do
	{	
		system("cls");
		Config(profiles, players, enemies, teams);	
		system("cls");

		Schedule(teams, players, enemies, day, team_selection);
			
		cout << "Exit (y/n)" << endl;
		cin >> exit;
		
	}while(exit != 'y' || exit != 'Y' || day != 7);
	
	//Code for the final logic
	int Finalist1 = 0;
	int Finalist2 = 1;
	
	for (int i = 0; i < NUMBER_OF_TEAMS; i++) //To set the finalists 
	{
		if (teams[i].wins > teams[Finalist1].wins)
			{	Finalist1 = i;	}
		
		else if(teams[i].wins > teams[Finalist2].wins && Finalist1 != i)
			{	Finalist2 = i;	}
	}

	Battle_Selector(players, enemies, teams, Finalist1, Finalist2);
	
	return(0);	
};