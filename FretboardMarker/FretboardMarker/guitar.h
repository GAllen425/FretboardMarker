/*
* guitar.h
*
*  Created on: Aug 24, 2017
*      Author: Gareth
*/
#pragma once

#ifndef GUITAR_H_
#define GUITAR_H_

#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstring>
#include <iterator>
#include <vector>
#include <algorithm>
#include "CImg.h"
using namespace cimg_library;
using namespace std;




class guitar {
public:
	int max_string = 6;
	int total_fret_number;
	vector<string> tuningString;
	vector<vector <int> > fretboard_values;

	//need to make another 2d vector that assigns 0's to values that dont match
	//and 1's to values that do match

	vector<string> input;
	vector<int> inputValues;

	int semitoneValue(string note)
	{
		int noteValue=0;
		//defaults to C

		if (note == "C") { noteValue = 0; }
		else if (note == "C#") { noteValue = 1; }
		else if (note == "D") { noteValue = 2; }
		else if (note == "D#") { noteValue = 3; }
		else if (note == "E") { noteValue = 4; }
		else if (note == "F") { noteValue = 5; }
		else if (note == "F#") { noteValue = 6; }
		else if (note == "G") { noteValue = 7; }
		else if (note == "G#") { noteValue = 8; }
		else if (note == "A") { noteValue = 9; }
		else if (note == "A#") { noteValue = 10; }
		else if (note == "B") { noteValue = 11; }
		return noteValue;
	}

	string numberToLetter(int number)
	{
		std::string letter;
		if (number == 0) { letter = "C"; }
		else if (number == 1) { letter = "C#"; }
		else if (number == 2) { letter = "D"; }
		else if (number == 3) { letter = "D#"; }
		else if (number == 4) { letter = "E"; }
		else if (number == 5) { letter = "F"; }
		else if (number == 6) { letter = "F#"; }
		else if (number == 7) { letter = "G"; }
		else if (number == 8) { letter = "G#"; }
		else if (number == 9) { letter = "A"; }
		else if (number == 10) { letter = "A#"; }
		else if (number == 11) { letter = "B"; }
		return letter;
	}

	void getUserInput()
	{
		cout << "Input notes: " << endl;
		string s;
		bool loop_test = true;
		cin.ignore();
		while (loop_test)
		{
			getline(cin, s);
			if (s.empty())
			{
				loop_test = false;
				cin.clear();
				break;
			}
			input.push_back(s);
		}

		for (unsigned int i = 0; i<input.size(); i++)
		{
			inputValues.push_back(semitoneValue(input[i]));
			cout << input[i] << endl;
		}

	};

	void setTuning()
	{
		string s;
		//fretboard_values.resize(6);
		cout << "Enter the tuning in all caps from lowest to highest eg. E A D G B E" << endl;
		getline(cin, s);
		max_string = totalNumberStrings(s);
		cout << max_string << endl;
		fretboard_values.resize(max_string);
		for (int i = 0; i < max_string; i++)
			tuningString.push_back(tuningToGuitarString(s, i));

		for (int i = 0; i<max_string; i++)
		{
			fretboard_values[i].push_back(semitoneValue(tuningString[i]));
		}

	}

	void setFretboardArray()
	{
		for (int i = 0; i<max_string; i++)
		{
			for (int j = 1; j<total_fret_number; j++)
			{
				fretboard_values[i].push_back((fretboard_values[i][j - 1] + 1) % 12);
			}
		}
	}

	void printFretboard()
	{
		cout << "\t";
		for (int k = 0; k<max_string; k++)
			cout << setw(6) << max_string - k;

		cout << endl;

		for (int j = 0; j<total_fret_number; j++)
		{
			cout << j << "\t";
			for (int i = 0; i<max_string; i++)
			{
				cout << setw(6) << numberToLetter(fretboard_values[i][j]);
			}
			cout << endl;
		}
	}

	void setTotalFrets()
	{
		cout << "Enter the number of frets" << endl;
		cin >> total_fret_number;
		total_fret_number++;
	}

	vector<vector <int> > matches;
	void getMatches()
	{
		matches.resize(max_string);

		for (int i = 0; i<max_string; i++)
			matches[i].push_back(semitoneValue(tuningString[i]));


		for (int i = 0; i<max_string; i++)
		{
			for (int j = 1; j<total_fret_number; j++)
			{
				if (find(inputValues.begin(), inputValues.end(), fretboard_values[i][j]) != inputValues.end())
					matches[i].push_back(fretboard_values[i][j]);
				else
					matches[i].push_back(-1);
			}
		}
	}

	void printMatches()
	{
		cout << "\t";
		for (int k = 0; k<max_string; k++)
			cout << setw(6) << 6 - k;

		cout << endl;

		for (int j = 0; j<total_fret_number; j++)
		{
			cout << j << "\t";
			for (int i = 0; i<max_string; i++)
			{
				cout << setw(6) << matches[i][j];
			}
			cout << endl;
		}

	}

	void printMatchesImage()
	{
		//1 fret length , 2.5 fret width i.e. width:length ratio is 2.5:1 per fret
		int length_fret = 70;

		//dimensions
		int size_y = length_fret*total_fret_number;
		int size_x = length_fret * 3;
		int size_z = 1;
		int numberOfColorChannels = 3; // R G B
		unsigned char initialValue = 0;

		//set up base layer
		CImg<unsigned char> image(size_x, size_y, size_z, numberOfColorChannels, initialValue);
		image.fill(255);
		unsigned char black[] = { 0,0,0 };
		unsigned char white[] = { 255,255,255 };

		//inner and outer box dimensions i.e. hollow black rectangle
		int line_width = 5;
		int outer_x1 = size_x / 10 + 20, outer_y1 = size_y / 20 * 3,
			outer_x2 = size_x - size_x / 10, outer_y2 = size_y - size_y / 20;
		int inner_x1 = outer_x1 + line_width, inner_y1 = outer_y1 + line_width;
		int inner_x2 = outer_x2 - line_width, inner_y2 = outer_y2 - line_width;

		int board_length = inner_y2 - inner_y1;
		int board_width = inner_x2 - outer_x1;

		image.draw_rectangle(outer_x1, outer_y1, outer_x2, outer_y2, black, 1);
		image.draw_rectangle(inner_x1, inner_y1, inner_x2, inner_y2, white, 1);



		//setting up x coordinate of string
		//and y coordinate of frets
		int* fret_y = new int[total_fret_number + 1];
		int* string_x = new int[max_string + 1];

		fret_y[0] = inner_y1;
		fret_y[total_fret_number] = inner_y2;
		for (int i = 1; i<total_fret_number; i++)
		{
			fret_y[i] = inner_y1 + i*board_length / total_fret_number + 0.5*line_width;
			image.draw_rectangle
			(inner_x1, fret_y[i] - line_width / 2,
				inner_x2, fret_y[i] + line_width / 2,
				black, 1);
		}

		string_x[1] = outer_x1;
		string_x[max_string - 1] = inner_x2;
		for (int i = 1; i<max_string; i++)
		{
			string_x[i + 1] = outer_x1 + board_width*i / (max_string - 1) + line_width / 2;
			image.draw_rectangle(string_x[i + 1] - line_width / 2, inner_y1,
				string_x[i + 1] + line_width / 2, inner_y2,
				black, 1);
		}

		//draw tuning
		for (unsigned int i = 1; i<tuningString.size() + 1; i++)
		{
			image.draw_text(string_x[i], outer_y1 - 20, tuningString[i - 1].c_str(), black);
		}

		//draw fret numbers
		for (int j = 1; j<total_fret_number; j++)
		{
			string text;
			stringstream convert;
			convert << j;
			text = convert.str();
			image.draw_text(outer_x1 - 20, (fret_y[j] + fret_y[j - 1]) / 2, text.c_str(), black);
		}

		// colour array for each note
		// C=0 -> B=11
		int colour[12][3] =
		{
			{ 102,0,0 },  	//C
			{ 255,0,0 },		//C#
			{ 255,153,153 }, 	//D
			{ 255,128,0 },	//D#
			{ 255,255,0 },	//E
			{ 102,102,0 },	//F
			{ 64,255,0 },		//F#
			{ 0,255,255 },	//G
			{ 0,128,128 },	//G#
			{ 51,153,255 },	//A
			{ 0,0,255 },		//A#
			{ 128,0,128 }		//B
		};

		for (int i = 1; i<max_string + 1; i++)
		{
			for (int j = 1; j<total_fret_number; j++)
			{
				if (matches[i - 1][j] > -1)
				{
					image.draw_circle(string_x[i], (fret_y[j] + fret_y[j - 1]) / 2, length_fret / 6, colour[matches[i - 1][j]], 1);
				}
			}
		}
		if (total_fret_number > 9)
			image.rotate(270);

		CImgDisplay display(image, "Fretboard");

		while (!display.is_closed())
		{
			image.display(display);
		}

	}
	string tuningToGuitarString(string tuning, int stringNumber)
	{
		string array[20];
		stringstream ssin(tuning);
		int i = 0;
		while (ssin.good())
		{
			ssin >> array[i];
			++i;
		}
		return array[stringNumber];
	}

	int	totalNumberStrings(string tuning)
	{
		string array[20];
		stringstream ssin(tuning);
		int i = 0;
		while (ssin.good())
		{
			ssin >> array[i];
			++i;
		}
		return i;
	}


};




#endif /* GUITAR_H_ */
