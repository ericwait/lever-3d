#include "Vec.h"
#include "mex.h"
#include "windows.h"
#include "MessageProcessor.h"
#include "Globals.h"

//unsigned char* gImage = NULL;

HANDLE gTermEvent = NULL;
HANDLE messageLoopHandle = NULL;

HINSTANCE gDllInstance = NULL;
DWORD threadID;

bool registerExitFunction = false;

void startThread()
{
	if (gTermEvent==NULL)
	{
		gTermEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if ( !gTermEvent )
			mexErrMsgTxt("Could not create thread-monitor event");
	}

	if (messageLoopHandle==NULL)
	{
		messageLoopHandle = CreateThread(NULL, 0, messageLoop, (LPVOID)(NULL), 0, &threadID);
		if ( !messageLoopHandle )
		{
			mexErrMsgTxt("Could not create thread");
		}
	}
}

void termThread()
{


	// Send thread a termination event
	SetEvent(gTermEvent);
	PostThreadMessage(threadID, WM_QUIT, (WPARAM)0, (LPARAM)0);

	// Resume the thread in case it's suspended
	ResumeThread(messageLoopHandle);

	// Wait for thread termination/force termination if it times out
	DWORD waitTerm = WaitForSingleObject(messageLoopHandle, 30000);
	if ( waitTerm == WAIT_TIMEOUT )
		TerminateThread(messageLoopHandle, -1000);

	CloseHandle(messageLoopHandle);
	CloseHandle(gTermEvent);
	messageLoopHandle = NULL;
	gTermEvent = NULL;
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		gDllInstance = hInstDLL;
		registerExitFunction = TRUE;
	}

	return TRUE;
}

extern "C" void exitFunc()
{
	if ( messageLoopHandle )
	{
		termThread();
	}
}

void loadHulls( const mxArray* verts, const mxArray* faces, const mxArray* normals) 
{

	size_t numVerts = mxGetM(verts);
	size_t numFaces = mxGetM(faces);
	size_t numNormals = mxGetM(normals);

	if (numVerts<1)
		mexErrMsgTxt("No Verts!");

	if (numFaces<1)
		mexErrMsgTxt("No faces!");

	if (numNormals<1)
		mexErrMsgTxt("No norms!");

		if (numVerts!=numNormals) mexErrMsgTxt("Number of normals does not match the number of verts!\n");
		if (numVerts<=0)
			return;

		gVertsDebug.resize(numVerts);
		gFacesDebug.resize(numFaces);
		gNormsDebug.resize(numNormals);

		double* vertData = (double*)mxGetData(verts);
		double* normData = (double*)mxGetData(normals);
		Vec<float> curVert, curNormal;
		for (int i=0; i<numVerts; ++i)
		{
			curVert.x = vertData[i];
			curVert.y = vertData[i+numVerts];
			curVert.z = vertData[i+2*numVerts];

			curNormal.x = normData[i];
			curNormal.y = normData[i+numVerts];
			curNormal.z = normData[i+2*numVerts];

			gVertsDebug[i] = curVert;
			gNormsDebug[i] = curNormal;
		}

		double* faceData = (double*)mxGetData(faces);
		Vec<unsigned int> curFace;
		for (int i=0; i<numFaces; ++i)
		{
			curFace.x = faceData[i];
			curFace.y = faceData[i+numFaces];
			curFace.z = faceData[i+2*numFaces];

			curFace = curFace - 1;

			gFacesDebug[i] = curFace;
		}
}

// This is the entry point from Matlab
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	if (nrhs<1 || !mxIsChar(prhs[0])) mexErrMsgTxt("Usage:\n");// TODO make this useful

	char* command = mxArrayToString(prhs[0]);

	if (_strcmpi("init",command)==0)
	{
		if (nrhs>3)
			loadHulls(prhs[1],prhs[2],prhs[3]);

		if ( !messageLoopHandle )
			startThread();

		if ( registerExitFunction )
		{
			mexAtExit(exitFunc);
			registerExitFunction = FALSE;
		}
	}

	else if (_strcmpi("loadTexture",command)==0)
	{
		size_t numDims = mxGetNumberOfDimensions(prhs[1]);
		if (numDims!=3)
			mexErrMsgTxt("Image must have three dimensions!");

		const mwSize* DIMS = mxGetDimensions(prhs[1]);
		Vec<size_t> dims;
		dims.x = DIMS[0];//,DIMS[1],DIMS[2]);

		//gImage = (unsigned char*)mxGetData(prhs[1]);
	}

	else if (_strcmpi("getData",command)==0)
	{
		;
	}

	else if (_strcmpi("peelUpdate",command)==0)
	{
		if (nrhs!=2) mexErrMsgTxt("not the right arguments for peelUpdate!");

		//g_peelFactor = (float)mxGetScalar(prhs[1]);
	}

	else if (_strcmpi("lightingUpdate",command)==0)
	{
		if (nrhs!=2) mexErrMsgTxt("not the right arguments for lightingUpdate!");

		//g_lightingOn = (float)mxGetScalar(prhs[1]);
	}

	else if (_strcmpi("transferUpdate",command)==0)
	{
		if (nrhs!=19) mexErrMsgTxt("not the right arguments for transferUpdate!");

		// 		gTransferFunctions[0].x = (float)mxGetScalar(prhs[1]);
		// 		gTransferFunctions[0].y = (float)mxGetScalar(prhs[2]);
		// 		gTransferFunctions[0].z = (float)mxGetScalar(prhs[3]);
		// 
		// 		gTransferFunctions[1].x = (float)mxGetScalar(prhs[4]);
		// 		gTransferFunctions[1].y = (float)mxGetScalar(prhs[5]);
		// 		gTransferFunctions[1].z = (float)mxGetScalar(prhs[6]);
		// 
		// 		gTransferFunctions[2].x = (float)mxGetScalar(prhs[7]);
		// 		gTransferFunctions[2].y = (float)mxGetScalar(prhs[8]);
		// 		gTransferFunctions[2].z = (float)mxGetScalar(prhs[9]);
		// 
		// 		gTransferFunctions[3].x = (float)mxGetScalar(prhs[10]);
		// 		gTransferFunctions[3].y = (float)mxGetScalar(prhs[11]);
		// 		gTransferFunctions[3].z = (float)mxGetScalar(prhs[12]);
		// 
		// 		gTransferFunctions[4].x = (float)mxGetScalar(prhs[13]);
		// 		gTransferFunctions[4].y = (float)mxGetScalar(prhs[14]);
		// 		gTransferFunctions[4].z = (float)mxGetScalar(prhs[15]);
		// 
		// 		gTransferFunctions[5].x = (float)mxGetScalar(prhs[16]);
		// 		gTransferFunctions[5].y = (float)mxGetScalar(prhs[17]);
		// 		gTransferFunctions[5].z = (float)mxGetScalar(prhs[18]);

	}

	else if (_strcmpi("rangesUpdate",command)==0)
	{
		if (nrhs!=19) mexErrMsgTxt("Wrong input arguments for rangesUpdate");

		// 		gRanges[0].x = (float)mxGetScalar(prhs[1]);
		// 		gRanges[0].y = (float)mxGetScalar(prhs[2]);
		// 		gRanges[0].z = (float)mxGetScalar(prhs[3]);
		// 
		// 		gRanges[1].x = (float)mxGetScalar(prhs[4]);
		// 		gRanges[1].y = (float)mxGetScalar(prhs[5]);
		// 		gRanges[1].z = (float)mxGetScalar(prhs[6]);
		// 
		// 		gRanges[2].x = (float)mxGetScalar(prhs[7]);
		// 		gRanges[2].y = (float)mxGetScalar(prhs[8]);
		// 		gRanges[2].z = (float)mxGetScalar(prhs[9]);
		// 
		// 		gRanges[3].x = (float)mxGetScalar(prhs[10]);
		// 		gRanges[3].y = (float)mxGetScalar(prhs[11]);
		// 		gRanges[3].z = (float)mxGetScalar(prhs[12]);
		// 
		// 		gRanges[4].x = (float)mxGetScalar(prhs[13]);
		// 		gRanges[4].y = (float)mxGetScalar(prhs[14]);
		// 		gRanges[4].z = (float)mxGetScalar(prhs[15]);
		// 
		// 		gRanges[5].x = (float)mxGetScalar(prhs[16]);
		// 		gRanges[5].y = (float)mxGetScalar(prhs[17]);
		// 		gRanges[5].z = (float)mxGetScalar(prhs[18]);
	}

	else if (_strcmpi("colorsUpdate",command)==0)
	{
		if (nrhs!=19) mexErrMsgTxt("not the right arguments for transferUpdate!");

		// 		gChannelColor[0].x = (float)mxGetScalar(prhs[1]);
		// 		gChannelColor[0].y = (float)mxGetScalar(prhs[2]);
		// 		gChannelColor[0].z = (float)mxGetScalar(prhs[3]);
		// 
		// 		gChannelColor[1].x = (float)mxGetScalar(prhs[4]);
		// 		gChannelColor[1].y = (float)mxGetScalar(prhs[5]);
		// 		gChannelColor[1].z = (float)mxGetScalar(prhs[6]);
		// 
		// 		gChannelColor[2].x = (float)mxGetScalar(prhs[7]);
		// 		gChannelColor[2].y = (float)mxGetScalar(prhs[8]);
		// 		gChannelColor[2].z = (float)mxGetScalar(prhs[9]);
		// 
		// 		gChannelColor[3].x = (float)mxGetScalar(prhs[10]);
		// 		gChannelColor[3].y = (float)mxGetScalar(prhs[11]);
		// 		gChannelColor[3].z = (float)mxGetScalar(prhs[12]);
		// 
		// 		gChannelColor[4].x = (float)mxGetScalar(prhs[13]);
		// 		gChannelColor[4].y = (float)mxGetScalar(prhs[14]);
		// 		gChannelColor[4].z = (float)mxGetScalar(prhs[15]);
		// 
		// 		gChannelColor[5].x = (float)mxGetScalar(prhs[16]);
		// 		gChannelColor[5].y = (float)mxGetScalar(prhs[17]);
		// 		gChannelColor[5].z = (float)mxGetScalar(prhs[18]);

	}

	else if (_strcmpi("poll",command)==0)
	{
		if (nlhs!=1) mexErrMsgTxt("Wrong number of return arguments");
	}

	else if (_strcmpi("load hulls",command)==0)
	{
		const mxArray* hulls = prhs[1];
		if (hulls==NULL) mexErrMsgTxt("No hulls passed as the second argument!\n");

		//loadHulls(hulls);
	}

	else if (_strcmpi("close",command)==0)
	{
		termThread();
	}

	else
	{
		char buff[255];
		sprintf_s(buff,"%s is not a valid command!\n",command);
		mexErrMsgTxt(buff);
	}

	mxFree(command);
}