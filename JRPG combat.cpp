#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//Character Class
class Entity{
	public:
		string name;	
		int HP, ATK, SPD, DEF;
		bool Turn = true;
		
		//Constructor (Ideally the reading file info will be done in the constructor)
		Entity(string n): name(n)
		{
			HP = 100, ATK = 20, SPD = 10, DEF = 10;
		}
		
		void Display_stats();
		void Attack_Check(Entity &target);
};

//Sub-classes of Entity
class Player : public Entity {
	public:
		using Entity::Entity;
};

class Enemy : public Entity {
	public:
		using Entity :: Entity;
};

//Entity Methods
void Entity :: Display_stats()
{
	cout << "\n" << name << "HP: " << HP << endl;
	cout << "------------------------------------------------------------------------------" << endl;		
};

void Entity :: Attack_Check(Entity &target)
{
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - target.SPD; 
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = ATK - target.DEF;
	
		//in case damage gives a negative number (Ternaray operator)
		damage = (damage < 0) ? 0 : damage;
		
		target.HP -= damage;
			
		//Ensure HP does not go below 0
        target.HP = (NPC2.HP < 0) ? 0 : target.HP;
        
		cout << name << "'s attack hits! Damage dealt: " << damage << endl;
	}
	
	else{	cout << "The attack missed!" << endl;	}
}

//Action input function
void Player_Action(Entity& player, Entity& enemy){
	
	player.Display_stats();
	
	int input;
	
	//Define availaible actions
	enum Actions{
		Attack = 1,
		SPD_UP,
		DEF_UP,
		ATK_UP,
		Heal
	};

	if (player.Turn == true)
	{
		cout << "1.Attack \n2.SPD UP \n3.DEF UP \n4.ATK UP \n5.Heal" << endl;
		cin >> input;	
			
		switch (input)
		{
			case Attack:
			{
				player.Attack_Check(enemy);
				break;
			}
				
			case SPD_UP:
			{
				player.SPD += 5;
				break;
			}
				
			case DEF_UP:
			{
				player.DEF += 5;
				break;
			}
					
			case ATK_UP:
			{
				player.ATK += 5;
				break;
			}
	
			case Heal:
			{
				player.HP += 30;
				break;
			}
				
        	default: 
			{
        	    cout << "Invalid choice!" << endl;
			}						
		}
	}
	player.Display_stats();
	//player.Turn = false;
}

//MAIN
int main(){
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//Entity objects
	Player player("Player1");
	Player player2("Player2");
	Player player3("Player3");
	
	Enemy enemy1("enemy");
	Enemy enemy2("enemy2");
	Enemy enemy3("enemy3");
	
	//Combat Loop
	while (enemy.HP > 0){
		
		//Players Turn 
		Player_Action(player, enemy);
		Player_Action(player2, enemy);
		Player_Action(player3, enemy);
		
		player.Turn = false;
			
		//Enemy Turn
		if (player.Turn != true && enemy.HP > 0)
		{	
			enemy.Attack_Check(player);
			enemy.Display_stats();
			player.Turn = true;
		}
	}
	
	//Win Loss states
	if (enemy.HP <= 0)
	{	cout << "player wins";	}
	
	if (player.HP <= 0)
	{	cout << "player losses";}
	
	return(0);	
};