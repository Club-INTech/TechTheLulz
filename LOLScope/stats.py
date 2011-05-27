from sys import stdout


def filestats(file):
	f = open(file,"r")
	print "file : " + file
	max = 0
	min = 9999999999
	sum = 0
	q=0
	stdout.write("Reading length...")
	qtot = len(f.readlines())
	stdout.write("\rdone           ")
	f.seek(0)
	for number in f.readlines():
		#print q
		if q%1000==0 :
			stdout.write("\rparsing %d/%d : %f%%  "%(q,qtot,(100.0*q)/qtot))
		n = int(number.split(",")[2])
		if(n>max):
			max=n
		if(n<min):
			min=n
		q+=1
		sum+=n
	av=sum/q
	print "\rparsed %d numbers                 "%q
	print "min : %d" % min
	print "max : %d" % max
	print "center : %d" % (min + (max - min) / 2)
	print "average : %d" % av
	print "***********************"
	print ""
	f.close()

filestats("trameCh0-0.csv")
filestats("trameCh1-0.csv")
filestats("trameCh2-0.csv")
filestats("trameCh3-0.csv")

