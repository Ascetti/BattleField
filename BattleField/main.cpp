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

	Init(window);
	
	PageLayout(Page, window, Game);

	if (SettingsData.Music != 0)
		PlayMusic(BackgroundMusic, SettingsData.Volume);

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
			MainMenu(mode, Page, window, SettingsData, previous);
			break;
		case 2:
			ExitMenu(mode, Page, window, SettingsData);
			break;
		case 3:
			Rules(mode, Page, window, SettingsData);
			break;
		case 4:
			Ending(mode, Page, window, SettingsData, previous);
			break;
		case 5:
			CheckPoint(mode, Page, window, SettingsData, previous);
			break;
		case 6:
			Identification(mode, Page, window, SettingsData, GameProgress);
			break;
		case 7:
			Play(mode, window, Game, GameProgress, Page, SettingsData, previous);
			break;
		}
	}


	DeInit(0);

	return 0;
}