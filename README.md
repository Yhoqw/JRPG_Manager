
CS-112 Project Proposal: JRPG Combat Simulation and League Manager
CS-112 Project
Yazdan Ali Khan 2024665, Muhammed Hammad Shahid 2024389 [FEE Section E]


Overview
This project blends the turn based combat of a text based Japanese Role-Playing Game (JRPG) with the strategy and structure of a sports league manager. Built in C++, the simulator showcases core object-oriented programming (OOP) concepts—encapsulation, inheritance, polymorphism, and abstraction. It sets up a competitive environment where AI- and player-controlled teams, made up of unique characters with varying randomly-generated stats, face off in turn-based battles until every team plays each other once after which the top 2 teams face in a final game that decides the winner.
Below are some images of reference projects that we took inspiration from

:-Please dont pay attention to the fancy graphics or images in this picture and instead try to consider how the information is arranged and what kind of information is considered useful by sports sims such as these


Objectives
Recreate turn-based RPG combat using OOP design.


Build a team management system with randomized character stats.


Simulate a full season: scheduled matches, stat tracking, and a final championship.


Enable user input during battles (like attacks, buffs, and healing).


Apply and demonstrate OOP fundamentals through structured class design and modular methods.



Key-Features 
Entity System: At the heart is a base class, Entity, with sub-classes like Player and NPC defining specific behaviors.


Team System: Two main team types—AI-controlled Team and user-driven PlayerTeam—manage character lineups and overall stats.


Battle Manager: Orchestrates combat flow, triggers player and enemy actions, and checks win/loss conditions.


Schedule System: Handles match timing, tracks progression, and ensures seasonal consistency. Will be stored in the Game Manager Class


Game Manager: Ties everything together—manages the season, trading, and the climactic final match.


Console UI: A simple but engaging color-coded console interface using windows.h.



Object Oriented Design
Inheritance: For instance, Player derives from Entity, and PlayerTeam from Team.


Polymorphism: Methods like Actions() and Choose_Target() behave differently depending on which class calls them.


Encapsulation: Attributes are kept secure and accessed through getters and setters.


Abstraction: Complex operations like resolving combat or generating stats are tucked into manager classes, keeping them separate from other systems.

Tools 
Language: C++


Libraries Used: <iostream>, <vector>, <cstdlib>, <ctime>, <windows.h>, <string>


Platform: Windows (due to use of windows.h and SetConsoleTextAttribute for UI coloring)

Flowchart and Diagrams


Conclusion
This project successfully demonstrates how object-oriented programming can be used to model complex, interactive systems like a game simulation. It showcases an integration of core OOP concepts in a creative and engaging way, making it not only functional but also a solid demonstration of real-world application of C++.


