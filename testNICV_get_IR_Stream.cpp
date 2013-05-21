#include <OpenNI.h>
#include "../opencv2/core/core.hpp"
#include "../opencv2/highgui/highgui.hpp"
#include "../opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace openni;
int main(int argc, char** argv)
{
  // -----------------OpenNI Initializations-------------------//
	Device device;        // Software object for the physical device i.e. 
				          // PrimeSense Device Class
	VideoStream ir;       // IR VideoStream Class Object
	VideoFrameRef irf;    //IR VideoFrame Class Object
	VideoMode vmode;      // VideoMode Object
	Status rc = STATUS_OK;

	rc = openni::OpenNI::initialize();    // Initialize OpenNI
	rc = device.open(openni::ANY_DEVICE); // Open the Device
	rc = ir.create(device, SENSOR_IR);    // Create the VideoStream for IR
	rc = ir.start();                      // Start the IR VideoStream

	Mat frame;				// OpenCV Matrix Object, also used to store images
	int h, w;				// Height and Width of the IR VideoFrame

	while(true)				// Crux of this project
	{
		if(device.getSensorInfo(SENSOR_IR) != NULL)
		{
			rc = ir.readFrame(&irf);		// Read one IR VideoFrame at a time
			if(irf.isValid())				// If the IR VideoFrame is valid
			{
				vmode = ir.getVideoMode();  // Get the IR VideoMode Info for this video stream. 
										    // This includes its resolution, fps and stream format.
				const uint16_t* imgBuf = (const uint16_t*)irf.getData(); 
										    // PrimeSense gives the IR stream as 16-bit data output
				h=irf.getHeight(); 
				w=irf.getWidth();
				frame.create(h, w, CV_16U); // Create the OpenCV Mat Matrix Class Object 
											// to receive the IR VideoFrames
				memcpy(frame.data, imgBuf, h*w*sizeof(uint16_t)); 
											// Copy the ir data from memory imgbuf -> frame.data 
											// using memcpy (a string.h) function
				frame.convertTo(frame, CV_8U); 
											// OpenCV displays 8-bit data (I'm not sure why?) 
											// So, convert from 16-bit to 8-bit
				namedWindow("ir", 1);		// Create a named window
				imshow("ir", frame);		// Show the IR VideoFrame in this window
				char key = waitKey(10);
				if(key==27) break;			// Escape key number
			}
		}
	}
	//--------Safe closing--------//
	ir.stop();								// Stop the IR VideoStream
	ir.destroy();
	device.close();							// Close the PrimeSense Device
}
