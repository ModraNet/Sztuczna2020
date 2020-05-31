#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

//global variables related with initial settings of application
short int ThreeDimensionalFunctionMode, ApplicationMode;
float minXLoc, maxXLoc, minYLoc, maxYLoc, minVel, maxVel; //to limit generation range in particles initialization
float inertiaScale, localLearningFactor, globalLearningFactor;
float resolution;
short int numberOfParticles, numberOfIterations;
float bestGlobalResult[3];
short int minMaxTarget;
float particleData[1600];
short int sizeOfParticleData;
float rl, rg;

//functionPrototypes
void calculateMinMax();
void initialPSOSettings();
void particlesInitialization(float *particleData, short int sizeOfParticleData);
void PSO();
void ThreeDimensionalSettings();

//functions
int selectApplicationMode(){
	cout << cout << "Wybierz tryb:" << endl << "0 - wyznaczenie min i max dla wybranej funkcji" << endl << "1 - algorytm PSO" << endl << "2 - zmiana funkcji celu i rozdzielczosci" << endl << "3 - zakonczenie programu"<<endl;
	cin >> ApplicationMode;
	switch(ApplicationMode){
				case 0:
					calculateMinMax();
					selectApplicationMode();
				case 1:
					initialPSOSettings();
					particlesInitialization(particleData, sizeOfParticleData);
					PSO();
					selectApplicationMode();
				case 2:
					ThreeDimensionalSettings();
					selectApplicationMode();
				case 3:
					abort();
	}
}
int genNum(short int min, short int max){//to generate numbers
	return rand() % (max - min -1) + min;
}
void initialPSOSettings(){
	cout << "Ustawienia zakresu do wygenerowania poczatkowych wartosci czastek:" <<endl;
	cout << "1. Okresl minX dla pozycji poczatkowej:";
	cin >> minXLoc;
	cout << "2. Okresl maxX dla pozycji poczatkowej:";
	cin >> maxXLoc;
	cout << "3. Okresl minY dla pozycji poczatkowej:";
	cin >> minYLoc;
	cout << "4. Okresl maxY dla pozycji poczatkowej:";
	cin >> maxYLoc;
	cout << "5. Okresl min dla predkosci poczatkowej:";
	cin >> minVel;
	cout << "6. Okresl max dla predkosci poczatkowej:";
	cin >> maxVel;
	
	cout << "Szukac wartosci min czy max? (0 - min, 1 - max)";
	cin >> minMaxTarget;
	
	cout << "Ustawienia algorytmu PSO"<<endl;
	cout << "1. Okresl liczbe czasteczek (1-200):";
	cin >> numberOfParticles;
	
	sizeOfParticleData = 8 * numberOfParticles;
	
	cout << "2. Okresl liczbe iteracji:";
	cin >> numberOfIterations;
	cout << "3. Okresl wage wspolczynnika inercji (bezwladnosci):";
	cin >> inertiaScale;
	cout << "4. Okresl wage wspolczynnika uczenia (kognitywnego):";
	cin >> localLearningFactor;
	cout << "5. Okresl wage wspolczynnika uczenia (socjalnego):";
	cin >> globalLearningFactor;
}
void particlesInitialization(float *particleData, short int sizeOfParticleData){
	bool firstTime=1;
	for(short int x=0; x<sizeOfParticleData;){
		//generating starting positions
		particleData[x]=genNum(minXLoc*(1/resolution),maxXLoc*(1/resolution))/(1/resolution);//x
		particleData[x+1]=genNum(minYLoc*(1/resolution),maxYLoc*(1/resolution))/(1/resolution);//y
		switch(ThreeDimensionalFunctionMode){
				case 1:
					particleData[x+2] = -(10 * 2 + (particleData[x] * particleData[x] - 10 * cos(2 * M_PI * particleData[x])) + (particleData[x+1] * particleData[x+1] - 10 * cos(2 * M_PI * particleData[x+1])));
					break;
				case 2:
					particleData[x+2] = sin(particleData[x]) * pow(sin(particleData[x] * particleData[x] / M_PI), 2 * 10) + sin(particleData[x+1]) * pow(sin(particleData[x+1] * particleData[x+1] / M_PI), 2 * 10);
					break;
				case 3:
					particleData[x+2] = cos(particleData[x]) * cos(particleData[x+1]) * exp(-(pow(particleData[x] - M_PI, 2) + pow(particleData[x+1] - M_PI, 2)));
					break;
				case 4:
					particleData[x+2] = -(100 * pow((particleData[x+1] - particleData[x] * particleData[x]), 2) + pow((particleData[x] - 1), 2));
					break;
		}
		//generating starting velocity
		particleData[x+3]=genNum(minVel*(1/resolution),maxVel*(1/resolution))/(1/resolution);//x
		particleData[x+4]=genNum(minVel*(1/resolution),maxVel*(1/resolution))/(1/resolution);//y
		particleData[x+5]=particleData[x];//initialize best known position for x, y, z
		particleData[x+6]=particleData[x+1];
		particleData[x+7]=particleData[x+2];
		if(firstTime){
			firstTime=0;
			bestGlobalResult[0]=particleData[x];
			bestGlobalResult[1]=particleData[x+1];
			bestGlobalResult[2]=particleData[x+2];
		}
		else{
			switch(minMaxTarget){
				case 0:
					if(particleData[x+2]<bestGlobalResult[2]){
					bestGlobalResult[0]=particleData[x];
					bestGlobalResult[1]=particleData[x+1];
					bestGlobalResult[2]=particleData[x+2];
			     	}  
					break;
				case 1:
					if(particleData[x+2]>bestGlobalResult[2]){
					bestGlobalResult[0]=particleData[x];
					bestGlobalResult[1]=particleData[x+1];
					bestGlobalResult[2]=particleData[x+2];
			    	}
			    	break;
			}
	    }
		x=x+8;
	}
}
void PSO(){
	int iterCounter=0;
	while(numberOfIterations){
	iterCounter++;
	for(short int x=0; x<sizeOfParticleData;){
		particleData[x]+=particleData[x+3];
		particleData[x+1]+=particleData[x+4];
		switch(ThreeDimensionalFunctionMode){
				case 1:
					particleData[x+2] = -(10 * 2 + (particleData[x] * particleData[x] - 10 * cos(2 * M_PI * particleData[x])) + (particleData[x+1] * particleData[x+1] - 10 * cos(2 * M_PI * particleData[x+1])));
					break;
				case 2:
					particleData[x+2] = sin(particleData[x]) * pow(sin(particleData[x] * particleData[x] / M_PI), 2 * 10) + sin(particleData[x+1]) * pow(sin(particleData[x+1] * particleData[x+1] / M_PI), 2 * 10);
					break;
				case 3:
					particleData[x+2] = cos(particleData[x]) * cos(particleData[x+1]) * exp(-(pow(particleData[x] - M_PI, 2) + pow(particleData[x+1] - M_PI, 2)));
					break;
				case 4:
					particleData[x+2] = -(100 * pow((particleData[x+1] - particleData[x] * particleData[x]), 2) + pow((particleData[x] - 1), 2));
					break;	
		}
		rl = genNum(0, 1/resolution)*resolution;
		rg = genNum(0, 1/resolution)*resolution;
		particleData[x+3]=inertiaScale*particleData[x+3]+localLearningFactor*rl*(particleData[x+5]-particleData[x])+globalLearningFactor*rg*(bestGlobalResult[0]-particleData[x]);
		particleData[x+4]=inertiaScale*particleData[x+4]+localLearningFactor*rl*(particleData[x+6]-particleData[x+1])+globalLearningFactor*rg*(bestGlobalResult[1]-particleData[x+1]);
		switch(minMaxTarget){
				case 0:
					if(particleData[x+2]<particleData[x+7]) particleData[x+7] = particleData[x+2];
					if(bestGlobalResult[2]>particleData[x+7]) bestGlobalResult[2] = particleData[x+7];
					break;
				case 1:
					if(particleData[x+2]>particleData[x+7]) particleData[x+7] = particleData[x+2];
					if(bestGlobalResult[2]<particleData[x+7]) bestGlobalResult[2] = particleData[x+7];
					break;
		}
		x+=8;
	}
	cout<<"Iteracja nr:"<<iterCounter<<". Najlepszy wynik globalny : "<<bestGlobalResult[2]<<endl;
	numberOfIterations--;
	}
	cout <<endl<<"Koniec."<<endl;
}
void calculateMinMax(){
	float minX, maxX, minY, maxY, minYCopy;
	float value, min, max;
	cout << "1. Okresl dolny zakres argumentow (X):";
	cin >> minX;
	cout << "2. Okresl gorny zakres argumentow (X):";
	cin >> maxX;
	cout << "3. Okresl dolny zakres argumentow (Y):";
	cin >> minY;
	cout << "4. Okresl gorny zakres argumentow (Y):";
	cin >> maxY;
	bool firstTime = true;
	int intMaxX=maxX/resolution;
	minYCopy=minY;
	for(int intMinX=minX/resolution;intMinX<=intMaxX;intMinX++){
		int intMaxY=maxY/resolution;
		minY=minYCopy;
		for(int intMinY=minY/resolution;intMinY<=intMaxY;intMinY++){
			minX=intMinX*resolution;
			minY=intMinY*resolution;
			switch(ThreeDimensionalFunctionMode){
				case 1:
					value = -(10 * 2 + (minX * minX - 10 * cos(2 * M_PI * minX)) + (minY * minY - 10 * cos(2 * M_PI * minY)));
					break;
				case 2:
					value = sin(minX) * pow(sin(minX * minX / M_PI), 2 * 10) + sin(minY) * pow(sin(minY * minY / M_PI), 2 * 10);
					break;
				case 3:
					value = cos(minX) * cos(minY) * exp(-(pow(minX - M_PI, 2) + pow(minY - M_PI, 2)));
					break;
				case 4:
					value = -(100 * pow((minY - minX * minX), 2) + pow((minX - 1), 2));
					break;	
			}
			if(firstTime){
						firstTime=false;
						min=value;
						max=value;
						break;
			}
			if(value>max) max=value;
			if(value<min) min=value;
		}
	}
	cout << endl << "Minimum: " << min;
	cout << endl << "Ekstremum: " << max << endl;
}
void ThreeDimensionalSettings(){
	cout << "Wybierz funkcje celu:"<<endl;
	cout << "1 - Funkcja Rastrigina"<<endl;
	cout << "2 - Funkcja Michalewicza"<<endl;
	cout << "3 - Funkcja Easoma"<<endl;
	cout << "4 - Funkcja Rosenbrocka"<<endl;
	cin >> ThreeDimensionalFunctionMode;
	cout << "Okresl rozdzielczosc dla wprowadzanych danych (dokladnosc, np. 1, 0.1, 0.01, 0.005, itp.):";
	cin >> resolution;
}
int main(int argc, char** argv) {
	srand( time( NULL ) );
	cout << "Algorytm optymalizacji rojem czastek (PSO)"<<endl;
	cout << "Wybierz funkcje celu:"<<endl;
	cout << "1 - Funkcja Rastrigina"<<endl;
	cout << "2 - Funkcja Michalewicza"<<endl;
	cout << "3 - Funkcja Easoma"<<endl;
	cout << "4 - Funkcja Rosenbrocka"<<endl;
	cin >> ThreeDimensionalFunctionMode;
	cout << "Okresl rozdzielczosc dla wprowadzanych danych (dokladnosc, np. 1, 0.1, 0.01, 0.005, itp.):";
	cin >> resolution;
	selectApplicationMode();
	return 0;
}
