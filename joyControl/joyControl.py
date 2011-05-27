#!/usr/bin/env python
import serial
from time import sleep
k=0
ser=None
while k<256:
	p='/dev/ttyUSB%d' % k
	try:
		print "Tentative de connexion sur %s..." % p
		ser = serial.Serial(p, 57600, timeout=.1)
		break
	except serial.serialutil.SerialException:
		if k==255:
			raise Exception("Carte introuvable.")
		k+=1
		continue
	
print "Connexion ouverte sur %s" % p
	
sleep(2)
	
import pygame
#from pygame import joystick, event

pygame.init()
#joystick.init()
j = pygame.joystick.Joystick(0)
j.init()

avantmin=1400#1445
avantmax=1760
rangeavant=avantmax-avantmin

arrieremin=1350#1315
arrieremax=1125
rangearriere=arrieremin-arrieremax

print 'Initialized Joystick : %s' % j.get_name()
try:
    c_g=1400
    c_d=1400
    while True:
        pygame.event.pump()
        trans=-j.get_axis(1)
        rot=j.get_axis(2)
        g=(trans+rot)/1.0
        if g>1.0:
        	g=1.0
        if g<-1.0:
        	g=-1.0
        
        d=(trans-rot)/1.0
        if d>1.0:
        	d=1.0
        if d<-1.0:
        	d=-1.0
        
        if g>0.0:
        	tc_g = avantmin+g*rangeavant
        else:
        	tc_g = arrieremin+g*rangearriere
        
        if d>0.0:
        	tc_d = avantmin+d*rangeavant
        else:
        	tc_d = arrieremin+d*rangearriere

        
        c_g += (tc_g-c_g)*.01
        c_d += (tc_d-c_d)*.01
        
        print c_g,c_d
        ser.write("G%d#"%c_g)
        ser.write("D%d#"%c_d)
        ser.flush()
        
      #for i in range(0, j.get_numbuttons()):
        #    if j.get_button(i) != 0:
except KeyboardInterrupt:
    j.quit()

