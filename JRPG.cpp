#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <fstream>

using namespace std;

//JRPG Character generator 
string names[] = {"Rei", "Kai", "Akira", "Shiro", "Kris"};
string classes[] = {"Fighter", "Mage", "Thief" , "Ranger" , "Healer"};

vector<string> backstories = {
        "A mysterious warrior with a hidden past.",
        "A young mage seeking to master the elemental forces.",
        "A thief from a long line of criminals, looking for redemption.",
        "A noble archer with a tragic story of lost family.",
        "A healer on a quest to save a dying kingdom.",
        "A berserker on a path of vengeance against the forces that destroyed their home.",
        "A paladin sworn to protect the innocent from dark forces.",
        "A summoner seeking the ancient powers of lost gods."
};

//class that holds data held by each charcter object
class Character {	
	public:
		string name;	
		string Class;
		string Backstory;
		
		void Display_character();
};


void Character :: Display_character(){
	
	cout << "Name: " << name << endl;
	cout << "Class: " << Class << endl;
	cout << "Backstory: " << Backstory << endl;
};

//function for generating Party Members
void generate_character(){
	
	Character NPC;
	
	NPC.name = names[rand() % (sizeof(names) / sizeof(names[0]))];
	NPC.Class = classes[rand() % (sizeof(classes) / sizeof(classes[0]))];
	NPC.Backstory = backstories[rand() % backstories.size()]; 
	
	NPC.Display_character();
};

int main(){ 
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//console output
	int input;
	
	cout << "Press 1 to generate a characeter" << endl;
	cin >> input;
	
	if (input == 1){
		generate_character();
	}
	
	return (0);
}