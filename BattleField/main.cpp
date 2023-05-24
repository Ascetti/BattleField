#include "general.h"
#include "GameCore.h"

int main(int argc, char** argv)
{
	system("chcp 1251");

	Proportions window;
	Appearance Page;
	Zones Game;
	Elements GameProgress;
	Control SettingsData;

	ReadGameProgressDataFile(GameProgress);
	ReadSettingsFile(SettingsData);

	if (GameProgress.GameStatus == 0)
	{
		strcpy_s(GameProgress.Gambler1, "Player 1");
		strcpy_s(GameProgress.Gambler2, "Player 2");
	}

	Init(window);

	/*cout << GameProgress.GameStatus << endl;
	cout << GameProgress.Round << endl;
	cout << GameProgress.OutcomeText << endl;
	cout << GameProgress.FinalText << endl;
	cout << GameProgress.p1 << endl;
	cout << GameProgress.p2 << endl;
	cout << GameProgress.Queue << endl;
	cout << GameProgress.Gambler1 << endl;
	cout << GameProgress.Gambler2 << endl;
	cout << GameProgress.Combo1 << endl;
	cout << GameProgress.Combo2 << endl;
	cout << GameProgress.ThrowMax << endl;
	cout << GameProgress.LeadSwap << endl;
	cout << GameProgress.LeadThrows << endl;
	for (int i = 0; i < dq; i++)
		cout << GameProgress.DiceValues[i] << " ";*/
	
	PageLayout(Page, window, Game);

	int mode = 1;
	bool previous;
	if (GameProgress.GameStatus != 0)
	{
		previous = true;
	}
	else
	{
		previous = false;
	}


	while (mode != 0)
	{
		switch (mode)
		{
		case 1:
			MainMenu(mode, Page, window, previous);
			break;
		case 2:
			ExitMenu(mode, Page, window);
			break;
		case 3:
			Rules(mode, Page, window);
			break;
		case 4:
			Settings(mode, Page, window);
			break;
		case 5:
			CheckPoint(mode, Page, window, previous);
			break;
		case 6:
			Identification(mode, Page, window, GameProgress.Gambler1, GameProgress.Gambler2);
			break;
		case 7:
			Play(mode, window, Game, GameProgress, Page, previous);
			break;
		}
	}


	DeInit(0);

	return 0;
}