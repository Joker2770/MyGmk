#include "Game.h"
#include <algorithm>
using std::vector;

bool cfg_show_information;

Board emptygameboard = {
	{
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,
		 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  } };

void swap3(Board &board)
{
	for (int i = 0; i < BLSIZE; i++)
		if (board[i])
			board[i] = board[i] % 2 + 1;
}

int judgeWin(Board &board)
{
	for (int i = 0; i < BSIZE; i++)
		for (int j = 0; j < BSIZE; j++)
			if (board(i,j))
			{
				int col = board(i,j);
				for (int d = 0; d < 4; d++)
				{
					int dx = i, dy = j, cnt = 0;
					do
					{
						dx += cx[d]; dy += cy[d];
						cnt++;
					} while (inBorder({ dx,dy }) && board(dx, dy) == col);
					if (cnt >= 5)
						return col;
				}
			}
	return 0;
}


bool Player::is_message(string line)
{
	stringstream ss;
	ss << line;
	string s;
	ss >> s;
	if (s == "MESSAGE" || s == "DEBUG")
	{
		if (cfg_show_information) std::cout << name << ": " << line << endl;
		return 1;
	}
	if (s == "ERROR")
	{
		std::cout << "[ERROR] " << name << " returned " << s << endl;
		exit(0);
	}
	return 0;
}

void Player::reset()
{
	if (cfg_show_information) std::cout << "match: RESTART" << " ->" << name << endl;
	pipe_put << "RESTART" << endl;
	string s; pipe_get >> s;
	if (cfg_show_information) std::cout << name << ": " << s << endl;
	if (s != "OK")
	{
		std::cout << "[Error] program returned " << s << endl;
		exit(0);
	}
}

Coord Player::turn_move(Coord move)
{
	pipe_put << "TURN " << move.x << "," << move.y << endl;
	if (cfg_show_information) std::cout << "match:  TURN " << move.x << "," << move.y << " ->" << name << endl;
	//stringstream ss; string line;
	//std::getline(pipe_get, line);
	//if (cfg_show_information) std::cout << name << ": " << line << endl;
	//ss << line;
	char dot; Coord input;
	pipe_get >> input.x >> dot >> input.y;
	return input;
}

Coord Player::begin()
{
	pipe_put << "BEGIN" << endl;
	if (cfg_show_information) std::cout << "match: BEGIN" << " ->"<< name << endl;
	//stringstream ss; string line;
	//std::getline(pipe_get, line);
	//if (cfg_show_information) std::cout << name << ": " << line << endl;
	//ss << line;
	char dot; Coord input;
	pipe_get >> input.x >> dot >> input.y;
	return input;
}

void Game::make_move(Coord pos)
{
	if (pos.p() == BLSIZE)
		gameboard.swap();
	else
		gameboard(pos) = nowcol;
	history.push_back(pos.p());
	nowcol = nowcol % 2 + 1;
}

void Game::unmake_move()
{
	if (history.back() == BLSIZE)
		gameboard.swap();
	else
		gameboard[history.back()] = 0;
	history.pop_back();
	nowcol = nowcol % 2 + 1;
}

void Game::reset()
{
	gameboard = emptygameboard;
	gamestep = 0;
	nowcol = 1;
	history.clear();
}

void Game::printWinner(int z)
{
	std::cout << std::endl;
	if (z == 1) 
		std::cout << "Black win!";
	else if (z==2) 
		std::cout << "White win!";
	else 
		std::cout << "Draw!";
	std::cout << std::endl;
}

int Game::runGame(Player &player1, Player &player2)
{
	reset();
	player1.reset();
	player2.reset();
	//if (show_mode == 1) print(gameboard);
	while (gamestep < BLSIZE)
	{
		Coord c;
		if (nowcol == 1)
		{
			if (gamestep == 0)
				c = player1.begin();
			else
				c = player1.turn_move(history.back()); 
		}
		else
			c = player2.turn_move(history.back());
		make_move(c);
		//if (show_mode == 1) print(gameboard);
		/*else */if (show_mode == 0) std::cout << c.format() << ' ';
		if (judgeWin(gameboard))
			break;
		gamestep++;
	}
	int winner = nowcol % 2 + 1;
	if (gamestep == BLSIZE) winner = 0;
	printWinner(winner);
	return winner;
}

void Game::match(Player &player1, Player &player2)
{
	float w1 = 0, w2 = 0;
	for (int i = 0; i < match_count; i++)
	{
		std::cout << "game " << i << '\n';
		int ret = runGame(player1, player2);
		if (ret == 1) w1 += 1;
		else if (ret == 0) w1 += 0.5;
	}
	for (int i = 0; i < match_count; i++)
	{
		std::cout << "game " << i << '\n';
		int ret=runGame(player2, player1);
		if (ret == 2) w2 += 1;
		else if (ret == 0) w2 += 0.5;
	}
	std::cout << "black: " << w1 << "  white: " << w2 << endl;

	std::ofstream fresult("match.txt", std::ios::app);
	auto time = std::time(NULL);
	fresult << "\n" << std::ctime(&time) << endl;
	fresult << player1.name + "  vs  " + player2.name << endl;
	fresult << "b " << w1 << "   " << match_count - w1 << endl;
	fresult << "w " << w2 << "   " << match_count - w2 << endl;
	fresult << (w1 + w2) * 100 / match_count / 2 << "%" << endl;
	fresult.close();
}
