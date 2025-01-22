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

//class that holds data held by each character object
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
Character generate_character(){
	
	Character NPC;
	
	NPC.name = names[rand() % (sizeof(names) / sizeof(names[0]))];
	NPC.Class = classes[rand() % (sizeof(classes) / sizeof(classes[0]))];
	NPC.Backstory = backstories[rand() % backstories.size()]; 
	
	NPC.Display_character();
	
	return (NPC);
};

// Function to save NPC details to a file
void save_NPCs(Character NPC) {
    ofstream outfile("NPC_data.txt", ios::app); // Open file in append mode

    if (outfile) 
	{
        outfile << "Name: " << NPC.name << endl;
        outfile << "Class: " << NPC.Class << endl;
        outfile << "Backstory: " << NPC.Backstory << endl;
        outfile << "---------------------------" << endl;
        outfile.close();
        cout << "NPC details saved successfully!" << endl;
    }
	 
	else 
	{
        cout << "Error: Unable to open file for writing." << endl;
    }
};

int main(){ 
	
	//for seeding
	srand(static_cast<unsigned int>(time(0)));
	
	//console output
	int input;
	
	cout << "Press 1 to generate a character" << endl;
	cin >> input;
	
	if (input == 1)
	{
		//Generate 20 NPCs
		for (int i = 1; i <= 20; i++)
		{
			Character new_NPC = generate_character();
			save_NPCs(new_NPC);
		}
	}
	
	return (0);
}
