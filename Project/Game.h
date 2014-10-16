#pragma once

#define PLAYER 0
#define ENEMY 1
#define MISSILE 2

class Game
{

private:

	list<Sprite*> sprites;
	vector<Cell<>*> cells;
	Cell<>* stars;

	ofstream fout;
	ifstream fin;

	bool gameIsRunning;
	int frames;
	int score;
	double time;

	int numStars;
	
	clock_t beginTime;
	clock_t endTime;

	int enemyMovementX;
	int enemyMovementY;
	int mapX;
	int mapY;

public:

	Game();
	~Game();

	void Play();
	void Input();
	void Update();
	void Refresh() const;
	void LoadFromFile();
	void SaveToFile();
	void SaveToBinaryFile();
	void ShowHighScores();
	void Menu();
	void Summary();
	double DiffClock(clock_t clock1, clock_t clock2);

};

struct BinaryData
{
	char name[31];
	int score;
	double time;
};
