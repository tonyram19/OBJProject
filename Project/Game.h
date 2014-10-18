#pragma once

#define PLAYER 0
#define ENEMY 1
#define MISSILE 2

#define FLAG_ALIVE 1
#define FLAG_GHOST 2
#define FLAG_AIMOVE 3
#define FLAG_AISHOOT 4

struct BinaryData
{
	char name[31];
	int score;
	double time;
};


class Game
{

private:

	list<Sprite*> sprites;
	vector<Cell<>*> cells;
	Cell<>* stars;
	vector<BinaryData> highScores;

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

	char flags;


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

