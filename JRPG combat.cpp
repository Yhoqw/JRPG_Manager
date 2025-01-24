#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//JRPG Combat Mechanics

//ADD Turn Manager
//implement SPD and DEF in attack calculation

//function for displaying all stats
void Display_stats(int player_HP, int enemy_HP)
{
	cout << "\nHP: " << player_HP  << "\n" "Enemy HP: " << enemy_HP << endl;		
};

//Attack Action
void Attack_Check(int &HP, int ATK, int DEF)
{
	int damage = ATK - DEF;
	
	//in case damage gives a negative number
	if (damage < 0)
	{
		damage = 0;
	}
	
	HP -= damage;
}

int main(){
	
	//variables
	int player_HP = 100, enemy_HP = 50;
	int player_ATK = 20, player_SPD = 10, player_DEF = 0;
	int enemy_ATK = 20, enemy_SPD = 10, enemy_DEF = 0;
	bool players_turn = true;

	int input;
	
	Display_stats(player_HP, enemy_HP);	

	//player Actions
	enum Actions{
		Attack = 1,
		SPD_UP,
		DEF_UP,
		ATK_UP,
		Heal
	};

	while (enemy_HP > 0 && player_HP > 0){
		
		//Players Turn
		if (players_turn == true)
		{
			cout << "1.Attack \n2.SPD UP \n3.DEF UP \n4.ATK UP \n5.Heal" << endl;
			cin >> input;	
			
			//player Actions 
			switch (input)
			{
				case Attack:
				{
					Attack_Check(enemy_HP, player_ATK, enemy_DEF);
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
				case SPD_UP:
				{
					player_SPD += 5;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
				case DEF_UP:
				{
					player_DEF += 5;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
				case ATK_UP:
				{
					player_ATK += 5;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}

				case Heal:
				{
					player_HP += 30;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
                default: 
				{
                    cout << "Invalid choice! Try again." << endl;
                    break;
            	}
																
			}
			
		}
		
		//Enemy Turn
		if (players_turn != true && enemy_HP > 0)
		{	
			Attack_Check(player_HP, enemy_ATK, player_DEF);
			Display_stats(player_HP, enemy_HP);
			players_turn = true;
		}
	}
	
	if (enemy_HP <= 0)
	{	cout << "player wins";	}
	
	if (player_HP <= 0)
	{	cout << "player losses";}
	
	return(0);	
};