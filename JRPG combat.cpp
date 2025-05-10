#include "JRPG.h"

//Entity Methods
void Entity::Display_Stats(){
	
	SetColor(11); // Light cyan for stats
	cout << name << " |HP: " << Cur_HP << "/" << HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << endl;
	SetColor(7); //Reset to white		
};

void Entity::generate_character(){

	//Generates a random name 	
	name = first_names[rand() % (sizeof(first_names) / sizeof(first_names[0]))] + last_names[rand() % (sizeof(last_names) / sizeof(last_names[0]))] ;
	
	ATK = (rand() % 20) + 1;
	SPD = (rand() % 10);
	DEF = (rand() % 10);
	
};

void Entity::set_HP(int new_HP) {
	Cur_HP = new_HP;
	Is_Alive = (Cur_HP > 0);
}

void Entity::Reset_Stats(){		//Defaults the stats
	
	set_HP(HP);
	Cur_ATK = ATK;
	Cur_SPD = SPD;
	Cur_DEF = DEF;
	Is_Alive = true; 
}
										
void Entity::Actions(){

	if (!get_Is_Alive())	{	return;	}	//Skip turn if dead	
	
	else
	{	
		Display_Stats();

    	if (Opposing_TeamPtr) 
		{
			PlayerTeam *enemy_team = static_cast<PlayerTeam*>(Opposing_TeamPtr);
    	    Choose_Target(*enemy_team);
    	} 
			
		else 
		{
    	    cout << "No opposing team assigned! Turn wasted.\n";
    	}
	}
}

void Entity::Attack_Check(Entity &Target){
	
	//Check whether the hit lands (based on SPD)
    int Hit_chance = rand() % 100;
    int Hit_threshold = 100 - Target.get_SPD(); 
    
    SetColor(12); //Red for attack messages  
    cout << name << " Attacks " << Target.get_Name() << endl; 
	Sleep(200); //Reduced delay for impact
    
    //Damage calculation
    if (Hit_chance < Hit_threshold)
	{
		int damage = max(0,Cur_ATK - Target.get_Cur_DEF()); //in case damage gives a negative number

		Target.set_HP(Target.get_Cur_HP() - damage);
			
        Target.set_HP( (Target.get_Cur_HP() < 0) ? 0 : Target.get_Cur_HP() ); //Ensure HP does not go below 0
        
		cout << name << "'s attack HITS! [Damage dealt: " << damage << "]\n" << endl;
		PlaySoundEffect("attack");
	}
	
	else
	{	
		SetColor(14); //Yellow for misses
		cout << "The attack MISSED!" << endl;	
		PlaySoundEffect("miss");
	}
	
	SetColor(7); //White
	Sleep(200);
}

void Entity::Choose_Target(PlayerTeam &enemy_Team){
	
	int target = rand() % (PLAYERS_PER_TEAM);

	if (target >= 0 && target < PLAYERS_PER_TEAM) 
	{   Attack_Check(enemy_Team.Members[target]);	} 
	
	else 
	{	cout << "Invalid target! Turn wasted.\n";	}
}

void Entity::Choose_Target(Team &enemy_Team) {		//Isnt used anywhere anymore but it does make CPU vs CPU possible if I allow that in again
    int target = rand() % (PLAYERS_PER_TEAM);
    
    if (target >= 0 && target < PLAYERS_PER_TEAM) 
    {
        Attack_Check(enemy_Team.Members[target]);
    } 

    else 
    {	cout << "Invalid target! Turn wasted.\n";	}
}

bool Entity::get_Is_Alive() const {
    return Cur_HP > 0; 
}

//Player Methods
void Player::Display_Stats() {
	
    SetColor(10); // Green for player stats (to distinguish them)
    cout << name << " |HP: " << Cur_HP << "/" << HP << ", ATK: " << Cur_ATK << ", SPD: " << Cur_SPD << ", DEF: " << Cur_DEF << endl;
    SetColor(7); //Reset to white
}

void Player::Actions(){														//Calls all the available actions for the Entity
	
	#ifdef DEBUG
	cout << "\nPlayer::Actions() operational\n" << endl;
	#endif	
	
	if (Cur_HP <= 0)
	{	return;	} 		//Skip turn if dead	
	
	Display_Stats();
	int input;
	
	//Define availaible actions
	enum Actions
	{	Attack = 1, SPD_UP, DEF_UP, ATK_UP, Heal	};	
	
	ConsoleManager::PrintMenu();

	cin >> input;	

	switch (input)
	{
		case Attack:		
		{	
            if (Opposing_TeamPtr)				 
			{
                Team* enemy_team = static_cast<Team*>(Opposing_TeamPtr);
                Choose_Target(*enemy_team);
			} 
			
			else 
			{
                cout << "No opposing team assigned! Turn wasted.\n";
            }
			break;
		}
				
        case SPD_UP:
            Change_Stat(Cur_SPD, 5, "SPD");
            break;

        case DEF_UP:
            Change_Stat(Cur_DEF, 5, "DEF");
            break;

        case ATK_UP:
            Change_Stat(Cur_ATK, 5, "ATK");
            break;

        case Heal:
            Change_Stat(Cur_HP, 20, "HP");
            break;

		
        default:
		{
			SetColor(14);
            cout << "Invalid choice! Turn Wasted.\n" << endl;
            break;
		}
	}
		
		SetColor(7); 
    	Sleep(500); 
}

void Player::Choose_Target(Team &enemy_Team){								//Called in Actions() it selects the target and calls Attck_check()
	
	#ifdef DEBUG
	cout << "\nPlayer::Choose_target Operational\n" << endl;
	#endif	
	
    cout << "Select a target (Enter an Integer 1-3):\n";
    
	enemy_Team.Display_Members();

	int target;
	cin >> target;
       
    if (target >= 1 && target <= PLAYERS_PER_TEAM	) 
	{	Attack_Check(enemy_Team.Members[target - 1]);	} //-1 because the player will input playerIDs starting from 1
	
	else 
	{	cout << "Invalid target! Turn wasted.\n";	}
}

void Player::Change_Stat(int &stat, int amount, const string statName) {
	
    stat += amount;
    SetColor(10);
    cout << statName << " UP +" << amount << "\n" << endl;
	PlaySoundEffect("buff");
    SetColor(7);
}

void Player::set_Player_Name(){

	cin.ignore();
	cout << "\nInput Name: " << endl;
	string temp_name;
	getline(cin, temp_name);

	cout << name << " is now called " << temp_name << endl;
	name = temp_name;
}

//Team Methods
void Team::Run_For_All_Members(void (Entity::*method)()) {
	
	Run_For_All(Members, PLAYERS_PER_TEAM, method);
}

void Team::Display_Members(){
	
	SetColor(3); //Blue
	cout << name << ": " << endl;
	SetColor(7); //White
	
	Run_For_All_Members(&Entity::Display_Stats);

    cout << endl;
};

int Team::get_OVR(){
	
	OVR_ATK = 0; // Reset before summing
	
	for (int i = 0; i < PLAYERS_PER_TEAM; i++)
	{	OVR_ATK += Members[i].get_ATK();	}
		  
	return (OVR_ATK /= PLAYERS_PER_TEAM);
}

void Team::Display_stats(){

	SetColor(3); //Cyan
	cout << "Team: " << name << " |wins: " << wins << " |losses: " << losses << endl;
	SetColor(7); //White
	cout << endl;

}

void Team::reset_Stats(){
	wins = 0;
	losses = 0;
}

bool Team::Has_Members_Alive() {
    for (int i = 0; i < PLAYERS_PER_TEAM; i++) 
	{
        if (Members[i].get_Is_Alive()) {return true; } // At least one member is alive
    }
    return false; 
}

//Player Team Methods
void PlayerTeam::Display_Members() {
	
    SetColor(3); // Blue
    cout << name << ": " << endl;
    SetColor(7); // White

	for (int i = 0; i < PLAYERS_PER_TEAM; i++) 
	{
        Members[i].Display_Stats();
	}

}

void PlayerTeam::Display_stats() {

	SetColor(6); //Yellow
	cout << "Team: " << name << " |wins: " << wins << " |losses: " << losses << endl;
	SetColor(7); //White
	cout << endl;	
}

void PlayerTeam::set_Member_Name() {
	
	short id;
	cout << "Select Which Characters name you wish to change (1-3)" << endl;
	Display_Members();
	cin >> id;

	if (id <= 0 || id > PLAYERS_PER_TEAM)
	{
		cout << "Invalid Input! Enter a number between 1-4" << endl;
		return;
	}

	Members[id-1].set_Player_Name();
}

void PlayerTeam::set_Team_Name() {

	cin.ignore();
	cout << "\nInput Name: " << endl;
	string temp_name;
	getline(cin, temp_name);

	cout << name << " is now called " << temp_name << endl;
	name = temp_name;
}

bool PlayerTeam::Has_Members_Alive(){
    for (int i = 0; i < PLAYERS_PER_TEAM; i++) 
	{
        if (Members[i].get_Is_Alive()) {return true; } // At least one member is alive
    }
    return false; 
}

//Battle Manager Functions

void BattleManager::PC_Battle(PlayerTeam &PC_Team, Team &Opposing_Team){		//Player Controlled Team Battles
	
	#ifdef DEBUG 
	cout << "\nPC Battle Operational\n" << endl;
	#endif
	
	ConsoleManager::ClearScreen();

    for (int i = 0; i < PLAYERS_PER_TEAM; i++) // Correctly assigns pointers for each member and Resets stats
	{
		PC_Team.Members[i].Reset_Stats();
		Opposing_Team.Members[i].Reset_Stats();

        PC_Team.Members[i].Opposing_TeamPtr = &Opposing_Team;
        Opposing_Team.Members[i].Opposing_TeamPtr = &PC_Team;
    }
	
    // Display both teams before battle starts
    cout << "Your team:" << endl;
    PC_Team.Display_Members();
    
    cout << "Opposing team:" << endl;
    Opposing_Team.Display_Members();	
	
	// Main battle loop
	cout << "\n--- BATTLE BEGINS! ---\n";
    Sleep(1000);
	
	while ( PC_Team.Has_Members_Alive() && Opposing_Team.Has_Members_Alive() )
	{
		//Players Turn 
		for (int i = 0; i < PLAYERS_PER_TEAM; i++)	
		{
			PC_Team.Members[i].Actions();
		}
	
		//Enemy Turn
		for (int i = 0; i < PLAYERS_PER_TEAM; i++)
		{
			Opposing_Team.Members[i].Actions();
		}
		
        // Show the current state after each round
        Display_Round_Summary(PC_Team, Opposing_Team);
	}
	
	//Win Loss states
	if (!Opposing_Team.Has_Members_Alive())  //Win
	{
		Declare_Winner(PC_Team, Opposing_Team);	
	}

	else if (!PC_Team.Has_Members_Alive()) //lose
	{	
		Declare_Winner(Opposing_Team, PC_Team);	
	}
	
	SetColor(7);
	
}

void BattleManager::Battle_Simulation(Team &team1, Team &team2) {				//Battle Simulations

	#ifdef DEBUG
	cout << "\nBattle_Simulation operational\n" << endl;
	#endif

    team1.get_OVR(); 
    team2.get_OVR();
    
    int victory_Threshold = 50 + (team1.get_OVR() - team2.get_OVR());
    int team1_Victory = rand() % 100;

    if (team1_Victory > victory_Threshold) 
	{	Declare_Winner(team1, team2);	} //Team1 Victory
	else
	{	Declare_Winner(team2, team1);	} //Team2 Victory
}

void BattleManager::Declare_Winner(Team &winner, Team &loser){					//Print Winning Team message
	
	SetColor(10); // Green 
    cout << "\n" << winner.get_Name() << " win!\n" << endl;
    SetColor(7); //Reset to white	

    winner.add_Win();
    loser.add_Loss();
}

void BattleManager::Display_Round_Summary(PlayerTeam &PC_Team, Team &Opposing_Team) {
    
    cout << "\n--- Current Battle Status ---" << endl;
    cout << "Your team:" << endl;
	PC_Team.Display_Members();
    
	cout << "\n Opposing team:" << endl;
    Opposing_Team.Display_Members();
    Sleep(2000); 
}

//Schedule Methods 
void Schedule::Matchup(){												//Cycles through matchups to Select Matchups
	
	#ifdef DEBUG 
	cout << "\nMatchup operational\n" << endl;
	#endif
	
    if (Day >= TOTAL_MATCHES ) //stop after playing all games in the season
	{
        cout << "Season is over! No more battles" << endl;
        return;
    }
    
    else if (Day == 0) 		//Before the season begins
    {
    	Day++;
    	return;
	}
	
    int teamA = matchups[Day][0];
    int teamB = matchups[Day][1];

    cout << "\nDay " << (Day) << ": "; 
    
    if (teamA == 4) 					// If teamA is the PlayerTeam
	{  
        cout << playerTeam->get_Name() << " vs " << teams[teamB].get_Name() << "\n";
		Sleep(1000);
        BattleManager::PC_Battle(*playerTeam, teams[teamB]);
    } 
    else if (teamB == 4) 				// If teamB is the PlayerTeam
	{ 	
        cout << teams[teamA].get_Name() << " vs " << playerTeam->get_Name() << "\n";
        BattleManager::PC_Battle(*playerTeam, teams[teamA]);
    } 
    else 
	{									//Games with all NPC teams		
        cout << teams[teamA].get_Name() << " vs " << teams[teamB].get_Name() << "\n";
        BattleManager::Battle_Simulation(teams[teamA], teams[teamB]);
    }
	
    Day++;	
}

void Schedule::Print_Schedule(){

	cout << "The Schedule is:\n " << endl;

	SetColor(5); //Pruple

	for (int i = 1; i < TOTAL_MATCHES; i++)
	{
		cout << "Day " << (i + 1) << ": " << teams[matchups[i][0]].get_Name() << " vs " << teams[matchups[i][1]].get_Name() << endl;
	}

	SetColor(7);

	cout << endl;
}

//Game Manager Methods
void GameManager::Display_Roster() {
	cout << "=========|Teams|===========" << endl;
	Run_For_All_Teams(&Team::Display_Members); PC_Team.Display_Members();
}

void GameManager::Run_For_All_Teams(void (Team::*method)()) {
	
	Run_For_All(NPC_Teams, NUMBER_OF_TEAMS, method);
}

void GameManager::Swap_Entities(int teamA_id, int playerA_id, int teamB_id, int playerB_id) {	
	
	if (teamA_id == 5 || teamB_id == 5)
	{
		cout << "You cannot trade with your own team" << endl;
		return;
	}

    else if (teamA_id >= NUMBER_OF_TEAMS || teamB_id >= NUMBER_OF_TEAMS || playerA_id >= PLAYERS_PER_TEAM || playerB_id >= PLAYERS_PER_TEAM) //Error
	{
        cout << "Invalid indices. Swap failed" << endl;
        return;
    }
	
	//Actual Swap
    Entity temp = NPC_Teams[teamA_id].Members[playerA_id];

    NPC_Teams[teamA_id].Members[playerA_id] = NPC_Teams[teamB_id].Members[playerB_id];
    NPC_Teams[teamB_id].Members[playerB_id] = temp;


	SetColor(6); //Yellow
    cout << "Traded " << NPC_Teams[teamA_id].Members[playerA_id].get_Name() << " for " << NPC_Teams[teamB_id].Members[playerB_id].get_Name() << endl;
	SetColor(7); //Reset to white
}

void GameManager::Select_Team_For_Trade(int Teamid){			//Just exists to make the trade function cleaner

	if (Teamid == 5)
	{	PC_Team.Display_Members();	}

	else 
	{	NPC_Teams[Teamid-1].Display_Members();	}
}

void GameManager::Trade(){											//Chooses the parameters for swaping entities (Accessed from Management Mode) 
	
	#ifdef DEBUG
	cout << "\nTrade Operational\n" << endl;
	#endif

	cout << "------------------------------------------------------" << endl;

	int teamA_id, playerA_id, teamB_id, playerB_id;
	
	cout << "Choose the team you wnat to trade a player from " << endl;
	cin >> teamA_id;
	Select_Team_For_Trade(teamA_id);
	
	cout << "Choose the player ID you want to trade " << endl;
	cin >> playerA_id;
	
	cout << "Choose the team you want to trade a player to " << endl;
	cin >> teamB_id;
	Select_Team_For_Trade(teamB_id);
	
	cout << "Choose the player ID you want to trade " << endl;
	cin >> playerB_id;	
	
	Swap_Entities(teamA_id-1, playerA_id-1, teamB_id-1, playerB_id-1);
}

void GameManager::Display_Standings(){

	#ifdef DEBUG
	cout << "\nDisplay Standings operational\n" << endl;
	#endif	
	
	ConsoleManager::ClearScreen();
	
	cout << "Standings (Wins/Losses): " << endl;
	cout << "==================================" << endl;
	
	Run_For_All_Teams(&Team::Display_stats);
	PC_Team.Display_stats();
}

void GameManager::Run_Game(){											//Check numbers of days remaining in the season then cycle through basic game loop until final
	
	while ( season.get_Days() < TOTAL_MATCHES )
	{	
		season.Matchup();
		Management_Mode();
	}	
	
	Display_Standings();
	Final();
}

void GameManager::Management_Mode(){									//Options in between matches and match simulations
	
	int input, loop = 1; 
	enum Actions{SIM_DAY = 5, STANDINGS = 2, ROSTERS = 1, TRADE = 4, SCHEDULE = 3, CHANGE_CHARACTER_NAME = 6, CHANGE_TEAM_NAME = 7};
		
	do{
		ConsoleManager::PrintManagementMenu();
		cin >> input;
		
		switch(input)			//They say nobody uses switch anymore but I have no idea what the alternative is 
		{				
			case SIM_DAY:
			{
				loop = 0;
				break;
			}
			
			case STANDINGS:
			{	
				ConsoleManager::ClearScreen();
				Display_Standings();
				break;
			}
				
			case ROSTERS:
			{	
				ConsoleManager::ClearScreen();
				Display_Roster();
				break;
			}
			
			case TRADE:
			{
				ConsoleManager::ClearScreen();
				Display_Roster();
				Trade();
				break;
			}

			case SCHEDULE:
			{	
				ConsoleManager::ClearScreen();
				season.Print_Schedule();
				break;
			}

			case CHANGE_CHARACTER_NAME:
			{
				ConsoleManager::ClearScreen();
				PC_Team.set_Member_Name();
				break;
			}

			case CHANGE_TEAM_NAME:
			{
				ConsoleManager::ClearScreen();
				PC_Team.set_Team_Name();
				break;
			}
			
			default:
				break;
		}
	} while (loop != 0);	
}

void GameManager::Final(){
	
	#ifdef DEBUG
	cout << "\nFinals operational\n" << endl;
	#endif	
	
	cout << "Finals!" << endl;
	
	//Code for the final logic
	int Finalist1_index = 0;
	int Finalist2_index = 1;
	
	for (int i = 0; i < NUMBER_OF_TEAMS; i++) //To set the finalists 
	{
		if ( NPC_Teams[i].get_Wins() > NPC_Teams[Finalist1_index].get_Wins() )
			{	Finalist1_index = i;	}
		
		else if(NPC_Teams[i].get_Wins() > NPC_Teams[Finalist2_index].get_Wins() && Finalist1_index != i)
			{	Finalist2_index = i;	}
	} 
	
    cout << NPC_Teams[Finalist1_index].get_Name() << " vs " << NPC_Teams[Finalist2_index].get_Name() << "\n" << endl;
    Sleep(1000); 	
	
	BattleManager::Battle_Simulation(NPC_Teams[Finalist1_index], NPC_Teams[Finalist2_index]);
}

//Console Manager Functions	
void ConsoleManager::PrintTitle(){

	SetColor(6); //Yellow
	cout<<"::::::::::: :::::::::  :::::::::       ::::    ::::      :::     ::::    :::     :::      ::::::::  :::::::::: :::::::::"<<endl;
	cout<<"    :+:     :+:    :+: :+:    :+:      +:+:+: :+:+:+   :+: :+:   :+:+:   :+:   :+: :+:   :+:    :+: :+:        :+:    :+:"<<endl;
	cout<<"    +:+     +:+    +:+ +:+    +:+      +:+ +:+:+ +:+  +:+   +:+  :+:+:+  +:+  +:+   +:+  +:+        +:+        +:+    +:+ "<<endl;
	cout<<"    +#+     +#++:++#:  +#++:++#+       +#+  +:+  +#+ +#++:++#++: +#+ +:+ +#+ +#++:++#++: :#:        +#++:++#   +#++:++#:"<<endl;
	cout<<"    +#+     +#+    +#+ +#+             +#+       +#+ +#+     +#+ +#+  +#+#+# +#+     +#+ +#+   +#+# +#+        +#+    +#+"<<endl;
	cout<<"#+# #+#     #+#    #+# #+#             #+#       #+# #+#     #+# #+#   #+#+# #+#     #+# #+#    #+# #+#        #+#    #+#"<<endl;
	cout<<" #####      ###    ### ###             ###       ### ###     ### ###    #### ###     ###  ########  ########## ###    ### "<<endl;
	SetColor(7); //Reset to white
	cout<<"__________________________________________________________________________________________________________________________"<<endl;		
	cout << "Welcome to RPG manager! This is a blend of sport sims and JRPGs. This program currently generates 4 Teams and 1 player team they will play each other in simulations of JRPG style \ncombat (3 vs 3 combat where one team makes all its moves in one turn then the CPU makes its moves) by the end of the season, 2 teams will play a final to pick a winner. " << endl;
}

void ConsoleManager::PrintMenu() {
    cout << "=========================" << endl;
    cout << "       Action MENU       " << endl;
    cout << "=========================" << endl;
    cout << "1. Attack" << endl;
    cout << "2. SPD UP" << endl;
    cout << "3. DEF UP" << endl;
    cout << "4. ATK UP" << endl;
    cout << "5. Heal" << endl;
    cout << "=========================" << endl;
}

void ConsoleManager::ClearScreen() 
{
    system("cls");
}

void ConsoleManager::PrintManagementMenu(){
    cout << "====================================" << endl;
    cout << "       	Management MENU       " << endl;
    cout << "====================================" << endl;
	cout << "1. Show Rosters" << endl;
    cout << "2. Show Standings (Wins/Losses)" << endl;
    cout << "3. Show Schedule" << endl;
    cout << "4. Make Trades" << endl;
	cout << "5. Simulate to next Day" << endl;
	cout << "6. Change Players Names" << endl;
	cout << "7. Change Teams Name" << endl;
    cout << "====================================" << endl;
	
}

//MAIN
int main(){
	
	#ifdef DEBUG
	cout << "\ndebugging operational\n" << endl;
	#endif
	
	GameManager GM;
	srand(static_cast<unsigned int>(time(0)));	//for seeding
		
	ConsoleManager::PrintTitle(); 
	
	GM.Run_Game();
	
	return(0);	
};
