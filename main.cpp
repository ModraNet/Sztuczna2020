#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
//#include <sstream>

using namespace std;

constexpr double M_PI = 3.14159265358979323846;

//global variables related with initial settings of application
short int ThreeDimensionalFunctionMode, ApplicationMode;
float minXLoc, maxXLoc, minYLoc, maxYLoc, minVel, maxVel; //to limit generation range in particles initialization
float inertiaScale, localLearningFactor, globalLearningFactor;
float resolution;
short int numberOfParticles;
int numberOfIterations, CopyNumberOfIterations,numberOfAttempts;
bool logSetting;
float bestGlobalResult[3];
short int minMaxTarget;
float particleData[1600];
float xyMinPoints[100];
float xyMaxPoints[100];
short int sizeOfParticleData;
float rl, rg;

class my_ostream
{
public:
  my_ostream() : my_fstream("logger.txt") {}; // check if opening file succeeded!!
  // for regular output of variables and stuff
  template<typename T> my_ostream& operator<<(const T& something)
  {
    std::cout << something;
    my_fstream << something;
    return *this;
  }
  // for manipulators like std::endl
  typedef std::ostream& (*stream_function)(std::ostream&);
  my_ostream& operator<<(stream_function func)
  {
    func(std::cout);
    func(my_fstream);
    return *this;
  }
private:
  std::ofstream my_fstream;
};

my_ostream mout;

//functionPrototypes
void calculateMinMax();
void initialPSOSettings();
void particlesInitialization(float *particleData, short int sizeOfParticleData);
void PSO();
void ThreeDimensionalSettings();

//functions
int selectApplicationMode(){
	mout << "\nWybierz tryb:" << endl << "0 - wyznaczenie min i max dla wybranej funkcji" << endl << "1 - algorytm PSO" << endl << "2 - zmiana funkcji celu i rozdzielczosci" << endl << "3 - zakonczenie programu"<<endl;
	cin >> ApplicationMode;
	mout << to_string(ApplicationMode);
	switch(ApplicationMode){
				case 0:
					calculateMinMax();
					selectApplicationMode();
				case 1:
					initialPSOSettings();
					for(int x=1;x<=numberOfAttempts;x++){
						particlesInitialization(particleData, sizeOfParticleData);
						mout <<endl<<"Uruchomienie nr "<< to_string(x) <<endl;
						PSO();
					}
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
	mout << "\nUstawienia zakresu do wygenerowania poczatkowych wartosci czastek:" <<endl;
	mout << "\n1. Okresl minX dla pozycji poczatkowej:";
	cin >> minXLoc;
	mout << to_string(minXLoc);
	mout << "\n2. Okresl maxX dla pozycji poczatkowej:";
	cin >> maxXLoc;
	mout << to_string(maxXLoc);
	mout << "\n3. Okresl minY dla pozycji poczatkowej:";
	cin >> minYLoc;
	mout << to_string(minYLoc);
	mout << "\n4. Okresl maxY dla pozycji poczatkowej:";
	cin >> maxYLoc;
	mout << to_string(maxYLoc);
	mout << "\n5. Okresl min dla predkosci poczatkowej:";
	cin >> minVel;
	mout << to_string(minVel);
	mout << "\n6. Okresl max dla predkosci poczatkowej:";
	cin >> maxVel;
	mout << to_string(maxVel);
	
	mout << "\nSzukac wartosci min czy max? (0 - min, 1 - max)";
	cin >> minMaxTarget;
	mout << to_string(minMaxTarget);
	
	mout << "\nUstawienia algorytmu PSO"<<endl;
	mout << "\n1. Okresl liczbe czasteczek (1-200):";
	cin >> numberOfParticles;
	mout << to_string(numberOfParticles);
	
	sizeOfParticleData = 8 * numberOfParticles;
	
	mout << "\n2. Okresl liczbe iteracji:";
	cin >> CopyNumberOfIterations;
	mout << to_string(CopyNumberOfIterations);
	mout << "\n3. Okresl wage wspolczynnika inercji (bezwladnosci):";
	cin >> inertiaScale;
	mout << to_string(inertiaScale);
	mout << "\n4. Okresl wage wspolczynnika uczenia (kognitywnego):";
	cin >> localLearningFactor;
	mout << to_string(localLearningFactor);
	mout << "\n5. Okresl wage wspolczynnika uczenia (socjalnego):";
	cin >> globalLearningFactor;
	mout << to_string(globalLearningFactor);
	mout << "\n6. Okresl ile razy algorytm ma byc powtarzany: ";
	cin >> numberOfAttempts;
	mout << to_string(numberOfAttempts);
	mout << "\n6. Czy raportowac wszystkie proby (0 - nie, 1 - tak) (wybierz 0 dla duzej liczby iteracji): ";
	cin >> logSetting;
	mout << to_string(logSetting);
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
	numberOfIterations = CopyNumberOfIterations;
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
				case 0://min
					if(particleData[x+2]<particleData[x+7]) {//update best local position
						particleData[x+5] = particleData[x+0];
						particleData[x+6] = particleData[x+1];
						particleData[x+7] = particleData[x+2];
					}
					if(bestGlobalResult[2]>particleData[x+7]) {
						bestGlobalResult[0] = particleData[x+5];//update best global position
						bestGlobalResult[1] = particleData[x+6];
						bestGlobalResult[2] = particleData[x+7];
					}
					break;
				case 1://max
					if(particleData[x+2]>particleData[x+7]) {//update best local position
						particleData[x+5] = particleData[x+0];
						particleData[x+6] = particleData[x+1];
						particleData[x+7] = particleData[x+2];
					}
					if(bestGlobalResult[2]<particleData[x+7]) {
						bestGlobalResult[0] = particleData[x+5];//update best global position
						bestGlobalResult[1] = particleData[x+6];
						bestGlobalResult[2] = particleData[x+7];
					}
					break;
		}
		x+=8;
	}
	if(logSetting || iterCounter<10 || numberOfIterations==1) mout<<"Iteracja nr:"<<iterCounter<<". Najlepszy wynik globalny : "<<bestGlobalResult[2]<<"           dla X="<<bestGlobalResult[0] << " Y="<<bestGlobalResult[1]<<endl;
	numberOfIterations--;
	}
}
void calculateMinMax(){
	float minX, maxX, minY, maxY, minYCopy, minXCopy;
	float value, min, max;
	int xyMinCounter=50, xyMaxCounter=50;
	mout << "\n1. Okresl dolny zakres argumentow (X):";
	cin >> minX;
	mout << to_string(minX);
	mout << "\n2. Okresl gorny zakres argumentow (X):";
	cin >> maxX;
	mout << to_string(maxX);
	mout << "\n3. Okresl dolny zakres argumentow (Y):";
	cin >> minY;
	mout << to_string(minY);
	mout << "\n4. Okresl gorny zakres argumentow (Y):";
	cin >> maxY;
	mout << to_string(maxY);
	bool firstTime = true;
	int intMaxX=maxX/resolution;
	minYCopy=minY;
	minXCopy=minX;
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
			if(value>max){
				max=value;
			}
			if(value<min){
				min=value;
			}
		}
	}
	minX=minXCopy;
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
			if(value==min && xyMinCounter!=0){
				xyMinPoints[(50-xyMinCounter)*2]=minX;
				xyMinPoints[(50-xyMinCounter)*2+1]=minY;
				xyMinCounter--;
			}
			else if(value==max && xyMaxCounter!=0){
				xyMaxPoints[(50-xyMaxCounter)*2]=minX;
				xyMaxPoints[(50-xyMaxCounter)*2+1]=minY;
				xyMaxCounter--;
			}
			if(xyMinCounter==0 && xyMaxCounter==0) break;
		}
		if(xyMinCounter==0 && xyMaxCounter==0) break;
	}
	mout << endl << "Minimum: " << min;
	mout << endl << "Ekstremum: " << max << endl;
	mout << "Wspolrzedne minimum: "<<endl;
	for(int x=50;x>xyMinCounter;x--){
		mout << "X: " << xyMinPoints[(50-x)*2] << " Y: " << xyMinPoints[(50-x)*2+1] << endl;
	}
	mout << "Wspolrzedne ekstremum: "<<endl;
	for(int x=50;x>xyMaxCounter;x--){
		mout << "X: " << xyMaxPoints[(50-x)*2] << " Y: " << xyMaxPoints[(50-x)*2+1] << endl;
	}
}
void ThreeDimensionalSettings(){
	mout << "Wybierz funkcje celu:"<<endl;
	mout << "1 - Funkcja Rastrigina"<<endl;
	mout << "2 - Funkcja Michalewicza"<<endl;
	mout << "3 - Funkcja Easoma"<<endl;
	mout << "4 - Funkcja Rosenbrocka"<<endl;
	cin >> ThreeDimensionalFunctionMode;
	//mout << NumberToString(ThreeDimensionalFunctionMode);
	mout << to_string(ThreeDimensionalFunctionMode);
	mout << endl << "Okresl rozdzielczosc dla wprowadzanych danych (dokladnosc, np. 1, 0.1, 0.01, 0.005, itp.):";
	cin >> resolution;
	mout << to_string(resolution);
}
int main(int argc, char** argv) {
	srand( time( NULL ) );  
	mout << "Algorytm optymalizacji rojem czastek (PSO)"<<endl;
	mout << "Wybierz funkcje celu:"<<endl;
	mout << "1 - Funkcja Rastrigina"<<endl;
	mout << "2 - Funkcja Michalewicza"<<endl;
	mout << "3 - Funkcja Easoma"<<endl;
	mout << "4 - Funkcja Rosenbrocka"<<endl;
	cin >> ThreeDimensionalFunctionMode;
	//mout << NumberToString(ThreeDimensionalFunctionMode);
	mout << to_string(ThreeDimensionalFunctionMode);
	mout << endl << "Okresl rozdzielczosc dla wprowadzanych danych (dokladnosc, np. 1, 0.1, 0.01, 0.005, itp.):";
	cin >> resolution;
	mout << to_string(resolution);
    //mout << NumberToString(resolution);
	selectApplicationMode();
	return 0;
}
