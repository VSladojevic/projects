import re

def generateBudgets(d,m,ff):
    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]

    def ranacXigraca(minkap, kap, zap, vr, imena, brIgraca, timovi):
        n=len(zap)
        h, w, d = kap+1, n+1, brIgraca+1
        a = [[[0 for z in range(d)] for x in range(w)] for y in range(h)]
        for j in xrange(1, n+1):
            for i in xrange(0,kap+1):
                for z in xrange(1,d):
                    a[i][j][z]=a[i][j-1][z]
                    if(zap[j-1]<=i):
                        if(a[i][j][z]<a[i-zap[j-1]][j-1][z-1]+vr[j-1]):
                            a[i][j][z]=a[i-zap[j-1]][j-1][z-1]+vr[j-1]

        i=n
        b=brIgraca
        rez=[]
        
        for b in xrange(minkap,kap+1):
            if(a[b][n][brIgraca]>0):
                #print b/2.0,"$ DEF",a[b][n][brIgraca],"points"
                rez.append([b/2.0,a[b][n][brIgraca]])

        #while(i>0):
         #   if(a[kap][i][b]!=a[kap][i-1][b]):
          #     print imena[i-1],"("+timovi[i-1]+")",zap[i-1]/2.0,"$",vr[i-1],"points"
           #    kap-=zap[i-1]
            #   b-=1
            #i-=1
            
        #print "PTS:",rez
        return rez


    #ako zatreba
    folder='C:/Users/Vladimir/Desktop/FPL Ranac/'

    pos='GK'
    with open(pos+"s.txt") as f:
        for line in f.readlines():
            matchObj = re.match( r'(.*)\t\xc2\xa3 (.*)\t(.*)\t(.*)', line, re.M|re.I)
            if matchObj:
                player = matchObj.group(1)
                price = matchObj.group(2)
                points = matchObj.group(3)
                team = matchObj.group(4)
                newLine = player+"-"+team+"-"+points+"-"+price
                content.append(newLine)
                doublePrices.append(int(2*float(price)))
                totalPoints.append(int(points))
                teams.append(team)
                names.append(player)
    maxGKBudget=6.0
    minGKBudget=4.0
    rez1=ranacXigraca(int(minGKBudget*2),int(maxGKBudget*2),doublePrices,totalPoints,names,1,teams)


    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]


    pos='DEF'
    with open(pos+"s.txt") as f:
        for line in f.readlines():
            matchObj = re.match( r'(.*)\t\xc2\xa3 (.*)\t(.*)\t(.*)', line, re.M|re.I)
            if matchObj:
                player = matchObj.group(1)
                price = matchObj.group(2)
                points = matchObj.group(3)
                team = matchObj.group(4)
                newLine = player+"-"+team+"-"+points+"-"+price
                content.append(newLine)
                doublePrices.append(int(2*float(price)))
                totalPoints.append(int(points))
                teams.append(team)
                names.append(player)
    maxDEFBudget=7*d
    minDEFBudget=4.0*d
    rez2=ranacXigraca(int(minDEFBudget*2),int(maxDEFBudget*2),doublePrices,totalPoints,names,d,teams)


    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]


    pos='MID'
    with open(pos+"s.txt") as f:
        for line in f.readlines():
            matchObj = re.match( r'(.*)\t\xc2\xa3 (.*)\t(.*)\t(.*)', line, re.M|re.I)
            if matchObj:
                player = matchObj.group(1)
                price = matchObj.group(2)
                points = matchObj.group(3)
                team = matchObj.group(4)
                newLine = player+"-"+team+"-"+points+"-"+price
                content.append(newLine)
                doublePrices.append(int(2*float(price)))
                totalPoints.append(int(points))
                teams.append(team)
                names.append(player)
    maxMIDBudget=12*m
    minMIDBudget=4.5*m
    rez3=ranacXigraca(int(minMIDBudget*2),int(maxMIDBudget*2),doublePrices,totalPoints,names,m,teams)


    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]

    pos='FW'
    with open(pos+"s.txt") as f:
        for line in f.readlines():
            matchObj = re.match( r'(.*)\t\xc2\xa3 (.*)\t(.*)\t(.*)', line, re.M|re.I)
            if matchObj:
                player = matchObj.group(1)
                price = matchObj.group(2)
                points = matchObj.group(3)
                team = matchObj.group(4)
                newLine = player+"-"+team+"-"+points+"-"+price
                content.append(newLine)
                doublePrices.append(int(2*float(price)))
                totalPoints.append(int(points))
                teams.append(team)
                names.append(player)
    maxFWBudget=12.5*ff
    minFWBudget=4.5*ff
    rez4=ranacXigraca(int(minFWBudget*2),int(maxFWBudget*2),doublePrices,totalPoints,names,ff,teams)

    return rez1,rez2,rez3,rez4
