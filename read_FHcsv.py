import csv
import pandas as pd 
import argparse


def calcula_calib1():
	if cross_x == 700.00:
		gazex_crossx_700.append(gaze__heading)

	if cross_y == 400.00:
		gazey_crossy_400.append(gaze_pitch)

	if 25000.0 < time and time < 28900.0:
		gazex_road.append(gaze__heading)
		gazey_road.append(gaze_pitch)
	if 32000.0 < time and time < 34000.0:
		gazex_screen.append(gaze__heading)
		gazey_screen.append(gaze_pitch)
	if 36300.0 < time and time < 40000.0:
		gazex_phone.append(gaze__heading)
		gazey_phone.append(gaze_pitch)



def calcula_calib2():
	if cross_x == 350.00:
		gazex_crossx_350.append(gaze__heading)
	elif cross_x == 1400.00:
		gazex_crossx_1400.append(gaze__heading)

	if cross_y == 80.00:
		gazey_crossy_80.append(gaze_pitch)
	elif cross_y == 315.00:
		gazey_crossy_315.append(gaze_pitch)
	elif cross_y == 550.00:
		gazey_crossy_550.append(gaze_pitch)

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

data = pd.read_csv(args["path"], delimiter=',',na_values=['nan', 'NaN'])#print(data.head())
data.fillna(0, inplace=True)
lastrow = data.count()-1
lastrow = lastrow[0].item()
n_frames=lastrow+1
t_tot = data.loc[lastrow,'time']-data.loc[0,'time']
ts = data.loc[81,'time'] - data.loc[80,'time']
# ts2 = data.loc[131,'time'] - data.loc[130,'time']
# ts4 = data.loc[141,'time'] - data.loc[140,'time']
# ts5 = data.loc[211,'time'] - data.loc[210,'time']
# ts3 = data.loc[151,'time'] - data.loc[150,'time']
# print(data.loc[31,'time'], data.loc[30,'time'])

print(F'Ts: {ts} ms, T_total: {t_tot} ms')
print(f'n_frames: {n_frames}')

gazex_mx=data.loc[0,'gaze_x']
gazex_min=data.loc[0,'gaze_x']
gazey_mx=data.loc[0,'gaze_y']
gazey_min=data.loc[0,'gaze_y']
file_gaze = "/home/sandra/PROGRAMAS/OpenFace/build/processed/OutputParams/" +name + "_FHgaze.txt"
open(file_gaze, 'w').close()

f = open(file_gaze, 'a+')

primero = 0
star=0

f.write('time, cross_x,  cross_y, gazex, gazey, gazez, gaze_heading, gaze_pitch     \n')

for index, row in data.iterrows():
	if row[['background_star']].item() == 1 and star==0:
		print(f'TIME STAR: {index}')
		dif_time = row[['time']].item() -1150
		star=1 

	if star==1:
		time = row[['time']].item()-dif_time #+1000
		gazex=( row[['gaze_x']].item()*180/3.141592)
		gazey=( row[['gaze_y']].item()*180/3.141592)
		gazez=( row[['gaze_z']].item()*180/3.141592)
		gaze__heading= row[['gaze__heading']].item()*180/3.14159
		gaze_pitch=row[['gaze_pitch']].item()*180/3.14159
		head_pitch = row[['head_pitch']].item()*180/3.14159
		head_roll = row[['head_roll']].item()*180/3.14159
		head_heading = row[['head_heading']].item()*180/3.14159
			
	
		cross_x = row[['cross_x']].item()
		cross_y = row[['cross_y']].item()

		#calcula_calib2()
		calcula_calib1()

		f.write(str('%.3f'%(time)) + ', ' + str('%.2f'%(cross_x)) + ', ' + str('%.2f'%(cross_y))+ ',  '+str('%.3f'%(gazex))+ ', '+str('%.3f'%(gazey)) +', '+str('%.3f'%(gazez)) +
			',  ' +str('%.3f'%(gaze__heading))+', ' + str('%.3f'%(gaze_pitch))+ '\n')
		#print(f'{gazex}, {gazey}')

#write_calib2()
write_calib1()

	
print(f'gazeX max, min = {gazex_mx}, {gazex_min}')
print(f'gazeY max, min = {gazey_mx}, {gazey_min}')


f.close()
	


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