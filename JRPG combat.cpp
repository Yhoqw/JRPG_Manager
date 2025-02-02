#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//Variable Arrays
string first_names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris", "Ren", "Ash", "Taro"};
string last_names[] = {" Van Damme", " Dimitrescu", " Lynx", " Kitagawa", " Crescent", " Reynolds", " Tatsumi"};

//create seperate functions for player actions

//Entity Class
class Entity{
	public:
		
		//Entity Class Attributes/Member variables
		string name;	
		int HP = 100, ATK, SPD, DEF;
		
		bool Turn = true;

		//Entity Class member functions
		void Display_stats();
		void Attack_Check(Entity &target);
		void Read_File();		
		void generate_character();
		
		//Constructor (Ideally the reading file info will be done in the constructor)
		Entity(int hp) : HP(hp)
		{
			HP = 100;	
			generate_character();	
		}
};

//Sub-classes of Entity
class Player : public Entity {
	public:
		using Entity::Entity;
		
		void Actions(Entity& enemy1, Entity& enemy2, Entity& enemy3);
		
};

class Opposition : public Entity {
	public:
		using Entity :: Entity;
		
		//Enemy Sub-Class member functions
		void Action(Player &player);
};

//Entity Methods
void Entity :: Display_stats()
{
	cout << "\n" << name << " |HP: " << HP << ", ATK: " << ATK << ", SPD: " << SPD << ", DEF: " << DEF << endl;
	cout << "------------------------------------------------------------------------------" << endl;		
};

void Entity :: Attack_Check(Entity &target)
{
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - target.SPD; 
    
    cout << name << " Attacks " << target.name << endl; 
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = ATK - target.DEF; //int damage = max(0, ATK - target.DEF); is a proposed solution that could outright circumvent the negatives issue
	
		//in case damage gives a negative number (Ternaray operator)
		damage = (damage < 0) ? 0 : damage;
		
		target.HP -= damage;
			
		//Ensure HP does not go below 0
        target.HP = (target.HP < 0) ? 0 : target.HP;
        
		cout << name << "'s attack hits! Damage dealt: " << damage << endl;
	}
	
	else{	cout << "The attack missed!" << endl;	}
}

//keep this for now but possibly replace this with entity generation for the time being
void Entity :: Read_File()
{
	ifstream infile("NPC_data.txt");
	if (infile)
	{
		getline(infile, name);
		infile >> ATK;
		infile >> SPD;
		infile >> DEF;
	}
	
	infile.close();
} 

void Entity :: generate_character(){

	//Generates a random name 		
	name = first_names[rand() % (sizeof(first_names) / sizeof(first_names[0]))] + last_names[rand() % (sizeof(last_names) / sizeof(last_names[0]))] ;
	
	ATK = (rand() % 20) + 1;
	SPD = (rand() % 10);
	DEF = (rand() % 10);
};

//Action input function
void Player :: Actions(Entity& enemy1, Entity& enemy2, Entity& enemy3) {
	
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
				//give options to choose who to attack
				int target;
				cout << "Select who you wish to attack" "\n1." << enemy1.name << "\n2." << enemy2.name << "\n3." << enemy3.name << endl;
				cin >> target;
				
				//switch statement after selecting who to attack
				switch (target){
					
					case 1: 
						Attack_Check(enemy1);
						break;
					
					case 2:
						Attack_Check(enemy2);
						break;						

					case 3:
						Attack_Check(enemy3);
						break;
						
					default:
						cout << "Invalid selection" << endl; 
				}
				
				break;
			}
				
			case SPD_UP:
			{
				SPD += 5;
				cout << "SPD UP +5" << endl;
				break;
			}
				
			case DEF_UP:
			{
				DEF += 5;
				cout << "DEF UP +5" << endl;
				break;
			}
					
			case ATK_UP:
			{
				ATK += 5;
				cout << "ATK UP +5" << endl;
				break;
			}
	
			case Heal:
			{
				HP += 30;
				cout << "Healed" << name << " +30" << endl;
				break;
			}
				
        	default: 
			{
        	    cout << "Invalid choice!" << endl;
			}						
		}
	}
}

void Opposition::Action(Player &player) {
	
	Display_stats();
    Attack_Check(player);
}

//MAIN
int main(){
	
	int players_turn = true;
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//objects
	Player player1(100);
	Player player2(100);
	Player player3(100);
		
	Opposition enemy1(100);
	Opposition enemy2(100);
	Opposition enemy3(100);	
	
	//Combat Loop
	while (enemy1.HP > 0 || enemy2.HP > 0 || enemy3.HP > 0){
		
		//Players Turn 
		player1.Actions(enemy1, enemy2, enemy3);
		player2.Actions(enemy1, enemy2, enemy3);
		player3.Actions(enemy1, enemy2, enemy3);
		
		players_turn = false;

		//Enemy Turn
		if (players_turn != true && enemy1.HP > 0)
		{	enemy1.Action(player1);	}
		
		if (players_turn != true && enemy2.HP > 0)
		{	enemy2.Action(player2);	}
		
		if (players_turn != true && enemy3.HP > 0)
		{	enemy3.Action(player3);	}
		
		players_turn = true;
	}
	
	//Win Loss states
	if (enemy1.HP <= 0 && enemy2.HP <= 0 && enemy3.HP <= 0)
	{	cout << "player wins";	}
	
	if (player1.HP <= 0 && player2.HP <= 0 && player3.HP <= 0)
	{	cout << "player losses"; }
	
	return(0);	
};