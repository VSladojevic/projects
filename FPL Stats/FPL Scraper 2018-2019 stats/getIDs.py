from bs4 import BeautifulSoup
import requests
import json

file1kIds = open('PostGW17top1kIds.txt', 'a', encoding='utf8')

for i in range(20): 
    url = 'https://fantasy.premierleague.com/drf/leagues-classic-standings/313?phase=1&le-page=1&ls-page='+str(i+1)
    source = requests.get(url).text
    soup = BeautifulSoup(source,'lxml') 
    jsonText = soup.find('p').getText()
        
    data = json.loads(jsonText)
    data = data['standings']['results']
    for player in data:
        print(player['entry'], file=file1kIds) 
        # treba entry, a ne id, sta je id?
        
    print('Finished page ' + str(i+1) + ', players ' + str(i*50+1) + '-' + str((i+1)*50))

""" file1k = open('top1k.txt', 'a', encoding='utf8')


for i in range(20):
    url = 'https://fantasy.premierleague.com/drf/leagues-classic-standings/313?phase=1&le-page=1&ls-page='+str(i+1)
    source = requests.get(url).text
    soup = BeautifulSoup(source,'lxml') 
    print(soup.find('p').getText(), file=file1k)
    print('Page ' + str(i+1) + ', players ' + str(i*50+1) + '-' + str((i+1)*50))
 """