#include "stdafx.h"
#include "Game.h"

//Global sorts
bool SortScoreDescending(const BinaryData& a, const BinaryData& b)
{
	return a.score > b.score;
}

bool SortTimeDescending(const BinaryData& a, const BinaryData& b)
{
	return a.time > b.time;
}

bool SortNameAscending(const BinaryData& a, const BinaryData& b)
{
	return strcmp(a.name, b.name) < 0;
}

bool SortNameDescending(const BinaryData& a, const BinaryData& b)
{
	return strcmp(a.name, b.name) > 0;
}


Game::Game() 
{
	Console::WindowWidth = 110;
	Console::WindowHeight = 60;

	gameIsRunning = true;
	frames = 0;
	score = 0;
	time = 0;

	enemyMovementX = 0;
	enemyMovementY = 1;
	mapX = 0;
	mapY = 0;

	flags = 1;

	flags |= 1 << FLAG_ALIVE;
	flags |= 1 << FLAG_AIMOVE;
	flags |= 1 << FLAG_AISHOOT;

	this->LoadFromFile();
	
	do {
		numStars = rand() % 11 + 10;
	} while (numStars % 2 != 0);

	stars = new Cell<>[numStars];

	signed char syms[4] = { '^', '@', '*', 'o' };

	for (int i = 0; i < numStars; i++)
	{
		stars[i].bg = static_cast<ConsoleColor>(rand() % 16);
		stars[i].fg = static_cast<ConsoleColor>(rand() % 16);
		stars[i].oX = rand() % Console::WindowWidth;
		stars[i].oY = rand() % Console::WindowHeight;
		stars[i].sym = syms[rand() % 3];
	}

	
#if 0
	const Cell<long double, unsigned long long> testCell;
	cout << testCell[-1];
	Util::Pause();
#elif 0
	typedef DList<char> OurDList;
	OurDList testDList;
	testDList.PushBack('#');
	testDList.PushBack('@');
	testDList.Clear();
	testDList.PushBack('i');
	testDList.PushFront('r');
	testDList.PushFront('h');
	testDList.PushBack('s');
	testDList.PushFront('t');
	testDList.Erase(2);
	testDList.Erase(0);
	testDList.Erase(2);
	for (int i = 0; i < testDList.Size(); i++)
		cout << testDList[i];
	testDList.Erase(0);
	testDList.Erase(0);
	testDList.PushFront('!');
	testDList.PushBack('\"');
	testDList.Erase(1);
	for (int i = 0; i < testDList.Size(); i++)
		cout << testDList[i];
	cout << flush << endl;
	system("pause");

#endif

}


Game::~Game()
{
	decltype(cells.size()) i = 0;
	for (; i < cells.size(); ++i)
		delete cells[i];

	auto iter = sprites.begin();
	while (iter != sprites.end())
	{
		delete *iter;
		++iter;
	}
}


void Game::LoadFromFile()
{
	int count;

	//Cells
	fin.open("cells.txt");
	if (fin.is_open())
	{
		fin >> count;
		fin.ignore(INT_MAX, '\n');
		cells.resize(count);

		for (decltype(cells.size()) i = 0; i < cells.size(); i++)
		{
			int x, y;
			int sym;
			int fg, bg;

			fin >> x;
			fin >> y;
			fin >> fg;
			fin >> bg;
			fin >> sym;
			
			fin.ignore(INT_MAX, '\n');

			cells[i] = new Cell<>(x, y, (ConsoleColor) fg, (ConsoleColor) bg, (wchar_t) sym);

			if (fin.eof())
				break;
		}

		fin.close();
	}

	string buffer;

	//Sprites images
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

			sprites.push_back(
				new Sprite
				(
					(ConsoleColor) fg, 
					(ConsoleColor) bg, 
					buffer,
					"Player", 
					5, 
					Console::WindowHeight / 2
				)
			);

			i++;

			if (fin.eof())
				break;
		}

		auto iter = sprites.begin();
		iter++; 
		Sprite* enemy = dynamic_cast<Sprite*>(*iter);
		enemy->setTop(3);
		enemy->setLeft(Console::WindowWidth - enemy->getWidth() - 2);

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

	Console::Clear();
	
	string filename;
	int howManyScores;
	
	cout << "Specify a file name to save the high scores: ";
	cin >> filename;

	cout << "How many scores do you want to save? ";
	cin >> oct >> howManyScores >> dec;

	if (howManyScores > (int) highScores.size())
		howManyScores = highScores.size();

	if (filename == "")
		filename = "HighScores";

	fout.open(filename + ".txt");

	if (fout.is_open())
	{
		for (int i = 0; i < howManyScores - 1; i++)
		{
			fout << highScores[i].name << "\t";
			fout << highScores[i].score << "\t";
			fout << highScores[i].time << "\n";	
		}

		auto iter = sprites.begin();
		Sprite* player = dynamic_cast<Sprite*>(*iter);
		fout << player->getName() << "\t";
		fout << score << "\t";
		fout << time << "\n";

		fout.close();
	}

}

void Game::SaveToBinaryFile()
{
	ofstream fout("stats.bin", ios_base::app | ios_base::binary);

	BinaryData data;
	auto iter = sprites.begin();
	Sprite* player = dynamic_cast<Sprite*>(*iter);
	strcpy_s(data.name, 31, player->getName().c_str());
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
	//BinaryData data;
	cout << "HIGH SCORES" << endl << endl;

#if 0
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
		count = (int)fin.tellg();
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
#endif 

	//Read binary All
	fin.open("stats.bin", ios_base::binary);
	if (fin.is_open())
	{
		int count;

		fin.seekg(0, ios_base::end);
		count = (int) fin.tellg();
		count = count / sizeof(BinaryData);
		fin.seekg(0, ios_base::beg);
		highScores.resize(count);

		cout << "Name\tScore\tTime\t" << endl << endl;

		if (count > 0)
			fin.read((char*)&highScores[0], sizeof(BinaryData) * count);

		decltype(highScores.size()) i;
		
		//Sort by name ascending
		cout << "BY NAME ASCENDING" << endl << endl;
		sort(highScores.begin(), highScores.end(), SortNameAscending);
		for (i = 0; i < highScores.size(); i++)
			cout << highScores[i].name << "\t" << highScores[i].score << "\t" << highScores[i].time << endl;
		cout << endl;

		//Sort by name descending
		cout << "BY NAME DESCENDING" << endl << endl;
		sort(highScores.begin(), highScores.end(), SortNameDescending);
		for (i = 0; i < highScores.size(); i++)
			cout << highScores[i].name << "\t" << highScores[i].score << "\t" << highScores[i].time << endl;
		cout << endl;

		//Sort by time descending
		cout << "BY TIME DESCENDING" << endl << endl;
		sort(highScores.begin(), highScores.end(), SortTimeDescending);
		for (i = 0; i < highScores.size(); i++)
			cout << highScores[i].name << "\t" << highScores[i].score << "\t" << highScores[i].time << endl;
		cout << endl;

		//Sort by score ascending
		cout << "BY SCORE DESCENDING" << endl << endl;
		sort(highScores.begin(), highScores.end(), SortScoreDescending);
		for (i = 0; i < highScores.size(); i++)
			cout << highScores[i].name << "\t" << highScores[i].score << "\t" << highScores[i].time << endl;

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
	{
		auto iter = sprites.begin();
		Sprite* player = dynamic_cast<Sprite*>(*iter);
		player->setName(name);
	}
}

void Game::Summary()
{
	auto iter = sprites.begin();
	Sprite* player = dynamic_cast<Sprite*>(*iter);
	Console::Clear();
	cout << "Summary" << endl << endl;
	cout << "Name: " << player->getName() << endl;
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
	while (flags & (1 << FLAG_ALIVE))
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
	auto iter = sprites.begin();
	Sprite* player = dynamic_cast<Sprite*>(*iter);
	iter++;
	Sprite* enemy = dynamic_cast<Sprite*>(*iter);

	//Toggle flags
	if (GetAsyncKeyState(VK_F1))
		flags ^= 1 << FLAG_AIMOVE;

	if (GetAsyncKeyState(VK_F2))
		flags ^= 1 << FLAG_AISHOOT;
	
	if (GetAsyncKeyState(VK_F3))
		flags ^= 1 << FLAG_GHOST;

	//Quit game
	if (GetAsyncKeyState(VK_ESCAPE))
		flags&= ~(1 << FLAG_ALIVE);
	
	//Player shooting
	if (GetAsyncKeyState(VK_SPACE))
	{	
		Missile* m = new Missile
		(
			ConsoleColor::Magenta,
			ConsoleColor::Black,
			"*",
			"Missile",
			player->getLeft() + 6,
			player->getTop() + 3,
			1,
			0
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
	{
		dx = -1;
		mapX +=2;
	}

	if (GetAsyncKeyState('D'))
	{
		dx = 1;
		mapX -=2;
	}
	
	//Update player
	if (dx || dy)
	{
		int newx = player->getLeft() + dx;
		int newy = player->getTop() + dy;

		if (!enemy->Collides(newx, newy, player->getWidth(), player->getHeight())
			&&
			!player->OutOfBounds(newx, newy))
		{
			if (newx <= Console::WindowWidth / 2 - 15)
				player->setLeft(newx);

			player->setTop(newy);
		}
	}
			
}

void Game::Update()
{
	auto iter = sprites.begin();
	Sprite* player = dynamic_cast<Sprite*>(*iter);
	iter++;
	Sprite* enemy = dynamic_cast<Sprite*>(*iter);

	int newx = enemy->getLeft() + enemyMovementX;
	int newy = enemy->getTop() + enemyMovementY;

	//Update the enemy
	if (flags & (1 << FLAG_AIMOVE))
	{
		
		if (!enemy->OutOfBounds(newx, newy) &&
			!player->Collides(newx, newy, enemy->getWidth(), enemy->getHeight()))
		{
			enemy->setLeft(newx);
			enemy->setTop(newy);
		}
		else
		{
			if (newy >= Console::WindowHeight - enemy->getHeight())
				enemyMovementY = -1;

			if (newy <= 0)
				enemyMovementY = 1;
		}
	}

	if (flags & (1 << FLAG_AISHOOT))
	{
		//Enemy Shooting
		if (frames % (rand() % 15 + 5) == 0)
		{
			Missile* m = new Missile
			(
				ConsoleColor::Red,
				ConsoleColor::Black,
				"*",
				"Missile",
				enemy->getLeft() - 4,
				enemy->getTop() + 1,
				-1,
				0
			);

			m->Enable();
			sprites.push_back(m);
		}
	}

	//Update the missiles
	iter = sprites.begin();
	iter++;
	iter++;
	Missile* m;
	while (iter != sprites.end())
	{
		m = dynamic_cast<Missile*>(*iter);

		Missile::Delta dx;
		Missile::Delta dy;
		m->GetDeltas(dx, dy);

		newx = m->getLeft() + dx;
		newy = m->getTop() + dy;


		//Check missile-enemy collision
		if (m->Collides(enemy->getLeft(), enemy->getTop(), enemy->getWidth(), enemy->getHeight()))
		{
			delete *iter;
			sprites.erase(iter--);
			score++;
			++iter;
			PlaySound((LPCTSTR)SND_ALIAS_SYSTEMASTERISK, NULL, SND_ALIAS_ID | SND_NOSTOP|  SND_ASYNC);
			continue;
		}


		//Check missile-player collision
		if (!(flags & (1 << FLAG_GHOST)))
		{
			player->setFg(ConsoleColor::Cyan);
			if (m->Collides(player->getLeft(), player->getTop(), player->getWidth(), player->getHeight()))
			{
				delete *iter;
				sprites.erase(iter--);
				player->setFg(ConsoleColor::Red);
				++iter;
				score -= 10;
				PlaySound((LPCTSTR)SND_ALIAS_SYSTEMEXCLAMATION, NULL, SND_ALIAS_ID | SND_NOSTOP | SND_ASYNC);
				continue;
			}
		}


		//Check missile out of bounds collision
		if (m->OutOfBounds(newx, newy))
		{
			delete *iter;
			sprites.erase(iter--);
			++iter;
			continue;
		}

		//Update missile position
		m->setLeft(newx);
		m->setTop(newy);
		
		++iter;
	}

	//Update time
	endTime = clock();
	time = DiffClock(endTime, beginTime);
}


void Game::Refresh() const
{
	LockWindowUpdate(GetConsoleWindow());
	Console::Clear();
	
	auto iter = sprites.begin();
	Sprite* player = dynamic_cast<Sprite*>(*iter);

	//Print stars
	for (int i = 0; i < numStars; i++)
		stars[i].Show(0 + mapX, 0 + mapY);

	//Print player's name
	Console::SetCursorPosition(0, 0);
	Console::ForegroundColor = ConsoleColor::Cyan;
	cout << player->getName() << "\t\t\t\t\t\t";

	//Print score
	Console::ForegroundColor = ConsoleColor::Yellow;
	cout << "Score: " << hex << score << dec << "\t\t\t\t\t";
	
	//Print time
	Console::ForegroundColor = ConsoleColor::Magenta;
	cout << "Time " << time;

	//Print sprites
	Sprite* sprite;
	while (iter != sprites.end())
	{
		sprite = dynamic_cast<Sprite*>(*iter);
		sprite->Show();
		iter++;
	}

	//Print flags
	Console::SetCursorPosition(Console::WindowWidth >> 1, Console::WindowHeight - 1);
	for (int i = 7; i > 0; i--)
	{ 
		if (flags & (1 << i))
			cout << 1;
		else
			cout << 0;
	}
	
	//Print cells
	for (decltype(cells.size())i = 0; i < cells.size(); i++)
		cells[i]->Show(Console::WindowWidth / 2 + mapX, Console::WindowHeight / 2);
	
	LockWindowUpdate(NULL);

	System::Threading::Thread::Sleep(15);
}