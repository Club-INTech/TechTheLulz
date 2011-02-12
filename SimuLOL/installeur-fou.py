#! /usr/bin/python
from time import sleep
import sys
from os import system
from os import chdir
from os import path


bar_len=40

def write_lol(s):
	sys.stdout.write("\033[1;33m" + s + "\033[0m")
	sys.stdout.flush()

def write_rouge(s):
	sys.stdout.write("\033[1;31m" + s + "\033[0m")
	sys.stdout.flush()

def print_lol(s):
	write_lol(s)
	print ""

def print_rouge(s):
	write_rouge(s)
	print ""

def bar(ratio,msg=""):
	n1=int(round(ratio*bar_len))
	n2=bar_len-n1
	pct=(int(round(ratio*100)))
	msgstr=""
	if msg!="":
		msgstr="[" + msg + "]"
	sys.stdout.write("\r[" + "\033[34m" + "|" * n1 + "\033[0m" + " " * n2 + "]" + "[%d%%]"%pct + msgstr)
	sys.stdout.flush()

def question(q,default=True):
	while True:
		s=q
		if(default==True):
			s+="[O/n]"
		else:
			s+="[o/N]"
		s+=" "
		write_lol(s)
		rep = raw_input()
		if rep=="":
			return default
		if rep=="o" or rep=="O":
			return True
		if rep=="n" or rep=="N":
			return False

def run(command, arreter_si_echec=True):
	print_rouge(command)
	sleep(1)
	if system(command) != 0:
		if(arreter_si_echec):
			if not question("Il semble qu'il y ait eu une erreur... Continuer?",False):
				print_rouge("Abandon.")
				sys.exit()

def cd(path):
	print_rouge("cd "+path)
	sleep(1)
	chdir(path)

def attente_inutile(duree_tres_approximative):
	for n in range(0,101):
		bar(.01*n)
		sleep(.01*duree_tres_approximative)
	print ""



print_lol("Ceci est un script de preparation-compilation du projet SimuLOL. Prevu pour fonctionner sous ubuntu 10.10. (non-teste en 32-bit)")
print_lol("Test de la barre qui sert a rien...")

attente_inutile(3)

print_lol("OK")
question("Voulez-vous lancer le script de construction en mode lol?")
print_lol("On s'en tape.")



#installe les deps
paquets = "subversion cmake cmake-curses-gui libglut3-dev build-essential libxxf86vm-dev"
if not question("Le script va d'abord installer les paquets necessaires. Pour cela vous allez devoir donner votre mot de passe sudo. Cependant si vous avez deja installe ces paquets vous pouvez passer cette etape. Les paquets a installer sont :\n%s\nVoulez-vous sauter cette etape?"%paquets):
	print_lol("Installation des paquets necessaires... Merci de donner votre mdp sudo:")
	run("sudo apt-get install "+paquets)

#download et build bullet
print_lol("Telechargement de Bullet (lib de simulation physique)...")
run("svn checkout http://bullet.googlecode.com/svn/trunk/ bullet-read-only")
print_lol("Construction de Bullet:")
cd("bullet-read-only")
run("mkdir build",False)
cd("build")
run("cmake ..")
run("make")
cd("../..")

#irrlicht:
if(path.exists("irrlicht-1.7.2.zip")):
	print_lol("L'archive irrlicht existe deja. Elle ne sera pas re-telechargee")
else:
	print_lol("Telechargement de Irrlicht (Moteur 3D)...")
	run("wget http://downloads.sourceforge.net/irrlicht/irrlicht-1.7.2.zip")

if(path.exists("irrlicht-1.7.2")):
	print_lol("l'archive irrlicht est deja extraite...")
else:
	run("unzip irrlicht-1.7.2.zip")
	
print_lol("Construction de Irrlicht:")
cd("irrlicht-1.7.2/source/Irrlicht")
run("make")
cd("../../..")

#build irrBP
print_lol("construction de irrBP (pontage Bullet-Irrlicht)")
cd("irrBP")
run("mkdir build",False)
cd("build")
run("cmake ..")
run("make")
cd("../..")

#build SimuLOL
print_lol("construction de SimuLOL")
run("mkdir build",False)
cd("build")
run("cmake ..")
run("make")
cd("..")

#Ronald final
system("firefox http://www.youtube.com/watch?v=yRgzLAmbuws &")
print_lol("Lancement de Ronald Insanity 2...")
attente_inutile(10)

#lancement du programme
run("./SimuLOL-preprepreprealpha")
