#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//JRPG Combat Mechanics

//ADD Turn Manager
//ENUM for switch statement

//function for displaying all stats
int Display_stats(int player_HP, int enemy_HP)
{
	cout << "\nHP: " << player_HP  << "\n" "Enemy HP: " << enemy_HP << endl;		
};

//Attack Action
void Attack(int &HP, int &ATK)
{
	HP -= ATK;
}

int main(){
	
	//variables
	int player_HP = 100, enemy_HP = 50;
	int player_ATK = 20, player_SPD = 10, player_DEF = 10;
	int enemy_ATK = 20, enemy_SPD = 10, enemy_DEF = 10;
	bool players_turn = true;

	int input;
	
	Display_stats(player_HP, enemy_HP);	

	while (enemy_HP > 0){
		
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
					Attack(enemy_HP, player_ATK);
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
				case 2:
				{
					player_SPD += 5;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
				case 3:
				{
					player_DEF += 5;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}
				
				case 4:
				{
					player_ATK += 5;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}

				case 5:
				{
					player_HP += 30;
					Display_stats(player_HP, enemy_HP);
					players_turn = false;
					break;
				}													
			}
			
		}
		
		//Enemy Turn
		if (players_turn != true)
		{
			Attack(player_HP, enemy_ATK);
			Display_stats(player_HP, enemy_HP);
			players_turn = true;
		}
	}
	
	if (enemy_HP <= 0){
		
		cout << "player wins";
		
	}
	
	return(0);	
};