#include "deroulementJeu.h"

void jouer(Board *board)
{
	char playAgain;
	do
	{
		int score1 = 0;
		int score2 = 0;
		int high = 0;

		char x = 'X';
		char o = 'O';


		afficheEnteteBienvenue();
		int choixMode;
		do
		{
			afficheDemandeJoueurOuIA();
			readInt(&choixMode, "num"); //TODO : probleme => quand on saisi 0 1 2 ca marche
		} while ((choixMode != PLAYER && choixMode != IA) || isalpha(choixMode));

		//////////////////////////
		//PLAYER MODE ////////////
		//////////////////////////
		if (choixMode == PLAYER)
		{
			afficherBoard(board);
			int j1 = 0;
			do //tant que la grille n'est pas pleine faire
			{
				j1 = !j1;
				affichageOptionRedo();

				if (j1) //si c'est au joueur 1 de jouer TODO : pas opti, a chaque fois il reparcours tout le tableau
				{
					tourJoueur(board, 1, o, &score1);
				}
				else //si c'est au joueur 2 de jouer
				{
					tourJoueur(board, 2, x, &score2);
				}
				afficheScorePlayer(score1, score2);
			} while (!checkfull(board));

			if (score2 > score1)
			{
				high = score2;
				printf("\nPLAYER 2 WINS");
			}
			else if (score1 == score2)
			{
				high = score1;
				printf("\nDRAW");
			}
			else
			{
				high = score1;
				printf("\nPLAYER 1 WINS");
			}
		}
		else
		//////////////////////////
		//IA MODE ////////////////
		//////////////////////////
		{
			int choixNiveau;
			do
			{
				afficheDemandeSelectionNiveau();
				readInt(&choixNiveau, "choixNiveau");
			} while (choixNiveau != EASY && choixNiveau != MEDIUM && choixNiveau != HARD); //TODO : marche pour 0 1 2 3

			afficherBoard(board);
			do
			{
				affichageOptionRedo();
				//int numSaisie = tourJoueur(board, 1, x, &score1);

				//Pour test temps IA
				int numSaisie = 0;
				tourIA(board, x, &score1, choixNiveau, numSaisie);


				afficheScoreIA(score1, score2);

				tourIA(board, o, &score2, choixNiveau, numSaisie);
				afficheScoreIA(score1, score2);

			} while (!checkfull(board));

			if (score2 > score1)
			{
				high = score2;
				printf("\nComputer WINS\n");
			}
			else if (score1 == score2)
			{
				high = score1;
				printf("\nDRAW\n");
			}
			else
			{
				high = score1;
				printf("\nUser WINS\n");
			}
		}

		highscore(high);
		printf("\nif you you want to play again press y, else press any key\n");
		scanf("\n%c", &playAgain);
		libererBoard(board);

	} while (playAgain == 'y');

}

int tourJoueur(Board *board, int numJoueur, char lettreJoueur, int *score)
{
	int num;

	int numOK = 0;

	do //tant que le numero saisi n'est pas correct
	{
		printf("\nPlayer %i : Enter number of the column : ", numJoueur);
		readInt(&num, "num");
		saveLoad(&num, board);
		if(num > -20){
			if (num < 0)
			{
				//////////////////////////////
				//UNDO REDO SAVE LOAD/////////
				switch (num)
				{
				case UNDO:
					if (checkEmpty(board))
					{
						printf("\nCannot undo !");
						continue;
					}
					break;
				case REDO:
					if (board->undoRedo.redoCounter >= board->undoRedo.undoCounter)
					{
						printf("\nCannot redo !");
						continue;
					}
					break;
				case -10000000:
					printf("\nErreur");
					continue;
				}

				//undoRedoLimit(num, board);
				numOK = 1;
			}
			else
			{
				//////////////////////////////
				//NUM COL/////////////////////
				if ((num < 0) || (num > board->width - 1)) //si numCol depasse le board, on resaisie
				{
					printf("\nBad num col ! Please enter a number between 1 and %d.", board->width);
					continue;
				}
				else if (checkColPleine(num, board)) //si la colonne est pleine, on resaisi
				{
					printf("\nFull col ! Please enter another num of col.");
					continue;
				}
				numOK = 1;
			}
		}
	} while (!numOK);

	undoRedo(&lettreJoueur, board, num);
	*score = totalScore(board, lettreJoueur);

	return num;
}

void tourIA(Board *board, char lettreIA, int *score, int choixNiveau, int num)
{
	if (num >= 0) //si le numero saisie est une colonnes
	{
		switch (choixNiveau)
		{
		case EASY:
			Easy(board, &num);
			break;
		case MEDIUM:
			if (Medium(board, lettreIA, &num) == 0)
			{
				Easy(board, &num);
			}
			break;
		case HARD:
			Hard(board, &num);
			break;
		}


	}

	//Pas besoin de UNDO pour l'IA



	afficherBoard(board);
	*score = totalScore(board, lettreIA);
}