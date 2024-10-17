// AssignmentCarRace.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <vector>
#include <sstream>
#include <iomanip>
using namespace tle;
using namespace std;


// *** Enum Initialisations
enum EGameStates { READY, ONGOING, END };
enum EGameStages { NONE, ONE, TWO, THREE, FOUR, FIVE, SIX };
enum ECollisionArea { NOSIDE, FRONTSIDE, BACKSIDE, LEFTSIDE, RIGHTSIDE };

// *** Structures Initialisation
struct Vector2D
{
	float x;
	float z;
};

struct Vector3D
{
	float x;
	float y;
	float z;
};

// *** Constants
const float kGameSpeed = 1.0f;

// - - Position Constants
const Vector3D kCarPos = { 0.0f, 0.0f, -25.0f };
const Vector3D kCheckpoint2Pos = { 10.0f, 0.0f, 120.0f };
const Vector3D kCheckpoint3Pos = { 25.0f, 0.0f, 56.0f };
const Vector3D kCheckpoint4Pos = { 100.0f, 0.0f, 10.0f };
const Vector3D kCheckpoint5Pos = { 175.0f, 0.0f, 10.0f };
const Vector3D kCheckpoint6Pos = { 200.0f, 0.0f, 150.0f };
const Vector3D kTank1Pos = { 55.0f, 0.0f, 35.0f };
const Vector3D kTank2Pos = { 37.5f, 0.0f, 10.0f };
const Vector3D kTank3Pos = { 135.0f, -8.42f, 0.0f };
const Vector3D kTank4Pos = { 177.5f, 0.0f, 27.5f };
const Vector3D kTank5Pos = { 200.0f, 0.0f, 15.0f };
const Vector3D kTribune1Pos = { 120.0f, 0.0f, 45.0f };
const Vector3D kTribune2Pos = { 150.0f, 0.0f, 45.0f };
const Vector3D kTribune3Pos = { 120.0f, 0.0f, -25.0f };
const Vector3D kTribune4Pos = { 150.0f, 0.0f, -25.0f };
const Vector3D kNPCar1Pos = { 45.0f, 0.0f, 0.0f };
const Vector3D kNPCarFinalPos = { (kCheckpoint6Pos.x + 5.0f), 0.0f ,
(kCheckpoint6Pos.z + 20.0f) };

const float kZeroFloat = 0.0f;
const float kSkyBoxPosY = -960.0f;
const float kIsle1PosX = 10.0f;
const float kIsle1aPosZ = 40.0f;
const float kIsle1bPosZ = 56.0f;  //adjust by 16 for a wall between isle's
const float kIsle1cPosZ = 72.0f;
const float kIsleSideOfCheckAdj = 10.0f;  //adjust so isle at side of checkpoints
const float kIsle2aPosX = kCheckpoint6Pos.x - kIsleSideOfCheckAdj;
const float kIsle2bPosX = kCheckpoint6Pos.x + kIsleSideOfCheckAdj;
const float kWallBetweenIsleAdj = 8.0f;  //adjust so 1 wall between 2 isles
const float kWall1aPosZ = kIsle1aPosZ + kWallBetweenIsleAdj;
const float kWall1bPosZ = kIsle1bPosZ + kWallBetweenIsleAdj;

// - - Setup Constants
const float kRotateBy90 = 90.0f;
const float kRotateBy45 = 45.0f;
const int kCheckpointTotal = 6;
const int kCheckpoint2 = 1;
const int kCheckpoint4 = 3;
const int kCheckpoint5 = 4;
const int kIsleTotal = 12;
const int kIslePerWall = 3;
const int kIncrementBy1 = 1;
const int kWallTotal = 8;
const int kIsleIndexReset = 0;
const int kTanksTotal = 5;
const int kTank3 = 2;
const int kTribuneTotal = 4;

const int kStageIncrement = 1;
const int kStageTwo = 2;
const int kStageEnd = kCheckpointTotal + kIncrementBy1;  //one more than total stages
const int kHPtoStart = 100;
const int kHPtoEnd = 0;
const int kHPtoBoost = 30;

// - - Keyboard Controls
const EKeyCode kCameraUp = Key_Up;
const EKeyCode kCameraDown = Key_Down;
const EKeyCode kCameraRight = Key_Right;
const EKeyCode kCameraLeft = Key_Left;
const EKeyCode kChaseCamera = Key_1;
const EKeyCode kFPCamera = Key_2;
const EKeyCode kCarForward = Key_W;
const EKeyCode kCarBack = Key_S;
const EKeyCode kCarRight = Key_D;
const EKeyCode kCarLeft = Key_A;
const EKeyCode kStartGameKey = Key_Space;
const EKeyCode kBoostKey = Key_Space;

// - - Camera Constants
const float kCamerPosX = kCarPos.x;
const float kCameraPosY = kCarPos.y + 10.0f;
const float kCameraPosZ = kCarPos.z - 25.0f;
const float kCameraSpeed = kGameSpeed * 25.0f;
const float kMouseRotation = kGameSpeed * 0.15f;
const float kFPCamMoveZ = 55.0f;
const float kFPCamMoveY = -5.0f;

// - - Car Constants
const float kCarMaxSpeed = kGameSpeed * 3.0f; 
const float kCarThrustFactor = kCarMaxSpeed * 0.05f; 
const float kCarRevThrustFactor = -kCarThrustFactor * 0.5f;
const float kCarDragFactor = -kCarMaxSpeed * 0.001f;  
const float kCarSteerSpeed = kGameSpeed * 50.0f;
const float kCarRevMomentum = -0.5f;

const int kMatrixArrSize = 4;
const int kFirstElement = 0;
const int kReverseMomentum = -1;
const int kMatrixLocalZRow = 2;  //third row
const int kMatrixXVal = 0;  //first element
const int kMatrixZVal = 2;  //third element

// - - Backdrop Constants
const string kBackdropStr = "ui_backdrop.jpg";
const float kBackdropDimX = 700.0f;
const float kBackdropDimY = 60.0f;
const float kBackdropPosX = 570.0f;  //positions on screen
const float kBackdropPosY = 650.0f;  //(both) used to centre text using all of backdrop
const float kBackdropWidth = kBackdropPosX + kBackdropDimX;  //added to positions
const float kBackdropHeight = kBackdropPosY + kBackdropDimY;  //..for text placement
const float kBackdropStagePosX = kBackdropPosX + 300.0f; //for stage dialogue placement
const float kBackdropSpeedPosY = kBackdropPosY + 30.0f; //for speed dialogue placement
const float kBackdropBoostPosX = kBackdropPosX + 170.0f;  //for boost dialogue placement
const float kBackdropSpeedEndPosX = kBackdropPosX + 180.0f;  //ONLY for speed placement
const float kLTBackdropPosX = -520.0f;  //LT = Lap Time
const float kLTBackdropPosY = -20.0f; //..positions in x and y axis
const float kLTBackdropEndX = kBackdropDimX + kLTBackdropPosX;  //difference between dimensions
const float kLTBackdropEndY = kBackdropDimY + kLTBackdropPosY;  //..uses addition for small answer

// - - Text Constants
const int kReadyFontSize = 50;
const int kCDownFontSize = 70;
const int kStageFontSize = 45;
const int kSpeedFontSize = 25;
const int kBoostFontSize = 20;
const int kTextSizeArrSize = 2;
const int kTextPosX = 0;  //element 1 in array but 0 indexing
const int kTextPosY = 1;  //..element 2, relates to text height
const int kThreeSecs = 3;
const int kTwoSecs = 2;
const int kOneSec = 1;
const float kThreenHalfSecs = 3.5f;

const string kReadyFontName = "Anta";
const string kCDownFontName = "Georgia";
const string kStageFontName = "Helvetica";
const string kSpeedFontName = "Digital-7";
const string kBoostFontName = "Lucida Sans";
const string kSpaceToStart = "- - - Hit Space to Start - - -";
const string kThreeSecStr = "- - - 3 - - -";
const string kTwoSecStr = "- - 2 - -";
const string kOneSecStr = "- 1 -";
const string kGoStr = "GO!";
const string kStageCompP1Str = "- Stage ";
const string kStageCompP2Str = " Complete -";
const string kRaceCompStr = "RACE COMPLETE";
const string kRaceFailedStr = "RACE FAILED";
const string kSpeedStr = "Speed : ";
const string kHealthStr = "Health : ";
const string kLapTimeStr = "Lap Time : ";
const string kInSecsStr = " s";

// - - Collision Constants
const float kCheckpointRadii = 9.86f;
const float kCheckpointDimY = 10.6; //dim = dimensions
const float kCheckpointDimZ = 1.28f; //from model centre point
const float kCarRadius = 3.0f; //balanced, largest dim is approx 6.5 units
const float kStrutsRaduis = kCheckpointDimZ;  //struts on the end of checkpoints
const float kIsleDimX = 2.68f - 1.0f;  //decreased for smoother collisions
const float kIsleDimY = 2.73f - 1.0f; 
const float kIsleDimZ = 3.42f - 1.0f;  
const float kTankRadius = 4.12f - 1.0f;
const float kTankDimY = 8.42f;
const float kTribuneRadius = 3.5f;  //reduced since base of model smaller than top

// - - Boost Constants
const float kBoostThrustInc = kCarThrustFactor * 1.5f;  //thrust factor when boosting
const float kBoostRevThrustInc = kCarRevThrustFactor * 1.5f;
const float kBoostMaxTime = 3.0f;  //amount over which boost 'overheats'
const float kBoostWarning = kBoostMaxTime - 1.0f; //warning before overheating
const float kBoostCooldown = 5.0f;  //cooldown time after overheating
const float kBoostDragInc = kCarDragFactor * 2.0f;  //drag factor whilst boost is overheated

const int kPrecise2DP = 2;
const string kBoostTimeStr = "Boost Time:";
const string kBoostWarningStr = "Overheating!";
const string kBoostCooldownStr = "Cooldown Time:";

// - - Scale Constants
const int kMatrixX = 0;
const int kMatrixY = 1;
const int kMatrixZ = 2;
const float kScaleConstant = 0.774f; //my constant to convert tl-engine units to metres (units / metres)
const float kKmPersTokmPerH = 3.6f;

// - - Class Constants
const int kWayPointsTotal = 5;
const string kNPCModelSkin = "spBlue02_02.jpg";  //in ExtraModels Folder
const float kNPCarMoveSpeed = kCarThrustFactor * 150.0f;
const float kDummyRadiusSqrd = 1.5f;  //distance within which the current waypoint increments
const float kNPCarSpawnDist = 40.0f;  //spawn if car is in this range
const bool kNPCarEnable = true;
const bool kNPCarDisable = false;


// *** Classes
class cNPCar
{
//Attributes
private:
	IMesh* mCarMesh;
	IMesh* mDummyMesh;
	bool mSpawned = false;  //has car begun to move
	Vector3D mSpawnPoint;  //starting position
	int mCurrentWayPoint = 0; //start at first index of waypoints
	vector<IModel*> mDummyModels;

	//WayPoints Locations (currently static for one model only)
	//..must be same number as kWayPointsTotal
	vector<Vector3D> mPresetWayPoints = {
		{kCheckpoint4Pos.x, kZeroFloat, kCheckpoint4Pos.z},
		{kTank3Pos.x, kZeroFloat, -kTank2Pos.z},
		{kCheckpoint5Pos.x, kZeroFloat, kCheckpoint5Pos.z},
		{kCheckpoint6Pos.x, kZeroFloat, kTank4Pos.z},
		{kNPCarFinalPos.x, kNPCarFinalPos.y, kNPCarFinalPos.z} };


//Functions & Logic
public:

	//Constructor Class & Model Creation
	IModel* mNPCarModel;
	cNPCar(IMesh* carMesh, IMesh* dummyMesh, Vector3D spawnPoint)
	{
		mCarMesh = carMesh;
		mSpawnPoint = spawnPoint;
		mDummyMesh = dummyMesh;

		mNPCarModel = mCarMesh->CreateModel(mSpawnPoint.x, mSpawnPoint.y, mSpawnPoint.z);
		mNPCarModel->SetSkin(kNPCModelSkin);  //different colour
		for (int i = 0; i < kWayPointsTotal; i++)
		{
			mDummyModels.push_back(mDummyMesh->CreateModel(mPresetWayPoints[i].x, mPresetWayPoints[i].y, mPresetWayPoints[i].z));
		}
	}

	//Returns distance from next waypoint
	float DistanceFromWayPoint()
	{
		float distX, distY, distZ;
		distX = mNPCarModel->GetX() - mDummyModels.at(mCurrentWayPoint)->GetX();
		distY = mNPCarModel->GetY() - mDummyModels.at(mCurrentWayPoint)->GetY();
		distZ = mNPCarModel->GetZ() - mDummyModels.at(mCurrentWayPoint)->GetZ();
		return ((distX * distX) + (distY * distY) + (distZ * distZ));
	}

	//Moves towards the next way point
	void MoveTowardsWayPoint(float frameTime)
	{
		if (mSpawned)
		{
			mNPCarModel->LookAt(mDummyModels[mCurrentWayPoint]);
			mNPCarModel->MoveLocalZ(kNPCarMoveSpeed * frameTime);

			//Check to change waypoint using Point-Sphere calculation
			float distanceSqrd = DistanceFromWayPoint();
			if (distanceSqrd < kDummyRadiusSqrd)
			{
				//Completed all way points, stop movement
				if (mCurrentWayPoint + kIncrementBy1 == kWayPointsTotal)
				{
					mSpawned = kNPCarDisable;
				}
				else
				{
					mCurrentWayPoint++;
				}
			}
		}
	}

	IModel* GetModel()
	{
		return mNPCarModel;
	}

	void SetSpawned(bool spawn)
	{
		mSpawned = spawn;
	}
};


//Forward Declarations
Vector2D Scalar(Vector2D v, float scale);
Vector2D AddThreeVectors(Vector2D v1, Vector2D v2, Vector2D v3);
void TextAlignment(float textSizeArr[kTextSizeArrSize], string text, IFont* font, float startPosX,
	float startPosY, float maxPosX, float maxPosY);
bool CheckpointCollDetection(IModel* car, IModel* checkpoint, const int checkpointNum);
void DisplayStageComplete(int currentStage, IFont* font, float textSizeArr[kTextSizeArrSize]);
ECollisionArea SphereBoxCollision(IModel* car, IModel* model1, IModel* model2, float oldPosX, float oldPosZ);
bool SphereSphereCollision(IModel* car, IModel* modelX, float modelXRadius, float modelOffset, int matrixAxis,
	const float modelMatrix[kMatrixArrSize][kMatrixArrSize]);
float CarSpeedInMetres(Vector2D momentum, float scale);
void BounceCar(IModel* car, ECollisionArea& collisionSide, Vector2D& carMomentum);
void ResolveCollision(IModel* car, Vector2D& carMomentum, bool& collisioncheck, int& carHP);


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine ->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );
	myEngine->AddMediaFolder(".\\A2Models");
	myEngine->AddMediaFolder(".\\ExtraModels");

	/**** Set up your scene here ****/

	//Camera Setup (Chase)
	ICamera* myCamera = myEngine->CreateCamera(kManual, kCamerPosX, kCameraPosY, kCameraPosZ);
	//Mouse Movement for Camera
	float mouseMoveX = 0;  //track cumulative movement of mouse in y axis
	float mouseMoveY = 0; //track cumulative movement of mouse in x axis
	myEngine->StartMouseCapture();

	//Mesh Initialisation
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	IMesh* isleMesh = myEngine->LoadMesh("IsleStraight.x");
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IMesh* carMesh = myEngine->LoadMesh("race2.x");
	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IMesh* tanksMesh = myEngine->LoadMesh("TankSmall2.x");
	IMesh* interstellarMesh = myEngine->LoadMesh("Tribune1.x");
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");

	//Model's Intialisation
	IModel* skybox = skyboxMesh->CreateModel(kZeroFloat, kSkyBoxPosY, kZeroFloat);
	IModel* ground = groundMesh->CreateModel();
	IModel* car = carMesh->CreateModel(kCarPos.x, kCarPos.y, kCarPos.z);

	//Checkpoint's Setup
	vector<Vector3D> checkpointPos = {
		{kZeroFloat, kZeroFloat, kZeroFloat},
		{kCheckpoint2Pos.x, kCheckpoint2Pos.y, kCheckpoint2Pos.z},
		{kCheckpoint3Pos.x, kCheckpoint3Pos.y, kCheckpoint3Pos.z},
		{kCheckpoint4Pos.x, kCheckpoint4Pos.y, kCheckpoint4Pos.z},
		{kCheckpoint5Pos.x, kCheckpoint5Pos.y, kCheckpoint5Pos.z},
		{kCheckpoint6Pos.x, kCheckpoint6Pos.y, kCheckpoint6Pos.z}};
	vector<IModel*> checkpointVector;
	for (int i = 0; i < kCheckpointTotal; i++)
	{
		checkpointVector.push_back(checkpointMesh->CreateModel(checkpointPos[i].x, checkpointPos[i].y, checkpointPos[i].z));
	}
	checkpointVector[kCheckpoint2]->RotateLocalY(kRotateBy90);
	checkpointVector[kCheckpoint4]->RotateLocalY(kRotateBy90);
	checkpointVector[kCheckpoint5]->RotateLocalY(kRotateBy45);
	float collisionsMatrix[kMatrixArrSize][kMatrixArrSize];

	//Isle Setup
	vector<Vector3D> islePos = {
		{kIsle1PosX, kZeroFloat, kIsle1aPosZ},
		{kIsle1PosX, kZeroFloat, kIsle1bPosZ},
		{kIsle1PosX, kZeroFloat, kIsle1cPosZ},
		{-kIsle1PosX, kZeroFloat, kIsle1aPosZ},
		{-kIsle1PosX, kZeroFloat, kIsle1bPosZ},
		{-kIsle1PosX, kZeroFloat, kIsle1cPosZ},
		{kIsle2aPosX, kZeroFloat, kIsle1aPosZ},
		{kIsle2aPosX, kZeroFloat, kIsle1bPosZ},
		{kIsle2aPosX, kZeroFloat, kIsle1cPosZ},
		{kIsle2bPosX, kZeroFloat, kIsle1aPosZ},
		{kIsle2bPosX, kZeroFloat, kIsle1bPosZ},
		{kIsle2bPosX, kZeroFloat, kIsle1cPosZ}};
	vector<IModel*> isleVector;
	int isleIndex = 0; //used for looping in game code
	for (int i = 0; i < kIsleTotal; i++)
	{
		isleVector.push_back(isleMesh->CreateModel(islePos[i].x, islePos[i].y, islePos[i].z));
	}

	//Wall Setup
	vector<Vector3D> wallPos = {
		{kIsle1PosX, kZeroFloat, kWall1aPosZ},
		{kIsle1PosX, kZeroFloat, kWall1bPosZ},
		{-kIsle1PosX, kZeroFloat, kWall1aPosZ},
		{-kIsle1PosX, kZeroFloat, kWall1bPosZ},
		{kIsle2aPosX, kZeroFloat, kWall1aPosZ},
		{kIsle2aPosX, kZeroFloat, kWall1bPosZ},
		{kIsle2bPosX, kZeroFloat, kWall1aPosZ},
		{kIsle2bPosX, kZeroFloat, kWall1bPosZ}};
	vector<IModel*> wallVector;
	for (int i = 0; i < kWallTotal; i++)  //can re-use some Isle Positions
	{
		wallVector.push_back(wallMesh->CreateModel(wallPos[i].x, wallPos[i].y, wallPos[i].z));
	}

	//Tanks Setup
	vector<Vector3D> tanksPos = {
		{kTank1Pos.x, kTank1Pos.y, kTank1Pos.z},
		{kTank2Pos.x, kTank2Pos.y, kTank2Pos.z},
		{kTank3Pos.x, kTank3Pos.y, kTank3Pos.z},
		{kTank4Pos.x, kTank4Pos.y, kTank4Pos.z},
		{kTank5Pos.x, kTank5Pos.y, kTank5Pos.z}};
	vector<IModel*> tanksVector;
	for (int i = 0; i < kTanksTotal; i++)
	{
		tanksVector.push_back(tanksMesh->CreateModel(tanksPos[i].x, tanksPos[i].y, tanksPos[i].z));
	}
	tanksVector[kTank3]->RotateLocalX(kRotateBy45);

	//Tribune Setup
	vector<Vector3D> tribunePos{
		{kTribune1Pos.x, kTribune1Pos.y, kTribune1Pos.z},
		{kTribune2Pos.x, kTribune2Pos.y, kTribune2Pos.z},
		{kTribune3Pos.x, kTribune3Pos.y, kTribune3Pos.z},
		{kTribune4Pos.x, kTribune4Pos.y, kTribune4Pos.z} };
	vector<IModel*> tribuneVector;
	for (int i = 0; i < kTribuneTotal; i++)
	{
		tribuneVector.push_back(interstellarMesh->CreateModel(tribunePos[i].x, tribunePos[i].y, tribunePos[i].z));
	}

	//Car Movement Setup
	myCamera->AttachToParent(car);  //camera movement with car
	Vector2D carMomentum = { kZeroFloat, kZeroFloat };
	Vector2D carThrust = { kZeroFloat, kZeroFloat };
	Vector2D carDrag = { kZeroFloat, kZeroFloat };
	Vector2D carFacingVector = { kZeroFloat, kZeroFloat };
	Vector3D oldCarPos = { kCarPos.x, kCarPos.y, kCarPos.z };
	float carMatrix[kMatrixArrSize][kMatrixArrSize];

	//Backdrop Setup - (screen size = 720x1280)
	ISprite* backdrop = myEngine->CreateSprite(kBackdropStr, kBackdropPosX, kBackdropPosY);
	ISprite* ltBackdrop = myEngine->CreateSprite(kBackdropStr, kLTBackdropPosX, kLTBackdropPosY);  //laptime backdrop

	//Text Setup
	float textSizeArr[kTextSizeArrSize];  //re-used to store the size of text
	IFont* readyFont = myEngine->LoadFont(kReadyFontName, kReadyFontSize);
	IFont* countdownFont = myEngine->LoadFont(kCDownFontName, kCDownFontSize);
	IFont* stageFont = myEngine->LoadFont(kStageFontName, kStageFontSize);
	IFont* speedFont = myEngine->LoadFont(kSpeedFontName, kSpeedFontSize);
	IFont* boostFont = myEngine->LoadFont(kBoostFontName, kBoostFontSize);

	//Non-Player Car (NPCar) Setup
	cNPCar npCar1(carMesh, dummyMesh, kNPCar1Pos);

	//Game Control Variables
	////////////////////////

	myEngine->Timer(); //make movement constant on all machines
	float frameTime;  //stores time taken to render each frame
	float carSpeed = kZeroFloat;  //tracks cumulative car speed
	float carSteering = kZeroFloat;  //tracks cumulative car steering amount
	float countdownTimer = kZeroFloat;
	float lapTimer = kZeroFloat;  //time taken to complete a lap
	float boostTimer = kZeroFloat;  //tracks cumulative time boost is active
	float boostCooldown = kBoostCooldown;  //boost unusable for set amount of time
	float tempTankRadius;  //different radii for underground tank
	float activeThrustFactor; //changes thrust value when boosting
	float activeRevThrustFactor;
	float activeDragFactor;

	int currentCarSpeed = kZeroFloat;  //tracks car speed to display in km/h
	int carHP = kHPtoStart;  //decreased by 1 per collision

	bool startCountdown = false;  //true when collision has occurred, reused
	bool checkpointColl = false;  //true when collision has occurred, reused
	bool strutColl = false;  //true when collision has occurred, reused
	bool tanksColl = false;  //true when collision has occurred, reused
	bool tribuneColl = false;  //true when collision has occurred, reused
	bool npCarColl = false;  //true when collision has occurred, reused
	bool userWin = false;  //true when user completes all checkpoints
	bool chaseCamera = true;  //controls camera change between first-person and default chase
	bool boostAble = true;  //boost usable in certain conditions
	bool boostAvailable = true;  //boost cool down control
	bool npc1Spawn = false;  //goes true once npcar1 is activated
	bool npc1Active = false;  //true once npcar1 activated, ensures only happens once

	string raceEndStr;
	stringstream carSpeedDisplay;
	stringstream carHealthDisplay;
	stringstream boostMessageDisplay;
	stringstream boostTimeDisplay;
	stringstream lapTimeDisplay;
	ECollisionArea wallColl = NOSIDE;

	//Game States Setup
	EGameStates currentState = READY;
	vector<EGameStages> vectorGameStages = { NONE, ONE, TWO, THREE, FOUR, FIVE, SIX };
	int currentStageNum = 1; //increments as we go through stages
	EGameStages currentStage = vectorGameStages.at(currentStageNum);

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		//Values to update at the beginning of each frame
		frameTime = myEngine->Timer();
		oldCarPos.x = car->GetX();
		oldCarPos.y = car->GetY();
		oldCarPos.z = car->GetZ();

		/**** Update your scene each frame here ****/

		//Camera Control Setup
		//////////////////////

		//Default Chase Camera Setup
		if (chaseCamera)
		{
			//Arrow Keys Control
			if (myEngine->KeyHeld(kCameraUp))
			{
				myCamera->MoveLocalZ(kCameraSpeed * frameTime);
			}
			if (myEngine->KeyHeld(kCameraDown))
			{
				myCamera->MoveLocalZ(-kCameraSpeed * frameTime);
			}
			if (myEngine->KeyHeld(kCameraRight))
			{
				myCamera->MoveLocalX(kCameraSpeed * frameTime);
			}
			if (myEngine->KeyHeld(kCameraLeft))
			{
				myCamera->MoveLocalX(-kCameraSpeed * frameTime);
			}

			//Mouse Movement
			mouseMoveX += myEngine->GetMouseMovementX();  //track movement
			mouseMoveY += myEngine->GetMouseMovementY();  //..cumulatively

			//Rotate camera based on mouse movement
			myCamera->ResetOrientation(); //no upside-down issues
			myCamera->RotateLocalY(mouseMoveX * kMouseRotation);
			myCamera->RotateLocalX(mouseMoveY * kMouseRotation);

			//Switch to First-Person Camera based on Key Hit
			if (myEngine->KeyHit(kFPCamera))
			{
				myCamera->SetLocalPosition(kCamerPosX, kCameraPosY, kCameraPosZ);  //back to starting position
				myCamera->MoveLocalZ(kFPCamMoveZ);  //position camera in front of car
				myCamera->MoveLocalY(kFPCamMoveY);  //not dependant on movement per frame
				chaseCamera = false;
			}
		}
		else
		{
			//Switch to Chase Camera based on Key Hit
			if (myEngine->KeyHit(kChaseCamera))
			{
				myCamera->MoveLocalZ(-kFPCamMoveZ);  //reverse camera positioning
				myCamera->MoveLocalY(-kFPCamMoveY);
				chaseCamera = true;
			}
		}

		//Game Logic
		////////////

		//While the game state is in ready
		if (currentState == READY)
		{
			//Begin countdown when space key is hit
			if (myEngine->KeyHit(kStartGameKey))
			{
				startCountdown = true;
			}

			//Change backdrop output based on time left for game to start (in secs)
			if (startCountdown)
			{
				countdownTimer += frameTime; //increment to track countdown
				if (countdownTimer <= kOneSec)
				{
					TextAlignment(textSizeArr, kThreeSecStr, countdownFont, kBackdropPosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
					countdownFont->Draw(kThreeSecStr, textSizeArr[kTextPosX], textSizeArr[kTextPosY], kRed);
				}
				else if (countdownTimer > kOneSec && countdownTimer < kTwoSecs)  //two secs remaining
				{
					TextAlignment(textSizeArr, kTwoSecStr, countdownFont, kBackdropPosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
					countdownFont->Draw(kTwoSecStr, textSizeArr[kTextPosX], textSizeArr[kTextPosY], kRed);
				}
				else if (countdownTimer > kTwoSecs && countdownTimer < kThreeSecs) //one sec remaining
				{
					TextAlignment(textSizeArr, kOneSecStr, countdownFont, kBackdropPosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
					countdownFont->Draw(kOneSecStr, textSizeArr[kTextPosX], textSizeArr[kTextPosY], kRed);
				}
				else if (countdownTimer > kThreeSecs) //countdown up
				{
					currentState = ONGOING;  //begin game
				}
			}
			//User has not hit space key to start countdown yet
			else
			{
				TextAlignment(textSizeArr, kSpaceToStart, readyFont, kBackdropPosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
				readyFont->Draw(kSpaceToStart, textSizeArr[kTextPosX], textSizeArr[kTextPosY], kBlack);
			}
		}

		//While current game state is ongoing
		if (currentState == ONGOING)
		{
			lapTimer += frameTime;  //increase laptime

			//Boost Setup
			/////////////////////

			//Boost Functionality
			if (myEngine->KeyHeld(kBoostKey) && boostAble)
			{
				activeThrustFactor = kBoostThrustInc;
				activeRevThrustFactor = kBoostRevThrustInc;

				//Boost overheats beyond a certain time and car drag increases
				boostTimer += frameTime;
				if (boostTimer > kBoostMaxTime)
				{
					activeDragFactor = kBoostDragInc;  //increase drag factor
					boostAble = false;
				}

				//Warning in boost dialogue, a set time before about to overheat
				boostTimeDisplay << fixed << setprecision(kPrecise2DP) << boostTimer;
				if ((boostTimer > kBoostWarning) && (boostTimer < kBoostMaxTime))
				{
					boostMessageDisplay << kBoostWarningStr;
				}
				else
				{
					boostMessageDisplay << kBoostTimeStr;
				}
			}
			else
			{
				activeThrustFactor = kCarThrustFactor;
				activeRevThrustFactor = kCarRevThrustFactor;
				activeDragFactor = kCarDragFactor;

				if (boostAble)  //User choosing not to boost
				{
					boostMessageDisplay << kBoostTimeStr;
					boostTimeDisplay << fixed << setprecision(kPrecise2DP) << boostTimer;
				}

				//Decrease timer so boost doesn't overheat
				boostTimer -= frameTime;
				if (boostTimer < kZeroFloat)
				{
					boostTimer = kZeroFloat;
				}
			}

			//Boost Cooldown
			if (!boostAble)
			{
				boostCooldown -= frameTime;  //decrease cooldown
				boostMessageDisplay << kBoostCooldownStr;
				boostTimeDisplay << fixed << setprecision(kPrecise2DP) << boostCooldown;
				if (boostCooldown <= kZeroFloat)
				{
					boostCooldown = kBoostCooldown;  //reset cooldown
					boostAble = true;  //now able to boost
				}
			}
			else
			{
				activeDragFactor = kCarDragFactor;  //drag factor returns to normal
			}

			//Display Boost Information
			TextAlignment(textSizeArr, boostMessageDisplay.str(), boostFont, kBackdropBoostPosX, kBackdropPosY, kBackdropStagePosX, kBackdropSpeedPosY);
			boostFont->Draw(boostMessageDisplay.str(), textSizeArr[kTextPosX], textSizeArr[kTextPosY], kRed);
			boostMessageDisplay.str("");  //clear sstream
			TextAlignment(textSizeArr, boostTimeDisplay.str(), boostFont, kBackdropBoostPosX, kBackdropSpeedPosY, kBackdropStagePosX, kBackdropHeight);
			boostFont->Draw(boostTimeDisplay.str(), textSizeArr[kTextPosX], textSizeArr[kTextPosY], kBlack);
			boostTimeDisplay.str("");  //clear sstream

			//Car Control Setup
		    ///////////////////

		    //Find facing vector (getting the local z axis, x and z components)
			car->GetMatrix(&carMatrix[kFirstElement][kFirstElement]); //get the model matrix
			carFacingVector = { carMatrix[kMatrixLocalZRow][kMatrixXVal], carMatrix[kMatrixLocalZRow][kMatrixZVal] };

			//WS Key's Control - Acceleration & Deceleration
			if ((myEngine->KeyHeld(kCarForward)) && (carSpeed < kCarMaxSpeed))
			{
				carThrust = Scalar(carFacingVector, activeThrustFactor);
			}
			else if ((myEngine->KeyHeld(kCarBack)) && (carSpeed > -kCarMaxSpeed))
			{
				carThrust = Scalar(carFacingVector, activeRevThrustFactor);
			}
			else
			{
				carThrust = { kZeroFloat, kZeroFloat }; //no thrust
			}

			//Calculate Drag, update Momentum & Move Car
			carDrag = Scalar(carMomentum, activeDragFactor);  //drifting
			carMomentum = AddThreeVectors(carMomentum, carThrust, carDrag);
			car->Move(carMomentum.x * frameTime, kZeroFloat, carMomentum.z * frameTime);  //no movement in y-axis

			//Display Car Speed (in km/h)
			currentCarSpeed = CarSpeedInMetres(carMomentum, kScaleConstant);  //speed in meters per second
			currentCarSpeed *= kKmPersTokmPerH;  //speed now in kilometres per hour
			carSpeedDisplay << kSpeedStr << currentCarSpeed << " km/h";
			TextAlignment(textSizeArr, carSpeedDisplay.str(), speedFont, kBackdropPosX, kBackdropSpeedPosY, kBackdropSpeedEndPosX, kBackdropHeight);
			speedFont->Draw(carSpeedDisplay.str(), textSizeArr[kTextPosX], textSizeArr[kTextPosY], kBlue);
			carSpeedDisplay.str("");  //clear sstream
			carHealthDisplay << kHealthStr << carHP;
			TextAlignment(textSizeArr, carHealthDisplay.str(), speedFont, kBackdropPosX, kBackdropPosY, kBackdropBoostPosX, kBackdropSpeedPosY);
			speedFont->Draw(carHealthDisplay.str(), textSizeArr[kTextPosX], textSizeArr[kTextPosY], kBlack);
			carHealthDisplay.str("");  //clear sstream

			//AD Key's Control - Steering
			if (myEngine->KeyHeld(kCarRight))
			{
				car->RotateLocalY(kCarSteerSpeed * frameTime);
			}
			if (myEngine->KeyHeld(kCarLeft))
			{
				car->RotateLocalY(-kCarSteerSpeed * frameTime);
			}

			//Show "GO" for a second follow up
			if (countdownTimer > kThreeSecs && countdownTimer < kThreenHalfSecs)
			{
				TextAlignment(textSizeArr, kGoStr, countdownFont, kBackdropStagePosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
				countdownFont->Draw(kGoStr, textSizeArr[kTextPosX], textSizeArr[kTextPosY], kRed);
				countdownTimer += frameTime;
			}

			//NPCar Movement.. begins to move when car travels within a certain area, only happens ONCE
			npc1Spawn = SphereSphereCollision(car, npCar1.GetModel(), kNPCarSpawnDist, kZeroFloat, kMatrixX, collisionsMatrix);
			if (npc1Spawn)
			{
				if (!npc1Active)
				{
					npc1Active = true;
					npCar1.SetSpawned(kNPCarEnable);
				}
			}
			npCar1.MoveTowardsWayPoint(frameTime);  //keep moving once activated

			//Stage Changes
			///////////////

			if (currentStage == vectorGameStages.at(currentStageNum))
			{
				//Vector adjusted with -1 since it is a 0 index start
				checkpointColl = CheckpointCollDetection(car, checkpointVector.at(currentStageNum - kStageIncrement), 
					currentStageNum - kStageIncrement);
				if (checkpointColl)  //collision occurred as car passes the correct checkpoint
				{
					checkpointColl = false; //reuse in collision detection
					currentStageNum += kStageIncrement; //increment to next stage

					//End Game once past final checkpoint
					if (currentStageNum == kStageEnd)
					{
						userWin = true;  //user completed the track
						currentState = END;
					}
					else
					{
						currentStage = vectorGameStages.at(currentStageNum); //change currentStage variable
					}
				}
			}
			if (currentStageNum >= kStageTwo)  //display latest stage completion
			{
				DisplayStageComplete(currentStageNum - kStageIncrement, stageFont, textSizeArr); //only completed one stage
			}

			//Changes in gameplay based on Car's Health
			if (carHP < kHPtoEnd)  //game ends if car drops below 0 hp
			{
				userWin = false;  //user failed to complete the track
				currentState = END;
			}
			if (!(carHP >= kHPtoBoost)) //unable to boost if health drops below amount
			{
				boostAble = false;
			}

			//Collision Detection
			/////////////////////

			//Collision between Car and Struts (the end of checkpoints)
			for (int i = 0; i < kCheckpointTotal; i++)
			{
				checkpointVector.at(i)->GetMatrix(&collisionsMatrix[kFirstElement][kFirstElement]);  //get matrix for relevant checkpoint
				strutColl = SphereSphereCollision(car, checkpointVector.at(i), kStrutsRaduis, kCheckpointRadii, kMatrixX , collisionsMatrix);
				if (strutColl)
				{
					//Return car to position it was at before collision occurred
					car->SetLocalPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					ResolveCollision(car, carMomentum, strutColl, carHP);
				}
			}

			//Collision between Car and Isle/Wall barriers
			for (int i = 0; i < kWallTotal; i++)
			{
				//Isles next to each other in pairs with index difference 1 (e.g. 0-1, 1-1, 2-3, 3-4)
				wallColl = SphereBoxCollision(car, isleVector.at(isleIndex), isleVector.at(isleIndex + kIncrementBy1), oldCarPos.x, oldCarPos.z);
				if (wallColl != NOSIDE)
				{
					//Return car to position it was at before collision occurred
					car->SetLocalPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					BounceCar(car, wallColl, carMomentum);
					carHP--;  //decrease car health
				}

				//Increment to next set of Isle/Walls
				isleIndex++;
				if (((isleIndex + kIncrementBy1) % kIslePerWall) == 0) //skip isle/walls between (e.g. 3-4)
				{
					isleIndex++;
				}
			}
			isleIndex = kIsleIndexReset;  //reset for reuse 

			//Collision between Car and Tanks
			for (int i = 0; i < kTanksTotal; i++)
			{
				tanksVector.at(i)->GetMatrix(&collisionsMatrix[kFirstElement][kFirstElement]);
				(i == kTank3) ? (tempTankRadius = kTankDimY) : (tempTankRadius = kZeroFloat);  //adjust offset for underground tanks
				tanksColl = SphereSphereCollision(car, tanksVector.at(i), kTankRadius, tempTankRadius, kMatrixY ,collisionsMatrix);
				if (tanksColl)
				{
					//Return car to position it was at before collision occurred
					car->SetLocalPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					ResolveCollision(car, carMomentum, tanksColl, carHP);
				}
			}

			//Collision between Car and Tribunes
			for (int i = 0; i < kTribuneTotal; i++)
			{
				tribuneColl = SphereSphereCollision(car, tribuneVector.at(i), kTribuneRadius, kZeroFloat, kMatrixX, collisionsMatrix);
				if (tribuneColl)  //no rotation therefore no matrix manipulation required
				{
					//Return car to position it was at before collision occurred
					car->SetLocalPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
					ResolveCollision(car, carMomentum, tribuneColl, carHP);
				}
			}

			//Collision between Car and Non-Player Cars
			npCarColl = SphereSphereCollision(car, npCar1.GetModel(), kCarRadius, kZeroFloat, kMatrixX, collisionsMatrix);
			if (npCarColl)
			{
				//Return car to position it was at before collision occurred
				car->SetLocalPosition(oldCarPos.x, oldCarPos.y, oldCarPos.z);
				ResolveCollision(car, carMomentum, npCarColl, carHP);
			}
		}

		//While current game state is 'End'
		if (currentState == END)
		{
			(userWin) ? (raceEndStr = kRaceCompStr) : (raceEndStr = kRaceFailedStr);  //display complete or failed based on player's progress
			TextAlignment(textSizeArr, raceEndStr, countdownFont, kBackdropPosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
			countdownFont->Draw(raceEndStr, textSizeArr[kTextPosX], textSizeArr[kTextPosY], kMagenta);
		}

		//Display Lap Time at all times
		lapTimeDisplay << kLapTimeStr << fixed << setprecision(kPrecise2DP) <<lapTimer << kInSecsStr;
		TextAlignment(textSizeArr, lapTimeDisplay.str(), speedFont, kZeroFloat, kZeroFloat, kLTBackdropEndX, kLTBackdropEndY);
		speedFont->Draw(lapTimeDisplay.str(), textSizeArr[kTextPosX], textSizeArr[kTextPosY], kDarkGrey);
		lapTimeDisplay.str(""); //clear sstream

		//End Program Immediately when the end key is hit (esc)
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}


//Multiplies a Vector2D's values by scalar given
Vector2D Scalar(Vector2D v, float scale)
{
	return { v.x * scale, v.z * scale };
}

//Add's all 3 vector values per component, returned as a 2D vector
Vector2D AddThreeVectors(Vector2D v1, Vector2D v2, Vector2D v3)
{
	return { v1.x + v2.x + v3.x , v1.z + v2.z + v3.z };
}

//Sets a float value to place text based on the size of the backdrop
//Should add on to the given backdrop positions to place text in centre
void TextAlignment(float textSizeArr[kTextSizeArrSize], string text, IFont* font, float startPosX, 
	float startPosY, float maxPosX, float maxPosY)
{
	//Measure text size
	const int kSides = 2;
	float textWidth = font->MeasureTextWidth(text);
	float textHeight = font->MeasureTextHeight(text);

	//Finds the difference between the axis & divides to get central placement for text
	float difference = (maxPosX - startPosX) - textWidth;  //X-axis placement
	float alignment = difference / kSides;
	textSizeArr[kTextPosX] = startPosX + alignment;
	difference = (maxPosY - startPosY) - textHeight; //Y-axis placement
	alignment = difference / kSides;
	textSizeArr[kTextPosY] = startPosY + alignment;
}

//Point-Box collision detection to check whether car has gone through a given checkpoint
//..returns true if it has or false if it hasn't
bool CheckpointCollDetection(IModel* car, IModel* checkpoint, const int checkpointNum)
{
	//Find required position values & Initialise Bounding Box
	float carX = car->GetX();
	float carY = car->GetY();
	float carZ = car->GetZ();
	bool collisionResult = false;

	//Adjust bounding box since Get returns centre of model
	float cPointMinX, cPointMaxX, cPointMinY, cPointMaxY, cPointMinZ, cPointMaxZ;
	if (checkpointNum == kCheckpoint2 || checkpointNum == kCheckpoint4) //rotated models
	{
		cPointMinX = checkpoint->GetX() - (kCheckpointDimZ);
		cPointMaxX = checkpoint->GetX() + (kCheckpointDimZ);
		cPointMinZ = checkpoint->GetZ() - (kCheckpointRadii);
		cPointMaxZ = checkpoint->GetZ() + (kCheckpointRadii);
	}
	else
	{
		cPointMinX = checkpoint->GetX() - (kCheckpointRadii);
		cPointMaxX = checkpoint->GetX() + (kCheckpointRadii);
		cPointMinZ = checkpoint->GetZ() - (kCheckpointDimZ);
		cPointMaxZ = checkpoint->GetZ() + (kCheckpointDimZ);
	} 
	cPointMinY = checkpoint->GetY() - (kCheckpointDimY);
	cPointMaxY = checkpoint->GetY() + (kCheckpointDimY);

	if ((carX > cPointMinX) && (carX < cPointMaxX) &&
		(carY > cPointMinY) && (carY < cPointMaxY) &&
		(carZ > cPointMinZ) && (carZ < cPointMaxZ))
	{
		collisionResult = true;  //collision has occurred
	}
	return collisionResult;  //no collision will return false
}	

//Displays a message in accordance to the latest stage completed by the player
void DisplayStageComplete(int currentStage, IFont* font, float textSizeArr[kTextSizeArrSize])
{
	stringstream stageCompStr;
	stageCompStr << kStageCompP1Str << currentStage << kStageCompP2Str;  //based on current stage
	TextAlignment(textSizeArr, stageCompStr.str(), font, kBackdropStagePosX, kBackdropPosY, kBackdropWidth, kBackdropHeight);
	font->Draw(stageCompStr.str(), textSizeArr[kTextPosX], textSizeArr[kTextPosY], kGreen);
}

//Sphere-Box collision detection, uses 2 models to create a bounding box between
//Returns true if a collision has occurred, false if there hasn't
ECollisionArea SphereBoxCollision(IModel* car, IModel* model1, IModel* model2, float oldPosX, float oldPosZ)
{
	//Adapted bounding box for 2 isle models
	ECollisionArea collisionSide = NOSIDE;
	float boxMinX, boxMaxX, boxMinY, boxMaxY, boxMinZ, boxMaxZ;
	//Find which model length is smaller and set that as min, the other as max
	if (model1->GetX() <= model2->GetX())
	{
		boxMinX = model1->GetX() - kIsleDimX - kCarRadius;
		boxMaxX = model2->GetX() + kIsleDimX + kCarRadius;
	}
	else
	{
		boxMinX = model2->GetX() - kIsleDimX - kCarRadius;
		boxMaxX = model1->GetX() + kIsleDimX + kCarRadius;
	}
	if (model1->GetY() <= model2->GetY())  //negligible since models on ground
	{
		boxMinY = model1->GetY() - kIsleDimY - kCarRadius;
		boxMaxY = model2->GetY() + kIsleDimY + kCarRadius;
	}
	else
	{
		boxMinY = model2->GetY() - kIsleDimY - kCarRadius;
		boxMaxY = model1->GetY() + kIsleDimY + kCarRadius;
	}
	if (model1->GetZ() <= model2->GetZ())
	{
		boxMinZ = model1->GetZ() - kIsleDimY - kCarRadius;
		boxMaxZ = model2->GetZ() + kIsleDimZ + kCarRadius;
	}
	else
	{
		boxMinZ = model2->GetZ() - kIsleDimY - kCarRadius;
		boxMaxZ = model1->GetZ() + kIsleDimZ + kCarRadius;
	}

	//Get car Positions as a Point
	float carX = car->GetX();
	float carY = car->GetY();
	float carZ = car->GetZ();

	//Point-Box Check for Sphere-Box since Bounding Box is adjusted
	if ((carX > boxMinX) && (carX < boxMaxX) &&
		(carY > boxMinY) && (carY < boxMaxY) &&
		(carZ > boxMinZ) && (carZ < boxMaxZ))
	{
		//Find out which side collision has occurred, Y axis is negligible
		if (oldPosX <= boxMinX)
		{
			collisionSide = LEFTSIDE;
		}
		else if (oldPosX >= boxMaxX)
		{
			collisionSide = RIGHTSIDE;
		}
		else if (oldPosZ <= boxMinZ)
		{
			collisionSide = FRONTSIDE;
		}
		else if (oldPosZ >= boxMaxZ)
		{
			collisionSide = BACKSIDE;
		}
	}

	return collisionSide;
}

//Sphere-Sphere Bounding box, squares instead of sqr root for speed
//Returns true if a collision has occurred, false if there hasn't
bool SphereSphereCollision(IModel* car, IModel* modelX, float modelXRadius, float modelOffset, int matrixAxis,
	const float modelMatrix[kMatrixArrSize][kMatrixArrSize])
{
	//Matrix Values used to accomodate model rotation
	float matrixValX = modelMatrix[matrixAxis][kMatrixX]; 
	float matrixValY = modelMatrix[matrixAxis][kMatrixY];  
	float matrixValZ = modelMatrix[matrixAxis][kMatrixZ];  

	//Find distance between models
	//Offsets used since struts are at the side of model, parameters should be 0 when not needed
	float valX = car->GetX() - (modelX->GetX() - (modelOffset * matrixValX));
	float valY = car->GetY() - (modelX->GetY() - (modelOffset * matrixValY));  
	float valZ = car->GetZ() - (modelX->GetZ() - (modelOffset * matrixValZ));

	//Second side of model, modelOffset or matrixVal being 0 would render this useless
	float valX2 = car->GetX() - (modelX->GetX() + (modelOffset * matrixValX));
	float valY2 = car->GetY() - (modelX->GetY() + (modelOffset * matrixValY)); 
	float valZ2 = car->GetZ() - (modelX->GetZ() + (modelOffset * matrixValZ));

	float distSqrd = ((valX * valX) + (valY * valY) + (valZ * valZ));
	float distSqrd2 = ((valX2 * valX2) + (valY2 * valY2) + (valZ2 * valZ2));
	float totalRadiusSqrd = (kCarRadius + modelXRadius) * (kCarRadius + modelXRadius);
	bool collisionResult = false;

	//Check if a collision has occurred
	if ((distSqrd < totalRadiusSqrd) || (distSqrd2 < totalRadiusSqrd))
	{
		collisionResult = true;
	}
	return collisionResult;  //no collision will return false
}

//Returns the car speed by multiplying the momentum vector length by scale constant
float CarSpeedInMetres(Vector2D momentum, float scale)
{
	float length = sqrt((momentum.x * momentum.x) + (momentum.z * momentum.z));
	return (scale * length);
}

//Changes car movement so that it has a 'bounce' effect by using the side where collision has occurred
void BounceCar(IModel* car, ECollisionArea& collisionSide, Vector2D& carMomentum)
{
	if ((collisionSide == FRONTSIDE) || (collisionSide == BACKSIDE))
	{
		carMomentum.z = -(carMomentum.z);  //reverse z axis movement
	}
	else if ((collisionSide == RIGHTSIDE) || (collisionSide == LEFTSIDE))
	{
		carMomentum.x = -(carMomentum.x);  //reverse x axis movement
	}
	collisionSide = NOSIDE;  //reset for reuse
}

//Basic collision resolution - reverse momentum, decrease HP, allow further collisions
void ResolveCollision(IModel* car, Vector2D& carMomentum, bool& collisioncheck, int& carHP)
{
	carMomentum = Scalar(carMomentum, kReverseMomentum);
	collisioncheck = false;  //reset for reuse
	carHP--;
}