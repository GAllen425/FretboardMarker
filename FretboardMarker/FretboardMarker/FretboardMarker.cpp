
#include "stdafx.h"

using namespace cimg_library;

int main() {

	guitar g;
	g.setTuning();
	g.setTotalFrets();
	g.setFretboardArray();
	g.getUserInput();
	//g.printFretboard();
	g.getMatches();
	//g.printMatches();
	g.printMatchesImage();

	return 0;
}
