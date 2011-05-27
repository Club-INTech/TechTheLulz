#!/usr/bin/env python
import serial
from time import sleep
import sys
k=1
ser=None
while k<256:
	p='/dev/ttyUSB%d' % k
	try:
		print "Tentative de connexion sur %s..." % p
		ser = serial.Serial(p, 9600, timeout=.1)
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
    c_g=0
    c_d=0
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
        sg=0
        if g>0:
        	sg=1
       	else:
       		sg=-1
       	
       	sd=0
        if d>0:
        	sd=1
       	else:
       		sd=-1
       	
       	
       	g=.5+abs(g)/2
       	d=.5+abs(d)/2
       	
       	
       	tc_g = sg*abs(g**6*10000)
    	tc_d = sd*abs(d**6*10000)
    
        
        c_g += (tc_g-c_g)*.5
        c_d += (tc_d-c_d)*.5
        
        print "G%i#C%d#"%(c_g,int(c_g)**2%91) , "D%i#C%d#"%(c_d,int(c_d)**2%91)
        ser.write("G%i#C%d#"%(c_g,int(c_g)**2%91))
        ser.write("D%i#C%d#"%(c_d,int(c_d)**2%91))
     #   sys.stdout.write(ser.read(50))
        ser.flush()
        sleep(.1)
        
      #for i in range(0, j.get_numbuttons()):
        #    if j.get_button(i) != 0:
except KeyboardInterrupt:
    j.quit()

