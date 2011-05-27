WINDOW = 9


def min(a, b):
	if(a<b):
		return a
	else:
		return b

def max(a, b):
	if(a>b):
		return a
	else:
		return b

def lissage(tab):
	global WINDOW
	ret = []
	first = 0
	last = len(tab)
	for i in range(first, last):
		if i == 0:
			ret.append(-3.141592654)
			continue
		if i == last:
			ret.append(3.141592654)
			continue
		sum=0.0
		num=0
		for k in range(max(i-WINDOW,first),min(i+WINDOW+1,last)):
			try:
				sum+=float(tab[k])
				num+=1
			except:
				continue

		av=sum/num
		if(av>3.14159265359):
				av=3.14159265359
		ret.append(av)
		
	
	return ret

def monotoniser(tab):
	ret = []
	first = 0
	last = len(tab)
	curmax=-4.0
	for i in range(first, last):
		try:
			#print "tab[%d] : %s" % (i,tab[i])
			curmax=max(curmax,float(tab[i]))
			if(curmax>3.14159265359):
				curmax=3.14159265359
			ret.append(curmax)
		except:
			continue
	return ret
		
			

#confirm = periode_raw
#lol1 = periode_lisse

infile = open("in-raw","r")
filestr = infile.read()
infile.close()
periode_raw_str=filestr.replace(",",".")
periode_raw=periode_raw_str.split("\n")

periode_lisse=lissage(periode_raw)
periode_mono=monotoniser(periode_lisse)
periode_lisse=lissage(periode_mono)
periode_mono=monotoniser(periode_lisse)
periode_lisse=periode_mono

confirm = periode_raw

PI = 3.14159265359

periode_s=periode_lisse
periode=[]
for i in range(0,len(periode_s)):
    periode.append(float(periode_s[i]))
    #print(periode[i])


def antecedent(per,y):
    #print(range(len(per)-1,-1,-1))
    for i in range(len(per)-1,-1,-1):
    #    print(i)
        if per[i]<y:
            J=0
            for j in range(i+1, len(per)):
                J=j
                if per[i]!=per[j]:
                    break
            #(y1-y2)/(x1-x2)
            A=(per[i]-per[J])/(i-J)
            x=(y-per[i])/A+i
            return x


calibr=[]
outfile = open("out_c","w")

for i in range(0,1000):
    y=(i/1000.0-.5)*2*PI
    #print(y," : ",antecedent(periode, y)/1001.0*2*PI-PI)
    calibr.append(antecedent(periode, y)/1001.0*2*PI-PI)
    #AAAAAAAAAAA
    outfile.write("%ff,\n"%calibr[i])
outfile.close()
#print("************************** REALIGN")

def realign(calibr,x):
    global PI
    i=(len(calibr)-1)*(x+PI)/(2*PI)
    y=calibr[int(i)]+(calibr[int(i)+1]-calibr[int(i)])*(i-int(i))
    return y

periode=[]
for i in range(0,len(confirm)):
	try:
		periode.append(float(confirm[i]))
	except:
		continue


print "raw,monotone,lisse,reciproque,verif"
for i in range(0,1000):
    x=periode[i]
    verif = realign(calibr,x)
    
    print "%s,%f,%f,%f,%f"%(periode_raw[i],periode_mono[i],periode_lisse[i],periode[i],verif)
    
    
    
    
    
    
    

