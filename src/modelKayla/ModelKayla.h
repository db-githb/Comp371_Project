#pragma once
#include "../modelBase/ModelBase.h"

#define ROWS 9
#define COLUMNS 7
#define PLANES 7


#define WALL 0
#define BLUE 1
#define RED 2
#define NONE 3

class ModelKayla : public ModelBase {

public:
	void initialize();
	void generateOriginalObject();
	int* generateRandomModel(int x, int y, int z);
	void resetUnitCubes();
	void resetModel();

	void generateCube(int rowStart, int rowEnd, int columnStart, int columnEnd, int planeStart, int planeEnd, int color);
	
	void draw(Camera inCam, glm::vec3* dirLight, glm::mat4 projection, glm::mat4 view, glm::mat4 model);

	void positionModel(float x, float y, float z);

private:
	int modelData[ROWS][COLUMNS][PLANES];

};

