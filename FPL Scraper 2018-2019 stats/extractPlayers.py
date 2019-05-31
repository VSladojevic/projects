# 24 minuta za 5k igraca
from bs4 import BeautifulSoup
import requests
import json
import time

start_time = time.time()


playerOccurrences = {}

gameweek = 14

f = open("PostGW14top1kIds.txt", "r")

idsFinished = 0

for userId in f:
    userId=userId[:-1]
    url = 'https://fantasy.premierleague.com/drf/entry/' + userId + '/event/' + str(gameweek) + '/picks'
    source = requests.get(url).text
    soup = BeautifulSoup(source,'lxml') 
    jsonText = soup.find('p').getText()
        
    data = json.loads(jsonText)
    data = data['picks']
    for player in data:
        playerId = player['element']
        if playerId not in playerOccurrences:
            playerOccurrences[playerId]=1  
        else:
            playerOccurrences[playerId]+=1


    idsFinished += 1
    if (idsFinished % 50 == 0):
        print('Finished ' + str(idsFinished) + ' players')

print(playerOccurrences)
print("Runtime --- %s seconds ---" % (time.time() - start_time))
    