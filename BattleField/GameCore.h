#pragma once
#include "general.h"

void GenerateRandValues(int* DiceValues, int queue);
void Reset(Elements& GameProgress);

void ReadGameProgressDataFile(Elements& GameProgress);
void PrintGameProgressDataFile(Elements GameProgress);

SDL_Texture* GenerateCombination(Elements& GameProgress, TTF_Font* font, SDL_Rect* rect, SDL_Color fg, int& worth);
int FindBestCombination(Elements& GameProgress);
void FPSControl();