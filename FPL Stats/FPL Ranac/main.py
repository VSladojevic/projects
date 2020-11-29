from budgets import generateBudgets
from prepare import pointsForBudget
from operator import itemgetter

def findBest(formation):
    a=formation.split("-")

    defs=int(a[0])
    mids=int(a[1])
    fws=int(a[2])


    bench=4.0+4.0*(5-defs)+4.5*(5-mids)+4.5*(3-fws)
    a,b,c,d=generateBudgets(defs,mids,fws)

    max=0
    amax=0
    bmax=0
    cmax=0
    dmax=0

    for ia in xrange(len(a)):
        for ib in xrange(len(b)):
            for ic in xrange(len(c)):
                for id in xrange(len(d)):
                    if(a[ia][0]+b[ib][0]+c[ic][0]+d[id][0]<=100-bench):
                        pts=a[ia][1]+b[ib][1]+c[ic][1]+d[id][1]
                        if(pts>max):
                            max=pts
                            amax=ia
                            bmax=ib
                            cmax=ic
                            dmax=id
                            
    print("------------------------------")
    print "Formation",defs,mids,fws
    print "Maximum Points",max
    print "GK budget",a[amax][0],u"\N{POUND SIGN}",a[amax][1],"pts"
    print "DEF budget",b[bmax][0],u"\N{POUND SIGN}",b[bmax][1],"pts"
    print "MID budget",c[cmax][0],u"\N{POUND SIGN}",c[cmax][1],"pts"
    print "FW budget",d[dmax][0],u"\N{POUND SIGN}",d[dmax][1],"pts"
    
    return pointsForBudget(a[amax][0],b[bmax][0],c[cmax][0],d[dmax][0],defs,mids,fws)
    


#main
validFormations=["3-4-3","3-5-2","4-3-3","4-4-2","4-5-1","5-2-3","5-3-2","5-4-1"]
formationPoints=[]
for formation in validFormations:
    pts=findBest(formation)
    formationPoints.append((formation,pts))

print "\n\n\n"
print "Valid formations - points"
for t in sorted(formationPoints,key=itemgetter(1),reverse=True):
    print t[0],"-",t[1],"points"


