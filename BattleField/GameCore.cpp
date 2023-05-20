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
		cout << "������ �������� �����! " << endl;
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
		cout << "������ �������� �����! " << endl;
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
				strcpy_s(GameProgress.Combo1, u8"���� ������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"���� ������");
			return 64;
		case 1:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"���� �����");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"���� �����");
			return 65;
		case 2:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"���� �����");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"���� �����");
			return 66;
		case 3:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"���� ��������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"���� ��������");
			return 67;
		case 4:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"���� �������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"���� �������");
			return 68;
		case 5:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"���� ��������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"���� ��������");
			return 69;
		}
	case 4:
		switch (digit)
		{
		case 0:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"������ �������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"������ �������");
			return 58;
		case 1:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"������ ������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"������ ������");
			return 57;
		case 2:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"������ ������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"������ ������");
			return 60;
		case 3:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"������ ��������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"������ ��������");
			return 61;
		case 4:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"������ �������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"������ �������");
			return 62;
		case 5:
			if (GameProgress.Queue == 1)
				strcpy_s(GameProgress.Combo1, u8"������ ��������");
			if (GameProgress.Queue == 2)
				strcpy_s(GameProgress.Combo2, u8"������ ��������");
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
						strcpy_s(GameProgress.Combo1, u8"������ ��� (1 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (1 + 2)");
					return 28;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (1 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (1 + 3)");
					return 29;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (1 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (1 + 4)");
					return 30;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (1 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (1 + 5)");
					return 31;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (1 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (1 + 6)");
					return 32;
				}
			case 1:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (2 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (2 + 1)");
					return 33;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (2 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (2 + 3)");
					return 34;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (2 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (2 + 4)");
					return 35;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (2 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (2 + 5)");
					return 36;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (2 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (2 + 6)");
					return 37;
				}
			case 2:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (3 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (3 + 1)");
					return 38;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (3 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (3 + 2)");
					return 39;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (3 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (3 + 4)");
					return 40;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (3 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (3 + 5)");
					return 41;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (3 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (3 + 6)");
					return 42;
				}
			case 3:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (4 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (4 + 1)");
					return 43;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (4 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (4 + 2)");
					return 44;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (4 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (4 + 3)");
					return 45;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (4 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (4 + 5)");
					return 46;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (4 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (4 + 6)");
					return 47;
				}
			case 4:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (5 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (5 + 1)");
					return 48;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (5 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (5 + 2)");
					return 49;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (5 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (5 + 3)");
					return 50;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (5 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (5 + 4)");
					return 51;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (5 + 6)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (5 + 6)");
					return 52;
				}
			case 5:
				switch (repeatTwo)
				{
				case 0:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (6 + 1)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (6 + 1)");
					return 53;
				case 1:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (6 + 2)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (6 + 2)");
					return 54;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (6 + 3)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (6 + 3)");
					return 55;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (6 + 4)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (6 + 4)");
					return 56;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"������ ��� (6 + 5)");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"������ ��� (6 + 5)");
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
					strcpy_s(GameProgress.Combo1, u8"��� �������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"��� �������");
				return 22;
			case 1:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"��� ������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"��� ������");
				return 23;
			case 2:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"��� ������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"��� ������");
				return 24;
			case 3:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"��� ��������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"��� ��������");
				return 25;
			case 4:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"��� �������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"��� �������");
				return 26;
			case 5:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"��� ��������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"��� ��������");
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
						strcpy_s(GameProgress.Combo1, u8"���� ������ + ���� �����");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ������ + ���� �����");
					return 7;
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ������ + ���� �����");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ������ + ���� �����");
					return 8;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ������ + ���� �����");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ������ + ���� �����");
					return 9;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ������ + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ������ + ���� ��������");
					return 10;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ������ + ���� �������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ������ + ���� �������");
					return 11;
				}
			case 1:
				switch (repeatTwo)
				{
				case 2:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� �����");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� �����");
					return 12;
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� ��������");
					return 13;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� �������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� �������");
					return 14;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� ��������");
					return 15;
				}
			case 2:
				switch (repeatTwo)
				{
				case 3:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� ��������");
					return 16;
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� �������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� �������");
					return 17;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ����� + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ����� + ���� ��������");
					return 18;
				}
			case 3:
				switch (repeatTwo)
				{
				case 4:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� �������� + ���� �������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� �������� + ���� �������");
					return 19;
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� �������� + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� �������� + ���� ��������");
					return 20;
				}
			case 4:
				switch (repeatTwo)
				{
				case 5:
					if (GameProgress.Queue == 1)
						strcpy_s(GameProgress.Combo1, u8"���� ������� + ���� ��������");
					if (GameProgress.Queue == 2)
						strcpy_s(GameProgress.Combo2, u8"���� ������� + ���� ��������");
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
					strcpy_s(GameProgress.Combo1, u8"���� ������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"���� ������");
				return 1;
			case 1:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"���� �����");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"���� �����");
				return 2;
			case 2:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"���� �����");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"���� �����");
				return 3;
			case 3:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"���� ��������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"���� ��������");
				return 4;
			case 4:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"���� �������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"���� �������");
				return 5;
			case 5:
				if (GameProgress.Queue == 1)
					strcpy_s(GameProgress.Combo1, u8"���� ��������");
				if (GameProgress.Queue == 2)
					strcpy_s(GameProgress.Combo2, u8"���� ��������");
				return 6;
			}
		}
	case 1:
		if (GameProgress.Queue == 1)
			strcpy_s(GameProgress.Combo1, u8"������");
		if (GameProgress.Queue == 2)
			strcpy_s(GameProgress.Combo2, u8"������");
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