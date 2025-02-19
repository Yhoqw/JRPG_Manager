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
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi" };

const int NUMBER_OF_TEAMS = 4, PLAYERS_PER_TEAM = 3;

//create seperate functions for player actions?

//CLASSES
class Entity{
	public:
		
		//Entity Class Attributes/Member variables
		string name;	
		int HP = 100, ATK, SPD, DEF, ID, Team_ID = 1;
		
		bool Turn = true;

		//Entity Class member functions
		void Display_stats();
		void Attack_Check(Entity &target);
		void Read_File();		
		void generate_character();
		
		//Constructor (Ideally the reading file info will be done in the constructor)
		Entity()
		{	generate_character();	}
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
		Entity party_members[3];
		
		void calulate_OVR()
		{
			for (int i = 0; i < PLAYERS_PER_TEAM; i++)
			{
				OVR_ATK += party_members[i].ATK;
			}
			 
			OVR_ATK /= PLAYERS_PER_TEAM; 
		}
};

//METHODS

//Entity Methods
void Entity :: Display_stats(){
	
	SetColor(11); // Light cyan for stats
	cout << name << " |HP: " << HP << ", ATK: " << ATK << ", SPD: " << SPD << ", DEF: " << DEF << " |Team: " << Team_ID << endl;
	SetColor(7); //Reset to white		
};

void Entity :: Attack_Check(Entity &target){
	
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - target.SPD; 
    
    SetColor(12); //Red for attack messages  
    cout << name << " Attacks " << target.name << endl; 
    Sleep(500); //Small delay for impact
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = ATK - target.DEF; //int damage = max(0, ATK - target.DEF); is a proposed solution that could outright circumvent the negatives issue
	
		damage = (damage < 0) ? 0 : damage; //in case damage gives a negative number (Ternaray operator)
		target.HP -= damage;
			
        target.HP = (target.HP < 0) ? 0 : target.HP; //Ensure HP does not go below 0
        
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

//Player/Opposition methods
void Player :: Actions(Entity enemys[]) {
	
	Display_stats();
	
	short input;
	
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
				//give options to choose who to attack
				int target;
				cout << "Select who you wish to attack" "\n1." << enemys[0].name << "\n2." << enemys[1].name << "\n3." << enemys[2].name << endl;
				cin >> target;
				
				target = target+1;
				
				Attack_Check(enemys[target]);
								
				break;
			}
				
			case SPD_UP:
			{
				SPD += 5;
				SetColor(10); //.h
				cout << "SPD UP +5" << endl;
				break;
			}
				
			case DEF_UP:
			{
				DEF += 5;
				SetColor(10); //.h
				cout << "DEF UP +5" << endl;
				break;
			}
					
			case ATK_UP:
			{
				ATK += 5;
				SetColor(10); //.h
				cout << "ATK UP +5" << endl;
				break;
			}
	
			case Heal:
			{
				HP += 30;
				SetColor(10); //.h
				cout << "Healed" << name << " +30" << endl;
				break;
			}
				
        	default: 
			{
				SetColor(14);
        	    cout << "Invalid choice!" << endl;
			}						
		}
		
		SetColor(7); //.h
    	Sleep(500); //.h
	}
}

void Opposition :: Action(Player &player) {
	
	Display_stats();
    Attack_Check(player);
}

//Game (Non-Class functions) :- Maybe I should make a seperate class for handling game flow?

void Battle(Player players[], Opposition enemies[], Team &opposition_team, Team &player_team) //Where the combat happens
{
	//set enemies based on the team you select
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
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
	while ( (enemies[0].HP > 0 || enemies[1].HP > 0 || enemies[2].HP > 0) && (players[0].HP > 0 || players[1].HP > 0 || players[2].HP > 0) ){
		
		//Players Turn 
		for (int i = 0; i < PLAYERS_PER_TEAM; i++)
		{	players[i].Actions(enemies);	}
	
		players_turn = false;

		//Enemy Turn
		for (int i = 0; i < PLAYERS_PER_TEAM; i++)
		{
			if (enemies[i].HP > 0)
			{	enemies[i].Action(players[i]);	}
		}
		
		players_turn = true;
	}
	
	//Win Loss states
	if (enemies[0].HP <= 0 && enemies[1].HP <= 0 && enemies[2].HP <= 0)  //Win
	{
		SetColor(10); //green for victory	
		cout << "\nPlayers win!\n" << endl;
		opposition_team.losses += 1;
		player_team.wins += 1;	
	}

	if (players[0].HP <= 0 && players[1].HP <= 0 && players[2].HP <= 0) //lose
	{	
		SetColor(12); //Red for defeat
		cout << "\nEnemies win!\n" << endl;	
		opposition_team.wins += 1;
		player_team.losses += 1;
	}
	
	SetColor(7);
}

//CPU Sim function
void Battle_Simulation(Team &team1, Team &team2)
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

//Calander function
void Schedule(Team teams[])
{
	
}

//Draft function
void Draft()
{
	
}

void Config(Entity entitites[], Player players[], Opposition enemies[], Team teams[]) //Where all the the managerial stuff happens
{
	
	int Entitity_ID, player_ID;
	int input, loop = 1; 
	
	enum Actions{Trade = 1, Team_Stats, CPU_SIM, Exit};
	
	//Management Actions
	do{
		cout << "Press 1.To Trade, 2.Check Team Stats 3.Sim CPU Game 4.To Exit" << endl;
		cin >> input;
		
		switch(input)
		{
			//Trade player
			case Trade:
			{
				//for loop displays all npc data
				for (int i = 0; i < NUMBER_OF_TEAMS; i++)
				{
					for (int j = 0; j < PLAYERS_PER_TEAM; j++)
					{
						cout << "ID: " << (j + (i * PLAYERS_PER_TEAM)) + 1 << " ";
						teams[i].party_members[j].Display_stats();	
					}		
				}	
	
	
				//Currently keep player selection through this technique
				cout << "\n" "Enter the ID of the Entity you want to Trade with a player" << endl;
				cin >> Entitity_ID;
			
				for (int i = 0; i < PLAYERS_PER_TEAM; i++)
				{
					cout << "ID: " << i + 1 << " ";
					players[i].Display_stats();				
				}
	
				cout << "Enter ID of the player you want to trade" << endl;
				cin >> player_ID;
				
				//players swap
				players[player_ID - 1].name = entitites[Entitity_ID - 1].name;
				players[player_ID - 1].ATK = entitites[Entitity_ID - 1].ATK;
				players[player_ID - 1].SPD = entitites[Entitity_ID - 1].SPD;
				players[player_ID - 1].DEF = entitites[Entitity_ID - 1].DEF;
				players[player_ID - 1].Team_ID = entitites[Entitity_ID - 1].Team_ID;
				
				break;
			}
			
			//Check team stats
			case Team_Stats:
			{
				for (int i = 0; i < NUMBER_OF_TEAMS; i++)
				{
					SetColor(11);
					cout << "Team: " << teams[i].name << " wins: " << teams[i].wins << " losses: " << teams[i].losses << endl;
					cout << endl;
					SetColor(7); 
				}
				break;
			}
			
			case CPU_SIM:
			{	
				cout << "Select the teams you want to play each other?" << endl;
				Battle_Simulation(teams[1], teams[2]);
				break;
			}
			
			//Exit case
			case Exit:
			{
				loop = 0;
				break;
			}
		}
	} while (loop != 0);
}

//MAIN
int main(){
	
	int team_selection, opp_team;
	char exit;
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//objects
 	Team teams[4];
	Entity profiles[12];
	Player players[3];
	Opposition enemies[3] = {Opposition(), Opposition(), Opposition()};			
	
	//set profiles team IDs
	for (int i = 0; i < NUMBER_OF_TEAMS; i++)
	{
		teams[i].ID = i+1;
		
		for (int j = 0; j < PLAYERS_PER_TEAM; j++)
		{	
			profiles[j + ( i * PLAYERS_PER_TEAM )].Team_ID = i + 1;	
			teams[i].party_members[j] = profiles[j + ( i *PLAYERS_PER_TEAM )];
		}
	}
	
	//Gameloop
	
	//Title Screen
	cout<<"::::::::::: :::::::::  :::::::::       ::::    ::::      :::     ::::    :::     :::      ::::::::  :::::::::: :::::::::"<<endl;
	cout<<"    :+:     :+:    :+: :+:    :+:      +:+:+: :+:+:+   :+: :+:   :+:+:   :+:   :+: :+:   :+:    :+: :+:        :+:    :+:"<<endl;
	cout<<"    +:+     +:+    +:+ +:+    +:+      +:+ +:+:+ +:+  +:+   +:+  :+:+:+  +:+  +:+   +:+  +:+        +:+        +:+    +:+ "<<endl;
	cout<<"    +#+     +#++:++#:  +#++:++#+       +#+  +:+  +#+ +#++:++#++: +#+ +:+ +#+ +#++:++#++: :#:        +#++:++#   +#++:++#:"<<endl;
	cout<<"    +#+     +#+    +#+ +#+             +#+       +#+ +#+     +#+ +#+  +#+#+# +#+     +#+ +#+   +#+# +#+        +#+    +#+"<<endl;
	cout<<"#+# #+#     #+#    #+# #+#             #+#       #+# #+#     #+# #+#   #+#+# #+#     #+# #+#    #+# #+#        #+#    #+#"<<endl;
	cout<<" #####      ###    ### ###             ###       ### ###     ### ###    #### ###     ###  ########  ########## ###    ### "<<endl;
	cout<<"__________________________________________________________________________________________________________________________"<<endl;
	
	
	cout << "Welcome to RPG manager! This is a blend of sport sims and JRPGs. There are 4 teams. Choose your team (1-4)" << endl;
	cin >> team_selection;
			
		//set players based on the team you select (maybe ill make seperate functions for these long loops)
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
	
		cout << "Select Your Opponent (1-4)" << endl;
		cin >> opp_team;
	
		if (opp_team != team_selection)
		{	Battle(players, enemies, teams[opp_team - 1], teams[team_selection - 1]);	}
		
		cout << "Exit (y/n)" << endl;
		cin >> exit;
		
	}while(exit != 'y' || exit != 'Y');
	
	return(0);	
};