#include <iostream>
#include <fstream>
#include <Windows.h>
#include <ctime>

#define ROW 4 

void printScreen(int glasscount, char** colors, int count, HANDLE h);
void shuffleColors(int glasscount, char** colors);
void moveWater(int from, int to, char** colors);
int findFirstColor(int n, char** colors);
bool checkWin(int glasscount, char** colors);
bool checkMovePossible(int from, int to, int glasscount, char** colors, HANDLE h);
void printMenu();

int main()
{	//Renk vermeye yariyor
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int glasscount, from, to, count = 0;
	char flag = 0;
	bool isSaved = false;
	//Oyun sirasinda menuye donmeye yarayan while dongusu
	while (1) {
		//Oyun tekrar baslatilirsa diye hamle sayisini 0'a esitliyor.
		count = 0;
		while (flag != 1 && flag != 2 && flag != 3) {
			printMenu();
			std::cin >> flag;
			flag = flag - '0';
		}

		if (flag == 3) {
			isSaved = true;
			std::ifstream iFile;
			iFile.open("saves.txt");
			iFile >> glasscount;
			iFile.close();
		}
		//2 girilirse oyunu sonlandiriyor.
		if (flag == 2) {
			SetConsoleTextAttribute(h, 12);
			std::cout << "Exitting the game...";
			SetConsoleTextAttribute(h, 7);
			//Menuye donmeden colors arrayini sildigimiz icin bir daha silmemize gerek yok.
			return 0;
		}
		flag = 0;

		//4 ile 15 arasinda deger girilene kadar tekrar ediyor.
		if (!isSaved) {
			do {
				system("cls");
				std::cout << "+----------------------------------+\n";
				std::cout << "|Enter glass count between 4 and 15|\n";
				std::cout << "+----------------------------------+\n";
				std::cin >> glasscount;
			} while (glasscount < 4 || glasscount>15);
		}

		char** colors = new char* [ROW];
		for (int i = 0; i < ROW; i++) {
			colors[i] = new char[glasscount];
		}
		if (isSaved) {
			std::ifstream iFile;
			iFile.open("saves.txt");
			iFile >> glasscount;
			for (int i = 0; i < ROW; i++) {
				for (int j = 0; j < glasscount; j++) {
					iFile >> colors[i][j];
					if (colors[i][j] == '0') colors[i][j] = ' ';
				}
			}
			iFile >> count;
			iFile.close();
		}

		if (!isSaved) shuffleColors(glasscount, colors);
		isSaved = false;
		while (!checkWin(glasscount, colors)) {
			printScreen(glasscount, colors, count, h);
			do {
				std::cout << "\n\nPick first and second glass: ";
				std::cin >> from;
				//Menuye cikmak icin 0 girilirse ilk donguden cikiyor.
				if (from == 0) {
					to = 0;
					from--, to--;
					break;
				}
				std::cin >> to;
				from--, to--;

			} while ((!checkMovePossible(from, to, glasscount, colors, h)));
			//Cikmak icin 0 girilirse 1 cikariliyor. Array 0'dan basladigi icin kullancinin
			//degerleri 1 azaltiliyor. 0 girilmesine ragmen 1 azaltmamin sebebi 1 girilirse
			//0'a dusecegi icin yine donguden cikiyordu.
			if (from == -1 || to == -1) {
				do {
					system("CLS");
					std::cout << "+-----------------------------------------+\n";
					std::cout << "|Press 0 to resume the game               |\n";
					std::cout << "+-----------------------------------------+\n";
					std::cout << "|Press 1 to start a new game              |\n";
					std::cout << "+-----------------------------------------+\n";
					std::cout << "|Press 2 to exit the game without saving  |\n";
					std::cout << "+-----------------------------------------+\n";
					std::cout << "|Press 3 to save the game and exit        |\n";
					std::cout << "+-----------------------------------------+\n";
					std::cin >> flag;
					flag = flag - '0';
				} while (flag != 1 && flag != 2 && flag != 0 && flag != 3);
				if (flag == 2) {
					SetConsoleTextAttribute(h, 12);
					std::cout << "Exitting the game...";
					SetConsoleTextAttribute(h, 7);
					//Arrayi hafizadan siliyor.
					for (int i = 0; i < ROW; i++) {
						delete[] colors[i];
					}
					delete[] colors;
					colors = NULL;
					return 0;
				}
				else if (flag == 0) continue;

				else if (flag == 3) {
					std::ofstream oFile;
					oFile.open("saves.txt");
					oFile << glasscount << "\n";
					for (int i = 0; i < ROW; i++) {
						for (int j = 0; j < glasscount; j++) {
							if (colors[i][j] == ' ') {
								oFile << "0 ";
							}
							else {
								oFile << colors[i][j] << ' ';
							}
						}
						oFile << "\n";
					}
					oFile << count;
					oFile.close();
					SetConsoleTextAttribute(h, 10);
					std::cout << "Game saved...\n";
					SetConsoleTextAttribute(h, 12);
					std::cout << "Exitting the game...\n";
					SetConsoleTextAttribute(h, 7);
					//Arrayi hafizadan siliyor.
					for (int i = 0; i < ROW; i++) {
						delete[] colors[i];
					}
					delete[] colors;
					colors = NULL;
					return 0;
				}
				break;

			}
			count++;
			moveWater(from, to, colors);

		}
		//CheckWin true olursa you win yazdirip bitis menusunu bastiriyor.
		if (checkWin(glasscount, colors)) {
			printScreen(glasscount, colors, count, h);
			SetConsoleTextAttribute(h, 10);
			std::cout << "\n+---------+\n";
			std::cout << "|YOU WIN!!|\n";
			std::cout << "+---------+\n\n\n";
			SetConsoleTextAttribute(h, 7);
			std::cout << "+--------------------------------+\n";
			std::cout << "|Do you want to start a new game?|\n";
			std::cout << "+--------------------------------+\n";
			std::cout << "|Press 1 to start a new game     |\n";
			std::cout << "+--------------------------------+\n";
			std::cout << "|Press 2 to exit the game        |\n";
			std::cout << "+--------------------------------+\n";
			//Flag degerini bir daha alıyor. Eger 1 secilirse program basindaki ana menuyu atliyor. 
			//2 secilirse yine en basa gidip oyunu sonlandiriyor.
			do {
				std::cin >> flag;
				flag = flag - '0';
			} while (flag != 1 && flag != 2);

			//Ana menüye çıkarsa array'i hafızadan siliyor.
			//Oyun ortasinda ana menuye cikarsa da burada siliyor.
			for (int i = 0; i < ROW; i++) {
				delete[] colors[i];
			}
			delete[] colors;
			colors = NULL;
		}
	}
}


//FONKSIYONLAR

//Oyun ekranini bastiriyor.
void printScreen(int glasscount, char** colors, int count, HANDLE h) {
	system("CLS");
	std::cout << "+------------------------------------------+\n";
	std::cout << "|Enter 0 if you want to go to the main menu|\n";
	std::cout << "+------------------------------------------+\n\n\n";
	std::cout << "   Move Count\n";
	std::cout << "+---------------+\n";
	std::cout << "|\t" << count << "\t|\n";
	std::cout << "+---------------+\n\n\n";
	//Girilen bardak sayisina gore dinamik olarak bardak bastiriyor.
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < glasscount; j++) {
			if (i == 4) {
				std::cout << "|---| ";
			}
			else {
				//std::cout << "| "<< colors[i][j] <<" | ";
				if (colors[i][j] != ' ') {
					std::cout << "| ";
					SetConsoleTextAttribute(h, colors[i][j] + 1 - 'A');
					std::cout << "*";
					SetConsoleTextAttribute(h, 7);
					std::cout << " | ";
				}
				else
					std::cout << "| " << colors[i][j] << " | ";

			}
		}
		std::cout << "\n";
	}
	//Bardaklarin altina kacinci bardak oldugunu yazdiriyor.
	for (int i = 0; i < glasscount; i++) {
		if (i < 10) {
			std::cout << "  " << i + 1 << "   ";
		}
		else {
			std::cout << " " << i + 1 << "   ";
		}
	}
	std::cout << "\n";

}

//Pointer kullanilarak yapildi.
//Renklerin tutuldugu diziye once alfabetik olarak harfleri atıyor. (Ascii kodlarindan yararlanarak)
//Daha sonra bu harfleri rastgele karistiriyor.
void shuffleColors(int glasscount, char** colors) {
	//Alfabetik olarak harf atama
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < glasscount; j++) {
			if (j < glasscount - 2) {
				*(*(colors + i) + j) = 65 + j;
			}
			else {
				*(*(colors + i) + j) = ' ';
			}
		}
	}
	char temp;
	srand(time(nullptr));
	//Diziyi rastgele karma
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < glasscount - 2; j++) {
			temp = colors[i][j];
			int row = rand() % 4, colomn = rand() % (glasscount - 2);
			*(*(colors + i) + j) = *(*(colors + row) + colomn);
			*(*(colors + row) + colomn) = temp;
		}
	}
}

//En ustteki rengin bulundugu satirin degerini return ediyor.
int findFirstColor(int n, char** colors) {
	int count = 0;
	while (colors[count][n] == ' ') {
		count++;
		if (count == ROW) return ROW;
	}
	return count;
}

//En ustteki rengi diger bardaga tasiyor. Birden fazla ayni renk alt alta ise while dongusu ile tek tek tasiyor.
void moveWater(int from, int to, char** colors) {
	do {
		colors[findFirstColor(to, colors) - 1][to] = colors[findFirstColor(from, colors)][from];
		colors[findFirstColor(from, colors)][from] = ' ';
		if (findFirstColor(from, colors) == 4) break;
	} while ((colors[findFirstColor(from, colors)][from] == colors[findFirstColor(to, colors)][to]) && findFirstColor(to, colors) != 0);

}

//Bardaklarin icinde gezerek ayni renkten alt alta 4 tane var mi diye kontrol ediyor, varsa count'u arttiriyor.
//Eger count bardak sayisinden 2 eksik olursa true donduruyor.
bool checkWin(int glasscount, char** colors) {
	int count = 0;
	for (int i = 0; i < glasscount; i++) {
		if (colors[0][i] == colors[1][i] && colors[1][i] == colors[2][i] && colors[2][i] == colors[3][i] && colors[0][i] != ' ') {
			count++;
		}
	}
	if (count == glasscount - 2) {
		return true;
	}
	else {
		return false;
	}
}

//Hamlelerin mumkun olup olmadigini kontrol ediyor. Eger mumkun degilse hata mesaji veriyor ve false donduruyor.
bool checkMovePossible(int from, int to, int glasscount, char** colors, HANDLE h) {
	if ((from < 0 || to < 0 || from + 1 > glasscount || to + 1 > glasscount)) {
		SetConsoleTextAttribute(h, 12);
		std::cout << "Please enter 2 numbers between 1 and " << glasscount << "!\n";
		SetConsoleTextAttribute(h, 7);
		return false;
	}
	else if (from == to) {
		SetConsoleTextAttribute(h, 12);
		std::cout << "You cant pour a glass to the same glass.\n";
		SetConsoleTextAttribute(h, 7);
		return false;
	}
	else if (findFirstColor(from, colors) == 4) {
		SetConsoleTextAttribute(h, 12);
		std::cout << "You tried to pour an empty glass!\n";
		SetConsoleTextAttribute(h, 7);
		return false;
	}
	else if (findFirstColor(to, colors) == 0) {
		SetConsoleTextAttribute(h, 12);
		std::cout << "The glass is full!\n";
		SetConsoleTextAttribute(h, 7);
		return false;
	}
	else if ((findFirstColor(to, colors) != 4)) {
		if ((colors[findFirstColor(from, colors)][from] != colors[findFirstColor(to, colors)][to])) {
			SetConsoleTextAttribute(h, 12);
			std::cout << "You can only pour the same colors on top!\n";
			SetConsoleTextAttribute(h, 7);
			return false;
		}
	}
	return true;
}

//Ekrana menuyu bastiriyor.
void printMenu() {
	system("cls");
	std::cout << "+--------------------------------+\n";
	std::cout << "|Welcome to Water Sorting Game!  |\n";
	std::cout << "+--------------------------------+\n";
	std::cout << "|Press 1 to start a new game.    |\n";
	std::cout << "+--------------------------------+\n";
	std::cout << "|Press 2 to exit the game.       |\n";
	std::cout << "+--------------------------------+\n";
	std::cout << "|Press 3 to load saved game.     |\n";
	std::cout << "+--------------------------------+\n";

}
