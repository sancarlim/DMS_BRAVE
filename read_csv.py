import csv
import pandas as pd 
import argparse


def get_calib1():
	if 1.000 < time and time < 11.000:
		gazey_crossy_400.append(gazey)
		
	if 16.200 < time and time < 21.160:
		gazex_crossx_700.append(gazex)
		
	if 25.200 < time and time < 29.000:
		gazex_road.append(gazex)
		gazey_road.append(gazey)
	if 32.100 < time and time < 34.000:
		gazex_screen.append(gazex)
		gazey_screen.append(gazey)
	if 36.500 < time and time < 39.800:
		gazex_phone.append(gazex)
		gazey_phone.append(gazey)


def get_calib2():
	if 31.5 < time and time < 35.5:
		gazex_crossx_350.append(gazex)
	elif 19.5 < time and time < 23.5:
		gazex_crossx_1400.append(gazex)
	elif 11.5 < time and time < 19.5:
		gazey_crossy_80.append(gazey)
	elif 23.5 < time and time < 31.5:
		gazey_crossy_315.append(gazey)
	elif 35.5 < time and time < 43.5:
		gazey_crossy_550.append(gazey)

def write_calib2():

	f.write('gazex_crossx_350: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazex_crossx_350))) + ', ' + str('%.3f'%(min(gazex_crossx_350))) + ', ' + str('%.3f'%(sum(gazex_crossx_350)/len(gazex_crossx_350)))+  '\n')

	f.write('gazex_crossx_1400: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazex_crossx_1400))) + ', ' + str('%.3f'%(min(gazex_crossx_1400))) + ', ' + str('%.3f'%(sum(gazex_crossx_1400)/len(gazex_crossx_1400)))+ '\n')


	f.write('gazey_crossy_80: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_crossy_80))) + ', ' + str('%.3f'%(min(gazey_crossy_80))) + ', ' + str('%.3f'%(sum(gazey_crossy_80)/len(gazey_crossy_80)))+ '\n')


	f.write('gazey_crossy_315: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_crossy_315))) + ', ' + str('%.3f'%(min(gazey_crossy_315))) + ', ' + str('%.3f'%(sum(gazey_crossy_315)/len(gazey_crossy_315)))+ '\n')


	f.write('gazey_crossy_550: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_crossy_550))) + ', ' + str('%.3f'%(min(gazey_crossy_550))) + ', ' + str('%.3f'%(sum(gazey_crossy_550)/len(gazey_crossy_550)))+ '\n')


def write_calib1():

	f.write('gazex_crossx_700: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazex_crossx_700))) + ', ' + str('%.3f'%(min(gazex_crossx_700))) + ', ' + str('%.3f'%(sum(gazex_crossx_700)/len(gazex_crossx_700)))+  '\n')

	f.write('gazey_crossy_400: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_crossy_400))) + ', ' + str('%.3f'%(min(gazey_crossy_400))) + ', ' + str('%.3f'%(sum(gazey_crossy_400)/len(gazey_crossy_400)))+ '\n')

	f.write('gazex_road: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazex_road))) + ', ' + str('%.3f'%(min(gazex_road))) + ', ' + str('%.3f'%(sum(gazex_road)/len(gazex_road)))+ '\n')
	f.write('gazey_road: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_road))) + ', ' + str('%.3f'%(min(gazey_road))) + ', ' + str('%.3f'%(sum(gazey_road)/len(gazey_road)))+ '\n')

	f.write('gazex_screen: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazex_screen))) + ', ' + str('%.3f'%(min(gazex_screen))) + ', ' + str('%.3f'%(sum(gazex_screen)/len(gazex_screen)))+ '\n')
	f.write('gazey_screen: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_screen))) + ', ' + str('%.3f'%(min(gazey_screen))) + ', ' + str('%.3f'%(sum(gazey_screen)/len(gazey_screen)))+ '\n')

	f.write('gazex_phone: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazex_phone))) + ', ' + str('%.3f'%(min(gazex_phone))) + ', ' + str('%.3f'%(sum(gazex_phone)/len(gazex_phone)))+ '\n')
	f.write('gazey_phone: MAX  MIN  MEDIO  \n')
	f.write(str('%.3f'%(max(gazey_phone))) + ', ' + str('%.3f'%(min(gazey_phone))) + ', ' + str('%.3f'%(sum(gazey_phone)/len(gazey_phone)))+ '\n')





ap = argparse.ArgumentParser()
ap.add_argument("-p", "--path", required=True,
	help="path to csv file")
args = vars(ap.parse_args())

name = args["path"].split('/')[-1].split('.')[0]

data = pd.read_csv(args["path"], delimiter=',', usecols=[' AU26_c', ' AU25_c',' AU45_c', ' timestamp', ' eye_lmk_X_8', 
	' eye_lmk_Y_10', ' eye_lmk_Y_12', ' eye_lmk_X_14', ' eye_lmk_Y_16', ' eye_lmk_Y_18', ' eye_lmk_X_36', ' eye_lmk_Y_38', 
	' eye_lmk_Y_40', ' eye_lmk_X_42', ' eye_lmk_Y_44', ' eye_lmk_Y_46', ' gaze_angle_x', ' gaze_angle_y', ' pose_Rx', ' pose_Ry', ' pose_Rz', ' gaze_0_x',
	' gaze_0_y', ' gaze_0_z',' gaze_1_x',' gaze_1_y',' gaze_1_z' ]) 
#print(data.head())
lastrow = data.count()-1
lastrow = lastrow[0].item()
blinks = data[[' AU45_c']]
t_tot = data.loc[lastrow,' timestamp']
ts = data.loc[1,' timestamp'] - data.loc[0,' timestamp']
#t_tot = timestamp[-1]
print(F'Ts: {ts}, T_total: {t_tot}')
n_frames = len(blinks)
print(f'n_frames: {n_frames}')
n_blinks = 0
n_yawn=0
blink=0
n_perclos=0
EAR_list = []
EARini_list = []


gazex_crossx_350 = []
gazex_crossx_1400 = []
gazey_crossy_80 = []
gazey_crossy_315 = []
gazey_crossy_550 = []


gazex_crossx_700 = []
gazey_crossy_400 = []
gazex_phone = []
gazey_phone = []
gazex_road = []
gazey_road = []
gazex_screen = []
gazey_screen = []

rightEAR = 0.0
rightEAR = 0.0
t_blink=[]
gazex_mx=data.loc[0,' gaze_angle_x']
gazex_min=data.loc[0,' gaze_angle_x']
gazey_mx=data.loc[0,' gaze_angle_y']
gazey_min=data.loc[0,' gaze_angle_y']
file_gaze = "./OutputParams/" +name + "_gaze.txt"
file_DIP = "./OutputParams/" +name + "_DIP.txt"
open(file_gaze, 'w').close()
open(file_DIP, 'w').close()

f= open(file_gaze, 'a+')
f_DIP = open(file_DIP, 'a+')

f.write('time, posex, posey, posez, gazex, gazey, gazez, gaze_heading, gaze_pitch \n')

for index, row in data.iterrows():
	time = row[[' timestamp']].item()
	'''if index < 3:
		C_l = abs(row[[' eye_lmk_X_8']].item() - row[[' eye_lmk_X_14']].item())
		C_r = abs(row[[' eye_lmk_X_36']].item() - row[[' eye_lmk_X_42']].item())
		A_l = abs(row[[' eye_lmk_Y_10']].item() - row[[' eye_lmk_Y_18']].item())
		B_l = abs(row[[' eye_lmk_Y_12']].item() - row[[' eye_lmk_Y_16']].item())
		A_r = abs(row[[' eye_lmk_Y_38']].item() - row[[' eye_lmk_Y_46']].item())
		B_r = abs(row[[' eye_lmk_Y_44']].item() - row[[' eye_lmk_Y_40']].item())
		leftEAR =(A_l+B_l)/(2.0*C_l)
		rightEAR = (A_r+B_r)/(2.0*C_r)
		EAR = (leftEAR+rightEAR) / 2.0
		EARini_list.append(EAR)
		
	EARini = sum(EARini_list)/len(EARini_list)
	time = row[[' timestamp']].item()
	if row[[' AU45_c']].item() != 0.0:
		n_perclos+=1
		if blink == 0:
			n_blinks += 1
			blink = 1
			t_blink.append(time)  # t with blinking
	else:
		blink = 0


	if row[[' AU25_c']].item() != 0.0 and row[[' AU26_c']].item() != 0.0:
		n_yawn+=1
	
	C_l = abs(row[[' eye_lmk_X_8']].item() - row[[' eye_lmk_X_14']].item())
	C_r = abs(row[[' eye_lmk_X_36']].item() - row[[' eye_lmk_X_42']].item())
	A_l = abs(row[[' eye_lmk_Y_10']].item() - row[[' eye_lmk_Y_18']].item())
	B_l = abs(row[[' eye_lmk_Y_12']].item() - row[[' eye_lmk_Y_16']].item())
	A_r = abs(row[[' eye_lmk_Y_38']].item() - row[[' eye_lmk_Y_46']].item())
	B_r = abs(row[[' eye_lmk_Y_44']].item() - row[[' eye_lmk_Y_40']].item())
	leftEAR =(A_l+B_l)/(2.0*C_l)
	rightEAR = (A_r+B_r)/(2.0*C_r)
	EAR = (leftEAR+rightEAR) / 2.0
	EAR_list.append(EAR)'''

	gaze_x = '%.3f'%((row[[' gaze_0_x']].item() + row[[' gaze_1_x']].item())/2)
	gaze_y = '%.3f'%((row[[' gaze_0_y']].item() + row[[' gaze_1_y']].item())/2)
	gaze_z = '%.3f'%((row[[' gaze_0_z']].item() + row[[' gaze_1_z']].item())/2)

	pose_Rx = '%.3f'%(row[[' pose_Rx']].item())
	pose_Ry = '%.3f'%(row[[' pose_Ry']].item())
	pose_Rz = '%.3f'%(row[[' pose_Rz']].item())

	gazex=( row[[' gaze_angle_x']].item()*180/3.141592)
	gazey=( row[[' gaze_angle_y']].item()*180/3.141592)



	get_calib1()
	#get_calib2()

	
	f.write(str('%.3f'%(time)) + ', '+str(pose_Rx)+ ', '+str(pose_Ry)+ ', '+str(pose_Rz)+ ',  '+str(gaze_x)+ ', '+str(gaze_y)+ ', '+str(gaze_z)+ ',  '+str('%.3f'%(gazex)) + ', '+str('%.3f'%(gazey)) + '\n')
	'''blink_freq = n_blinks/t_tot
	PERCLOS = ((n_perclos-4*n_blinks)/n_frames) #t with eyes closed without blink time [0,1]
	yawning = n_yawn/n_frames #normilised [0,1]
	aperture = ( sum(EAR_list)/len(EAR_list) ) / EARini 
	f_DIP.write(str('%.3f'%(aperture) + ', '+str('%.3f'%(PERCLOS)) + ', '+str('%.3f'%(blink_freq)) + ', '+str('%.3f'%(yawning))))'''


write_calib1()
#write_calib2()

f.close()

f_DIP.close()

		


'''
with open('openface_Jose1.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    n_blinks = []
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {", ".join(row)}')
            print(f'Number of cols: {len(row)}')
            line_count += 1
        else:
            # print(f'\t{row[0]} works in the {row[1]} department, and was born in {row[2]}.')
            line_count += 1
            blink = row[713]
            print(blink)
            n_blinks = n_blinks.append(blink)
    print(f'Processed {line_count} lines.')
    f'n_blinks {n_blinks}'
'''
