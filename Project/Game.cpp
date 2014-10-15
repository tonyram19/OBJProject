#include "stdafx.h"
#include "Game.h"

Game::Game() 
{
	gameIsRunning = true;
	frames = 0;
	score = 0;
	time = 0;
	
	sprites.resize(2);

	enemyMovementX = 1;
	enemyMovementY = 0;

	this->LoadFromFile();
	
	do {
		numStars = rand() % 11 + 10;
	} while (numStars % 2 != 0);

	stars = new Cell[numStars];

	Symbol syms[4] = { '^', '@', '*', 'o' };

	for (int i = 0; i < numStars; i++)
	{
		stars[i].bg = static_cast<ConsoleColor>(rand() % 16);
		stars[i].fg = static_cast<ConsoleColor>(rand() % 16);
		stars[i].oX = rand() % Console::WindowWidth;
		stars[i].oY = rand() % Console::WindowHeight;
		stars[i].sym = syms[rand() % 3];
	}
	
}


Game::~Game()
{
	decltype(sprites.size()) i = 0;
	for (; i < sprites.size(); ++i)
		delete sprites[i];
}

void Game::LoadFromFile()
{
	string buffer;

	fin.open("images.txt");

	if (fin.is_open())
	{

		int i, fg, bg;
		i = 0;

		while (true)
		{
			getline(fin, buffer, ';');
			fin >> fg >> bg;
			fin.ignore(INT_MAX, '\n');

			sprites[i] =
				new Sprite
				(
					(ConsoleColor)(unsigned int) fg,
					(ConsoleColor)(unsigned int) bg,
					buffer,
					"Player",
					Console::WindowWidth / 2,
					Console::WindowHeight / 2
				);
			i++;

			if (fin.eof())
				break;
		}

		sprites[ENEMY]->setTop(3);

		fin.close();
	}

	ShowHighScores();
}

double Game::DiffClock(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1 - clock2;
	double diffms = (diffticks) / CLOCKS_PER_SEC;
	return diffms;
}

void Game::SaveToFile()
{
	ofstream fout("stats.txt", ios_base::app);

	if (fout.is_open())
	{
		fout << sprites[PLAYER]->getName() << "\t";
		fout << score << "\t";
		fout << time << "\n";

		fout.close();
	}

}

void Game::SaveToBinaryFile()
{
	ofstream fout("stats.bin", ios_base::app | ios_base::binary);

	BinaryData data;
	strcpy_s(data.name, 31, sprites[PLAYER]->getName().c_str());
	data.score = this->score;
	data.time = this->time;

	if (fout.is_open())
	{
		fout.write((char*)&data, sizeof(BinaryData));
		fout.close();
	}

}

void Game::ShowHighScores()
{
	ifstream fin;
	BinaryData data;
	vector<BinaryData> vec;

	cout << "HIGH SCORES" << endl << endl;

	//Read text
	fin.open("stats.txt");
	if (fin.is_open())
	{
		cout << "TEXT:" << endl << endl;
		cout << "Name\tScore\tTime\t" << endl << endl;

		while (true)
		{
			
			fin >> data.name;
			fin >> data.score;
			fin >> data.time;
			fin.ignore(INT_MAX, '\n');
			
			if (fin.eof())
				break;
			
			cout << data.name << "\t" << data.score << "\t" << data.time << endl;
		}

		fin.close();
	}

	cout << endl;

	//Read binary
	fin.open("stats.bin", ios_base::binary);
	if (fin.is_open())
	{
		int count;

		fin.seekg(0, ios_base::end);
		count = (int) fin.tellg();
		count = count / sizeof(BinaryData);
		fin.seekg(0, ios_base::beg);
		cout << "BINARY:" << endl << endl;
		cout << "Name\tScore\tTime\t" << endl << endl;

		for (int i = 0; i < count; i++)
		{
			fin.read((char*)&data, sizeof(BinaryData));
			cout << data.name << "\t" << data.score << "\t" << data.time << endl;
		}

		fin.close();
	}

	cout << endl;

	//Read binaryall
	fin.open("stats.bin", ios_base::binary);
	if (fin.is_open())
	{
		int count;

		fin.seekg(0, ios_base::end);
		count = (int)fin.tellg();
		count = count / sizeof(BinaryData);
		fin.seekg(0, ios_base::beg);
		vec.resize(count);

		cout << "BINARY ALL:" << endl << endl;
		cout << "Name\tScore\tTime\t" << endl << endl;

		fin.read((char*)vec.data(), sizeof(BinaryData) * count);

		for (decltype(vec.size()) i = 0; i < vec.size(); i++)
			cout << vec[i].name << "\t" << vec[i].score << "\t" << vec[i].time << endl;

	}

	Util::Pause();
}

void Game::Menu()
{
	Console::Clear();
	string name;
	cout << "What's your name? ";
	getline(cin, name);
	if (name.length() > 0)
		sprites[PLAYER]->setName(name);
}

void Game::Summary()
{
	Console::Clear();
	cout << "Summary" << endl << endl;
	cout << "Name: " << sprites[PLAYER]->getName() << endl;
	cout << "Score: " << score << endl;
	cout << "Time: " << time;
	Util::Pause();
	Util::Pause();
}

void Game::Play()
{
	//Do before game starts
	this->Menu();

	//Start game - Main loop
	Util::EOLWrap(false);
	beginTime = clock();
	while (gameIsRunning)
	{
		Input();
		Update();
		Refresh();
		
		frames++;
	}
	Util::EOLWrap(true);
		
	//Do After game Ends
	Summary();
	SaveToFile();
	SaveToBinaryFile();

}

void Game::Input()
{
	//Quit game
	if (GetAsyncKeyState(VK_ESCAPE))
		gameIsRunning = false;
	
	//Player shooting
	if (GetAsyncKeyState(VK_SPACE))
	{	
		Missile* m = new Missile
		(
			ConsoleColor::Magenta,
			ConsoleColor::Black,
			"*",
			"Missile",
			sprites[PLAYER]->getLeft() + 3,
			sprites[PLAYER]->getTop() - 1,
			0,
			-1
		);
		
		m->Enable();
		sprites.push_back(m);
	}

	int dx = 0;
	int dy = 0;

	if (GetAsyncKeyState('W')) 
		dy = -1;
	if (GetAsyncKeyState('S')) 
		dy = 1;
	if (GetAsyncKeyState('A')) 
		dx = -1;
	if (GetAsyncKeyState('D')) 
		dx = 1;

	if (dx || dy)
	{
		int newx = sprites[PLAYER]->getLeft() + dx;
		int newy = sprites[PLAYER]->getTop() + dy;

		if (!sprites[ENEMY]->Collides(newx, newy, sprites[PLAYER]->getWidth(), sprites[PLAYER]->getHeight())
			&&
			!sprites[PLAYER]->OutOfBounds(newx, newy))
		{
			sprites[PLAYER]->setLeft(newx);
			sprites[PLAYER]->setTop(newy);
		}
	}
			
}

void Game::Update()
{
	//Uodate the enemy
	int newx = sprites[ENEMY]->getLeft() + enemyMovementX;
	int newy = sprites[ENEMY]->getTop() + enemyMovementY;

	if (!sprites[ENEMY]->OutOfBounds(newx, newy) &&
		!sprites[PLAYER]->Collides(newx, newy, sprites[ENEMY]->getWidth(), sprites[ENEMY]->getHeight()))
	{
		sprites[ENEMY]->setLeft(newx);
		sprites[ENEMY]->setTop(newy);
	}
	else
	{
		if (newx >= Console::WindowWidth - sprites[ENEMY]->getWidth())
			enemyMovementX = -1;

		if (newx < 0)
			enemyMovementX = 1;
	}

	//Enemy Shooting
	if (frames % 75 == 0)
	{
		Missile* m = new Missile
			(
			ConsoleColor::Red,
			ConsoleColor::Black,
			"*",
			"Missile",
			sprites[ENEMY]->getLeft() + 3,
			sprites[ENEMY]->getTop() + 3,
			0,
			1
			);

		m->Enable();
		sprites.push_back(m);
	}

	//Update the missiles
	decltype(sprites.size()) i;
	for (i = 2; i < sprites.size(); i++)
	{
		Missile* m = dynamic_cast<Missile*>(sprites[i]);

		if (m->isEnabled())
		{
			Missile::Delta dx;
			Missile::Delta dy;
			m->GetDeltas(dx, dy);

			newx = m->getLeft() + dx;
			newy = m->getTop() + dy;

			
			//Check missile-enemy collision
			if (m->Collides(sprites[ENEMY]->getLeft(), sprites[ENEMY]->getTop(), sprites[ENEMY]->getWidth(), sprites[ENEMY]->getHeight()))
			{
				delete sprites[i];
				sprites.erase(sprites.begin() + i--);
				score++;
				continue;
			}

			
			//Check missile-player collision
			sprites[PLAYER]->setFg(ConsoleColor::Cyan);
			if (m->Collides(sprites[PLAYER]->getLeft(), sprites[PLAYER]->getTop(), sprites[PLAYER]->getWidth(), sprites[PLAYER]->getHeight()))
			{
				delete sprites[i];
				sprites.erase(sprites.begin() + i--); 
				sprites[PLAYER]->setFg(ConsoleColor::Red);
				continue;
			}


			//Check missile out of bounds collision
			if (m->OutOfBounds(newx, newy))
			{
				delete sprites[i];
				sprites.erase(sprites.begin() + i--); 
				continue;
			}

			//Update missile position
			m->setLeft(newx);
			m->setTop(newy);
		}
	}

	//Update time
	endTime = clock();
	time = DiffClock(endTime, beginTime);
}


void Game::Refresh() const
{
	LockWindowUpdate(GetConsoleWindow());
	Console::Clear();
	
	//Print player's name
	Console::ForegroundColor = ConsoleColor::Cyan;
	cout << sprites[PLAYER]->getName() << "\t\t\t\t\t";

	//Print score
	Console::ForegroundColor = ConsoleColor::Yellow;
	cout << "Score: " << score << "\t\t\t";
	
	//Print time
	Console::ForegroundColor = ConsoleColor::Magenta;
	cout << "Time " << time;

	//Print stars
	for (int i = 0; i < numStars; i++)
		stars[i].Show(0, 0);
	//Print sprites
	decltype(sprites.size()) i;
	for (i = 0; i < sprites.size(); i++)
		sprites[i]->Show();
	
	LockWindowUpdate(NULL);

	System::Threading::Thread::Sleep(15);
}





