#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>
 
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//Variables Arrays and constants
string first_names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro", "Luca ", "Raven", "Skyler", "Noa", "Sage", "Shin", "Jin"};
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi", " Lion-Heart" };

string Team_name1[] = {"Midgar", "Zanarkand", "Palmacosta", "Alcamoth", "Tortuga"};
string Team_name2[] = {" Crimson Blades", " Sentinels", " Vanguard", " Covenant", " Syndicate"};

const int NUMBER_OF_TEAMS = 4; 
const int PLAYERS_PER_TEAM = 3;

//CLASSES
class Entity{
	protected:
		string name;	

		int HP = 50, ATK, SPD, DEF, ID, Team_ID = 1; //for the baseline stats of a given character
		int Cur_HP = HP, Cur_ATK = ATK, Cur_SPD = SPD, Cur_DEF = DEF; //For the stats of a given character doing combat 
		
    public:

		//Entity Class member functions
		void Display_stats();
		void Attack(Entity &target);
        virtual void Take_Action(Entity& target) = 0;    // Pure virtual
        bool Is_Alive() const { return Cur_HP > 0; }     
		
		//Setters and getters
		void Set_HP(int &HP);
		void Get_HP(int HP);
		
		void Set_SPD(int &SPD);
		void Get_SPD(int SPD);
		
		void Set_ATK(int &ATK);
		void Get_ATK(int ATK);
					
		void Set_DEF(int &DEF);
		void Get_DEF(int DEF);
						
		void Set_Stat(int &stat);
		void Get_Stat(int stat);
						
		void generate_character();
		void Reset_Stats();                             //Set the Cur_Stats same as the base line stats
		
		//Constructor
		Entity()
        {
			generate_character();	
			Reset_Stats();
		}
};

class Player : public Entity{
    public:
        using Entity::Entity;
        
        void Take_Action(Entity& target) override; // Player class overrides virtual function for taking action
};

class Opposition : public Entity{
	public:
		using Entity :: Entity;
		
		void Take_Action(Entity& target) override; // Opposition class overides the virtual function and 'AI' driven actions are taken
};

class Team{
	private:
		string team_name;
		short wins = 0, losses = 0;
		int OVR;
        bool is_player_Controlled = false;

		Entity *NPCs[PLAYERS_PER_TEAM];
		
    public:
		void Calulate_OVR();	
		void Display_Stats();
		
		Team() 
		{	
			team_name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];	
//			Calculate_OVR();
		}
};


class GameManager {
    private:
        Team NPC_Teams[NUMBER_OF_TEAMS];
        Team PC_Team;
};

class ConsoleManager {
    public:
        static void PrintTitle();
};


//METHODS
void ConsoleManager :: PrintTitle(){
	
	cout<<"::::::::::: :::::::::  :::::::::       ::::    ::::      :::     ::::    :::     :::      ::::::::  :::::::::: :::::::::"<<endl;
	cout<<"    :+:     :+:    :+: :+:    :+:      +:+:+: :+:+:+   :+: :+:   :+:+:   :+:   :+: :+:   :+:    :+: :+:        :+:    :+:"<<endl;
	cout<<"    +:+     +:+    +:+ +:+    +:+      +:+ +:+:+ +:+  +:+   +:+  :+:+:+  +:+  +:+   +:+  +:+        +:+        +:+    +:+ "<<endl;
	cout<<"    +#+     +#++:++#:  +#++:++#+       +#+  +:+  +#+ +#++:++#++: +#+ +:+ +#+ +#++:++#++: :#:        +#++:++#   +#++:++#:"<<endl;
	cout<<"    +#+     +#+    +#+ +#+             +#+       +#+ +#+     +#+ +#+  +#+#+# +#+     +#+ +#+   +#+# +#+        +#+    +#+"<<endl;
	cout<<"#+# #+#     #+#    #+# #+#             #+#       #+# #+#     #+# #+#   #+#+# #+#     #+# #+#    #+# #+#        #+#    #+#"<<endl;
	cout<<" #####      ###    ### ###             ###       ### ###     ### ###    #### ###     ###  ########  ########## ###    ### "<<endl;
	cout<<"__________________________________________________________________________________________________________________________"<<endl;		
}	


void Entity :: Display_stats(){
	
	cout << name << " |HP: " << Cur_HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << " |Team: " << Team_ID << endl;
	
};

void Team :: Calulate_OVR() {
		 
	OVR /= PLAYERS_PER_TEAM; 
}

int main()
{
	GameManager Manager;
	srand(static_cast<unsigned int>(time(0)));	//for seeding
	
	ConsoleManager :: PrintTitle();	
	cout << "Welcome to RPG manager! This is a blend of sport sims and JRPGs. This program currently generates 4 Teams they will play each in simulations of JRPG style \ncombat by the end of the season 2 teams will play a final to pick a winner" << endl;
}



