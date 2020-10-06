#include <snes.h>
#include "system/system.h"

#include "scenes/highscoreTest.h"
#include "scenes/startUpTest.h"
#include "scenes/characterTest.h"
#include "scenes/copTest.h"

int main(void) {

	systemInit();

	sceneCopTest();
	//sceneHighscoreTest(5000);
	//sceneCharacterTest();
	sceneStartUpTest();

	return 0;
}