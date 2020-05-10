
// FeatureExtraction.cpp : Defines the entry point for the feature extraction console application.

// Local includes
#include "LandmarkCoreIncludes.h"

#include <Face_utils.h>
#include <FaceAnalyser.h>
#include <GazeEstimation.h>
#include <RecorderOpenFace.h>
#include <RecorderOpenFaceParameters.h>
#include <SequenceCapture.h>
#include <Visualizer.h>
#include <VisualizationUtils.h>


#pragma GCC diagnostic ignored "-Wwrite-strings"
#include "fis.c"
#pragma GCC diagnostic warning "-Wwrite-strings"


#ifndef CONFIG_DIR
#define CONFIG_DIR "~"
#endif

#define INFO_STREAM( stream ) \
std::cout << stream << std::endl

#define WARN_STREAM( stream ) \
std::cout << "Warning: " << stream << std::endl

#define ERROR_STREAM( stream ) \
std::cout << "Error: " << stream << std::endl

using namespace std;

static void printErrorAndAbort(const std::string & error)
{
	std::cout << error << std::endl;
}


	//Fuzzy 

	FIS *m_fis;
    double **m_fis_inputs;
    double **m_fis_output;
    double **m_fisMatrix = NULL;
    double m_gananciaFIS = 200;

	FIS *m_fis_IL;
    double **m_fisIL_inputs;
    double **m_fisIL_output;
    double **m_fisMatrix_IL = NULL;
    double m_gananciaFIS_IL = 200;

	double distanceCalculate(double x1, double y1, double x2, double y2)
	{
		double x = x1 - x2; //calculating number to square in next step
		double y = y1 - y2;
		double dist;

		dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
		dist = sqrt(dist);                  

		return dist;
	}

	void activate_fis()
	{
		int fis_row_n = 0, fis_col_n = 0;
		m_fisMatrix = returnFismatrix((char*)"/home/sandra/PROGRAMAS/OpenFace/exe/FeatureExtraction/DIP_mamdani.fis", &fis_row_n, &fis_col_n);
		m_fisMatrix_IL = returnFismatrix((char*)"/home/sandra/PROGRAMAS/OpenFace/exe/FeatureExtraction/InattentionLevel.fis", &fis_row_n, &fis_col_n);
		if(m_fisMatrix == NULL || m_fisMatrix_IL == NULL)
			std::cout <<"Error leyendo FIS" << std::endl;
		else
			//std::cout <<"FIS leido" << std::endl;

		/* build FIS data structure */
		m_fis = (FIS *)fisCalloc(1, sizeof(FIS));
		m_fis_IL = (FIS *)fisCalloc(1, sizeof(FIS));
		fisBuildFisNode(m_fis, m_fisMatrix, fis_col_n, MF_POINT_N);
		fisBuildFisNode(m_fis_IL, m_fisMatrix_IL, fis_col_n, MF_POINT_N);


		/* obtain data matrix and FIS matrix */
		m_fis_inputs = (double**)fisCreateMatrix(1, m_fis->in_n, sizeof(double));
		m_fis_output = (double**)fisCreateMatrix(1, m_fis->out_n, sizeof(double));
		m_fisIL_inputs = (double**)fisCreateMatrix(1, m_fis->in_n, sizeof(double));
		m_fisIL_output = (double**)fisCreateMatrix(1, m_fis->out_n, sizeof(double));
	}

	void deactivate_fis()
	{
		fisFreeMatrix((void **)m_fis_inputs, m_fis->in_n);
		fisFreeMatrix((void **)m_fis_output, m_fis->out_n);
		fisFreeMatrix((void **)m_fisMatrix, m_fis->in_n * m_fis->out_n);
		fisFreeFisNode(m_fis);

		fisFreeMatrix((void **)m_fisIL_inputs, m_fis_IL->in_n);
		fisFreeMatrix((void **)m_fisIL_output, m_fis_IL->out_n);
		fisFreeMatrix((void **)m_fisMatrix_IL, m_fis_IL->in_n * m_fis_IL->out_n);
		fisFreeFisNode(m_fis_IL);
		
	}


#define FATAL_STREAM( stream ) \
printErrorAndAbort( std::string( "Fatal error: " ) + stream )

std::vector<std::string> get_arguments(int argc, char **argv)
{

	std::vector<std::string> arguments;

	// First argument is reserved for the name of the executable
	for (int i = 0; i < argc; ++i)
	{
		arguments.push_back(std::string(argv[i]));
	}
	return arguments;
}

int main(int argc, char **argv)
{

	std::vector<std::string> arguments = get_arguments(argc, argv);

	int prev=0, state=0, state_vis=0;
	float EAR_sum;
	int n_perclos=0;
	int n_blinks=0;
	int blink=0;
	double EARini = 0.0;
	int blink_thres = 2;
	int n_yawn;
	int yawn=0;
	double cnt_yawn = 0.0;
	double blink_freq = 0.0;
	double cnt_notblink = 0;
	double PERCLOS = 0.0;
	double yawning = 0.0;
	double aperture = 0.0;
	double fis_DIP = 0.0;
	double fis_IL = 0.0;
	double fis_distraction = 0.0;
	int count_vis =0;
	int count_state = 0;
	int EAR_count=0;
	double t = 0.0;	
	double timebuffer_ini = 0.0;
	double timebuffer = 0.0;
	int cnt_fis = 0.0;

	cv::Vec2d gazeAngle(0, 0); 

	fstream blink_txt("blink.txt");
	fstream aperture_txt("aperture.txt");
	fstream perclos_txt("perclos.txt");
	fstream yawn_txt("yawn.txt");	
	fstream dip_txt("dip.txt");
	//string blink_txt, aperture_txt, perclos_txt, yawn_txt, dip_txt;

	// no arguments: output usage
	if (arguments.size() == 1)
	{
		std::cout << "For command line arguments see:" << std::endl;
		std::cout << " https://github.com/TadasBaltrusaitis/OpenFace/wiki/Command-line-arguments";
		return 0;
	}

	// Load the modules that are being used for tracking and face analysis
	// Load face landmark detector
	LandmarkDetector::FaceModelParameters det_parameters(arguments);
	// Always track gaze in feature extraction
	LandmarkDetector::CLNF face_model(det_parameters.model_location);

	if (!face_model.loaded_successfully)
	{
		std::cout << "ERROR: Could not load the landmark detector" << std::endl;
		return 1;
	}

	// Load facial feature extractor and AU analyser
	FaceAnalysis::FaceAnalyserParameters face_analysis_params(arguments);
	FaceAnalysis::FaceAnalyser face_analyser(face_analysis_params);

	if (!face_model.eye_model)
	{
		std::cout << "WARNING: no eye model found" << std::endl;
	}

	if (face_analyser.GetAUClassNames().size() == 0 && face_analyser.GetAUClassNames().size() == 0)
	{
		std::cout << "WARNING: no Action Unit models found" << std::endl;
	}

	Utilities::SequenceCapture sequence_reader;

	// A utility for visualizing the results !!!
	Utilities::Visualizer visualizer(arguments);

	// Tracking FPS for visualization
	Utilities::FpsTracker fps_tracker;
	fps_tracker.AddFrame();

	while (true) // this is not a for loop as we might also be reading from a webcam
	{

		// The sequence reader chooses what to open based on command line arguments provided
		if (!sequence_reader.Open(arguments))
			break;

		INFO_STREAM("Device or file opened");

		if (sequence_reader.IsWebcam())
		{
			INFO_STREAM("WARNING: using a webcam in feature extraction, Action Unit predictions will not be as accurate in real-time webcam mode");
			INFO_STREAM("WARNING: using a webcam in feature extraction, forcing visualization of tracking to allow quitting the application (press q)");
			visualizer.vis_track = true;
		}

		cv::Mat captured_image;

		Utilities::RecorderOpenFaceParameters recording_params(arguments, true, sequence_reader.IsWebcam(),
			sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, sequence_reader.fps);
		if (!face_model.eye_model)
		{
			recording_params.setOutputGaze(false);
		}
		Utilities::RecorderOpenFace open_face_rec(sequence_reader.name, recording_params, arguments);

		if (recording_params.outputGaze() && !face_model.eye_model)
			std::cout << "WARNING: no eye model defined, but outputting gaze" << std::endl;

		captured_image = sequence_reader.GetNextFrame();

		// For reporting progress
		double reported_completion = 0;

		INFO_STREAM("Starting tracking");
		while (!captured_image.empty())
		{
			// Converting to grayscale
			cv::Mat_<uchar> grayscale_image = sequence_reader.GetGrayFrame();


			// The actual facial landmark detection / tracking
			bool detection_success = LandmarkDetector::DetectLandmarksInVideo(captured_image, face_model, det_parameters, grayscale_image);
			
			// Gaze tracking, absolute gaze direction
			cv::Point3f gazeDirection0(0, 0, 0); cv::Point3f gazeDirection1(0, 0, 0); cv::Vec2d gazeAngle_t(0, 0);

			int frame_n = sequence_reader.GetFrameNumber(); //starts with 1

			if (detection_success && face_model.eye_model) //!!OUTPUT GAZE ANGLE
			{
				GazeAnalysis::EstimateGaze(face_model, gazeDirection0, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, true);
				GazeAnalysis::EstimateGaze(face_model, gazeDirection1, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy, false);
				gazeAngle_t = GazeAnalysis::GetGazeAngle(gazeDirection0, gazeDirection1);
                //cout << "Gaze Angle: " << gazeAngle*100 << "\n";
			}
			if (frame_n % 3 == 0)
				gazeAngle = gazeAngle_t;


			
			prev = state; 
			if ( ((gazeAngle_t[0]*100 < -5) && (gazeAngle_t[1]*100 > 50) ) || (gazeAngle_t[1]*100>=55))
			{	
				state = 1; //mobile
				fis_distraction = 1.0;
			}
			else if ((-5 <= gazeAngle_t[0]*100 and gazeAngle_t[0]*100 < 10) && (gazeAngle_t[1]*100 > 50 and gazeAngle_t[1] < 55))
			{
				state = 2; //screen
				fis_distraction = 0.0;
			}
			else if ( (gazeAngle_t[0]*100 > 15 ) || (gazeAngle_t[1]*100 < 30) )
			{
				state = 3; //mirror
				fis_distraction = 0.0;
			}
			else 
			{
				state = 0; //road
				fis_distraction = 0.0;
			}	

				
			if (prev == state)
				count_state++;
			else 
				count_state = 0;


			if (count_state >= 3) //si el estado no ha cambiado en n frames lo visualizamos, si no visualizamos el anterior.
			{
				state_vis = state;
				if (state_vis == 2)
				{
					timebuffer = (cv::getTickCount() - timebuffer_ini)/cv::getTickFrequency();
					cout << ", timebuf2: " << timebuffer;
					fis_distraction = std::min(1.0, 0.5 + 0.1*timebuffer); //0.6 for 1s, 1 for 5s
				}
				else if (state_vis == 3)
				{
					timebuffer = (cv::getTickCount() - timebuffer_ini)/cv::getTickFrequency();
					cout << ", timebuf3: " << timebuffer;
					fis_distraction = min(1.0, 0.2*timebuffer); //0.2 for 1s, 1 for 5s	
				}
			}
			else
			{
				timebuffer_ini = cv::getTickCount();
			}
			
			
			// Do face alignment
			cv::Mat sim_warped_img;
			cv::Mat_<double> hog_descriptor; int num_hog_rows = 0, num_hog_cols = 0;

			// Perform AU detection and HOG feature extraction, as this can be expensive only compute it if needed by output or visualization
			if (recording_params.outputAlignedFaces() || recording_params.outputHOG() || recording_params.outputAUs() || visualizer.vis_align || visualizer.vis_hog || visualizer.vis_aus)
			{
				face_analyser.AddNextFrame(captured_image, face_model.detected_landmarks, face_model.detection_success, sequence_reader.time_stamp, sequence_reader.IsWebcam());
				face_analyser.GetLatestAlignedFace(sim_warped_img);
				face_analyser.GetLatestHOG(hog_descriptor, num_hog_rows, num_hog_cols);
			}
		
			// Work out the pose of the head from the tracked model
			cv::Vec6d pose_estimate = LandmarkDetector::GetPose(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);

			// Keeping track of FPS
			fps_tracker.AddFrame();

			// Displaying the tracking visualizations!!!
			visualizer.SetImage(captured_image, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);
			//visualizer.SetObservationFaceAlign(sim_warped_img);
			//visualizer.SetObservationHOG(hog_descriptor, num_hog_rows, num_hog_cols);
			visualizer.SetObservationLandmarks(face_model.detected_landmarks, face_model.detection_certainty, face_model.GetVisibilities());
			//visualizer.SetObservationPose(pose_estimate, face_model.detection_certainty);
			visualizer.SetObservationGaze(gazeDirection0, gazeDirection1, LandmarkDetector::CalculateAllEyeLandmarks(face_model), LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy), face_model.detection_certainty);
			visualizer.SetObservationActionUnits(face_analyser.GetCurrentAUsReg(), face_analyser.GetCurrentAUsClass());
			visualizer.SetFps(fps_tracker.GetFPS());
			
			visualizer.SetDIP(fis_DIP, fis_distraction);
			visualizer.SetIL(fis_IL);
			visualizer.SetParameters(aperture, yawn, yawning, blink_freq, PERCLOS, n_blinks);
            visualizer.SetGaze(gazeAngle, count_vis, state_vis);
			
			count_vis++;
			
			// detect key presses
			char character_press = visualizer.ShowObservation();
			
			// quit processing the current sequence (useful when in Webcam mode)
			if (character_press == 'q')
			{
				break;
			}

			// Setting up the recorder output
			open_face_rec.SetObservationHOG(detection_success, hog_descriptor, num_hog_rows, num_hog_cols, 31); // The number of channels in HOG is fixed at the moment, as using FHOG
			open_face_rec.SetObservationVisualization(visualizer.GetVisImage());
			open_face_rec.SetObservationActionUnits(face_analyser.GetCurrentAUsReg(), face_analyser.GetCurrentAUsClass());
			open_face_rec.SetObservationLandmarks(face_model.detected_landmarks, face_model.GetShape(sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy),
				face_model.params_global, face_model.params_local, face_model.detection_certainty, detection_success);
			open_face_rec.SetObservationPose(pose_estimate);
			open_face_rec.SetObservationGaze(gazeDirection0, gazeDirection1, gazeAngle, LandmarkDetector::CalculateAllEyeLandmarks(face_model), LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy));
			open_face_rec.SetObservationTimestamp(sequence_reader.time_stamp);
			open_face_rec.SetObservationFaceID(0);
			open_face_rec.SetObservationFrameNumber(sequence_reader.GetFrameNumber());
			open_face_rec.SetObservationFaceAlign(sim_warped_img);
			open_face_rec.WriteObservation();
			open_face_rec.WriteObservationTracked();

			
			double timestamp = sequence_reader.time_stamp;
			
			std::vector<cv::Point2f>  EyeLandmarks2D = LandmarkDetector::CalculateAllEyeLandmarks(face_model);
			std::vector<cv::Point3f> EyeLandmarks3D = LandmarkDetector::Calculate3DEyeLandmarks(face_model, sequence_reader.fx, sequence_reader.fy, sequence_reader.cx, sequence_reader.cy);
			std::vector<float> EARini_list(10);
			float C_l = distanceCalculate(EyeLandmarks2D[8].x,EyeLandmarks2D[8].y,EyeLandmarks2D[14].x,EyeLandmarks2D[14].y);//abs(EyeLandmarks2D[8].x - EyeLandmarks2D[14].x);
			float C_r = distanceCalculate(EyeLandmarks2D[36].x,EyeLandmarks2D[36].y,EyeLandmarks2D[42].x,EyeLandmarks2D[42].y);//abs(EyeLandmarks2D[36].x - EyeLandmarks2D[42].x);
			float A_l = distanceCalculate(EyeLandmarks2D[10].x,EyeLandmarks2D[10].y,EyeLandmarks2D[18].x,EyeLandmarks2D[18].y);//abs(EyeLandmarks2D[10].y - EyeLandmarks2D[18].y);
			float B_l = distanceCalculate(EyeLandmarks2D[12].x,EyeLandmarks2D[12].y,EyeLandmarks2D[16].x,EyeLandmarks2D[16].y);//abs(EyeLandmarks2D[12].y - EyeLandmarks2D[16].y);
			float A_r = distanceCalculate(EyeLandmarks2D[38].x,EyeLandmarks2D[38].y,EyeLandmarks2D[46].x,EyeLandmarks2D[46].y);//abs(EyeLandmarks2D[38].y - EyeLandmarks2D[46].y);
			float B_r = distanceCalculate(EyeLandmarks2D[44].x,EyeLandmarks2D[44].y,EyeLandmarks2D[40].x,EyeLandmarks2D[40].y);//abs(EyeLandmarks2D[44].y - EyeLandmarks2D[40].y);
			float leftEAR = (A_l+B_l)/(2.0*C_l);
			float rightEAR = (A_r+B_r)/(2.0*C_r);
			double EAR = (leftEAR+rightEAR) / 2.0;

			//cout << EAR <<  "frame: " << frame_n << "\n";

			if (frame_n <= 10)
			{
				EARini_list[frame_n-1] = EAR; 
				if (frame_n == 1)
				{
					EAR_sum  = 0.0;
					n_perclos = 0;
					n_blinks = 0;
					blink = 0;
					n_yawn = 0;
					t = cv::getTickCount();
				}
				EAR_sum += EAR;
				if(frame_n == 10)
				{
					EARini = EAR_sum/EARini_list.size();
					cout << "EAR inicial: " << EARini << "\n";
				}
			}
			else
			{
				aperture = EAR / EARini;
				if (face_analyser.GetCurrentAUsClass()[17].second == 1)
				//if (EAR <= 0.8*EARini)
					{
						blink=0;
						n_perclos++; 
						EAR_count ++;
						cnt_notblink = 0;

						//cout << face_analyser.GetCurrentAUsReg()[17].second << "frame: "<< frame_n << "\n";
					}
				else 
					{
						cnt_notblink++;
						blink=0;
						if (EAR_count >= blink_thres)
						{	if( cnt_notblink > 3) 
							{
								n_blinks++;
								//cout << "BLINK: " << n_blinks << "frame: " << frame_n << "\n";
								//cout << face_analyser.GetCurrentAUsReg()[17].second << "\n";
								blink=1;
								cnt_notblink = 0;
								EAR_count=0;
							}
						} else
						{
							EAR_count=0;
						}
						
						
					} 
			}
			
			/*if (EAR < EAR_thres)
			{
				EAR_count ++;
				n_perclos++;
			}	
			else
			{
				if EAR_count >= EAR_consecframes
					n_blinks++;
				EAR_count = 0;
			}*/
				
			 yawn=0;
			if (face_analyser.GetCurrentAUsClass()[14].second == 1 && face_analyser.GetCurrentAUsClass()[15].second == 1)
			{
				cnt_yawn += 1;
				if (cnt_yawn > 9)
				{
					n_yawn += 10;
					cout << "YAWNNNNN";
					yawn = 1;
					cnt_yawn = 0;
				}
			}

			double current_time = (cv::getTickCount() - t)/cv::getTickFrequency();	

			blink_freq = n_blinks / current_time;
			yawning =double(n_yawn) / double(frame_n);
			PERCLOS = (n_perclos*0.033) / current_time; 

			if (current_time >= 333)  //VENTANA DE 30s
			{
				blink_txt << "Blink: " << blink_freq << endl;
				aperture_txt << "Aperture: " << aperture << endl;
				perclos_txt << "Perclos: " << PERCLOS << endl;
				yawn_txt << "Yawning: " << yawning << endl;
				dip_txt << "DIP: " << fis_DIP << endl;

				blink_freq = n_blinks / current_time;
				yawning = double(n_yawn) / double(frame_n) ;
				PERCLOS = (n_perclos*0.033) / current_time;   
				//std::cout << "PERCLOS: " << PERCLOS << "\n";
				t = cv::getTickCount();
				n_perclos = 0;
				n_blinks = 0;
				cnt_fis = 0;
			}
			
			//cout << "Aperture: " << aperture << "\n";
			//cout << "Yawning: " << yawning << "\n";
			//cout << "Blink rate: " << blink_freq << "\n";
			
			
			// Fuzzy

			activate_fis();
			m_fis_inputs[0][0] = PERCLOS;
			m_fis_inputs[0][1] = aperture;
			m_fis_inputs[0][2] = blink_freq;
			m_fis_inputs[0][3] = yawning;

			m_fisIL_inputs[0][0] = fis_DIP;
			m_fisIL_inputs[0][1] = fis_distraction;

			//cout << "   State, DLI: " << state << ", " << fis_distraction;

			if (frame_n > 10 && cnt_fis > 20)  //Cuando finaliza la ventana temporal se resetean todos los parametros, 
											  //para que no se dispare el valor de DIP con un pestañeo (blink rate muy elevado), esperamos 6 frames.
			{
				getFisOutput(m_fis_inputs[0], m_fis, m_fis_output[0]);
				fis_DIP = m_fis_output[0][0];
				//cout << "DIP: " << fis_DIP << "\n";
				getFisOutput(m_fisIL_inputs[0], m_fis_IL, m_fisIL_output[0]);
				fis_IL = m_fisIL_output[0][0];
			}
			cnt_fis ++;

			// Reporting progress
			if (sequence_reader.GetProgress() >= reported_completion / 10.0)
			{
				std::cout << reported_completion * 10 << "% ";
				if (reported_completion == 10)
				{
					std::cout << std::endl;
				}
				reported_completion = reported_completion + 1;
			}

			// Grabbing the next frame in the sequence
			captured_image = sequence_reader.GetNextFrame();

		}
		

		INFO_STREAM("Closing output recorder");
		open_face_rec.Close();
		INFO_STREAM("Closing input reader");
		sequence_reader.Close();
		INFO_STREAM("Closed successfully");

		if (recording_params.outputAUs()) //!!!
		{
			INFO_STREAM("Postprocessing the Action Unit predictions");
			face_analyser.PostprocessOutputFile(open_face_rec.GetCSVFile());
		}

		// Reset the models for the next video
		face_analyser.Reset();
		face_model.Reset();

		
		deactivate_fis();

	}

	return 0;
}
