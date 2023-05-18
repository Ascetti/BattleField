#include "general.h"

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
Mix_Chunk* sound = NULL;
Mix_Music* fon = NULL;


void Init(Proportions &window)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "Couldn't init SDL! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	int res;
	if ((res = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) == 0)
	{
		cout << "Couldn't init SDL_Image! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	if (res & IMG_INIT_PNG)
		cout << "Initialized PNG library.\n";
	else
		cout << "Couldn't init PNG library.\n";
	if (res & IMG_INIT_JPG)
		cout << "Initialized JPG library.\n";
	else
		cout << "Couldn't init JPG library.\n";

	if (TTF_Init())
	{
		cout << "Couldn't init SDL_TTF! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		cout << "SDL_GetDesktopDisplayMode failed: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	window.width = dm.w;
	window.height = dm.h;

	win = SDL_CreateWindow(u8"Че?!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window.width, window.width, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	if (win == NULL)
	{
		cout << "Couldn't create Window! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL)
	{
		cout << "Couldn't create Renderer! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	Mix_Init(0);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
}

void DeInit(int error)
{
	Mix_CloseAudio();
	Mix_Quit();

	if (ren != NULL)
		SDL_DestroyRenderer(ren);

	if (win != NULL)
		SDL_DestroyWindow(win);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

void work()
{
	Proportions window;
	Appearance Page;
	Zones Game;
	Elements GameProgress;
	Control SettingsData;

	ReadGameProgressDataFile(GameProgress);

	PrintGameProgressDataFile(GameProgress);

	if (GameProgress.GameStatus == 0)
	{
		strcpy_s(GameProgress.Gambler1, "Player 1");
		strcpy_s(GameProgress.Gambler1, "Player 2");
	}

	Init(window);

	cout << GameProgress.GameStatus << endl;
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
	for (int i = 0; i < dq; i++)
		cout << GameProgress.DiceValues[i]  << " ";


	PageLayout(Page, window, Game);

	int mode = 1; 
	//bool flag = false;
	while (mode != 0)
	{
		switch (mode)
		{
		case 1:
			MainMenu(mode, Page, window);
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
			//PauseMenu(mode, Page, window);
			break;
		case 6:
			Identification(mode, Page, window, GameProgress.Gambler1, GameProgress.Gambler1);
			break;
		case 7:
			Play(mode, window, Game, GameProgress, Page);
			break;
		}
	}

	
	DeInit(0);
}

void MainMenu(int& mode, Appearance Page, Proportions window)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"Главное меню";
	SDL_Rect heading_rect;

	SDL_Texture* button_first = LoadTextureFromFile("images\\play.png");
	SDL_Texture* button_second = LoadTextureFromFile("images\\gamerules.png");
	SDL_Texture* button_third = LoadTextureFromFile("images\\settings.png");
	SDL_Texture* button_fourth = LoadTextureFromFile("images\\gameexit.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0});

	SDL_Event event;
	bool quit = false;
	int tap = 0;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.FirstTypeButton, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					/*Page.FirstTypeButton = { Page.FirstTypeButton.x + cv / 2, Page.FirstTypeButton.y + cv / 2, Page.FirstTypeButton.w - cv, Page.FirstTypeButton.h - cv };
					tap = 1;*/
					quit = true;
					mode = 6;
				}
				if (ButtonClick(Page.SecondTypeButton, event.button.x, event.button.y))
				{
					/*Page.SecondTypeButton = { Page.SecondTypeButton.x + cv / 2, Page.SecondTypeButton.y + cv / 2, Page.SecondTypeButton.w - cv, Page.SecondTypeButton.h - cv };
					tap = 2;*/
					/*quit = true;
					mode = 3;*/
					Rules(mode, Page, window);
					if (mode == 0)
					{
						quit = true;
						mode = 0;						
					}
				}					
				if (ButtonClick(Page.ThirdTypeButton, event.button.x, event.button.y))
				{
					/*Page.ThirdTypeButton = { Page.ThirdTypeButton.x + cv / 2, Page.ThirdTypeButton.y + cv / 2, Page.ThirdTypeButton.w - cv, Page.ThirdTypeButton.h - cv };
					tap = 3;*/
					/*quit = true;
					mode = 4;*/
					Settings(mode, Page, window);
					if (mode == 0)
					{
						mode = 0;
						quit = true;
					}
				}
				if (ButtonClick(Page.FourthTypeButton, event.button.x, event.button.y))
				{
					/*Page.FourthTypeButton = { Page.FourthTypeButton.x + cv / 2, Page.FourthTypeButton.y + cv / 2, Page.FourthTypeButton.w - cv, Page.FourthTypeButton.h - cv };
					tap = 4;*/
					quit = true;
					mode = 2;
				}
			}
			break;
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);
		
		/*SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
		SDL_RenderDrawRect(ren, &Page.StepBack);*/

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width , Page.UnderlineIndent + i);
		
		SDL_RenderCopy(ren, button_first, NULL, &Page.FirstTypeButton);
		SDL_RenderCopy(ren, button_second, NULL, &Page.SecondTypeButton);
		SDL_RenderCopy(ren, button_third, NULL, &Page.ThirdTypeButton);
		SDL_RenderCopy(ren, button_fourth, NULL, &Page.FourthTypeButton);
		heading_rect.x = window.width / 2 - heading_rect.w / 2;
		heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);

		SDL_RenderPresent(ren);

		/*if (tap == 1)
			Page.FirstTypeButton = { Page.FirstTypeButton.x - cv / 2, Page.FirstTypeButton.y - cv / 2, Page.FirstTypeButton.w + cv, Page.FirstTypeButton.h + cv };
		if (tap == 2)
			Page.SecondTypeButton = { Page.SecondTypeButton.x - cv / 2, Page.SecondTypeButton.y - cv / 2, Page.SecondTypeButton.w + cv, Page.SecondTypeButton.h + cv };
		if (tap == 3)
			Page.ThirdTypeButton = { Page.ThirdTypeButton.x - cv / 2, Page.ThirdTypeButton.y - cv / 2, Page.ThirdTypeButton.w + cv, Page.ThirdTypeButton.h + cv };
		if (tap == 4)
			Page.FourthTypeButton = { Page.FourthTypeButton.x - cv / 2, Page.FourthTypeButton.y - cv / 2, Page.FourthTypeButton.w + cv, Page.FourthTypeButton.h + cv };*/
	}

	

	SDL_DestroyTexture(button_first);
	SDL_DestroyTexture(button_second);
	SDL_DestroyTexture(button_third);
	SDL_DestroyTexture(button_fourth);
	SDL_DestroyTexture(heading);

	TTF_CloseFont(Franklin);
}

void ExitMenu(int& mode, Appearance Page, Proportions window)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	int TextFontSize = window.height / 12;
	TTF_Font* SanFrancisco = TTF_OpenFont("fonts\\San Francisco.ttf", TextFontSize);
	if (SanFrancisco == NULL)
	{
		cout << "Couldn't open TTF: San Francisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}


	char theme[] = u8"Выход из игры";
	char message[] = u8"Вы действительно хотите выйти из игры?";
	SDL_Rect heading_rect;
	SDL_Rect message_rect;

	SDL_Texture* button_fifth = LoadTextureFromFile("images\\exit.png");
	SDL_Texture* button_sixth = LoadTextureFromFile("images\\back.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* announcement = GenerateTextureFromText(message, SanFrancisco, &message_rect, { 255, 255, 255, 0 });


	SDL_Event event;
	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.FifthTypeButton, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
					mode = 0;
				}
				if (ButtonClick(Page.SixthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					mode = 1;
				}
				break;
			}

		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		/*SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
		SDL_RenderDrawRect(ren, &Page.StepBack);*/

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		heading_rect.x = window.width / 2 - heading_rect.w / 2;
		heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
		message_rect.x = window.width / 2 - message_rect.w / 2;
		message_rect.y = Page.FifthTypeButton.y / 2 + Page.UnderlineIndent + ut - message_rect.h / 2;
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, announcement, NULL, &message_rect);
		SDL_RenderCopy(ren, button_fifth, NULL, &Page.FifthTypeButton);
		SDL_RenderCopy(ren, button_sixth, NULL, &Page.SixthTypeButton);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(heading);
	SDL_DestroyTexture(announcement);
	SDL_DestroyTexture(button_fifth);
	SDL_DestroyTexture(button_sixth);

	TTF_CloseFont(SanFrancisco);
	TTF_CloseFont(Franklin);
}

void Rules(int& mode, Appearance Page, Proportions window)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	
	char theme[] = u8"Правила игры";
	SDL_Rect heading_rect;
	SDL_Rect rules_rect = { cv, Page.UnderlineIndent + ut + cv, (window.height - Page.UnderlineIndent - ut - cv * 2) / 9 * 16, window.height - Page.UnderlineIndent - ut - cv * 2 };
	
	SDL_Texture* stepback = LoadTextureFromFile("images\\stepback.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* rules = LoadTextureFromFile("images\\rules.png");

	SDL_Event event;
	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.StepBack, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
				}
				break;
			}

		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		heading_rect.x = window.width / 2 - heading_rect.w / 2;
		heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, stepback, NULL, &Page.StepBack);
		SDL_RenderCopy(ren, rules, NULL, &rules_rect);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(rules);
	SDL_DestroyTexture(stepback);
	SDL_DestroyTexture(heading);

	TTF_CloseFont(Franklin);
} 

void Settings(int& mode, Appearance Page, Proportions window)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"Настройки";
	SDL_Rect heading_rect;

	SDL_Texture* stepback = LoadTextureFromFile("images\\stepback.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });

	SDL_Event event;
	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.StepBack, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
				}
				break;
			}

		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		heading_rect.x = window.width / 2 - heading_rect.w / 2;
		heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, stepback, NULL, &Page.StepBack);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(stepback);
	SDL_DestroyTexture(heading);

	TTF_CloseFont(Franklin);
}

void PauseMenu(int& mode, Appearance Page, Proportions window)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\San Francisco.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: sanfrancisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"Меню паузы";
	SDL_Rect heading_rect;

	SDL_Texture* button_first = LoadTextureFromFile("images\\continue.png");
	SDL_Texture* button_second = LoadTextureFromFile("images\\settings.png");
	SDL_Texture* button_third = LoadTextureFromFile("images\\gamerules.png");
	SDL_Texture* button_fourth = LoadTextureFromFile("images\\menuexit.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });


	SDL_Event event;
	bool quit = false;
	int tap = 0;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.FirstTypeButton, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
				}
				if (ButtonClick(Page.SecondTypeButton, event.button.x, event.button.y))
				{
					Settings(mode, Page, window);
					if (mode == 0)
					{
						quit = true;
						//mode = 0;
					}
				}
				if (ButtonClick(Page.ThirdTypeButton, event.button.x, event.button.y))
				{
					/*quit = true;
					mode = 3;*/
					Rules(mode, Page, window);
					if (mode == 0)
					{
						quit = true;
						//mode = 0;
					}
				}
				if (ButtonClick(Page.FourthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					mode = 1;
				}
				break;
			}

		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		SDL_RenderCopy(ren, button_first, NULL, &Page.FirstTypeButton);
		SDL_RenderCopy(ren, button_second, NULL, &Page.SecondTypeButton);
		SDL_RenderCopy(ren, button_third, NULL, &Page.ThirdTypeButton);
		SDL_RenderCopy(ren, button_fourth, NULL, &Page.FourthTypeButton);
		heading_rect.x = window.width / 2 - heading_rect.w / 2;
		heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(button_first);
	SDL_DestroyTexture(button_second);
	SDL_DestroyTexture(button_third);
	SDL_DestroyTexture(button_fourth);
	SDL_DestroyTexture(heading);

	TTF_CloseFont(Franklin);
}

void Identification(int& mode, Appearance Page, Proportions window, char* gambler1, char* gambler2)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	int TextFontSize = window.height / 20;
	TTF_Font* SanFrancisco = TTF_OpenFont("fonts\\San Francisco.ttf", TextFontSize);
	if (SanFrancisco == NULL)
	{
		cout << "Couldn't open TTF: San Francisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"Имена игроков";
	char message[] = u8"Введите имена игроков:";
	char warning1[] = u8"В случае пропуска этого этапа идентификации";
	char warning2[] = u8"игроки получат имена по умолчанию: Player 1 и Player 2";
	char user1[] = u8"1 Игрок";
	char user2[] = u8"2 Игрок";
	//char currentname1[] = "Player 1";
	//char currentname2[] = "Player 2";
	SDL_Rect heading_rect;
	SDL_Rect nameinput_rect;
	SDL_Rect notice1_rect;
	SDL_Rect notice2_rect;
	SDL_Rect player1_rect;
	SDL_Rect player2_rect;

	SDL_Texture* stepback = LoadTextureFromFile("images\\stepback.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* skip = LoadTextureFromFile("images\\skip.png");
	SDL_Texture* nameinput = GenerateTextureFromText(message, SanFrancisco, &nameinput_rect, {255, 255, 255, 0});
	SDL_Texture* notice1 = GenerateTextureFromText(warning1, SanFrancisco, &notice1_rect, { 255, 255, 255, 0 });
	SDL_Texture* notice2 = GenerateTextureFromText(warning2, SanFrancisco, &notice2_rect, { 255, 255, 255, 0 });
	SDL_Texture* player1 = GenerateTextureFromText(user1, SanFrancisco, &player1_rect, { r, g, b, 0 });
	SDL_Texture* player2 = GenerateTextureFromText(user2, SanFrancisco, &player2_rect, { r, g, b, 0 });

	SDL_Event event;
	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.SeventhTypeButton, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
					mode = 7;
				}
				if (ButtonClick(Page.StepBack, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
					mode = 1;
				}
				break;
			}

		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		heading_rect.x = window.width / 2 - heading_rect.w / 2;
		heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
		nameinput_rect.x = Page.UnderlineIndent / 2;
		nameinput_rect.y = Page.UnderlineIndent * 5 / 3 - nameinput_rect.h / 2;
		notice1_rect.x = Page.UnderlineIndent / 2;
		notice1_rect.y = window.height - Page.UnderlineIndent - notice1_rect.h;
		notice2_rect.x = Page.UnderlineIndent / 2;
		notice2_rect.y = window.height - Page.UnderlineIndent;
		player1_rect.x = Page.UnderlineIndent / 2;
		player1_rect.y = Page.UnderlineIndent * 3;
		player2_rect.x = Page.UnderlineIndent / 2;
		player2_rect.y = Page.UnderlineIndent * 4 ;


		SDL_RenderCopy(ren, player1, NULL, &player1_rect);
		SDL_RenderCopy(ren, player2, NULL, &player2_rect);
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, nameinput, NULL, &nameinput_rect);
		SDL_RenderCopy(ren, notice1, NULL, &notice1_rect);
		SDL_RenderCopy(ren, notice2, NULL, &notice2_rect);
		SDL_RenderCopy(ren, stepback, NULL, &Page.StepBack);
		SDL_RenderCopy(ren, skip, NULL, &Page.SeventhTypeButton);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(player1);
	SDL_DestroyTexture(player2);
	SDL_DestroyTexture(notice1);
	SDL_DestroyTexture(notice2);
	SDL_DestroyTexture(nameinput);
	SDL_DestroyTexture(skip);
	SDL_DestroyTexture(stepback);
	SDL_DestroyTexture(heading);

	TTF_CloseFont(SanFrancisco);
	TTF_CloseFont(Franklin);
}

void Play(int& mode, Proportions window, Zones Game, Elements& GameProgress, Appearance Page)
{
	int TextFontSize = Game.ThrowButton1.h / 2;
	TTF_Font* SanFrancisco = TTF_OpenFont("fonts\\San Francisco.ttf", TextFontSize);
	if (SanFrancisco == NULL)
	{
		cout << "Couldn't open TTF: San Francisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	int InfFontSize = Game.Name1.h - cv * 2;
	TTF_Font* Franklin = TTF_OpenFont("fonts\\San Francisco.ttf", InfFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	
	//статичные 

	#pragma region HP
	//вспомогательные переменные для построения сети зон
	//ширина окна
	int width = window.width;
	//высота окна 
	int height = window.height;
	//ширина зоны игры / имен / комбинаций
	int infwidth = window.width / 8 * 3;
	//ширина зоны подсказок / результатов
	int midwidth = window.width / 8 * 2;
	//высота зоны подсказок
	int tipsheight = window.height / 2;
	//высота зоны имен / комбинаций 
	int infheight = window.height / 10;
	//ширина зоны счета
	int scorewidth = (midwidth - infheight) / 2 + ut * 2;
	//высота зоны результатов
	int resultsheight = tipsheight - infheight;
	//ширина зоны кнопки бросок
	int throwwidth = infwidth / 5 * 2;
	//ширина зоны кнопки пропуск хода
	int skipwidth = infwidth - throwwidth + ut;
	//высота зоны игры
	int playheight = height - infheight * 3;
#pragma endregion

	#pragma region Statc
	char to_throw[] = u8"Бросить";
	char to_skip[] = u8"Пропустить ход";
	char tips_l1[] = u8"Комбинации:";
	char tips_l2[] = u8"Одна пара";
	char tips_l3[] = u8"Две пары";
	char tips_l4[] = u8"Три одинаковых";
	char tips_l5[] = u8"Полный дом (3+2)";
	char tips_l6[] = u8"Четыре одинаковых";
	char tips_l7[] = u8"Пять одинаковых";

	char isleading[] = u8"*ходит";

	SDL_Rect ThrowButton1_rect;
	SDL_Rect SkipButton1_rect;
	SDL_Rect ThrowButton2_rect;
	SDL_Rect SkipButton2_rect;
	SDL_Rect tips1_rect;
	SDL_Rect tips2_rect;
	SDL_Rect tips3_rect;
	SDL_Rect tips4_rect;
	SDL_Rect tips5_rect;
	SDL_Rect tips6_rect;
	SDL_Rect tips7_rect;

	SDL_Rect gamblername1_rect;
	SDL_Rect gamblername2_rect;
	SDL_Rect lead_rect;

	SDL_Texture* table = LoadTextureFromFile("images\\table.jpg");
	SDL_Texture* PauseButton = LoadTextureFromFile("images\\pause.png");
	SDL_Texture* ThrowButton = GenerateTextureFromText(to_throw, SanFrancisco, &ThrowButton1_rect, { 255, 255, 255, 0 });
	SDL_Texture* SkipButton = GenerateTextureFromText(to_skip, SanFrancisco, &SkipButton1_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips1 = GenerateTextureFromText(tips_l1, SanFrancisco, &tips1_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips2 = GenerateTextureFromText(tips_l2, SanFrancisco, &tips2_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips3 = GenerateTextureFromText(tips_l3, SanFrancisco, &tips3_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips4 = GenerateTextureFromText(tips_l4, SanFrancisco, &tips4_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips5 = GenerateTextureFromText(tips_l5, SanFrancisco, &tips5_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips6 = GenerateTextureFromText(tips_l6, SanFrancisco, &tips6_rect, { 255, 255, 255, 0 });
	SDL_Texture* tips7 = GenerateTextureFromText(tips_l7, SanFrancisco, &tips7_rect, { 255, 255, 255, 0 });

	SDL_Texture* gambler1 = GenerateTextureFromText(GameProgress.Gambler1, Franklin, &gamblername1_rect, { 255, 255, 255, 0 });
	SDL_Texture* gambler2 = GenerateTextureFromText(GameProgress.Gambler2, Franklin, &gamblername2_rect, { 255, 255, 255, 0 });
	SDL_Texture* lead = GenerateTextureFromText(isleading, SanFrancisco, &lead_rect, { 255, 255, 255, 0 });

	tips1_rect.x = Game.TipsList.x + Game.TipsList.w / 2 - tips1_rect.w / 2;
	tips1_rect.y = Game.TipsList.y + Game.TipsList.h / 8 / 2 - tips1_rect.h / 2;
	tips2_rect.x = Game.TipsList.x + cv * 2;
	tips2_rect.y = Game.TipsList.y + Game.TipsList.y / 8 + Game.TipsList.h / 8 / 2 - tips2_rect.h / 2;
	tips3_rect.x = Game.TipsList.x + cv * 2;
	tips3_rect.y = Game.TipsList.y + Game.TipsList.y / 8 * 2 + Game.TipsList.h / 8 / 2 - tips3_rect.h / 2;
	tips4_rect.x = Game.TipsList.x + cv * 2;
	tips4_rect.y = Game.TipsList.y + Game.TipsList.y / 8 * 3 + Game.TipsList.h / 8 / 2 - tips4_rect.h / 2;
	tips5_rect.x = Game.TipsList.x + cv * 2;
	tips5_rect.y = Game.TipsList.y + Game.TipsList.y / 8 * 4 + Game.TipsList.h / 8 / 2 - tips5_rect.h / 2;
	tips6_rect.x = Game.TipsList.x + cv * 2;
	tips6_rect.y = Game.TipsList.y + Game.TipsList.y / 8 * 5 + Game.TipsList.h / 8 / 2 - tips6_rect.h / 2;
	tips7_rect.x = Game.TipsList.x + cv * 2;
	tips7_rect.y = Game.TipsList.y + Game.TipsList.y / 8 * 6 + Game.TipsList.h / 8 / 2 - tips7_rect.h / 2;

	ThrowButton2_rect = ThrowButton1_rect;
	SkipButton2_rect = SkipButton1_rect;

	ThrowButton1_rect.x = Game.ThrowButton1.x + Game.ThrowButton1.w / 2 - ThrowButton1_rect.w / 2;
	ThrowButton1_rect.y = Game.ThrowButton1.y + Game.ThrowButton1.h / 2 - ThrowButton1_rect.h / 2;
	SkipButton1_rect.x = Game.SkipButton1.x + Game.SkipButton1.w / 2 - SkipButton1_rect.w / 2;
	SkipButton1_rect.y = Game.SkipButton1.y + Game.SkipButton1.h / 2 - SkipButton1_rect.h / 2;
	ThrowButton2_rect.x = Game.ThrowButton2.x + Game.ThrowButton2.w / 2 - ThrowButton1_rect.w / 2;
	ThrowButton2_rect.y = Game.ThrowButton2.y + Game.ThrowButton2.h / 2 - ThrowButton1_rect.h / 2;
	SkipButton2_rect.x = Game.SkipButton2.x + Game.SkipButton2.w / 2 - SkipButton1_rect.w / 2;
	SkipButton2_rect.y = Game.SkipButton2.y + Game.SkipButton2.h / 2 - SkipButton1_rect.h / 2;

	gamblername1_rect.x = Game.Name1.x + cv;
	gamblername1_rect.y = Game.Name1.y + ut;
	gamblername2_rect.x = Game.Name2.x + cv;
	gamblername2_rect.y = Game.Name2.y + ut;

	lead_rect.y = Game.Name1.y + Game.Name1.h / 2 - lead_rect.h / 2 - ut;
#pragma endregion 

	//динамические
	bool animation = false;
	bool down = false;
	int ThrowMax;


	if (GameProgress.Round == 0)
	{
		ThrowMax = 1;
	}
	else
	{
		ThrowMax = 3;
	}

	char phasetext[100] = u8"Жеребьевка";

	int p1 = 0;
	int p2 = 0;
	char points1[4];
	char points2[4];
	_itoa_s(p1, points1, 10);
	_itoa_s(p2, points2, 10);

	SDL_Rect score1_rect;
	SDL_Rect score2_rect;
	SDL_Rect phase_rect;
	SDL_Rect outcome_rect;
	SDL_Rect final_rect;

	SDL_Rect glass_rect;
	SDL_Rect combo1_rect;
	SDL_Rect combo2_rect;

	SDL_Texture* score1 = GenerateTextureFromText(points1, Franklin, &score1_rect, { 255, 255, 255, 0 });
	SDL_Texture* score2 = GenerateTextureFromText(points2, Franklin, &score2_rect, { 255, 255, 255, 0 });

	//if (round == 0)
	SDL_Texture* phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 }); ;
	//else if (round == 1)
	SDL_Texture* dice1 = NULL;
	SDL_Texture* dice2 = NULL;
	SDL_Texture* glass = LoadTextureFromFile("images\\glass.png");
	SDL_Texture* combo1 = GenerateTextureFromText(GameProgress.Combo1, SanFrancisco, &combo1_rect, { 255, 255, 255, 0 }); 
	SDL_Texture* combo2 = GenerateTextureFromText(GameProgress.Combo2, SanFrancisco, &combo2_rect, { 255, 255, 255, 0 });
		

	score1_rect.x = Game.Score1.x + Game.Score1.w / 2 - score1_rect.w / 2;
	score1_rect.y = Game.Score1.y + Game.Score1.h / 2 - score1_rect.h / 2;
	score2_rect.x = Game.Score2.x + Game.Score2.w / 2 - score2_rect.w / 2;
	score2_rect.y = Game.Score2.y + Game.Score2.h / 2 - score2_rect.h / 2;
		
	phase_rect.x = Game.Results.x + Game.Results.w / 2 - phase_rect.w / 2;
	phase_rect.y = Game.Results.y + Game.Results.y / 3 /*- phase_rect.h / 2*/;

	combo1_rect.x = Game.Combinations1.x + cv;
	combo1_rect.y = Game.Combinations1.y + Game.Combinations1.h / 2 - combo1_rect.h / 2;
	combo2_rect.x = Game.Combinations2.x + cv;
	combo2_rect.y = Game.Combinations2.y + Game.Combinations2.h / 2 - combo2_rect.h / 2;
	

	SDL_Surface* surface = IMG_Load("images\\glass.png");
	glass_rect = { 0, 0, surface->w, surface->h };
	SDL_Rect glass_recttmp = glass_rect;
	SDL_Rect gameglasscopy = Game.Glass;
	SDL_FreeSurface(surface);
	gameglasscopy.h = 0;
	gameglasscopy.y = infheight;
	int i = 0;


	


	SDL_Event event;
	bool quit = false;

	//============================================================================
	//обработка событий
	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				mode = 0;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					PauseMenu(mode, Page, window);
					if (mode == 1 || mode == 0)
						quit = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Game.PauseButton, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					PauseMenu(mode, Page, window);
					if (mode == 1 || mode == 0)
						quit = true;
				}
				if (ButtonClick(Game.ThrowButton1, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 1)
				{
					animation = true;

				}
				if (ButtonClick(Game.ThrowButton2, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 2)
				{
					animation = true;
				}
				if (ButtonClick(Game.SkipButton1, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 1 && animation == false)
				{
					GameProgress.Queue = 2;
					gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
				}
				if (ButtonClick(Game.SkipButton2, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 2 && animation == false)
				{
					GameProgress.Queue = 1;
					gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
				}
				break;
			}


		}

		//==============================================================================
		//логика

		if (GameProgress.Queue == 1)
			lead_rect.x = Game.Name1.x + Game.Name1.w - lead_rect.w - ut;
		else
			lead_rect.x = Game.Name2.x + Game.Name2.w - lead_rect.w - ut;
		

		SDL_Rect dice1_rect = Game.Dice1;
		SDL_Rect dice2_rect = Game.Dice2;

		if (animation == true && down == false)
		{
			if (gameglasscopy.h < playheight / 12 * 11)
			{
				glass_recttmp.y = glass_rect.h - i * 0.755;
				glass_recttmp.h = i * 0.755;
				gameglasscopy.h = i;
				i += speed;
			}
			else if (gameglasscopy.y < infheight + playheight / 24)
				gameglasscopy.y += speed;
			else
			{
				GenerateRandValues(GameProgress.DiceValues, GameProgress.Queue);
				//animation = false;
				down = true;
			}
		}
		if (animation == true && down == true)
		{
			
			if (gameglasscopy.y >= infheight)
				gameglasscopy.y -= speed;
			else if (gameglasscopy.h >= 0)
			{
				glass_recttmp.y = glass_rect.h - i * 0.755;
				glass_recttmp.h = i * 0.755;
				gameglasscopy.h = i;
				i -= speed;
			}
			else
			{
				if (GameProgress.Queue == 1)
				{
					combo1 = GenerateCombination(GameProgress, SanFrancisco, &combo1_rect, { 255, 255, 255, 0 });
				}
				else
				{
					combo2 = GenerateCombination(GameProgress, SanFrancisco, &combo2_rect, { 255, 255, 255, 0 });
				}
				animation = false;
				down = false;
			}
			
		}
		

		//==============================================================================
		//отрисовка

		#pragma region StaticDrawing
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		//белые линии
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
		for (int i = 0; i < ut; i++)
		{
			SDL_Rect basis = { i , i ,width - 2 * i, height - 2 * i };
			SDL_Rect namefield1 = { i, i, infwidth - 2 * i, infheight - 2 * i };
			SDL_Rect namefield2 = { width - infwidth + i, i, infwidth - 2 * i, infheight - 2 * i };
			SDL_Rect scorefield1 = { infwidth - ut + i, i, scorewidth - 2 * i, infheight - 2 * i };
			SDL_Rect scorefield2 = { width - infwidth - scorewidth + ut + i, i, scorewidth - 2 * i, infheight - 2 * i };
			SDL_Rect pausefield = { infwidth + scorewidth - ut * 2 + i, i, infheight - 2 * i, infheight - 2 * i };
			SDL_Rect combinationsfield1 = { i, height - infheight + i, infwidth - 2 * i, infheight - 2 * i };
			SDL_Rect combinationsfield2 = { width - infwidth + i, height - infheight + i, infwidth - 2 * i, infheight - 2 * i };
			SDL_Rect tipsfield = { infwidth - ut + i, tipsheight + i, midwidth + ut * 2 - 2 * i, tipsheight - 2 * i };
			SDL_Rect resultsfield = { infwidth - ut + i, infheight - ut + i, midwidth + ut * 2 - 2 * i, resultsheight + ut * 2 - 2 * i };
			SDL_Rect throwfield1 = { i, height - 2 * infheight + ut + i, throwwidth - 2 * i, infheight - 2 * i };
			SDL_Rect skipfield1 = { throwwidth - ut + i, height - 2 * infheight + ut + i, skipwidth - 2 * i, infheight - 2 * i };
			SDL_Rect throwfield2 = { width - infwidth + i, height - 2 * infheight + ut + i, throwwidth - 2 * i, infheight - 2 * i };
			SDL_Rect skipfield2 = { width - infwidth + throwwidth - ut + i, height - 2 * infheight + ut + i, skipwidth - 2 * i, infheight - 2 * i };

			SDL_RenderDrawRect(ren, &basis);
			SDL_RenderDrawRect(ren, &namefield1);	
			SDL_RenderDrawRect(ren, &namefield2);
			SDL_RenderDrawRect(ren, &scorefield1);			
			SDL_RenderDrawRect(ren, &scorefield2);			
			SDL_RenderDrawRect(ren, &pausefield);			
			SDL_RenderDrawRect(ren, &combinationsfield1);			
			SDL_RenderDrawRect(ren, &combinationsfield2);					
			SDL_RenderDrawRect(ren, &tipsfield);			
			SDL_RenderDrawRect(ren, &resultsfield);		
			SDL_RenderDrawRect(ren, &throwfield1);						
			SDL_RenderDrawRect(ren, &skipfield1);			
			SDL_RenderDrawRect(ren, &throwfield2);			
			SDL_RenderDrawRect(ren, &skipfield2);
		}

		//зеленые линии
		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
		{
			SDL_Rect name1 = { ut * 2 + i, ut * 2 + i, infwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i};
			SDL_Rect name2 = { width - infwidth + ut * 2 + i, ut * 2 + i, infwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect combinations1 = { ut * 2 + i, height - infheight + ut * 2 + i, infwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect combinations2 = { width - infwidth + ut * 2 + i, height - infheight + ut * 2 + i, infwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect score1 = { infwidth - ut + ut * 2 + i, ut * 2 + i, scorewidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect score2 = { width - infwidth - scorewidth + ut + ut * 2 + i,  ut * 2 + i, scorewidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect pause = { infwidth + scorewidth - ut * 2 + ut * 2 + i,  ut * 2 + i, infheight - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect tips = { infwidth - ut + ut * 2 + i, tipsheight + ut * 2 + i, midwidth + ut * 2 - ut * 4 - 2 * i, tipsheight - ut * 4 - 2 * i };
			SDL_Rect results = { infwidth - ut + ut * 2 + i, infheight - ut + ut * 2 + i, midwidth + ut * 2 - ut * 4 - 2 * i, resultsheight + ut * 2 - ut * 4 - 2 * i };
			SDL_Rect throw1 = { ut * 2 + i, height - 2 * infheight + ut + ut * 2 + i, throwwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect skip1 = { throwwidth - ut + ut * 2 + i, height - 2 * infheight + ut + ut * 2 + i, skipwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect throw2 = { width - infwidth + ut * 2 + i, height - 2 * infheight + ut + ut * 2 + i, throwwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };
			SDL_Rect skip2 = { width - infwidth + throwwidth - ut + ut * 2 + i, height - 2 * infheight + ut + ut * 2 + i, skipwidth - ut * 4 - 2 * i, infheight - ut * 4 - 2 * i };

			SDL_RenderDrawRect(ren, &name1);
			SDL_RenderDrawRect(ren, &name2);
			SDL_RenderDrawRect(ren, &combinations1);
			SDL_RenderDrawRect(ren, &combinations2);
			SDL_RenderDrawRect(ren, &score1);
			SDL_RenderDrawRect(ren, &score2);
			SDL_RenderDrawRect(ren, &pause);
			SDL_RenderDrawRect(ren, &tips);
			SDL_RenderDrawRect(ren, &results);
			SDL_RenderDrawRect(ren, &throw1);
			SDL_RenderDrawRect(ren, &skip1);
			SDL_RenderDrawRect(ren, &throw2);
			SDL_RenderDrawRect(ren, &skip2);
		}

		//статичные объекты
		SDL_RenderCopy(ren, ThrowButton, NULL, &ThrowButton1_rect);
		SDL_RenderCopy(ren, SkipButton, NULL, &SkipButton1_rect);
		SDL_RenderCopy(ren, ThrowButton, NULL, &ThrowButton2_rect);
		SDL_RenderCopy(ren, SkipButton, NULL, &SkipButton2_rect);
		SDL_RenderCopy(ren, tips1, NULL, &tips1_rect);
		SDL_RenderCopy(ren, tips2, NULL, &tips2_rect);
		SDL_RenderCopy(ren, tips3, NULL, &tips3_rect);
		SDL_RenderCopy(ren, tips4, NULL, &tips4_rect);
		SDL_RenderCopy(ren, tips5, NULL, &tips5_rect);
		SDL_RenderCopy(ren, tips6, NULL, &tips6_rect);
		SDL_RenderCopy(ren, tips7, NULL, &tips7_rect);
		SDL_RenderCopy(ren, table, NULL, &Game.PlayZone1);
		SDL_RenderCopy(ren, table, NULL, &Game.PlayZone2);
		SDL_RenderCopy(ren, PauseButton, NULL, &Game.PauseButton);

		SDL_RenderCopy(ren, gambler1, NULL, &gamblername1_rect);
		SDL_RenderCopy(ren, gambler2, NULL, &gamblername2_rect);
		SDL_RenderCopy(ren, lead, NULL, &lead_rect);
#pragma endregion


		//динамичные объекты
		SDL_RenderCopy(ren, score1, NULL, &score1_rect);
		SDL_RenderCopy(ren, score2, NULL, &score2_rect);
		SDL_RenderCopy(ren, phase, NULL, &phase_rect);

		//кости
		for (int i = 0; i <= ut; i++)
		{
			switch (GameProgress.DiceValues[i])
			{
			case 1:
				dice1 = LoadTextureFromFile("images\\dice1.png");
				break;
			case 2:
				dice1 = LoadTextureFromFile("images\\dice2.png");
				break;
			case 3:
				dice1 = LoadTextureFromFile("images\\dice3.png");
				break;
			case 4:
				dice1 = LoadTextureFromFile("images\\dice4.png");
				break;
			case 5:
				dice1 = LoadTextureFromFile("images\\dice5.png");
				break;
			case 6:
				dice1 = LoadTextureFromFile("images\\dice6.png");
				break;
			}
			SDL_RenderCopy(ren, dice1, NULL, &dice1_rect);
			dice1_rect.y += dice1_rect.h;

			switch (GameProgress.DiceValues[i + 5])
			{
			case 1:
				dice2 = LoadTextureFromFile("images\\dice1.png");
				break;
			case 2:
				dice2 = LoadTextureFromFile("images\\dice2.png");
				break;
			case 3:
				dice2 = LoadTextureFromFile("images\\dice3.png");
				break;
			case 4:
				dice2 = LoadTextureFromFile("images\\dice4.png");
				break;
			case 5:
				dice2 = LoadTextureFromFile("images\\dice5.png");
				break;
			case 6:
				dice2 = LoadTextureFromFile("images\\dice6.png");
				break;
			}
			SDL_RenderCopy(ren, dice2, NULL, &dice2_rect);
			dice2_rect.y += dice2_rect.h;

			SDL_DestroyTexture(dice1);
			SDL_DestroyTexture(dice2);
		}

		if (animation == true)
			SDL_RenderCopy(ren, glass, &glass_recttmp, &gameglasscopy);
		
		SDL_RenderCopy(ren, combo1, NULL, &combo1_rect);
		SDL_RenderCopy(ren, combo2, NULL, &combo2_rect);
		

		SDL_RenderPresent(ren);
		FPSControl();
	}

	SDL_DestroyTexture(ThrowButton);
	SDL_DestroyTexture(SkipButton);
	SDL_DestroyTexture(tips1);
	SDL_DestroyTexture(tips2);
	SDL_DestroyTexture(tips3);
	SDL_DestroyTexture(tips4);
	SDL_DestroyTexture(tips5);
	SDL_DestroyTexture(tips6);
	SDL_DestroyTexture(tips7);
	SDL_DestroyTexture(table);
	SDL_DestroyTexture(PauseButton);

	SDL_DestroyTexture(gambler1);
	SDL_DestroyTexture(gambler2);
	SDL_DestroyTexture(lead);

	SDL_DestroyTexture(score1);
	SDL_DestroyTexture(score2);
	SDL_DestroyTexture(phase);
	SDL_DestroyTexture(dice1);
	SDL_DestroyTexture(dice2);
	SDL_DestroyTexture(glass);
	SDL_DestroyTexture(combo1);
	SDL_DestroyTexture(combo2);

}

void PageLayout(Appearance& Page, Proportions window, Zones& Game)
{
	//вспомогательные переменные
	//ширина окна
	int width = window.width;
	//высота окна 
	int height = window.height;
	//ширина зоны игры / имен / комбинаций
	int infwidth = window.width / 8 * 3;
	//ширина зоны подсказок / результатов
	int midwidth = window.width / 8 * 2;
	//высота зоны подсказок
	int tipsheight = window.height / 2;
	//высота зоны имен / комбинаций 
	int infheight = window.height / 10;
	//ширина зоны счета
	int scorewidth = (midwidth - infheight) / 2 + ut * 2;
	//высота зоны результатов
	int resultsheight = tipsheight - infheight;
	//ширина зоны кнопки бросок
	int throwwidth = infwidth / 5 * 2;
	//ширина зоны кнопки пропуск хода
	int skipwidth = infwidth - throwwidth + ut;
	//высота зоны игры
	int playheight = height - infheight * 3;

	Page.UnderlineIndent = height / 7;
	Page.HeadingFontSize = height / 12;
	Page.StepBack = { Page.UnderlineIndent / 2 - Page.UnderlineIndent / 7 * 4 / 2, Page.UnderlineIndent / 2 - Page.UnderlineIndent / 7 * 4 / 2, Page.UnderlineIndent / 7 * 4 / sh * 12, Page.UnderlineIndent / 7 * 4 };
	Page.FirstTypeButton = { width / 2 - width / 10, height / 4, width / 5, width / 5 / sw * sh };
	Page.SecondTypeButton = { Page.FirstTypeButton.x, Page.FirstTypeButton.y + Page.FirstTypeButton.h + cv, Page.FirstTypeButton.w, Page.FirstTypeButton.h };
	Page.ThirdTypeButton = { Page.SecondTypeButton.x, Page.SecondTypeButton.y + Page.SecondTypeButton.h + cv, Page.SecondTypeButton.w, Page.SecondTypeButton.h };
	Page.FourthTypeButton = { Page.ThirdTypeButton.x, Page.ThirdTypeButton.y + Page.ThirdTypeButton.h + cv, Page.ThirdTypeButton.w, Page.ThirdTypeButton.h };
	Page.FifthTypeButton = { width / 3 - Page.FirstTypeButton.w / 2,  height / 6 * 4, Page.FirstTypeButton.w, Page.FirstTypeButton.h };
	Page.SixthTypeButton = { width / 3 * 2 - Page.FirstTypeButton.w / 2,  Page.FifthTypeButton.y, Page.FifthTypeButton.w, Page.FifthTypeButton.h };
	Page.SeventhTypeButton = { width / 9 * 7,  height - Page.UnderlineIndent * 7 / 5, Page.FirstTypeButton.w, Page.FirstTypeButton.h };

	Game.Name1 = { ut * 2 + 3, ut * 2 + 3, infwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.Name2 = { width - infwidth + ut * 2 + 3, ut * 2 + 3, infwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.Combinations1 = { ut * 2 + 3, height - infheight + ut * 2 + 3, infwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.Combinations2 = { width - infwidth + ut * 2 + 3, height - infheight + ut * 2 + 3, infwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.Score1 = { infwidth + ut + 3, ut * 2 + 3, scorewidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.Score2 = { width - infwidth - scorewidth + ut * 3 + 3,  ut * 2 + 3, scorewidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.PauseButton = { infwidth + scorewidth + 3,  ut * 2 + 3, infheight - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.TipsList = { infwidth + ut + 3, tipsheight + ut * 2 + 3, midwidth - ut * 2 - 2 * 3, tipsheight - ut * 4 - 2 * 3 };
	Game.Results = { infwidth + ut + 3, infheight + ut + 3, midwidth - ut * 2 - 2 * 3, resultsheight - ut * 2 - 2 * 3 };
	Game.ThrowButton1 = { ut * 2 + 3, height - 2 * infheight + ut * 3 + 3, throwwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.SkipButton1 = { throwwidth + ut + 3, height - 2 * infheight + ut * 3 + 3, skipwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.ThrowButton2 = { width - infwidth + ut * 2 + 3, height - 2 * infheight + ut * 3 + 3, throwwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.SkipButton2 = { width - infwidth + throwwidth + ut + 3, height - 2 * infheight + ut * 3 + 3, skipwidth - ut * 4 - 2 * 3, infheight - ut * 4 - 2 * 3 };
	Game.PlayZone1 = { ut, infheight, infwidth - ut * 2, height - infheight * 3 };
	Game.PlayZone2 = { width - infwidth + ut, infheight, infwidth - ut * 2, height - infheight * 3 };
	Game.Dice1 = { infwidth / 2 - playheight / 12, infheight + playheight / 12, playheight / 6, playheight / 6 };
	Game.Dice2 = { width - infwidth / 2 - playheight / 10, infheight + playheight / 12, playheight / 6, playheight / 6 };
	Game.Glass = { infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2, infheight + playheight / 24, (playheight / 12 * 11) / 29 * 19, playheight / 12 * 11 };
}

void FPSabout()
{
	SDL_Rect objectrect = { 0, 0, 0, 0 };
	int frame = 0;
	int frame_count = 6;
	int cur_frametime = 0;
	int max_frametime = 1000/12;

	//инициализация
	int lasttime = SDL_GetTicks();
	int newtime;
	int dt = 0;


	//логика
	newtime = SDL_GetTicks();
	//время между 2-мя кадрами
	dt = newtime - lasttime;
	//1000/60 = 16
	if (dt < 16)
	{
		SDL_Delay(16 - dt);
		newtime = SDL_GetTicks();
		dt = newtime - lasttime;
	}
	lasttime = newtime;


	//отрисовка
	cur_frametime += dt;
	if (cur_frametime >= max_frametime)
	{
		cur_frametime -= max_frametime;
		frame = (frame + 1) % frame_count;
		objectrect.x = objectrect.w * frame;
	}
}

SDL_Texture* LoadTextureFromFile(const char* filename)
{
	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL)
	{
		cout << "Couldn't load image " << filename << "! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 7, 7, 7));
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* GenerateTextureFromText(char* str, TTF_Font* font, SDL_Rect* rect, SDL_Color fg)
{
	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, str, fg);
	rect->w = surface->w;
	rect->h = surface->h;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);

	return texture;
}

bool ButtonClick(SDL_Rect& rect, int x, int y)
{
	if ((x >= rect.x && x <= rect.x + rect.w) && (y >= rect.y && y <= rect.y + rect.h))
		return true;
	else
		return false;
}

void GenerateRandValues(int* DiceValues, int queue)
{
	srand(time(NULL));

	for (int i = 0; i < dq / 2; i++)
	{
		if (queue == 1)
			DiceValues[i] = rand() % 6 + 1;
		if (queue == 2)
			DiceValues[i + 5] = rand() % 6 + 1;
	}
}

void Reset(Elements& GameProgress)
{
	bool GameStatus = true;
	GameProgress.Round = 0;
	GameProgress.p1 = 0;
	GameProgress.p2 = 0;
	GameProgress.Queue = 0;
}

void ReadGameProgressDataFile(Elements& GameProgress)
{
	string container;

	ifstream GPFile;
	GPFile.open("GameProgress.txt", ios_base::in);

	if (!GPFile.is_open())
		cout << "Ошибка открытия файла! " << endl;
	else
	{
		GPFile >> GameProgress.GameStatus;
		GPFile >> GameProgress.Round;
		getline(GPFile, container);
		getline(GPFile, container);
		copy(container.begin(), container.end(), GameProgress.OutcomeText);
		GameProgress.OutcomeText[container.size()] = '\0';
		getline(GPFile, container);
		copy(container.begin(), container.end(), GameProgress.FinalText);
		GameProgress.FinalText[container.size()] = '\0';
		GPFile >> GameProgress.p1;
		GPFile >> GameProgress.p2;
		GPFile >> GameProgress.Queue;
		getline(GPFile, container);
		getline(GPFile, container);
		copy(container.begin(), container.end(), GameProgress.Gambler1);
		GameProgress.Gambler1[container.size()] = '\0';
		getline(GPFile, container);
		copy(container.begin(), container.end(), GameProgress.Gambler2);
		GameProgress.Gambler2[container.size()] = '\0';
		getline(GPFile, container);
		copy(container.begin(), container.end(), GameProgress.Combo1);
		GameProgress.Combo1[container.size()] = '\0';
		getline(GPFile, container);
		copy(container.begin(), container.end(), GameProgress.Combo2);
		GameProgress.Combo2[container.size()] = '\0';
		for (int i = 0; i < dq; i++)
			GPFile >> GameProgress.DiceValues[i];		
	}

	GPFile.close();
}

void PrintGameProgressDataFile(Elements GameProgress)
{
	ofstream GPFile;
	GPFile.open("GameProgress1.txt", ios_base::out);

	if (!GPFile.is_open())
		cout << "Ошибка открытия файла! " << endl;
	else
	{
		GPFile << GameProgress.GameStatus << endl;
		GPFile << GameProgress.Round << endl;
		GPFile << GameProgress.OutcomeText << endl;
		GPFile << GameProgress.FinalText << endl;
		GPFile << GameProgress.p1 << " ";
		GPFile << GameProgress.p2 << endl;
		GPFile << GameProgress.Queue << endl;
		GPFile << GameProgress.Gambler1 << endl;
		GPFile << GameProgress.Gambler2 << endl;
		GPFile << GameProgress.Combo1 << endl;
		GPFile << GameProgress.Combo2 << endl;
		for (int i = 0; i < dq; i++)
			GPFile << GameProgress.DiceValues[i] << " ";
	}

	GPFile.close();
}

SDL_Texture* GenerateCombination(Elements GameProgress, TTF_Font* font, SDL_Rect* rect, SDL_Color fg)
{
	SDL_Texture* texture = NULL;
	char str[ams];
	int k = 0;

	if (GameProgress.Round == 0)
	{
		for (int i = 0; i <= ut; i++)
		{
			if (GameProgress.Queue == 1)
				k += GameProgress.DiceValues[i];
			else
				k += GameProgress.DiceValues[i + 5];
		}
		_itoa_s(k, str, 10);
		if (GameProgress.Queue == 1)
		{
			strcpy_s(GameProgress.Combo1, str);
			return texture = GenerateTextureFromText(GameProgress.Combo1, font, rect, fg);
		}
		else
		{
			strcpy_s(GameProgress.Combo2, str);
			return texture = GenerateTextureFromText(GameProgress.Combo2, font, rect, fg);
		}
	}
	else
	{
		FindBestCombination(GameProgress);
		if (GameProgress.Queue == 1)
		{
			return texture = GenerateTextureFromText(GameProgress.Combo1, font, rect, fg);
		}
		else
		{
			return texture = GenerateTextureFromText(GameProgress.Combo2, font, rect, fg);
		}
	}
}

int FindBestCombination(Elements &GameProgress)
{
	int values[dq / 2];
	int combo = 0;

	for (int i = 0; i < dq / 2; i++)
	{
		if (GameProgress.Queue == 1)
			values[i] = GameProgress.DiceValues[i];
		if (GameProgress.Queue == 2)
			values[i] = GameProgress.DiceValues[i + 5];
	}

	int Quantities[dq / 2 + 1] = { 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < dq / 2; i++)
	{
		switch (values[i])
		{
		case 1:
			Quantities[0]++;
			break;
		case 2:
			Quantities[1]++;
			break;
		case 3:
			Quantities[2]++;
			break;
		case 4:
			Quantities[3]++;
			break;
		case 5:
			Quantities[4]++;
			break;
		case 6:
			Quantities[5]++;
			break;
		}
	}

	int repeatMax = 0;
	int repeatTwo = -1;
	int digit = -1;
	for (int i = 0; i < dq / 2 + 1; i++)
	{
		if (Quantities[i] > repeatMax)
		{
			repeatMax = Quantities[i];
			digit = i;
		}
	}
	for (int i = 0; i < dq / 2 + 1; i++)
	{
		if (Quantities[i] == 2 && i != digit)
			repeatTwo = i;
	}


	switch (repeatMax)
	{
	case 5:
		switch (digit)
		{
		case 0:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Пять единиц");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Пять единиц");
			return 64;
		case 1:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Пять двоек");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Пять двоек");
			return 65;
		case 2:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Пять троек");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Пять троек");
			return 66;
		case 3:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Пять четверок");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Пять четверок");
			return 67;
		case 4:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Пять пятерок");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Пять пятерок");
			return 68;
		case 5:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Пять шестерок");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Пять шестерок");
			return 69;
		}
	case 4:
		switch (digit)
		{
		case 0:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Четыре единицы");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Четыре единицы");
			return 58;
		case 1:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Четыре двойки");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Четыре двойки");
			return 57;
		case 2:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Четыре тройки");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Четыре тройки");
			return 60;
		case 3:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Четыре четверки");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Четыре четверки");
			return 61;
		case 4:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Четыре пятерки");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Четыре пятерки");
			return 62;
		case 5:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"Четыре шестерки");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"Четыре шестерки");
			return 63;
		}
	case 3:
		if (repeatTwo >= 0)
		{
			switch (digit)
			{
			case 0:
				switch (repeatTwo)
				{
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (1 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (1 + 2)");
					return 28;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (1 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (1 + 3)");
					return 29;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (1 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (1 + 4)");
					return 30;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (1 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (1 + 5)");
					return 31;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (1 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (1 + 6)");
					return 32;
				}
			case 1:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (2 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (2 + 1)");
					return 33;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (2 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (2 + 3)");
					return 34;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (2 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (2 + 4)");
					return 35;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (2 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (2 + 5)");
					return 36;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (2 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (2 + 6)");
					return 37;
				}
			case 2:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (3 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (3 + 1)");
					return 38;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (3 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (3 + 2)");
					return 39;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (3 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (3 + 4)");
					return 40;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (3 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (3 + 5)");
					return 41;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (3 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (3 + 6)");
					return 42;
				}
			case 3:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (4 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (4 + 1)");
					return 43;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (4 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (4 + 2)");
					return 44;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (4 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (4 + 3)");
					return 45;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (4 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (4 + 5)");
					return 46;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (4 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (4 + 6)");
					return 47;
				}
			case 4:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (5 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (5 + 1)");
					return 48;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (5 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (5 + 2)");
					return 49;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (5 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (5 + 3)");
					return 50;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (5 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (5 + 4)");
					return 51;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (5 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (5 + 6)");
					return 52;
				}
			case 5:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (6 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (6 + 1)");
					return 53;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (6 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (6 + 2)");
					return 54;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (6 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (6 + 3)");
					return 55;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (6 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (6 + 4)");
					return 56;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Полный дом (6 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Полный дом (6 + 5)");
					return 57;
				}
			}
		}
		else
		{
			switch (digit)
			{
			case 0:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Три единицы");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Три единицы");
				return 22;
			case 1:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Три двойки");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Три двойки");
				return 23;
			case 2:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Три тройки");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Три тройки");
				return 24;
			case 3:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Три четверки");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Три четверки");
				return 25;
			case 4:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Три пятерки");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Три пятерки");
				return 26;
			case 5:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Три шестерки");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Три шестерки");
				return 27;
			}
		}
	case 2:
		if (repeatTwo >= 0)
		{
			switch (digit)
			{
			case 0:
				switch (repeatTwo)
				{
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара единиц + Пара двоек");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара единиц + Пара двоек");
					return 7;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара единиц + Пара двоек");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара единиц + Пара двоек");
					return 8;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара единиц + Пара троек");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара единиц + Пара троек");
					return 9;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара единиц + Пара четверок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара единиц + Пара четверок");
					return 10;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара единиц + Пара пятерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара единиц + Пара пятерок");
					return 11;
				}
			case 1:
				switch (repeatTwo)
				{
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара двоек + Пара троек");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара двоек + Пара троек");
					return 12;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара двоек + Пара четверок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара двоек + Пара четверок");
					return 13;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара двоек + Пара пятерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара двоек + Пара пятерок");
					return 14;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара двоек + Пара шестерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара двоек + Пара шестерок");
					return 15;
				}
			case 2:
				switch (repeatTwo)
				{
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара троек + Пара четверок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара троек + Пара четверок");
					return 16;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара троек + Пара пятерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара троек + Пара пятерок");
					return 17;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара троек + Пара шестерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара троек + Пара шестерок");
					return 18;
				}
			case 3:
				switch (repeatTwo)
				{
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара четверок + Пара пятерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара четверок + Пара пятерок");
					return 19;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара четверок + Пара шестерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара четверок + Пара шестерок");
					return 20;
				}
			case 4:
				switch (repeatTwo)
				{
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"Пара пятерок + Пара шестерок");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"Пара пятерок + Пара шестерок");
					return 21;
				}
			}
		}
		else
		{
			switch (digit)
			{
			case 0:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Пара единиц");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Пара единиц");
				return 1;
			case 1:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Пара двоек");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Пара двоек");
				return 2;
			case 2:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Пара троек");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Пара троек");
				return 3;
			case 3:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Пара четверок");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Пара четверок");
				return 4;
			case 4:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Пара пятерок");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Пара пятерок");
				return 5;
			case 5:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"Пара шестерок");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"Пара шестерок");
				return 6;
			}
		}
	case 1:
		if (GameProgress.Queue == 1)
			strcpy_s(GameProgress.Combo1, u8"Ничего");
		if (GameProgress.Queue == 2)
			strcpy_s(GameProgress.Combo2, u8"Ничего");
		return 0;
	}
}

void FPSControl()
{
	static int time = 0;
	while (true)
	{
		if (clock() - time >= 1000 / 90.)
		{
			time = clock();
			break;
		}
		else
			SDL_Delay(1);
	}
}

//int Input_Data(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect& inputRect)
//{
//	SDL_Event event;
//	int quit = 0;
//
//	char inputText[5] = "|";
//	int inputIndex = 0;
//
//	TTF_Font* font = TTF_OpenFont("Button_2.ttf", 20);
//	int maxIndex = 0, end = 3;
//
//	while (!quit) {
//		while (SDL_PollEvent(&event)) {
//			switch (event.type) {
//			case SDL_QUIT:
//				quit = 1;
//				Fl = 1;
//				break;
//			case SDL_MOUSEBUTTONDOWN:
//			{
//				if (event.button.button == SDL_BUTTON_LEFT)
//				{
//					if (((event.button.x > inputRect.x + inputRect.w) or (event.button.x < inputRect.x)) or ((event.button.y > inputRect.y + inputRect.h) or (event.button.y < inputRect.y)))
//					{
//						quit = 1;
//						break;
//					}
//				}
//			}
//			case SDL_KEYDOWN:
//				switch (event.key.keysym.sym)
//				{
//				case SDLK_ESCAPE:
//					quit = 1;
//					TTF_CloseFont(font);
//					return -1;
//				case SDLK_RETURN:
//					quit = 1;
//					break;
//				case SDLK_BACKSPACE:
//					if (inputIndex > 0)
//					{
//						if (inputText[inputIndex + 1] != '\0')
//						{
//							for (int i = inputIndex - 1; i < end; i++)
//							{
//								inputText[i] = inputText[i + 1];
//							}
//							inputText[end] = '\0';
//							inputIndex--;
//							maxIndex--;
//						}
//						else
//						{
//							inputText[inputIndex] = '\0';
//							inputText[inputIndex - 1] = '|';
//							inputIndex--;
//							maxIndex--;
//						}
//					}
//					break;
//				case SDLK_LEFT:
//					if (inputIndex > 0)
//					{
//						inputText[inputIndex] = inputText[inputIndex - 1];
//						inputText[inputIndex - 1] = '|';
//						inputIndex--;
//						if (inputIndex <= 0) { inputIndex = 0; }
//					}
//					break;
//				case SDLK_RIGHT:
//					if (inputIndex < maxIndex)
//					{
//						inputText[inputIndex] = inputText[inputIndex + 1];
//						inputText[inputIndex + 1] = '|';
//						inputIndex++;
//						if (inputIndex >= maxIndex) { inputIndex = maxIndex; }
//					}
//					break;
//				default:
//					if ((event.key.keysym.sym >= 48 && event.key.keysym.sym <= 57 && inputIndex < end && inputIndex < 3) and (strlen(inputText) <= 3))
//					{
//						if (inputText[inputIndex + 1] != '\0')
//						{
//							for (int i = end; i > inputIndex; i--)
//							{
//								inputText[i] = inputText[i - 1];
//							}
//							inputText[inputIndex] = (char)event.key.keysym.sym;
//							inputIndex++;
//							maxIndex++;
//							inputText[inputIndex] = '|';
//						}
//						else
//						{
//							inputText[inputIndex] = (char)event.key.keysym.sym;
//							inputIndex++;
//							maxIndex++;
//							inputText[inputIndex] = '|';
//						}
//					}
//					break;
//				}
//				break;
//			}
//		}
//
//		int red_text = 214;
//
//		SDL_Rect textRect = { inputRect.x + 2, inputRect.y + 8, 0, 0 };
//		SDL_Color textColor = { 0, 0, 0 };
//		SDL_Color fore_color = { 0, 0, 0 };
//		SDL_Color back_color = { 255, 255, 255 };
//		SDL_Surface* textSurface = TTF_RenderText_Shaded(font, inputText, fore_color, back_color);
//		SDL_Texture* textTexture = get_text_texture(renderer, inputText, font, red_text);
//		textRect.w = textSurface->w;
//		textRect.h = textSurface->h;
//		SDL_SetRenderDrawColor(renderer, 64, 64, 64, 0);
//		SDL_RenderFillRect(renderer, &inputRect);
//		draw_Place(renderer, textTexture, textRect);
//
//		SDL_FreeSurface(textSurface);
//		SDL_DestroyTexture(textTexture);
//
//		SDL_RenderPresent(renderer);
//	}
//
//	TTF_CloseFont(font);
//
//	return atoi(inputText);
//}