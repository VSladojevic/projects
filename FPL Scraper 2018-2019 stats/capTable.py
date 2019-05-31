from bs4 import BeautifulSoup
import requests
import json
import ast

f = open("cap.txt", "r")

url = 'https://fantasy.premierleague.com/drf/bootstrap-static'    

playerIdToName = {}

source = requests.get(url).text
soup = BeautifulSoup(source,'lxml') 
jsonText = soup.find('p').getText()
    
data = json.loads(jsonText)
data = data['elements']

for playerInfo in data:
    playerIdToName[playerInfo['id']] = playerInfo['web_name']


gw2 = {}
gw5 = {}
tc = {}
for line in f:
    line = line[:-1]
    [a,b,c] = ast.literal_eval(line)
    if playerIdToName[a] in gw2:
        gw2[playerIdToName[a]]+=1
    else:
        gw2[playerIdToName[a]]=1

    if playerIdToName[b] in gw5:
        gw5[playerIdToName[b]]+=1
    else:
        gw5[playerIdToName[b]]=1

    if c in tc:
        tc[c]+=1
    else:
        tc[c]=1

print(gw2)
print(gw5)
print(tc)
