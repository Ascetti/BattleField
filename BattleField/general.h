#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <string>
#include <windows.h>

//correction value
#define cv 14
//underlibe thickness
#define ut 4
// width
#define sw 13
// heihgt
#define sh 5
#define r 5
#define g 255
#define b 184
//array max size
#define ams 50
//dice quantity
#define dq 10
//speed
#define speed 7

using namespace std;

struct Proportions
{
	int width;
	int height;
};

struct Zones
{
	SDL_Rect Name1;
	SDL_Rect Name2;
	SDL_Rect Combinations1;
	SDL_Rect Combinations2;
	SDL_Rect Score1;
	SDL_Rect Score2;
	SDL_Rect PauseButton;
	SDL_Rect TipsList;
	SDL_Rect Results;
	SDL_Rect ThrowButton1;
	SDL_Rect SkipButton1;
	SDL_Rect ThrowButton2;
	SDL_Rect SkipButton2;
	SDL_Rect PlayZone1;
	SDL_Rect PlayZone2;
	SDL_Rect Dice1;
	SDL_Rect Dice2;
	SDL_Rect Glass;
};

struct Appearance
{
	int UnderlineIndent;
	int HeadingFontSize;
	SDL_Rect FirstTypeButton;
	SDL_Rect SecondTypeButton;
	SDL_Rect ThirdTypeButton;
	SDL_Rect FourthTypeButton;
	SDL_Rect FifthTypeButton;
	SDL_Rect SixthTypeButton;
	SDL_Rect SeventhTypeButton;
	SDL_Rect StepBack;
};

struct Elements
{
	bool GameStatus;
	int Round;
	char OutcomeText[ams];
	char FinalText[ams];
	int p1;
	int p2;
	int Queue;
	char Gambler1[ams];
	char Gambler2[ams];
	char Combo1[ams];
	char Combo2[ams];
	int ThrowMax;
	int LeadSwap;
	int LeadThrows;
	int DiceValues[dq];
};

struct Control
{
	int Volume;
	bool Music;
	bool Sound;
};

void Init(Proportions &window);
void DeInit(int error);
void work();

bool ButtonClick(SDL_Rect &rect, int x, int y);
SDL_Texture* LoadTextureFromFile(const char* filename);
SDL_Texture* GenerateTextureFromText(char* str, TTF_Font* font, SDL_Rect* rect, SDL_Color fg);
void PageLayout(Appearance& Page, Proportions window, Zones& Game);
void FPSabout();
void GenerateRandValues(int* DiceValues, int queue);
void Reset(Elements& GameProgress);

void ReadGameProgressDataFile(Elements& GameProgress);
void PrintGameProgressDataFile(Elements GameProgress);

SDL_Texture* GenerateCombination(Elements GameProgress, TTF_Font* font, SDL_Rect* rect, SDL_Color fg, int &worth);
int FindBestCombination(Elements &GameProgress);
void FPSControl();
//void ChangeText(char* text, const char* newtext);
//int Input_Data(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect& inputRect);

void MainMenu(int &mode, Appearance Page, Proportions window);
void ExitMenu(int& mode, Appearance Page, Proportions window);
void Rules(int& mode, Appearance Page, Proportions window);
void Settings(int& mode, Appearance Page, Proportions window);
void PauseMenu(int& mode, Appearance Page, Proportions window);
void Identification(int& mode, Appearance Page, Proportions window, char* gambler1, char* gambler2);
void Play(int& mode, Proportions window, Zones Game, Elements& GameProgress, Appearance Page);


//кнопки под соотношение 12:5, ibv2 13:5
//цвет зеленого r = 5, g = 255, b = 184
//стакан 5 к 3