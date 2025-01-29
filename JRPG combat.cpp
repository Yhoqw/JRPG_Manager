#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//Character Class
class Character {
	public:
		string name;	
		int HP = 100, ATK = 20, SPD = 10, DEF = 0;
		bool Turn = true;
};

//function for displaying all stats
void Display_stats(Character NPC1, Character NPC2)
{
	cout << "\n" << NPC1.name << "HP: " << NPC1.HP  << "\n" "Enemy HP: " << NPC2.HP << endl;
	cout << "------------------------------------------------------------------------------" << endl;		
};

//Hit Check function
void Attack_Check(Character &NPC1, Character &NPC2)
{
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - NPC2.SPD; 
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = NPC1.ATK - NPC2.DEF;
	
		//in case damage gives a negative number
		if (damage < 0) {	damage = 0;	}
		
		NPC2.HP -= damage;
			
		//Ensure HP does not go below 0
        if (NPC2.HP < 0) { NPC2.HP = 0; }
        
		cout << NPC1.name << "'s attack hits! Damage dealt: " << damage << endl;
	}
	
	else{	cout << "The attack missed!" << endl;	}
}

//Action input function
void Player_Action(Character& player, Character& enemy){
	
	Display_stats(player, enemy);
	
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
				Attack_Check(player, enemy);
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
	Display_stats(player, enemy);
	//player.Turn = false;
}

//MAIN
int main(){
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//Character objects
	Character player;
	player.name = "player1";
	Character player2;
	player2.name = "player2";
	Character player3;
	player3.name = "player3";
	
	Character enemy;
	enemy.name = "enemy";
	
	//Combat Loop
	while (enemy.HP > 0 && player.HP > 0){
		
		//Players Turn 
		Player_Action(player, enemy);
		Player_Action(player2, enemy);
		Player_Action(player3, enemy);
		
		player.Turn = false;
			
		//Enemy Turn
		if (player.Turn != true && enemy.HP > 0)
		{	
			Attack_Check(enemy, player);
			Display_stats(player, enemy);
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