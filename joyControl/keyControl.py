#!/usr/bin/env python
import serial
from time import sleep
k=0
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
import termios, sys, os
TERMIOS = termios
def getkey():
        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)
        new = termios.tcgetattr(fd)
        new[3] = new[3] & ~TERMIOS.ICANON & ~TERMIOS.ECHO
        new[6][TERMIOS.VMIN] = 1
        new[6][TERMIOS.VTIME] = 0
        termios.tcsetattr(fd, TERMIOS.TCSANOW, new)
        c = None
        try:
                c = os.read(fd, 1)
        finally:
                termios.tcsetattr(fd, TERMIOS.TCSAFLUSH, old)
        return c
import pygame
#from pygame import joystick, event
c_g=0
c_d=0
pygame.init()
while True:
	events = pygame.event.get( )
#        for e in events:
#            if( e.type == QUIT ):
#                break
#            elif (e.type == KEYDOWN):
#                if( e.key == K_ESCAPE ):
#                    break
#                if( e.key == K_UP ):
#                    c_d+=1
#                if( e.key == K_DOWN ):
#                    c_g-=1
#                    c_d-=1
	c = getkey()
	if c=="a":
		c_g+=1
		c_d+=1
	if c=="q":
		c_g-=1
		c_d-=1
		
	if c=="z":
		c_g+=10
		c_d+=10
	if c=="s":
		c_g-=10
		c_d-=10
	
	if c=="e":
		c_g+=100
		c_d+=100
	if c=="d":
		c_g-=100
		c_d-=100
		
	if c=="w":
		c_g=0
		c_d=0
	if c==" ":
		break
		               
        print c_g,c_d
        ser.write("G%i#C%d#"%(c_g,int(c_g)**2%91))
        ser.write("D%i#C%d#"%(c_d,int(c_d)**2%91))
      	ser.flush()

