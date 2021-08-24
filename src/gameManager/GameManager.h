#pragma once
#include "../modelBase/ModelBase.h"
#include "../modelDamian/modelDamian.h"
#include "../modelElijah/modelElijah.h"
#include "../modelThomas/modelThomas.h"
#include "../modelMichael/modelMichael.h"
#include "../modelRichard/modelRichard.h"
#include "../shader/Shader.h"
#include "../soundManager/SoundManager.h"
#include "../textGenerator/TextGenerator.h"

class GameManager {

public:
	ModelBase* activeModel;
	void initialize(Shader* inBaseShader, Shader* inTextShader, SoundManager* inSoundManager, glm::vec2 windowSize);
	void toggleGame();
	void draw(Shader* inShader);
	void speedUp();
	void nextModel();
	void setWindowSize(float width, float height);
	double getStartTime();
	double getDeltaTime();

	int displayTime;

	bool scoreTimeApart;

private:

	int score;
	int nrModels;
	bool gameOn;
	bool pauseOn;
	
	int gameState;

	ModelBase** models;

	int currentModel;
	double startTime;
	double deltaTime;

	double pauseStartTime;
	double pauseEndTime;
	double deltaPause;

	SoundManager* soundManager;
	TextGenerator* textGenerator;
	Shader* textShader;
	bool endState;

	float windowWidth;
	float windowHeight;


	void shuffle();
};