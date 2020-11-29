# 24 minuta za 5k igraca
from bs4 import BeautifulSoup
import requests
import json


f = open("top1kIds.txt", "r")
fOut = open("cap.txt", "a")

idsFinished = 0

for userId in f:
    userId=userId[:-1]
    tripleCaptain = False
    info = []
    for gameweek in (2,5):
                
        url = 'https://fantasy.premierleague.com/drf/entry/' + userId + '/event/' + str(gameweek) + '/picks'
        source = requests.get(url).text
        soup = BeautifulSoup(source,'lxml') 
        jsonText = soup.find('p').getText()
        
        data = json.loads(jsonText)
        data = data['picks']
        for player in data:
                captain = player['is_captain']
                if(captain == True):
                        if(player['multiplier']==3):
                                tripleCaptain = gameweek 
                        info.append(player['element'])
                        break
        
    idsFinished += 1
    if (idsFinished % 50 == 0):
            print('Finished ' + str(idsFinished) + ' players')

    info.append(tripleCaptain)
    print(info, file=fOut)

        