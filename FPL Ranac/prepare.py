import re

def pointsForBudget(gkBudget,defBudget,midBudget,fwBudget,defs,mids,fws):

    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]

    def ranacXigraca(kap, zap, vr, imena, brIgraca, timovi):
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

        #for b in xrange(0,kap+1):
        #    print b/2.0,"$ DEF",a[b][3][1],"points"

        i=n
        b=brIgraca
        rez=a[kap][n][b]
        print "*******************"
        while(i>0):
            if(a[kap][i][b]!=a[kap][i-1][b]):
               print imena[i-1],"("+timovi[i-1]+")",zap[i-1]/2.0,u"\N{POUND SIGN}",vr[i-1],"points"
               kap-=zap[i-1]
               b-=1
            i-=1
            
        #print "PTS:",rez
        return rez


    #ako zatreba
    folder='C:/Users/Vladimir/Desktop/FPL Ranac/'

    #brisi prethodni sadrzaj
    pos='GK'
    #open(pos+'sReady.txt', 'w').close()

    #outFile = open(pos+'sReady.txt', 'a')

    #print gkBudget,"$ GK"
    with open(pos+"s.txt") as f:
        for line in f.readlines():
            matchObj = re.match( r'(.*)\t\xc2\xa3 (.*)\t(.*)\t(.*)', line, re.M|re.I)
            if matchObj:
                player = matchObj.group(1)
                price = matchObj.group(2)
                points = matchObj.group(3)
                team = matchObj.group(4)
                newLine = player+"-"+team+"-"+points+"-"+price
                #outFile.write(newLine+"\n")
                content.append(newLine)
                doublePrices.append(int(2*float(price)))
                totalPoints.append(int(points))
                teams.append(team)
                names.append(player)
    #outFile.close()

    #ranacKonacno(60,doublePrices,totalPoints,names)
    gkP=ranacXigraca(int(gkBudget*2),doublePrices,totalPoints,names,1,teams)
            
    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]
    #print "***********************"

    #print defBudget,"$ DEF"
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

    defP=ranacXigraca(int(defBudget*2),doublePrices,totalPoints,names,defs,teams)

    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]
    #print "***********************"

    #print midBudget,"$ MID"
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

    midP=ranacXigraca(int(midBudget*2),doublePrices,totalPoints,names,mids,teams)

    content=[]
    doublePrices=[]
    totalPoints=[]
    names=[]
    teams=[]
    #print "***********************"

    #print fwBudget,"$ FW"
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

    fwP=ranacXigraca(int(fwBudget*2),doublePrices,totalPoints,names,fws,teams)
    #print "***********************"

    #print "Total points",gkP+defP+midP+fwP
    return gkP+defP+midP+fwP
