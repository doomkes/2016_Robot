 
//**************************************************************************
//* WARNING: This file was automatically generated.  Any changes you make  *
//*          to this file will be lost if you generate the file again.     *
//**************************************************************************
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <nivision.h>
#include <nimachinevision.h>
//#include <windows.h>

// If you call Machine Vision functions in your script, add NIMachineVision.c to the project.

#define IVA_MAX_BUFFERS 20
#define IVA_STORE_RESULT_NAMES

#define VisionErrChk(Function) {if (!(Function)) {success = 0; goto Error;}}

typedef enum IVA_ResultType_Enum {IVA_NUMERIC, IVA_BOOLEAN, IVA_STRING} IVA_ResultType;

typedef union IVA_ResultValue_Struct    // A result in Vision Assistant can be of type double, BOOL or string.
{
    double numVal;
    int boolVal;
    char*  strVal;
} IVA_ResultValue;

typedef struct IVA_Result_Struct
{
#if defined (IVA_STORE_RESULT_NAMES)
    char resultName[256];           // Result name
#endif
    IVA_ResultType  type;           // Result type
    IVA_ResultValue resultVal;      // Result value
} IVA_Result;

typedef struct IVA_StepResultsStruct
{
#if defined (IVA_STORE_RESULT_NAMES)
    char stepName[256];             // Step name
#endif
    int         numResults;         // number of results created by the step
    IVA_Result* results;            // array of results
} IVA_StepResults;

typedef struct IVA_Data_Struct
{
    Image* buffers[IVA_MAX_BUFFERS];            // Vision Assistant Image Buffers
    IVA_StepResults* stepResults;              // Array of step results
    int numSteps;                               // Number of steps allocated in the stepResults array
    CoordinateSystem *baseCoordinateSystems;    // Base Coordinate Systems
    CoordinateSystem *MeasurementSystems;       // Measurement Coordinate Systems
    int numCoordSys;                            // Number of coordinate systems
} IVA_Data;



static IVA_Data* IVA_InitData(int numSteps, int numCoordSys);
static int IVA_DisposeData(IVA_Data* ivaData);
static int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex);
static int IVA_CLRExtractGreen(Image* image);
static int IVA_Classification(Image* image, char* fileName, ROI* roi);

int IVA_ProcessImage(Image *image)
{
	int success = 1;
    IVA_Data *ivaData;
    BCGOptions bcgOptions;
    ROI *roi;

    // Initializes internal data (buffers and array of points for caliper measurements)
    VisionErrChk(ivaData = IVA_InitData(4, 0));

	VisionErrChk(IVA_CLRExtractGreen(image));

    //-------------------------------------------------------------------//
    //                             Brightness                            //
    //-------------------------------------------------------------------//

    bcgOptions.brightness = 0;
    bcgOptions.contrast = 89;
    bcgOptions.gamma = 1.02;

    // Applies brightness, contrast, and gamma correction to the image
    VisionErrChk(imaqBCGTransform(image, image, &bcgOptions, NULL));

    // Creates a new, empty region of interest.
    VisionErrChk(roi = imaqCreateROI());

    // Creates a new rectangle ROI contour and adds the rectangle to the provided ROI.
    VisionErrChk(imaqAddRectContour(roi, imaqMakeRect(5, 4, 475, 636)));

	VisionErrChk(IVA_Classification(image, 
		"U shaped.clf",
		roi));

    // Cleans up resources associated with the object
    imaqDispose(roi);

    // Releases the memory allocated in the IVA_Data structure.
    IVA_DisposeData(ivaData);

Error:
	return success;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRExtractGreen
//
// Description  : Extracts the green color plane from a color image.
//
// Parameters   : image  - Input image
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_CLRExtractGreen(Image* image)
{
    int success = 1;
    Image* plane;


    //-------------------------------------------------------------------//
    //                         Extract Color Plane                       //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image that contains the extracted plane.
    VisionErrChk(plane = imaqCreateImage(IMAQ_IMAGE_U8, 7));

    // Extracts the green color plane
    VisionErrChk(imaqExtractColorPlanes(image, IMAQ_RGB, NULL, plane, NULL));

    // Copies the color plane in the main image.
    VisionErrChk(imaqDuplicate(image, plane));

Error:
    imaqDispose(plane);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_Classification
//
// Description  : Classifies the object located in the given ROI.
//
// Parameters   : image     -  Input image
//                fileName  -  Character Set File Path
//                roi       -  Region of interest specifying the location of
//                             the object in the image.
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_Classification(Image* image, char* fileName, ROI* roi)
{
    int success = 1;
    ClassifierSession* classifierSession = NULL;
    char description[255];
    ClassifierReport* classifierReport = NULL;


    //-------------------------------------------------------------------//
    //                             Classify                              //
    //-------------------------------------------------------------------//

    // Reads the classifier file and properties.
    VisionErrChk(classifierSession = imaqReadClassifierFile(NULL, fileName, IMAQ_CLASSIFIER_READ_ALL, NULL, NULL, description));

    // Classifies the object located in the given ROI.
    VisionErrChk(classifierReport = imaqClassify(image, classifierSession, roi, NULL, 0));

Error:
    imaqDispose(classifierSession);
    imaqDispose(classifierReport);

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_InitData
//
// Description  : Initializes data for buffer management and results.
//
// Parameters   : # of steps
//                # of coordinate systems
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static IVA_Data* IVA_InitData(int numSteps, int numCoordSys)
{
    int success = 1;
    IVA_Data* ivaData = NULL;
    int i;


    // Allocate the data structure.
    VisionErrChk(ivaData = (IVA_Data*)malloc(sizeof (IVA_Data)));

    // Initializes the image pointers to NULL.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        ivaData->buffers[i] = NULL;

    // Initializes the steo results array to numSteps elements.
    ivaData->numSteps = numSteps;

    ivaData->stepResults = (IVA_StepResults*)malloc(ivaData->numSteps * sizeof(IVA_StepResults));
    for (i = 0 ; i < numSteps ; i++)
    {
        #if defined (IVA_STORE_RESULT_NAMES)
            sprintf(ivaData->stepResults[i].stepName, "");
        #endif
        ivaData->stepResults[i].numResults = 0;
        ivaData->stepResults[i].results = NULL;
    }

    // Create the coordinate systems
    ivaData->baseCoordinateSystems = NULL;
    ivaData->MeasurementSystems = NULL;
    if (numCoordSys)
    {
        ivaData->baseCoordinateSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
        ivaData->MeasurementSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
    }

    ivaData->numCoordSys = numCoordSys;

Error:
    return ivaData;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeData
//
// Description  : Releases the memory allocated in the IVA_Data structure
//
// Parameters   : ivaData  -  Internal data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_DisposeData(IVA_Data* ivaData)
{
    int i;


    // Releases the memory allocated for the image buffers.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        imaqDispose(ivaData->buffers[i]);

    // Releases the memory allocated for the array of measurements.
    for (i = 0 ; i < ivaData->numSteps ; i++)
        IVA_DisposeStepResults(ivaData, i);

    free(ivaData->stepResults);

    // Dispose of coordinate systems
    if (ivaData->numCoordSys)
    {
        free(ivaData->baseCoordinateSystems);
        free(ivaData->MeasurementSystems);
    }

    free(ivaData);

    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeStepResults
//
// Description  : Dispose of the results of a specific step.
//
// Parameters   : ivaData    -  Internal data structure
//                stepIndex  -  step index
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
static int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex)
{
    int i;

    
    for (i = 0 ; i < ivaData->stepResults[stepIndex].numResults ; i++)
    {
        if (ivaData->stepResults[stepIndex].results[i].type == IVA_STRING)
            free(ivaData->stepResults[stepIndex].results[i].resultVal.strVal);
    }

    free(ivaData->stepResults[stepIndex].results);

    return TRUE;
}


