#include "general.h"
#include "GameCore.h"

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

	Mix_Init(0);
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "Couldn't init SDL_Mixer! Error: " << Mix_GetError();
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

	win = SDL_CreateWindow(u8"Dice!!!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window.width, window.width, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
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

}

void DeInit(int error)
{
	

	if (ren != NULL)
		SDL_DestroyRenderer(ren);

	if (win != NULL)
		SDL_DestroyWindow(win);

	Mix_FreeMusic(fon);
	Mix_FreeChunk(sound);
	Mix_CloseAudio();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(0);
}

void MainMenu(int& mode, Appearance Page, Proportions window, Control& SettingsData, bool &previous)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"������� ����";
	SDL_Rect heading_rect;

	SDL_Texture* button_first = LoadTextureFromFile("images\\play.png");
	SDL_Texture* button_second = LoadTextureFromFile("images\\gamerules.png");
	SDL_Texture* button_third = LoadTextureFromFile("images\\settings.png");
	SDL_Texture* button_fourth = LoadTextureFromFile("images\\gameexit.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0});

	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;

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
					if (previous)
					{
						mode = 5;
					}
					else
					{
						mode = 6;
					}

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(Page.SecondTypeButton, event.button.x, event.button.y))
				{
					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);

					Rules(mode, Page, window, SettingsData);
					if (mode == 0)
					{
						quit = true;
						mode = 0;						
					}			
				}					
				if (ButtonClick(Page.ThirdTypeButton, event.button.x, event.button.y))
				{
					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);

					Settings(mode, Page, window, SettingsData);
					if (mode == 0)
					{
						mode = 0;
						quit = true;
					}
				}
				if (ButtonClick(Page.FourthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					mode = 2;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
			}
			break;
		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width , Page.UnderlineIndent + i);
		
		SDL_RenderCopy(ren, button_first, NULL, &Page.FirstTypeButton);
		SDL_RenderCopy(ren, button_second, NULL, &Page.SecondTypeButton);
		SDL_RenderCopy(ren, button_third, NULL, &Page.ThirdTypeButton);
		SDL_RenderCopy(ren, button_fourth, NULL, &Page.FourthTypeButton);
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

void ExitMenu(int& mode, Appearance Page, Proportions window, Control& SettingsData)
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


	char theme[] = u8"����� �� ����";
	char message[] = u8"�� ������������� ������ ����� �� ����?";
	SDL_Rect heading_rect;
	SDL_Rect message_rect;

	SDL_Texture* button_fifth = LoadTextureFromFile("images\\exit.png");
	SDL_Texture* button_sixth = LoadTextureFromFile("images\\back.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* announcement = GenerateTextureFromText(message, SanFrancisco, &message_rect, { 255, 255, 255, 0 });

	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
	message_rect.x = window.width / 2 - message_rect.w / 2;
	message_rect.y = Page.FifthTypeButton.y / 2 + Page.UnderlineIndent + ut - message_rect.h / 2;

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

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(Page.SixthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					mode = 1;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				break;
			}

		}

		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

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

void Rules(int& mode, Appearance Page, Proportions window, Control& SettingsData)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	
	char theme[] = u8"������� ����";
	SDL_Rect heading_rect;
	/*SDL_Rect rules_rect = { cv * 2, Page.UnderlineIndent + ut + cv, (window.height - Page.UnderlineIndent - ut - cv * 2) / 9 * 16, window.height - Page.UnderlineIndent - ut - cv * 2 };*/
	SDL_Rect rules2_rect = { cv, window.height / 2 - (window.width - cv * 2) / 13 * 5 / 2 + (Page.UnderlineIndent + ut + cv) / 2, window.width - cv * 2, (window.width - cv * 2) / 13 * 5 };
	
	SDL_Texture* stepback = LoadTextureFromFile("images\\stepback.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* rules = LoadTextureFromFile("images\\rules2.png");

	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;

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
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					quit = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.StepBack, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				break;
			}
		}

		
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, stepback, NULL, &Page.StepBack);
		SDL_RenderCopy(ren, rules, NULL, &rules2_rect);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(rules);
	SDL_DestroyTexture(stepback);
	SDL_DestroyTexture(heading);

	TTF_CloseFont(Franklin);
} 

void Settings(int& mode, Appearance Page, Proportions window, Control& SettingsData)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	int TextFontSize = window.height / 17;
	TTF_Font* SanFrancisco = TTF_OpenFont("fonts\\San Francisco.ttf", TextFontSize);
	if (SanFrancisco == NULL)
	{
		cout << "Couldn't open TTF: San Francisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"���������";
	char chapter1text[] = u8"����";
	char chapter2text[] = u8"�������";
	char volumetext[] = u8"����� ���������";
	char musictext[] = u8"������";
	char soundstext[] = u8"������� �����";
	char action1[] = u8"�����                                  Esc";

	int increase = TextFontSize / 2;

	SDL_Rect heading_rect;
	SDL_Rect chapter1_rect;
	SDL_Rect chapter2_rect;
	SDL_Rect volume_rect;
	SDL_Rect music_rect;
	SDL_Rect sounds_rect;
	SDL_Rect to_back_rect;
	SDL_Rect switchbutton1_rect;
	SDL_Rect switchbutton2_rect;
	SDL_Rect plus_rect;
	SDL_Rect minus_rect;
	SDL_Rect bar_rect;
	SDL_Rect loudness_rect;


	SDL_Texture* stepback = LoadTextureFromFile("images\\stepback.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* chapter1 = GenerateTextureFromText(chapter1text, SanFrancisco, &chapter1_rect, { r, g, b, 0 });
	SDL_Texture* chapter2 = GenerateTextureFromText(chapter2text, SanFrancisco, &chapter2_rect, { r, g, b, 0 });
	SDL_Texture* volume = GenerateTextureFromText(volumetext, SanFrancisco, &volume_rect, { 255, 255, 255, 0 });
	SDL_Texture* music = GenerateTextureFromText(musictext, SanFrancisco, &music_rect, { 255, 255, 255, 0 });
	SDL_Texture* sounds = GenerateTextureFromText(soundstext, SanFrancisco, &sounds_rect, { 255, 255, 255, 0 });
	SDL_Texture* to_back = GenerateTextureFromText(action1, SanFrancisco, &to_back_rect, { 255, 255, 255, 0 });
	SDL_Texture* switchbutton1 = NULL;
	SDL_Texture* switchbutton2 = NULL;
	SDL_Texture* minus = LoadTextureFromFile("images\\m.png");
	SDL_Texture* plus = LoadTextureFromFile("images\\p.png");
	
	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
	chapter1_rect.x = window.width / 3 / 2 - cv * 2;
	chapter1_rect.y = Page.UnderlineIndent + cv * 2 + heading_rect.h / 2;
	volume_rect.x = window.width / 3 + cv * 2;
	volume_rect.y = Page.UnderlineIndent + cv * 2 + volume_rect.h / 2;
	music_rect.x = volume_rect.x;
	music_rect.y = volume_rect.y + music_rect.h;
	sounds_rect.x = music_rect.x;
	sounds_rect.y = music_rect.y + sounds_rect.h;
	chapter2_rect.x = window.width / 3 / 2 - cv * 2;
	chapter2_rect.y = sounds_rect.y + heading_rect.h * 2;
	to_back_rect.x = window.width / 3 + cv * 2;
	to_back_rect.y = chapter2_rect.y;
	switchbutton1_rect = { window.width / 3 * 2, music_rect.y, music_rect.h * 2, music_rect.h };
	switchbutton2_rect = { window.width / 3 * 2, sounds_rect.y, sounds_rect.h * 2, sounds_rect.h };

	minus_rect = { window.width / 3 * 2 , volume_rect.y, TextFontSize, TextFontSize };
	plus_rect = { minus_rect.x + minus_rect.w + TextFontSize * 5 + ut * 2, volume_rect.y, TextFontSize, TextFontSize };
	loudness_rect = { minus_rect.x + minus_rect.w + ut * 2, volume_rect.y + 3, increase * SettingsData.Volume - 3, TextFontSize - 6 };

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
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					quit = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ButtonClick(Page.StepBack, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(switchbutton1_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					if (SettingsData.Music == 1)
					{
						SettingsData.Music = 0;
					}
					else
					{
						SettingsData.Music = 1;
					}

					if (Mix_PlayingMusic() == 0)
					{
						PlayMusic(BackgroundMusic, SettingsData.Volume);
					}
					else
					{
						if (Mix_PausedMusic() == 1)
						{
							Mix_ResumeMusic();
						}
						else
						{
							Mix_PauseMusic();
						}
					}

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(switchbutton2_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{

					if (SettingsData.Sounds == 1)
					{
						SettingsData.Sounds = 0;
					}
					else
					{
						SettingsData.Sounds = 1;
					}

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(minus_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					if (SettingsData.Volume != 0)
					{
						loudness_rect.w -= increase;
						SettingsData.Volume--;
						Mix_VolumeMusic(SettingsData.Volume * 12);
						Mix_VolumeChunk(sound, SettingsData.Volume * 12);
					}
				}
				if (ButtonClick(plus_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					if (loudness_rect.w + increase <= TextFontSize * 5)
					{
						loudness_rect.w += increase;
						SettingsData.Volume++;
						Mix_VolumeMusic(SettingsData.Volume * 12);
						Mix_VolumeChunk(sound, SettingsData.Volume * 12);
					}
				}
				break;
			}

		}

		//������
		if (SettingsData.Music == 1)
		{
			if (switchbutton1 != NULL)
			{
				SDL_DestroyTexture(switchbutton1);
			}
			switchbutton1 = LoadTextureFromFile("images\\switchon.png");
		}
		else
		{
			if (switchbutton1 != NULL)
			{
				SDL_DestroyTexture(switchbutton1);
			}
			switchbutton1 = LoadTextureFromFile("images\\switchoff.png");
		}
		//�����
		if (SettingsData.Sounds == 1)
		{
			if (switchbutton2!= NULL)
			{
				SDL_DestroyTexture(switchbutton2);
			}
			switchbutton2 = LoadTextureFromFile("images\\switchon.png");
		}
		else
		{
			if (switchbutton2 != NULL)
			{
				SDL_DestroyTexture(switchbutton2);
			}
			switchbutton2 = LoadTextureFromFile("images\\switchoff.png");
		}


		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
		{
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);
		}
		SDL_RenderFillRect(ren, &loudness_rect);

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
		for (int i = 0; i < ut; i++)
		{
			bar_rect = { minus_rect.x + minus_rect.w + ut + i, volume_rect.y + i, TextFontSize * 5 - 2 * i, TextFontSize - 2 * i };

			SDL_RenderDrawRect(ren, &bar_rect);
		}

		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, stepback, NULL, &Page.StepBack);
		SDL_RenderCopy(ren, chapter1, NULL, &chapter1_rect);
		SDL_RenderCopy(ren, chapter2, NULL, &chapter2_rect);
		SDL_RenderCopy(ren, volume, NULL, &volume_rect);
		SDL_RenderCopy(ren, music, NULL, &music_rect);
		SDL_RenderCopy(ren, sounds, NULL, &sounds_rect);
		SDL_RenderCopy(ren, to_back, NULL, &to_back_rect);
		SDL_RenderCopy(ren, switchbutton1, NULL, &switchbutton1_rect);
		SDL_RenderCopy(ren, switchbutton2, NULL, &switchbutton2_rect);
		SDL_RenderCopy(ren, minus, NULL, &minus_rect);
		SDL_RenderCopy(ren, plus, NULL, &plus_rect);

		SDL_RenderPresent(ren);
	}

	SDL_DestroyTexture(stepback);
	SDL_DestroyTexture(heading);
	SDL_DestroyTexture(chapter1);
	SDL_DestroyTexture(chapter2);
	SDL_DestroyTexture(volume);
	SDL_DestroyTexture(music);
	SDL_DestroyTexture(sounds);
	SDL_DestroyTexture(to_back);
	SDL_DestroyTexture(switchbutton1);
	SDL_DestroyTexture(switchbutton2);
	SDL_DestroyTexture(minus);
	SDL_DestroyTexture(plus);

	TTF_CloseFont(Franklin);
	TTF_CloseFont(SanFrancisco);

	//���������� � ����
	PrintSettingsFile(SettingsData);
}

void PauseMenu(int& mode, Appearance Page, Proportions window, Control& SettingsData)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\San Francisco.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: sanfrancisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}

	char theme[] = u8"���� �����";
	SDL_Rect heading_rect;

	SDL_Texture* button_first = LoadTextureFromFile("images\\continue.png");
	SDL_Texture* button_second = LoadTextureFromFile("images\\settings.png");
	SDL_Texture* button_third = LoadTextureFromFile("images\\gamerules.png");
	SDL_Texture* button_fourth = LoadTextureFromFile("images\\menuexit.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });

	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;

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

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(Page.SecondTypeButton, event.button.x, event.button.y))
				{
					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);

					Settings(mode, Page, window, SettingsData);
					if (mode == 0)
					{
						quit = true;
					}
				}
				if (ButtonClick(Page.ThirdTypeButton, event.button.x, event.button.y))
				{
					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);

					Rules(mode, Page, window, SettingsData);
					if (mode == 0)
					{
						quit = true;
					}
				}
				if (ButtonClick(Page.FourthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					mode = 1;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
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

void Identification(int& mode, Appearance Page, Proportions window, Control& SettingsData, Elements& GameProgress)
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

	char theme[] = u8"����� �������";
	char message[] = u8"������� ����� ������� (����������� ������ ���������� �������):";
	char warning1[] = u8"� ������ �������� ����� ����� �������������";
	char warning2[] = u8"������ ������� ����� �� ���������: Player 1 � Player 2";
	char user1[] = u8"1 �����";
	char user2[] = u8"2 �����";
	SDL_Rect heading_rect;
	SDL_Rect nameinput_rect;
	SDL_Rect notice1_rect;
	SDL_Rect notice2_rect;
	SDL_Rect player1_rect;
	SDL_Rect player2_rect;

	SDL_Texture* stepback = LoadTextureFromFile("images\\stepback.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* skip = NULL;
	SDL_Texture* nameinput = GenerateTextureFromText(message, SanFrancisco, &nameinput_rect, {255, 255, 255, 0});
	SDL_Texture* notice1 = GenerateTextureFromText(warning1, SanFrancisco, &notice1_rect, { 255, 255, 255, 0 });
	SDL_Texture* notice2 = GenerateTextureFromText(warning2, SanFrancisco, &notice2_rect, { 255, 255, 255, 0 });
	SDL_Texture* player1 = GenerateTextureFromText(user1, SanFrancisco, &player1_rect, { r, g, b, 0 });
	SDL_Texture* player2 = GenerateTextureFromText(user2, SanFrancisco, &player2_rect, { r, g, b, 0 });

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
	player2_rect.y = Page.UnderlineIndent * 4;


	int MaxInput = 11;
	int InputIndex1 = 1;
	int InputIndex2 = 1;
	char text[2] = u8"|";
	bool exit = false;
	bool StartInput1 = false, StartInput2 = false;
	bool EndInput1 = false, EndInput2 = false;

	SDL_Rect gambler1_rect;
	SDL_Rect gambler2_rect;
	SDL_Rect InputText1_rect;
	SDL_Rect InputText2_rect;

	SDL_Texture* gambler1 = NULL;
	SDL_Texture* gambler2 = NULL;

	char InputText1[ams] = u8" ";
	char InputText2[ams] = u8" ";

	gambler1_rect = { window.width / 5 + cv, player1_rect.y + player1_rect.h / 2 - TextFontSize / 2, 0, 0 };
	gambler2_rect = { window.width / 5 + cv, player2_rect.y + player2_rect.h / 2 - TextFontSize / 2, 0, 0 };
	
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

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(Page.StepBack, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					quit = true;
					mode = 1;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(InputText1_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					StartInput1 = true;
					EndInput1 = false;
					StartInput2 = false;
				}
				if (ButtonClick(InputText2_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					StartInput2 = true;
					EndInput2 = false;
					StartInput1 = false;
				}
				if (!ButtonClick(InputText1_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					StartInput1 = false;
					EndInput1 = true;
				}
				if (!ButtonClick(InputText2_rect, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT)
				{
					StartInput2 = false;
					EndInput2 = true;
				}
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					quit = true;
					mode = 1;
				}
				if (StartInput1)
				{
					if (event.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (InputIndex1 != 1)
						{
							InputText1[InputIndex1 - 1] = '\0';
							InputIndex1 -= 1;
							if (gambler1 != NULL)
							{
								SDL_DestroyTexture(gambler1);
							}
							gambler1 = GenerateTextureFromText(InputText1, SanFrancisco, &gambler1_rect, { 255, 255, 255, 255});
						}
					}
				}
				if (StartInput2)
				{
					if (event.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (InputIndex2 != 1)
						{
							InputText2[InputIndex2 - 1] = '\0';
							InputIndex2 -= 1;
							if (gambler2 != NULL)
							{
								SDL_DestroyTexture(gambler2);
							}
							gambler2 = GenerateTextureFromText(InputText2, SanFrancisco, &gambler2_rect, { 255, 255, 255, 255 });
						}
					}
				}
				break;
			case SDL_TEXTINPUT:
				if (StartInput1)
				{
					if (InputIndex1 < MaxInput)
					{
						SDL_TextInputEvent textinput;
						textinput.text[0] = event.text.text[0];
						if (textinput.text[0] == '_' || (textinput.text[0] >= 'A' && textinput.text[0] <= 'Z') || (textinput.text[0] >= 'a' && textinput.text[0] <= 'z') || textinput.text[0] == ' ' || ((textinput.text[0] >= '0' && textinput.text[0] <= '9')))
						{
							InputText1[InputIndex1] = textinput.text[0];
							InputIndex1 += 1;
							InputText1[InputIndex1] = '\0';
							if (gambler1 != NULL)
							{
								SDL_DestroyTexture(gambler1);
							}
							gambler1 = GenerateTextureFromText(InputText1, SanFrancisco, &gambler1_rect, { 255, 255, 255, 255 });
						}
					}
				}
				if (StartInput2)
				{
					if (InputIndex2 < MaxInput)
					{
						SDL_TextInputEvent textinput;
						textinput.text[0] = event.text.text[0];
						if (textinput.text[0] == '_' || (textinput.text[0] >= 'A' && textinput.text[0] <= 'Z') || (textinput.text[0] >= 'a' && textinput.text[0] <= 'z') || textinput.text[0] == ' ' || ((textinput.text[0] >= '0' && textinput.text[0] <= '9')))
						{
							InputText2[InputIndex2] = textinput.text[0];
							InputIndex2 += 1;
							InputText2[InputIndex2] = '\0';
							if (gambler2 != NULL)
							{
								SDL_DestroyTexture(gambler2);
							}
							gambler2 = GenerateTextureFromText(InputText2, SanFrancisco, &gambler2_rect, { 255, 255, 255, 255 });
						}
					}
				}
				break;
			}
			

		}

		if (!strcmp(InputText1, " ") && !strcmp(InputText2, " "))
		{
			if (skip != NULL)
			{
				SDL_DestroyTexture(skip);
			}
			skip = LoadTextureFromFile("images\\skip.png");
		}
		else
		{
			if (skip != NULL)
			{
				SDL_DestroyTexture(skip);
			}
			skip = LoadTextureFromFile("images\\next.png");
		}
		


		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 0);
		for (int i = 0; i < ut; i++)
		{
			InputText1_rect = { window.width / 5 + i, player1_rect.y + player1_rect.h / 2 - Page.HeadingFontSize / 2 + i, window.width / 5 * 3 - 2 * i, Page.HeadingFontSize - 2 * i };
			InputText2_rect = { window.width / 5 + i, player2_rect.y + player2_rect.h / 2 - Page.HeadingFontSize / 2 + i, window.width / 5 * 3 - 2 * i, Page.HeadingFontSize - 2 * i };

			SDL_RenderDrawRect(ren, &InputText1_rect);
			SDL_RenderDrawRect(ren, &InputText2_rect);
		}


		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);


		SDL_RenderCopy(ren, player1, NULL, &player1_rect);
		SDL_RenderCopy(ren, player2, NULL, &player2_rect);
		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, nameinput, NULL, &nameinput_rect);
		SDL_RenderCopy(ren, notice1, NULL, &notice1_rect);
		SDL_RenderCopy(ren, notice2, NULL, &notice2_rect);
		SDL_RenderCopy(ren, stepback, NULL, &Page.StepBack);
		SDL_RenderCopy(ren, skip, NULL, &Page.SeventhTypeButton);
 		SDL_RenderCopy(ren, gambler1, NULL, &gambler1_rect);
		SDL_RenderCopy(ren, gambler2, NULL, &gambler2_rect);
		

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
	SDL_DestroyTexture(gambler1);
	SDL_DestroyTexture(gambler2);

	TTF_CloseFont(SanFrancisco);
	TTF_CloseFont(Franklin);

	if (strcmp(InputText1, " ") && EndInput1)
	{
		strcpy_s(GameProgress.Gambler1, InputText1);
	}
	else
	{
		strcpy_s(GameProgress.Gambler1, " Player 1");
	}

	if (strcmp(InputText2, " ") && EndInput2)
	{
		strcpy_s(GameProgress.Gambler2, InputText2);
	}
	else
	{
		strcpy_s(GameProgress.Gambler2, " Player 2");
	}
}

void CheckPoint(int& mode, Appearance Page, Proportions window, Control& SettingsData, bool& previous)
{
	TTF_Font* Franklin = TTF_OpenFont("fonts\\Franklin.ttf", Page.HeadingFontSize);
	if (Franklin == NULL)
	{
		cout << "Couldn't open TTF: Franklin! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
	int TextFontSize = window.height / 14;
	TTF_Font* SanFrancisco = TTF_OpenFont("fonts\\San Francisco.ttf", TextFontSize);
	if (SanFrancisco == NULL)
	{
		cout << "Couldn't open TTF: San Francisco! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}


	char theme[] = u8"���� ����������";
	char message[] = u8"� ��� ���� �������������  ����, ������ ����������?";
	SDL_Rect heading_rect;
	SDL_Rect message_rect;

	SDL_Texture* button_fifth = LoadTextureFromFile("images\\newgame.png");
	SDL_Texture* button_sixth = LoadTextureFromFile("images\\continue.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* announcement = GenerateTextureFromText(message, SanFrancisco, &message_rect, { 255, 255, 255, 0 });

	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
	message_rect.x = window.width / 2 - message_rect.w / 2;
	message_rect.y = Page.FifthTypeButton.y / 2 + Page.UnderlineIndent + ut - message_rect.h / 2;

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
					previous = false;
					mode = 6;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(Page.SixthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					previous = true;
					mode = 7;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				break;
			}

		}


		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

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

void Ending(int& mode, Appearance Page, Proportions window, Control& SettingsData, bool& previous)
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


	char theme[] = u8"��������� ����";
	char message[] = u8"���� �����������, �������� ��������.";
	SDL_Rect heading_rect;
	SDL_Rect message_rect;

	SDL_Texture* button_fifth = LoadTextureFromFile("images\\menuexit.png");
	SDL_Texture* button_sixth = LoadTextureFromFile("images\\again.png");
	SDL_Texture* heading = GenerateTextureFromText(theme, Franklin, &heading_rect, { 255, 255, 255, 0 });
	SDL_Texture* announcement = GenerateTextureFromText(message, SanFrancisco, &message_rect, { 255, 255, 255, 0 });

	heading_rect.x = window.width / 2 - heading_rect.w / 2;
	heading_rect.y = Page.UnderlineIndent / 2 - heading_rect.h / 2;
	message_rect.x = window.width / 2 - message_rect.w / 2;
	message_rect.y = Page.FifthTypeButton.y / 2 + Page.UnderlineIndent + ut - message_rect.h / 2;

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
					mode = 1;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				if (ButtonClick(Page.SixthTypeButton, event.button.x, event.button.y))
				{
					quit = true;
					mode = 7;
					previous = false;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				break;
			}

		}
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, r, g, b, 0);
		for (int i = 0; i < ut; i++)
			SDL_RenderDrawLine(ren, 0, Page.UnderlineIndent + i, window.width, Page.UnderlineIndent + i);

		SDL_RenderCopy(ren, heading, NULL, &heading_rect);
		SDL_RenderCopy(ren, announcement, NULL, &message_rect);
		SDL_RenderCopy(ren, button_fifth, NULL, &Page.FifthTypeButton);
		SDL_RenderCopy(ren, button_sixth, NULL, &Page.SixthTypeButton);

		SDL_RenderPresent(ren);
	}
}

void Play(int& mode, Proportions window, Zones Game, Elements& GameProgress, Appearance Page, Control &SettingsData, bool &previous)
{
	if (!previous)
	{
		Reset(GameProgress);
	}

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

	
	//��������� 

	#pragma region HP
	//��������������� ���������� ��� ���������� ���� ���
	//������ ����
	int width = window.width;
	//������ ���� 
	int height = window.height;
	//������ ���� ���� / ���� / ����������
	int infwidth = window.width / 8 * 3;
	//������ ���� ��������� / �����������
	int midwidth = window.width / 8 * 2;
	//������ ���� ���������
	int tipsheight = window.height / 2;
	//������ ���� ���� / ���������� 
	int infheight = window.height / 10;
	//������ ���� �����
	int scorewidth = (midwidth - infheight) / 2 + ut * 2;
	//������ ���� �����������
	int resultsheight = tipsheight - infheight;
	//������ ���� ������ ������
	int throwwidth = infwidth / 5 * 2;
	//������ ���� ������ ������� ����
	int skipwidth = infwidth - throwwidth + ut;
	//������ ���� ����
	int playheight = height - infheight * 3;
#pragma endregion

	#pragma region Static
	char to_throw[] = u8"�������";
	char to_skip[] = u8"���������� ���";
	char tips_l1[] = u8"����������:";
	char tips_l2[] = u8"���� ����";
	char tips_l3[] = u8"��� ����";
	char tips_l4[] = u8"��� ����������";
	char tips_l5[] = u8"������ ��� (3+2)";
	char tips_l6[] = u8"������ ����������";
	char tips_l7[] = u8"���� ����������";
	char isleading[] = u8"*�����";

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

	gamblername1_rect.x = Game.Name1.x;
	gamblername1_rect.y = Game.Name1.y + ut;
	gamblername2_rect.x = Game.Name2.x;
	gamblername2_rect.y = Game.Name2.y + ut;

	lead_rect.y = Game.Name1.y + Game.Name1.h / 2 - lead_rect.h / 2 - ut;
#pragma endregion 

	//������������
	//��������
	bool animation = false;
	//������ ���������
	bool down = false;
	//��������� �����
	bool freeze = true;
	//����� ����
	bool end = false;
	//����� ������
	bool flag = false;
	//����� �������
	bool change = false;
	//�����
	bool draw = false;
	//��������������� ��� ������
	int pt;
	//��� ��������
	int i = 0;
	//������� ����
	char phasetext[ams];
	//�������� ����������
	int Compare[2];


	if (GameProgress.Round == 0)
	{
		strcpy_s(phasetext, u8"����������");
	}
	else
	{
		pt = GameProgress.Round;
		_itoa_s(pt, phasetext, 10);
		strcat_s(phasetext, u8" �����");
	}

	if (GameProgress.LeadSwap == 1 && GameProgress.LeadThrows == 0)
	{
		Compare[0] = FindBestCombination(GameProgress);
	}

	char points1[ams];
	char points2[ams];
	_itoa_s(GameProgress.p1, points1, 10);
	_itoa_s(GameProgress.p2, points2, 10);

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

	SDL_Texture* phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 });
	SDL_Texture* outcome = GenerateTextureFromText(GameProgress.OutcomeText, Franklin, &outcome_rect, { 255, 255, 255, 0 });
	SDL_Texture* final = GenerateTextureFromText(GameProgress.FinalText, Franklin, &final_rect, { 255, 255, 255, 0 });
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
	phase_rect.y = Game.Results.y + phase_rect.h / 2;
	outcome_rect.x = Game.Results.x + Game.Results.w / 2 - outcome_rect.w / 2;
	outcome_rect.y = Game.Results.y + Game.Results.h / 3 + outcome_rect.h / 2;
	final_rect.x = Game.Results.x + Game.Results.w / 2 - final_rect.w / 2;
	final_rect.y = Game.Results.y + Game.Results.h / 3 * 2 + final_rect.h / 2;

	combo1_rect.x = Game.Combinations1.x + cv;
	combo1_rect.y = Game.Combinations1.y + Game.Combinations1.h / 2 - combo1_rect.h / 2;
	combo2_rect.x = Game.Combinations2.x + cv;
	combo2_rect.y = Game.Combinations2.y + Game.Combinations2.h / 2 - combo2_rect.h / 2;
	

	SDL_Surface* surface = IMG_Load("images\\glass.png");
	glass_rect = { 0, 0, surface->w, surface->h };
	SDL_FreeSurface(surface);

	SDL_Rect glass_recttmp = glass_rect;
	SDL_Rect gameglasscopy = Game.Glass;

	if (GameProgress.Queue == 1)
	{
		gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
	}
	else if (GameProgress.Queue == 2)
	{
		gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
	}
	gameglasscopy.h = 0;
	gameglasscopy.y = infheight;

	SDL_Event event;
	bool quit = false;

	//============================================================================
	//��������� �������
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
				if (end)
				{
					mode = 4;
					quit = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE && !end && animation == false)
				{
					PauseMenu(mode, Page, window, SettingsData);
					if (mode == 1 || mode == 0)
						quit = true;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (end)
				{
					mode = 4;
					quit = true;
				}
				if (ButtonClick(Game.PauseButton, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && animation == false)
				{
					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);

					PauseMenu(mode, Page, window, SettingsData);
					if (mode == 1 || mode == 0)
						quit = true;
				}
				//first throw button
				if (ButtonClick(Game.ThrowButton1, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 1 && animation == false && end == false)
				{
					if (GameProgress.GameStatus == 0)
					{
						GameProgress.GameStatus = 1;
					}

					animation = true;

					if (GameProgress.LeadSwap == 2)
					{
						if (GameProgress.Round == 0 && draw)
						{
							GameProgress.ThrowMax = 1;
						}
						else
						{
							GameProgress.ThrowMax = 3;
						}
						GameProgress.LeadSwap = 0;
						GameProgress.LeadThrows = 0;
						flag = true;
					}
					else
					{
						freeze = false;
					}
					
					GameProgress.ThrowMax--;
					GameProgress.LeadThrows++;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				//second throw butoon
				if (ButtonClick(Game.ThrowButton2, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 2 && animation == false && end == false)
				{
					if (GameProgress.GameStatus == 0)
					{
						GameProgress.GameStatus = 1;
					}

					animation = true;

					if (GameProgress.LeadSwap == 2)
					{
						if (GameProgress.Round == 0 && draw)
						{
							GameProgress.ThrowMax = 1;
						}
						else
						{
							GameProgress.ThrowMax = 3;
						}
						GameProgress.LeadSwap = 0;
						GameProgress.LeadThrows = 0;
						flag = true;
					}
					else
					{
						freeze = false;
					}
					
					GameProgress.ThrowMax--;
					GameProgress.LeadThrows++;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				//first skip button
				if (ButtonClick(Game.SkipButton1, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 1 && animation == false && GameProgress.LeadThrows != 0 && GameProgress.LeadSwap < 2 && end == false)
				{
					change = true;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				//second skip button
				if (ButtonClick(Game.SkipButton2, event.button.x, event.button.y) && event.button.button == SDL_BUTTON_LEFT && GameProgress.Queue == 2 && animation == false && GameProgress.LeadThrows != 0 && GameProgress.LeadSwap < 2  && end == false)
				{
					change = true;

					if (SettingsData.Sounds != 0)
						PlayActSound(ClickSound, SettingsData.Volume);
				}
				break;
			}
		}

		//==============================================================================
		//������
		//����
  		if (animation == false)
		{
			if (GameProgress.LeadSwap == 0)
			{
				if (GameProgress.ThrowMax <= 0 || change == true)
				{
					if (GameProgress.Queue == 1)
					{
						GameProgress.Queue = 2;
						gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
					}
					else if (GameProgress.Queue == 2)
					{
						GameProgress.Queue = 1;
						gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
					}

					GameProgress.LeadSwap++;
					GameProgress.ThrowMax = GameProgress.LeadThrows;
					GameProgress.LeadThrows = 0;
				}
				change = false;
			}
			else if (GameProgress.LeadSwap == 1 && freeze == false)
			{
				if (GameProgress.ThrowMax <= 0 || change == true)
				{
					if (GameProgress.Round == 0)
					{
						strcpy_s(GameProgress.OutcomeText, u8" ");
						strcpy_s(GameProgress.FinalText, u8" ");

						if (Compare[0] > Compare[1])
						{
							strcpy_s(GameProgress.OutcomeText, u8"�������");
							strcpy_s(GameProgress.FinalText, GameProgress.Gambler1);

							GameProgress.Queue = 1;
							gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
						}
						else if (Compare[0] < Compare[1])
						{
							strcpy_s(GameProgress.OutcomeText, u8"�������");
							strcpy_s(GameProgress.FinalText, GameProgress.Gambler2);

							GameProgress.Queue = 2;
							gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
						}
						else if (Compare[0] == Compare[1])
						{
							strcpy_s(GameProgress.OutcomeText, u8"�����");
							strcpy_s(GameProgress.FinalText, u8"�����������");

							draw = true;

							GameProgress.Queue = 1;
							gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
						}

						strcpy_s(phasetext, " ");
						strcpy_s(phasetext, u8"����������");

						if (phase != NULL)
						{
							SDL_DestroyTexture(phase);
						}
						if (outcome != NULL)
						{
							SDL_DestroyTexture(outcome);
						}
						if (final != NULL)
						{
							SDL_DestroyTexture(final);
						}
		
						phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 });
						outcome = GenerateTextureFromText(GameProgress.OutcomeText, Franklin, &outcome_rect, { 255, 255, 255, 0 });
						final = GenerateTextureFromText(GameProgress.FinalText, Franklin, &final_rect, { 255, 255, 255, 0 });

						phase_rect.x = Game.Results.x + Game.Results.w / 2 - phase_rect.w / 2;
						outcome_rect.x = Game.Results.x + Game.Results.w / 2 - outcome_rect.w / 2;
						final_rect.x = Game.Results.x + Game.Results.w / 2 - final_rect.w / 2;

						GameProgress.LeadSwap++;
					}
					else
					{
						strcpy_s(GameProgress.OutcomeText, u8" ");
						strcpy_s(GameProgress.FinalText, " ");

						if (Compare[0] > Compare[1])
						{
							strcpy_s(phasetext, " ");
							GameProgress.p1++;
							if (GameProgress.p1 >= 3)
							{
								strcpy_s(phasetext, GameProgress.Gambler1);
								strcpy_s(GameProgress.OutcomeText, u8"������� ������");
								sprintf_s(GameProgress.FinalText, u8"�� ������ %d : %d", GameProgress.p1, GameProgress.p2);
								
								GameProgress.GameStatus = 0;
								end = true;
							}
							else
							{
								pt = GameProgress.Round;
								_itoa_s(pt, phasetext, 10);
								strcat_s(phasetext, u8" �����");
								strcpy_s(GameProgress.OutcomeText, u8"�������");
								strcpy_s(GameProgress.FinalText, GameProgress.Gambler1);

								if (GameProgress.Queue == 1)
								{
									GameProgress.Queue = 1;
									gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
								}
								else if (GameProgress.Queue == 2)
								{
									GameProgress.Queue = 2;
									gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
								}
							}

							if (outcome != NULL)
							{
								SDL_DestroyTexture(outcome);
							}
							if (final != NULL)
							{
								SDL_DestroyTexture(final);
							}
							if (score1 != NULL)
							{
								SDL_DestroyTexture(score1);
							}
							if (phase != NULL)
							{
								SDL_DestroyTexture(phase);
							}

							_itoa_s(GameProgress.p1, points1, 10);

							phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 });
							outcome = GenerateTextureFromText(GameProgress.OutcomeText, Franklin, &outcome_rect, { 255, 255, 255, 0 });
							final = GenerateTextureFromText(GameProgress.FinalText, Franklin, &final_rect, { 255, 255, 255, 0 });
							score1 = GenerateTextureFromText(points1, Franklin, &score1_rect, { 255, 255, 255, 0 });

							phase_rect.x = Game.Results.x + Game.Results.w / 2 - phase_rect.w / 2;
							outcome_rect.x = Game.Results.x + Game.Results.w / 2 - outcome_rect.w / 2;
							final_rect.x = Game.Results.x + Game.Results.w / 2 - final_rect.w / 2;		
							score1_rect.x = Game.Score1.x + Game.Score1.w / 2 - score1_rect.w / 2;
						}
						else if (Compare[0] < Compare[1])
						{
							GameProgress.p2++;
							strcpy_s(phasetext, " ");
							if (GameProgress.p2 >= 3)
							{
								
								strcpy_s(phasetext, GameProgress.Gambler2);
								strcpy_s(GameProgress.OutcomeText, u8"������� ������");
								sprintf_s(GameProgress.FinalText, u8"�� ������ %d : %d", GameProgress.p2, GameProgress.p1);
							
								GameProgress.GameStatus = 0;
								end = true;
							}
							else 
							{
								pt = GameProgress.Round;
								_itoa_s(pt, phasetext, 10);
								strcat_s(phasetext, u8" �����");
								strcpy_s(GameProgress.OutcomeText, u8"�������");
								strcpy_s(GameProgress.FinalText, GameProgress.Gambler2);
								
								if (GameProgress.Queue == 1)
								{
									GameProgress.Queue = 1;
									gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
								}
								else if (GameProgress.Queue == 2)
								{
									GameProgress.Queue = 2;
									gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
								}
							}

							if (outcome != NULL)
							{
								SDL_DestroyTexture(outcome);
							}
							if (final != NULL)
							{
								SDL_DestroyTexture(final);
							}
							if (score2 != NULL)
							{
								SDL_DestroyTexture(score2);
							}
							if (phase != NULL)
							{
								SDL_DestroyTexture(phase);
							}

							_itoa_s(GameProgress.p2, points2, 10);
							
							phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 });
							outcome = GenerateTextureFromText(GameProgress.OutcomeText, Franklin, &outcome_rect, { 255, 255, 255, 0 });
							final = GenerateTextureFromText(GameProgress.FinalText, Franklin, &final_rect, { 255, 255, 255, 0 });
							score2 = GenerateTextureFromText(points2, Franklin, &score2_rect, { 255, 255, 255, 0 });

							phase_rect.x = Game.Results.x + Game.Results.w / 2 - phase_rect.w / 2;
							outcome_rect.x = Game.Results.x + Game.Results.w / 2 - outcome_rect.w / 2;
							final_rect.x = Game.Results.x + Game.Results.w / 2 - final_rect.w / 2;
							score2_rect.x = Game.Score2.x + Game.Score2.w / 2 - score2_rect.w / 2;
						}
						else if (Compare[0] == Compare[1])
						{
							strcpy_s(phasetext, " ");
							pt = GameProgress.Round;
							_itoa_s(pt, phasetext, 10);
							strcat_s(phasetext, u8" �����");
							strcpy_s(GameProgress.OutcomeText, u8"�����");
							strcpy_s(GameProgress.FinalText, u8"�����������");

							if (phase != NULL)
							{
								SDL_DestroyTexture(phase);
							}
							if (outcome != NULL)
							{
								SDL_DestroyTexture(outcome);
							}
							if (final != NULL)
							{
								SDL_DestroyTexture(final);
							}


							phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 });
							outcome = GenerateTextureFromText(GameProgress.OutcomeText, Franklin, &outcome_rect, { 255, 255, 255, 0 });
							final = GenerateTextureFromText(GameProgress.FinalText, Franklin, &final_rect, { 255, 255, 255, 0 });

							phase_rect.x = Game.Results.x + Game.Results.w / 2 - phase_rect.w / 2;
							outcome_rect.x = Game.Results.x + Game.Results.w / 2 - outcome_rect.w / 2;
							final_rect.x = Game.Results.x + Game.Results.w / 2 - final_rect.w / 2;

							draw = true;

							if (GameProgress.Queue == 1)
							{
								GameProgress.Queue = 2;
								gameglasscopy.x = width - infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
							}
							else if (GameProgress.Queue == 2)
							{
								GameProgress.Queue = 1;
								gameglasscopy.x = infwidth / 2 - (playheight / 12 * 11) / 29 * 19 / 2;
							}
						}

						GameProgress.LeadSwap++;
					}
					freeze = true;
					change = false;
				}
			}
			
		}
		if (flag)
		{
			strcpy_s(GameProgress.OutcomeText, u8" ");
			strcpy_s(GameProgress.FinalText, " ");
			strcpy_s(phasetext, " ");
			strcpy_s(GameProgress.Combo1, " ");
			strcpy_s(GameProgress.Combo2, " ");

			if (!draw)
			{
				GameProgress.Round++;
			}

			if (GameProgress.Round == 0)
			{
				strcpy_s(phasetext, u8"����������");
			}
			else
			{
				pt = GameProgress.Round;
				_itoa_s(pt, phasetext, 10);
				strcat_s(phasetext, u8" �����");
			}

			if (phase != NULL)
			{
				SDL_DestroyTexture(phase);
			}
			if (outcome != NULL)
			{
				SDL_DestroyTexture(outcome);
			}
			if (final != NULL)
			{
				SDL_DestroyTexture(final);
			}
			if (combo1 != NULL)
			{
				SDL_DestroyTexture(combo1);
			}
			if (combo2 != NULL)
			{
				SDL_DestroyTexture(combo2);
			}

			phase = GenerateTextureFromText(phasetext, Franklin, &phase_rect, { 255, 255, 255, 0 });
			outcome = GenerateTextureFromText(GameProgress.OutcomeText, Franklin, &outcome_rect, { 255, 255, 255, 0 });
			final = GenerateTextureFromText(GameProgress.FinalText, Franklin, &final_rect, { 255, 255, 255, 0 });
			combo1 = GenerateTextureFromText(GameProgress.Combo1, SanFrancisco, &combo1_rect, { 255, 255, 255, 0 });
			combo2 = GenerateTextureFromText(GameProgress.Combo2, SanFrancisco, &combo2_rect, { 255, 255, 255, 0 });

			phase_rect.x = Game.Results.x + Game.Results.w / 2 - phase_rect.w / 2;
			outcome_rect.x = Game.Results.x + Game.Results.w / 2 - outcome_rect.w / 2;
			final_rect.x = Game.Results.x + Game.Results.w / 2 - final_rect.w / 2;

			flag = false;
			freeze = false;
			draw = false;
		}

		if (GameProgress.Queue == 1)
			lead_rect.x = Game.Name1.x + Game.Name1.w - lead_rect.w - ut;
		else
			lead_rect.x = Game.Name2.x + Game.Name2.w - lead_rect.w - ut;
		

		SDL_Rect dice1_rect = Game.Dice1;
		SDL_Rect dice2_rect = Game.Dice2;

		if (animation == true)
		{
			if (!down)
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
					PlayActSound(MixingSound, SettingsData.Volume);
					SDL_Delay(500);
					down = true;
				}
			}
			else
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
						strcpy_s(GameProgress.Combo1, " ");
						if (combo1 != NULL)
						{
							SDL_DestroyTexture(combo1);
						}
						combo1 = GenerateCombination(GameProgress, SanFrancisco, &combo1_rect, { 255, 255, 255, 0 }, Compare[0]);
					}
					else
					{
						strcpy_s(GameProgress.Combo2, " ");
						if (combo2 != NULL)
						{
							SDL_DestroyTexture(combo2);
						}
						combo2 = GenerateCombination(GameProgress, SanFrancisco, &combo2_rect, { 255, 255, 255, 0 }, Compare[1]);
					}
					animation = false;
					down = false;
				}
			}
		}

		//==============================================================================
		//���������

		//��������� �������
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
		SDL_RenderClear(ren);

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


		//���������� �������
		SDL_RenderCopy(ren, score1, NULL, &score1_rect);
		SDL_RenderCopy(ren, score2, NULL, &score2_rect);
		SDL_RenderCopy(ren, phase, NULL, &phase_rect);
		SDL_RenderCopy(ren, outcome, NULL, &outcome_rect);
		SDL_RenderCopy(ren, final, NULL, &final_rect);

		//�����
		if (GameProgress.DiceValues[0] != 0)
		{
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


		}
		SDL_RenderCopy(ren, combo1, NULL, &combo1_rect);
		SDL_RenderCopy(ren, combo2, NULL, &combo2_rect);

		if (animation == true)
			SDL_RenderCopy(ren, glass, &glass_recttmp, &gameglasscopy);



		//����� �����
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

		//������� �����
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
	SDL_DestroyTexture(outcome);
	SDL_DestroyTexture(final);
	SDL_DestroyTexture(combo1);
	SDL_DestroyTexture(combo2);

	//��������
	PrintGameProgressDataFile(GameProgress);
	if (GameProgress.GameStatus != 0)
	{
		previous = true;
	}
	else
	{
		previous = false;
	}
}

void PageLayout(Appearance& Page, Proportions window, Zones& Game)
{
	//��������������� ����������
	//������ ����
	int width = window.width;
	//������ ���� 
	int height = window.height;
	//������ ���� ���� / ���� / ����������
	int infwidth = window.width / 8 * 3;
	//������ ���� ��������� / �����������
	int midwidth = window.width / 8 * 2;
	//������ ���� ���������
	int tipsheight = window.height / 2;
	//������ ���� ���� / ���������� 
	int infheight = window.height / 10;
	//������ ���� �����
	int scorewidth = (midwidth - infheight) / 2 + ut * 2;
	//������ ���� �����������
	int resultsheight = tipsheight - infheight;
	//������ ���� ������ ������
	int throwwidth = infwidth / 5 * 2;
	//������ ���� ������ ������� ����
	int skipwidth = infwidth - throwwidth + ut;
	//������ ���� ����
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
	if (surface == NULL)
	{
		cout << "Couldn't load text! Error: " << SDL_GetError();
		system("pause");
		DeInit(1);
	}
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

void ReadSettingsFile(Control &SettingsData)
{
	string container;

	ifstream SFile;
	SFile.open("Settings.txt", ios_base::in);

	if (!SFile.is_open())
		cout << "������ �������� �����! " << endl;
	else
	{
		SFile >> SettingsData.Volume;
		SFile >> SettingsData.Music;
		SFile >> SettingsData.Sounds;
	}

	SFile.close();
}

void PrintSettingsFile(Control& SettingsData)
{
	ofstream SFile;
	SFile.open("Settings.txt", ios_base::out);

	if (!SFile.is_open())
		cout << "������ �������� �����! " << endl;
	else
	{
		SFile << SettingsData.Volume << endl;
		SFile << SettingsData.Music << endl;
		SFile << SettingsData.Sounds << endl;
	}

	SFile.close();
}

void PlayMusic(const char* name, int volume)
{
	fon = Mix_LoadMUS(name);
	if (fon == NULL)
	{
		cout << "Couldn't load fon! Error: " << Mix_GetError();
		system("pause");
		DeInit(1);
	}

	Mix_VolumeMusic(volume * 12);
	Mix_PlayMusic(fon, -1);
}

void PlayActSound(const char* name, int volume)
{
	sound = Mix_LoadWAV(name);
	if (sound == NULL)
	{
		cout << "Couldn't load sound! Error: " << Mix_GetError();
		system("pause");
		DeInit(1);
	}

	Mix_VolumeChunk(sound, volume * 12);
	Mix_PlayChannel(-1, sound, 0);
}