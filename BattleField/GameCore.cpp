#include "general.h"
#include "GameCore.h"

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
	GameProgress.GameStatus = false;
	GameProgress.Round = 0;
	strcpy_s(GameProgress.OutcomeText, " ");
	strcpy_s(GameProgress.FinalText, " ");
	GameProgress.p1 = 0;
	GameProgress.p2 = 0;
	GameProgress.Queue = 1;
	strcpy_s(GameProgress.Combo1, " ");
	strcpy_s(GameProgress.Combo2, " ");
	GameProgress.ThrowMax = 1;
	GameProgress.LeadSwap = 0;
	GameProgress.LeadThrows = 0;
	for (int i = 0; i < dq; i++)
	{
		GameProgress.DiceValues[i] = 0;
	}
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
		GPFile >> GameProgress.ThrowMax;
		GPFile >> GameProgress.LeadSwap;
		GPFile >> GameProgress.LeadThrows;
		for (int i = 0; i < dq; i++)
			GPFile >> GameProgress.DiceValues[i];
	}

	GPFile.close();
}

void PrintGameProgressDataFile(Elements GameProgress)
{
	ofstream GPFile;
	GPFile.open("GameProgress.txt", ios_base::out);

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
		GPFile << GameProgress.ThrowMax << endl;
		GPFile << GameProgress.LeadSwap << endl;
		GPFile << GameProgress.LeadThrows << endl;
		for (int i = 0; i < dq; i++)
			GPFile << GameProgress.DiceValues[i] << " ";
	}

	GPFile.close();
}

SDL_Texture* GenerateCombination(Elements GameProgress, TTF_Font* font, SDL_Rect* rect, SDL_Color fg, int& worth)
{
	SDL_Texture* texture = NULL;
	char str[ams] = " ";
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
		worth = k;
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
		worth = FindBestCombination(GameProgress);
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

int FindBestCombination(Elements& GameProgress)
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