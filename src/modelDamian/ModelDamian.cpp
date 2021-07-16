#include "ModelDamian.h"

void ModelDamian::initialize() {
	
	modelBasePosition = glm::vec3(-5.0f, 0.5f, -6.5f);

	resetModel();
	generateOriginalObject();

	colorPalette = new glm::vec3[NUM_COLORS];
	intializeColorPalette();
}

void ModelDamian::intializeColorPalette() {

	colorPalette[RED] = glm::vec3(.9f, 0.1f, 0.15f);
	colorPalette[BLUE] = glm::vec3(0.15f, 0.1f, 0.9f);
	colorPalette[VIOLET] = glm::vec3(0.25f, 0.1f, 0.85f);

	//colorPalette[ORANGE] = glm::vec3(0.25f, 0.0f, 0.0f);
	//colorPalette[YELLOW] = glm::vec3(0.0f, 0.0f, 0.35f);
	//colorPalette[VIOLET] = glm::vec3(0.0f, 0.0f, 0.25f);
}


int* ModelDamian::generateRandomModel(int x, int y, int z) {
	
	resetModel();

	// reset unitCubes in the model to their respective colors
	for (int r = 1; r < 8; r++) {

		//puts hole in the wall
		modelData[r][3][0] = NONE;

		// 10% chance of a blank discontinuity in the model (blank space? blank cartridge? i don't know how to describe this)
		if ((rand() % 10 == 0)) {
			continue;
		}

		// randomly generates blocks on z axis
		for (int z = 2; z < 7; z++) {

			if ((rand() % 2) == 1) {
				modelData[r][3][z] = rand()%NUM_COLORS;
			}
		}
	}

	for (int c = 1; c < 6; c++) {

		// puts hole in the wall
		modelData[2][c][0] = NONE;
		modelData[6][c][0] = NONE;
		modelData[4][c][0] = NONE;

		// same as above 10% chance...
		if ((rand() % 10 == 0)) {
			continue;
		}

		// randomly generates blocks on z axis
		for (int z = 2; z < 7; z++) {
			
			if ((rand() % 2) == 1) {
				modelData[2][c][z] = rand()%NUM_COLORS;
			}
			

			if (c != 3) {

				if ((rand() % 2) == 1) {
					modelData[6][c][z] = rand() % NUM_COLORS;
				}
			}

			if (c == 2 || c == 4) {
				
				if ((rand() % 2) == 1) {
					modelData[4][c][z] = rand() % NUM_COLORS;
				}
			}
		}
	}
	return NULL;
}

void ModelDamian::resetModel() {
	// initialize entire model to a wall or none (no unit cube)
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {
			for (int p = 0; p < PLANES; p++)
				if (p == 0) {
					modelData[r][c][p] = WALL;
				}
				else {
					modelData[r][c][p] = NONE;
				}
		}
	}
}

void ModelDamian::generateOriginalObject() {

	resetModel();

	// reset unitCubes in the model to their respective colors
	for (int r = 1; r < 8; r++) {
		modelData[r][3][PLANES - 3] = BLUE;
		modelData[r][3][0] = NONE;
	}

	for (int c = 1; c < 6; c++) {

		modelData[2][c][PLANES - 3] = RED;
		modelData[2][c][0] = NONE;

		if (c == 1) {
			modelData[6][1][PLANES - 5] = RED;
			modelData[6][1][PLANES - 4] = RED;

			modelData[2][1][PLANES - 1] = RED;
			modelData[2][1][PLANES - 2] = RED;
		}

		if (c == 5) {
			modelData[6][c][PLANES - 1] = RED;
			modelData[6][c][PLANES - 2] = RED;

			modelData[2][c][PLANES - 5] = RED;
			modelData[2][c][PLANES - 4] = RED;
		}

		if (c != 3) {
			modelData[6][c][PLANES - 3] = RED;
			modelData[6][c][0] = NONE;
		}

		if (c == 2 || c == 4) {
			modelData[4][c][PLANES - 3] = RED;
			modelData[4][c][0] = NONE;

			if (c == 2) {
				modelData[6][c][PLANES - 4] = RED;
				modelData[2][c][PLANES - 2] = RED;
			}

			if (c == 4) {
				modelData[6][c][PLANES - 2] = RED;
				modelData[2][c][PLANES - 4] = RED;
			}

		}
	}

	modelData[0][0][0] = BLUE;
	modelData[0][6][6] = VIOLET;
	modelData[0][0][6] = VIOLET;
}

// draw method works by rendering each unit cube in the model
void ModelDamian::draw(Camera inCam, glm::vec3* dirLight, glm::mat4 projection, glm::mat4 view, glm::mat4 model) {

	// activate the shader
	baseShader.use();

	// pass the camera position to the fragment shader.  This determines what is "shadowed" and what isn't relative to the camera.
	baseShader.setVec3("viewPos", inCam.Position);

	// pass the color/lighting values to the fragment shader (at this point in time outside of some shadowing on the faces of the unit cube not directly facing the light this pretty much colors the unit cubes.
	baseShader.setVec3("dirLight.direction", dirLight[LIGHT_DIRECTION]);
	baseShader.setVec3("dirLight.ambient", dirLight[LIGHT_AMBIENT]);
	baseShader.setVec3("dirLight.diffuse", dirLight[LIGHT_DIFFUSE]);
	baseShader.setVec3("dirLight.specular", dirLight[LIGHT_SPECULAR]);

	// pass transformation matrices to the vertex shader.  The model matrix is passed at the end after all the world transformations are applied to the unit cube.
	baseShader.setMat4("projection", projection);
	baseShader.setMat4("view", view);

	// put this in base class or leave 

	const glm::mat3 rotationMatrix = glm::mat3(
		glm::vec3(glm::cos(orientation), 0.0f, -glm::sin(orientation)),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(glm::sin(orientation), 0.0f, glm::cos(orientation))
	);

	// world transformation: glm::translate moves the model around the world
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {
			for (int p = 0; p < PLANES; p++) {

				if (modelData[r][c][p] == NONE) {
					continue;
				}

				// scale position of each unitCube
				// positions of each cube are relative to the axis of rotation
				float x = ((float)c - (COLUMNS * 0.5f)) * scaleFactor;
				float y = (float)r * scaleFactor;
				float z = ((float)p - 0.5f) * scaleFactor;

				// translation vector to move unit cube from base position
				glm::vec3 translation = glm::vec3(x, y, z);

				// apply a rotation to the translation vector so that the position of the unit cube is synchronized with the orientation of the cube
				translation = rotationMatrix * translation;
				

				// ensure that the model matrix passed is an identity matrix
				model = glm::mat4(1.0f);
				
				// apply any x or y translations
				model = glm::translate(model, glm::vec3(xTranslation, yTranslation, 0.0f));

				// wall cubes are offset from a different base position then the object cubes
				if (modelData[r][c][p] == WALL) {

					// shader colors the wall unit cube grey
					baseShader.setVec3("dirLight.ambient", dirLight[LIGHT_AMBIENT]);

					// TODO: make base position a variable (currently it's a constant)
					model = glm::translate(model, modelBasePosition + translation);

				}
				else {

					// translation vector to move unit cube from base position
					model = glm::translate(model, modelBasePosition + translation);

					// if-else statement colors the object cubes either red or blue
					baseShader.setVec3("dirLight.ambient", colorPalette[modelData[r][c][p]]);
				}
				// scale the size of each cube
				model = glm::scale(model, glm::vec3(1.0f) * scaleFactor);

				// apply the unit cube so that the cube's orientation is aligned with the model's orientation
				model = glm::rotate(model, orientation, glm::vec3(0.0f, 1.0f, 0.0f));

				// pass the model matrix to the vertex shader
				baseShader.setMat4("model", model);

				// render the cube
				glBindVertexArray(unitCube.getVAO());
				glDrawArrays(renderMode, 0, 36);
			}
		
		}
	}
}

bool ModelDamian::boundaryCollision() {

	glm::vec3* modelCorners = new glm::vec3[4];

	
	modelCorners[0] = glm::vec3((COLUMNS*-0.5f)-0.5f, 0.0f, 0.0f);
	modelCorners[1] = glm::vec3((COLUMNS*0.5f) - 0.5f, 0.0f, 0.0f);
	modelCorners[2] = glm::vec3((COLUMNS*0.5)-0.5f, 0.0f, PLANES - 0.5f);
	modelCorners[3] = glm::vec3((COLUMNS*-0.5) - 0.5f, 0.0f, PLANES - 0.5f);

	const glm::mat3 rotationMatrix = glm::mat3(
		glm::vec3(glm::cos(orientation), 0.0f, -glm::sin(orientation)),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(glm::sin(orientation), 0.0f, glm::cos(orientation))
	);

	for (int i = 0; i < 4; i++) {
		// move corner vector to base position
		modelCorners[i] = glm::vec3(
			modelCorners[i].x + modelBasePosition.x,
			modelCorners[i].y + modelBasePosition.y,
			modelCorners[i].z + modelBasePosition.z
			);

		// scale corner vector
		modelCorners[i] *= scaleFactor;

		// rotate corner vector
		modelCorners[i] = rotationMatrix * modelCorners[i];

		// apply any x or y translations to corner vector
		// translations are multiplied by 1.4 to prempt the collision
		if (xTranslation < 0) {
			modelCorners[i] = glm::vec3(modelCorners[i].x + xTranslation - 0.05f, modelCorners[i].y + yTranslation, modelCorners[i].z);
		}
		else {
			modelCorners[i] = glm::vec3(modelCorners[i].x + xTranslation + 0.05f, modelCorners[i].y + yTranslation, modelCorners[i].z);
		}

		std::cout << "model corner "<< i <<":" <<" x " << modelCorners[i].x << " y " << modelCorners[i].y << " z " << modelCorners[i].z << std::endl;

		// check x boundary
		if(modelCorners[i].x < -10.1f || modelCorners[i].x > 10.1f) {
			//std::cout << "x " << modelCorners[i].x << " y " << modelCorners[i].y << " z " << modelCorners[i].z << std::endl;
			std::cout << "i out of bounds: " << i << std::endl;
			return true;
		}

		// check y boundary (only applies to y translation, not scaling)
		if (modelCorners[i].y <= 0) {
			//std::cout << "x " << modelCorners[i].x << " y " << modelCorners[i].y << " z " << modelCorners[i].z << std::endl;
			std::cout << "i out of bounds: " << i << std::endl;
			return true;
		}

		//check z boundary
		if (modelCorners[i].z <= -10.0f || modelCorners[i].z >= 10.0f) {
			//std::cout << "x " << modelCorners[i].x << " y " << modelCorners[i].y << " z " << modelCorners[i].z << std::endl;
			std::cout << "i out of bounds: " << i << std::endl;
			return true;
		}
	}
	std::cout << " " << std::endl;
	return false;
}