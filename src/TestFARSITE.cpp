// TestMTT.cpp : Defines the entry point for the console application.
//

#include "FARSITE.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/timeb.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>
using namespace std;

bool cancelRequest = false;
int nFarsites = 0;

bool ParseCommands(char *in, char *lcp, char *inputs, char *ignitPath, char *barrierPath, char *outPath, int *outType)
{
	strcpy(lcp, "");
	strcpy(inputs, "");
	strcpy(outPath, "");
	*outType = 0;
	char seps[] = " ";
	char *p = strtok(in, seps);	// " " space로 split
	//printf("%s\n", p);
	if(p)
		strcpy(lcp, p);
	else	{
		printf("return false1\n");
		return false;
	}
	p = strtok(NULL, seps);		// NULL 문자부터 delimiter로 구분되는 부분까지 토큰화 후 다시 NULL 리턴
	//printf("%s\n", p);
	if(p)
		strcpy(inputs, p);
	else	{
		//printf("return false2\n");
		return false;
	}
	p = strtok(NULL, seps);
	//printf("%s\n", p);
	if(p)
		strcpy(ignitPath, p);
	else	{
		printf("return false3\n");
		return false;
	}
	p = strtok(NULL, seps);
	//printf("%s\n", p);
	if(p)
		strcpy(barrierPath, p);
	else	{
		printf("return false4\n");
		return false;
	}
	p = strtok(NULL, seps);
	//printf("%s\n", p);
	if(p)
		strcpy(outPath, p);
	else	{
		printf("return false5\n");
		return false;
	}
	p = strtok(NULL, seps);
	//printf("%s\n", p);
	if(p)
		*outType = atoi(p);
	else	{
		printf("return false6\n");
		return false;
	}		
	return true;
}

#define MAX_PATH 1512
int linuxMain(int argc, char* argv[])
{
    char lcpFileName[1512], inputsFileName[1512], ignitName[1512], barrierName[1512], baseOutputsPath[1512];
	if(argc < 2)
	{
		printf("TestFARSITE expects one parameter\nTestFARSITE Usage:\n"
			"TestFARSITE [commandfile]\n"
			"Where:\n\t[commandfile] is the path to the command file.\n");
		printf("The command file contains command lines for multiple Farsite runs, each run's command on a seperate line.\n");
		printf("Each command expects six parameters, all required\n"
			"[LCPName] [InputsFileName] [IgnitionFileName] [BarrierFileName] [outputDirPath] [outputsType]\n"
			"Where:\n\t[LCPName] is the path to the Landscape File\n"
			"\t[InputsFileName] is the path to the FARSITE Inputs File (ASCII Format)\n"
			"\t[IgnitionFileName] is the path to the Ignition shape File\n"
			"\t[BarrierFileName] is the path to the Barrier shape File (0 if no barrier)\n"
			"\t[outputDirPath] is the path to the output files base name (no extension)\n"
			"\t[outputsType] is the file type for outputs (0 = both, 1 = ASCII grid, 2 = FlamMap binary grid\n\n");
		exit(1);
	}
	FILE *cmd = fopen(argv[1], "rt");				// input 파일 경로가 저장되어 있는 파일
	if(!cmd)
	{
		printf("Error, cannot open %s\n", argv[1]);
		exit(1);
	}
    char outputName[1512] = "Test1177973";
	int outType;		// = atoi(argv[4]);
	nFarsites = 0;
    char buf[1512];
	while(!feof(cmd))	// feof(cmd) = 0 (cmd를 stream으로 받았을 때 end of line이 한번도 설정되지 않았기 때문에 0 리턴)
	{
		//printf("!feof(cmd) != 0\n");
        fgets(buf, 1512, cmd);

		if(ParseCommands(buf, lcpFileName, inputsFileName, ignitName, barrierName, baseOutputsPath, &outType))	//assume valid commands
			nFarsites++;
		//printf("nFarsites = %d\n", nFarsites);
	}
	//printf("nFarsites = %d\n", nFarsites);
	rewind(cmd);			// 포인터 파일 맨 앞으로 설정
	//SYSTEM_INFO sysInfo;
	//GetSystemInfo(&sysInfo);
	//nOmpFarsites =  sysInfo.dwNumberOfProcessors;

	//nOmpFarsites = nFarsites;
	char **lcps = new char *[nFarsites];
	char **inputs = new char *[nFarsites];
	char **ignits = new char *[nFarsites];
	char **barriers = new char *[nFarsites];
	char **outs = new char *[nFarsites];
	int *outTypes = new int[nFarsites];
	CFarsite **pFarsites = new CFarsite*[nFarsites];
	for(int i = 0; i < nFarsites; i++)
		pFarsites[i] = NULL;
	for(int i = 0; i < nFarsites; i++)
	{
		lcps[i] = new char[MAX_PATH];
		inputs[i] = new char[MAX_PATH];
		ignits[i] = new char[MAX_PATH];
		barriers[i] = new char[MAX_PATH];
		outs[i] = new char[MAX_PATH];
		strcpy(lcps[i], "");
		strcpy(inputs[i], "");
		strcpy(ignits[i], "");
		strcpy(barriers[i], "");
		strcpy(outs[i], "");
        fgets(buf, 1512, cmd);
		if(!ParseCommands(buf, lcps[i], inputs[i], ignits[i], barriers[i], outs[i], &outTypes[i]))
			continue;
			// lcps[i] = PATH/lcpfile
	}

	int ret;
	int f;
	timeb t;

	ftime(&t);
	srand(t.time);

	struct timespec begin, end;
	struct timespec begin2, end2;
	struct timespec begin3, end3;
	struct timespec begin4, end4;
	struct timespec begin5, end5;
	struct timespec begin6, end6;
	clock_gettime(CLOCK_MONOTONIC, &begin);

	for(f = 0; f < nFarsites; f++)
	{		
        pFarsites[f] = new CFarsite();
		clock_gettime(CLOCK_MONOTONIC, &begin2);
		if(!cancelRequest)
		{
			//printf("Loading lcp file for Farsite #%d: %s\n", f + 1, lcps[f]);
			if ( !pFarsites[f]->SetLandscapeFile(lcps[f]))
			{
				printf ("Can't open: %s \n", lcpFileName);
				delete pFarsites[f];
				pFarsites[f] = NULL;
				//_getch();
				continue;
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &end2);
		//cout << "SetlandscapeFile time:" << (end2.tv_sec - begin2.tv_sec) + (end2.tv_nsec - begin2.tv_nsec) / 10000000000.0 << endl << endl;

		clock_gettime(CLOCK_MONOTONIC, &begin3);
		if(!cancelRequest)
		{
			//printf("Loading inputs file for Farsite #%d: %s\n", f + 1, inputs[f]);
			ret = pFarsites[f]->LoadFarsiteInputs(inputs[f]);
			if ( ret != 1 )
			{
			   char *a = pFarsites[f]->CommandFileError(ret);
			   printf ("%s\n",a);
				delete pFarsites[f];
				pFarsites[f] = NULL;
				continue;
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &end3);
		//cout << "LoadFarsiteInputs time:" << (end3.tv_sec - begin3.tv_sec) + (end3.tv_nsec - begin3.tv_nsec) / 10000000000.0 << endl << endl;

		clock_gettime(CLOCK_MONOTONIC, &begin4);
		if(!cancelRequest)
		{
			//printf("Loading ignition file for Farsite #%d: %s\n", f + 1, ignits[f]);
			pFarsites[f]->SetIgnition(ignits[f]);
			if(strlen(barriers[f]) > 2)
			{
				//printf("Loading barrier file for Farsite #%d: %s\n", f + 1, barriers[f]);
				pFarsites[f]->SetBarriers(barriers[f]);
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &end4);
		//cout << "LoadFarsiteInputs time:" << (end4.tv_sec - begin4.tv_sec) + (end4.tv_nsec - begin4.tv_nsec) / 10000000000.0 << endl << endl;

		clock_gettime(CLOCK_MONOTONIC, &begin5);
		if(!cancelRequest)
		{
			//printf("Launching Farsite #%d\n", f + 1);
			ret = pFarsites[f]->LaunchFarsite();
			if(ret != 1)
			{
			   char *a = pFarsites[f]->CommandFileError(ret);
			   printf ("%s\n",a);
				delete pFarsites[f];
				pFarsites[f] = NULL;
				continue;
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &end5);
		//cout << "LaunchFarsite time:" << (end5.tv_sec - begin5.tv_sec) + (end5.tv_nsec - begin5.tv_nsec) / 10000000000.0 << endl << endl;

		clock_gettime(CLOCK_MONOTONIC, &begin6);
		if(!cancelRequest)
		{
			//printf("Writing outputs for Farsite #%d to %s\n", f + 1, outs[f]);
			if(outTypes[f] == 0 || outTypes[f] == 1 || outTypes[f] == 4)
			{
				if(outTypes[f] == 4)
				{
					pFarsites[f]->WriteOneHours(outs[f]);
					pFarsites[f]->WriteTenHours(outs[f]);
				}
				sprintf(outputName, "%s_CrownFire.asc", outs[f]);
				ret = pFarsites[f]->WriteCrownFireGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_Intensity.asc", outs[f]);
				ret = pFarsites[f]->WriteIntensityGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_FlameLength.asc", outs[f]);
				ret = pFarsites[f]->WriteFlameLengthGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_SpreadRate.asc", outs[f]);
				ret = pFarsites[f]->WriteSpreadRateGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_SpreadDirection.asc", outs[f]);
				ret = pFarsites[f]->WriteSpreadDirectionGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_HeatPerUnitArea.asc", outs[f]);
				ret = pFarsites[f]->WriteHeatPerUnitAreaGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_ReactionIntensity.asc", outs[f]);
				ret = pFarsites[f]->WriteReactionIntensityGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_ArrivalTime.asc", outs[f]);
				ret = pFarsites[f]->WriteArrivalTimeGrid(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
			}
			if(outTypes[f] == 0 || outTypes[f] == 2)
			{
				sprintf(outputName, "%s_CrownFire.fbg", outs[f]);
				ret = pFarsites[f]->WriteCrownFireGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_ArrivalTime.fbg", outs[f]);
				ret = pFarsites[f]->WriteArrivalTimeGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_Intensity.fbg", outs[f]);
				ret = pFarsites[f]->WriteIntensityGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_FlameLength.fbg", outs[f]);
				ret = pFarsites[f]->WriteFlameLengthGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_SpreadRate.fbg", outs[f]);
				ret = pFarsites[f]->WriteSpreadRateGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_SpreadDirection.fbg", outs[f]);
				ret = pFarsites[f]->WriteSpreadDirectionGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_HeatPerUnitArea.fbg", outs[f]);
				ret = pFarsites[f]->WriteHeatPerUnitAreaGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_ReactionIntensity.fbg", outs[f]);
				ret = pFarsites[f]->WriteReactionIntensityGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
				sprintf(outputName, "%s_ArrivalTime.fbg", outs[f]);
				ret = pFarsites[f]->WriteArrivalTimeGridBinary(outputName);
				if(ret != 1)
				{
					char *a = pFarsites[f]->CommandFileError(ret);
					printf ("%s for %s\n",a, outputName);
				}
			}
			/*sprintf(outputName, "%s_Moistures.txt", outs[f]);
			ret = pFarsites[f]->WriteMoistData(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}*/
			sprintf(outputName, "%s_Ignitions.asc", outs[f]);
			ret = pFarsites[f]->WriteIgnitionGrid(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}
			sprintf(outputName, "%s_Perimeters.shp", outs[f]);
			ret = pFarsites[f]->WritePerimetersShapeFile(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}
			sprintf(outputName, "%s_SpotGrid.asc", outs[f]);
			ret = pFarsites[f]->WriteSpotGrid(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}
			sprintf(outputName, "%s_Spots.csv", outs[f]);
			ret = pFarsites[f]->WriteSpotDataFile(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}
			sprintf(outputName, "%s_Spots.shp", outs[f]);
			ret = pFarsites[f]->WriteSpotShapeFile(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}
			sprintf(outputName, "%s_Timings.txt", outs[f]);
			ret = pFarsites[f]->WriteTimingsFile(outputName);
			if(ret != 1)
			{
				char *a = pFarsites[f]->CommandFileError(ret);
				printf ("%s for %s\n",a, outputName);
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &end6);
		//cout << "\nWrite files time:" << (end6.tv_sec - begin6.tv_sec) + (end6.tv_nsec - begin6.tv_nsec) / 10000000000.0 << endl;

        delete pFarsites[f];
        pFarsites[f] = NULL;
		
		//cout << "outputName = " << outputName << endl;
		//cout << "outs[f] = " << outs[f] << endl;
	}
	fclose(cmd);

	clock_gettime(CLOCK_MONOTONIC, &end);

	//cout << "\nTotal elapsed time: " << (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 10000000000.0 << endl;
	
	delete[] pFarsites;
	for(int i = 0; i < nFarsites; i++)
	{
		delete[] lcps[i];
		delete[] inputs[i];
		delete[] outs[i];
		delete[] ignits[i];
		delete[] barriers[i];
	}
	delete[] lcps;
	delete[] inputs;
	delete[] outs;
	delete[] ignits;
	delete[] barriers;
	delete[] outTypes;
	return 0;	
}

int main(int argc, char* argv[])
{
	//regMain(argc, argv);
	//multiMain(argc, argv);
#ifdef WIN32
	openMPMain(argc, argv);
#else
	linuxMain(argc, argv);
#endif
}
