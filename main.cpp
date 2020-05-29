#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

//variables related with area size of 3d function------------------
short int solAreaMinX, solAreaMaxX, solAreaMinY, solAreaMaxY;//decided to not implement float here
float solAreaX, solAreaY;
float minFSolArea, maxFSolArea;//for getting min and max values
float minXYSolArea[2], maxXYSolArea[2];//for getting arg of min and max values

float xf, yf;//supplementary variables

//variable storing data of selected mode and target---------------------------
bool minMaxMode;
float selectedPoint[3];

//variables related with PSO algorithm-----------------------------
short int numberOfParticles, numberOfIterations;
float inertiaScale, learningFactor1, learningFactor2;
float bestResultsData[4]; //array to store overallBestPosition and overallBestError

//functions
short int genNum(short int min, short int max){
	return rand() % (max - min -1) + min;
}
void particlesInitialization(float *solArea, float *particleData, short int sizeOfParticleData, short int solAreaMinX, short int solAreaMaxX, short int solAreaMinY, short int solAreaMaxY){
	srand( time( NULL ) );
	for(short int x=0; x<sizeOfParticleData;){
		//generating starting positions
		particleData[x]=genNum(solAreaMinX*10,solAreaMaxX*10)/10.0;//x
		particleData[x+1]=genNum(solAreaMinY*10,solAreaMaxY*10)/10.0;//y
		//int temp1 = (particleData[x]*10)-(int)solAreaMinX*10;
		//int temp2 = (particleData[x+1]*10)-(int)solAreaMinY*10;
		//particleData[x+2]=solArea[temp1][temp2];
		//DO ZAIMPLEMENTOWANIA - LOSOWANIE PREDKOSCI
		//generating starting velocity
		particleData[x+3]=genNum(solAreaMinX*10-particleData[x]*10,solAreaMaxX*10-particleData[x]*10)/10.0;//velocity for x
		particleData[x+4]=genNum(solAreaMinY*10-particleData[x+1]*10,solAreaMaxY*10-particleData[x+1]*10)/10.0;//velocity for y
		particleData[x+5]=genNum((particleData[x+3])*10,(particleData[x+4])*10)/10.0;//velocity for z
		particleData[x+6]=sqrt(pow(abs(selectedPoint[0]-particleData[x]),2)+pow(abs(selectedPoint[1]-particleData[x+1]),2)+pow(abs(selectedPoint[2]-particleData[x+2]),2));//computing scale of error based on current position
		particleData[x+7]=particleData[x];//initialize best known position
		particleData[x+8]=particleData[x+1];
		particleData[x+9]=particleData[x+2];
		
		cout << "randomize X location "<< particleData[x] << endl;
		cout << "randomize Y location "<< particleData[x+1] << endl;
		cout << "randomize X velocity "<< particleData[x+3] << endl;
		cout << "randomize Y velocity "<< particleData[x+4] << endl;
		cout << "randomize Z velocity "<< particleData[x+5] << endl;
		
		x=x+11;
	}
}

int main(int argc, char** argv) {
	//######################################################################
	//######################################################################
	//######################################################################
	cout << "Algorytm optymalizacji rojem czastek (PSO)"<<endl<<endl;
	
	cout << "Ustawienia przestrzeni rozwiazan:" <<endl; //user can define area size for representing 3d function
	cout << "1. Okresl minX:";
	cin >> solAreaMinX;
	cout << "2. Okresl maxX:";
	cin >> solAreaMaxX;
	cout << "3. Okresl minY:";
	cin >> solAreaMinY;
	cout << "4. Okresl maxY:";
	cin >> solAreaMaxY;
	
	cout << endl << "Dokladnosc odwzorowania punktow ustawiono domyslnie na 0.1"; //i had decided to not implement possibility to change this value yet
	
	//creating variables to store values of 3d function
	const short int solAreaXSize=1+(solAreaMaxX-solAreaMinX)*10;
	const short int solAreaYSize=1+(solAreaMaxY-solAreaMinY)*10;//for example: maxX-minX=2, so our area size for X dimension is: [-1,1]
	float solArea[solAreaXSize][solAreaYSize];//to have space for storing data, we need 10+1+10 elements (0.0, 0.1 - 1.0 and (-0,1) - (-1.0))
	
	
	cout << endl << endl << "Ustawienia funkcji celu"<<endl;
	//DO ZAIMPLEMENTOWANIA - WYBOR FUNKCJI CELU ---------------------------------------------------------------------------------------------------------------
	cout << "Wybrano: Funkcja ?"<<endl;
	cout << "Szukac wartosci min czy max? (0 - min, 1 - max)";
	cin >> minMaxMode;
	
	cout << endl << endl << "Ustawienia algorytmu PSO"<<endl;
	cout << "1. Okresl liczbe czasteczek:";
	cin >> numberOfParticles;
	cout << "2. Okresl liczbe iteracji:";
	cin >> numberOfIterations;
	cout << "3. Okresl wage wspolczynnika inercji (bezwladnosci):";
	cin >> inertiaScale;
	cout << "4. Okresl wage wspolczynnika uczenia (kognitywnego):";
	cin >> learningFactor1;
	cout << "5. Okresl wage wspolczynnika uczenia (socjalnego):";
	cin >> learningFactor2;
	
	//creating array to store informations about particles
	//3 - postion, 3 - velocity, 1 - error, 3 - bestPosition, 1 - bestError
	const short int sizeOfParticleData = numberOfParticles*11;
	float particleData[sizeOfParticleData];
	
	
	//######################################################################
	//######################################################################
	//######################################################################
	
	for(short int x=0; x<solAreaXSize; x++)//processing 3d function
	{
		for(short int y=0; y<solAreaYSize; y++)
		{
			xf=x/10.0+solAreaMinX;//we're going up by 0.1, beginning with given min value
			yf=y/10.0+solAreaMinY;
			solArea[x][y]= -1 / 4000.0 * (xf * xf + yf * yf) + cos(xf) * cos(yf / sqrt(2)) - 1;
			if(maxFSolArea<solArea[x][y]){
				maxFSolArea=solArea[x][y];//getting min and max value of function in selected area
				maxXYSolArea[0]=xf;
				maxXYSolArea[1]=yf;
			}
			else if(minFSolArea>solArea[x][y]){
				minFSolArea=solArea[x][y];
				minXYSolArea[0]=xf;
				minXYSolArea[1]=yf;
			}
			//inna funkcja:
			//solArea[x][y]=-(10 * 2 + (xf * xf - 10 * cos(2 * M_PI * xf)) + (yf * yf - 10 * cos(2 * M_PI * yf)));//3d function
		}
	}
	
	cout << endl << endl << "Najwyzsza wartosc lokalna: " <<maxFSolArea<<endl;
	cout << "Najnizsza wartosc lokalna: " <<minFSolArea<<endl;
	if(minMaxMode==0){
		selectedPoint[0]=minXYSolArea[0];
		selectedPoint[1]=minXYSolArea[1];
		selectedPoint[2]=minFSolArea;
	}
	else{
		selectedPoint[0]=maxXYSolArea[0];
		selectedPoint[1]=maxXYSolArea[1];
		selectedPoint[2]=maxFSolArea;
	}
	
	cout<<"Szukany punkt: "<< endl << "X: " << selectedPoint[0] << endl << "Y: " << selectedPoint[1] << endl << "Z: " << selectedPoint[2] << endl;
	
	
	
	//######################################################################
	//######################################################################
	//######################################################################
	
	particlesInitialization(solArea, particleData, sizeOfParticleData, solAreaMinX, solAreaMaxX, solAreaMinY, solAreaMaxY);
	
	return 0;
}
