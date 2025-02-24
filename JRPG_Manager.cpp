#include <iostream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>

//IDEAS: take_damage(), 
 
using namespace std;

//Variables Arrays and constants
string first_names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro", "Luca ", "Raven", "Skyler", "Noa", "Sage", "Shin", "Jin"};
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi", " Yamada", " Miyamoto", " Sakamoto", " Dracula", " Kazama", " Kisaragi", " Lion-Heart" };

string Team_name1[] = {"Midgar", "Zanarkand", "Palmacosta", "Alcamoth", "Tortuga"};
string Team_name2[] = {" Crimson Blades", " Sentinels", " Vanguard", " Covenant", " Syndicate"};

#define NUMBER_OF_TEAMS 4 
#define PLAYERS_PER_TEAM 3

//CLASSES
class Entity{
	protected:
		
		string name;	

		int HP = 50, ATK, SPD, DEF, ID, Team_ID = 1; //for the baseline stats of a given character
		int Cur_HP = HP, Cur_ATK = ATK, Cur_SPD = SPD, Cur_DEF = DEF; //For the stats of a given character doing combat 
		
		bool Turn = true; //I dont know if this is meaningfuly used anymore so I might remove it

    public:

		//Entity Class member functions
		void Display_stats();
		void Attack(Entity &target);
        virtual void Take_Action(Entity& target) = 0;    // Pure virtual
        bool Is_Alive() const { return Cur_HP > 0; }     

		void Read_File();                               //Is this used?
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
        int ID;                         //Is this used?
		int OVR_ATK;
        bool is_player_Controlled = false;

		Entity Entities[PLAYERS_PER_TEAM]; //might switch to a vector to hold members dynamically
        Entity *Entities_Ptr[PLAYERS_PER_TEAM]; //Still to be determined if a pointer should be used or a normal array

    public:
        void Add_Member(Entity* member);
		void Calulate_OVR();	
		void Display_Stats();
		
		Team() //Might set parameters for taking name and is_player_controlled boolean 
		{	team_name = Team_name1[rand() % (sizeof(Team_name1) / sizeof(Team_name1[0]))] + Team_name2[rand() % (sizeof(Team_name2) / sizeof(Team_name2[0]))];	}
};


class BattleManager {
    public:
        static void Battle(Team& playerTeam, Team& enemyTeam);
        static void SimulateBattle(Team& team1, Team& team2);
    };

class GameManager {
    private:

        Team teams;
        int Cur_Day;
    
    public:
        void Start_Game();
        void Run_BattleSchedule();
        void Display_Standings();

        GameManager();
};

class ConsoleManager {
    public:
        static void SetColor(int color);
        static void PrintTitle();
};


//METHODS

void Entity :: Display_stats(){
	
	SetColor(11); // Light cyan for stats
	cout << name << " |HP: " << Cur_HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << " |Team: " << Team_ID << endl;
	SetColor(7); //Reset to white		
};

void Entity :: Attack(Entity &target){
	
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - target.Cur_SPD;   //Check whether the hit lands (based on SPD) 
    
    SetColor(12); //Red for attack messages  
    cout << name << " Attacks " << target.name << endl; 
    Sleep(500); //Small delay for impact
    
    if (target.Cur_HP <= 0) 
    {
        cout << target.name << " has been defeated!\n";
    }
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = max(0, Cur_ATK - target.Cur_DEF);  //Take the difference if its +ve else it gives 0
	
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

void Entity :: generate_character(){        //Replace this outright in constructor?

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

