from bs4 import BeautifulSoup
import requests
import json
from colorama import Fore, Style


playerIdToName={1: 'Cech', 2: 'Leno', 3: 'Koscielny', 4: 'Bellerín', 5: 'Monreal', 6: 'Holding', 7: 'Mustafi', 8: 'Kolasinac', 10: 'Mavropanos', 11: 'Lichtsteiner', 12: 'Sokratis', 13: 'Özil', 14: 'Ramsey', 15: 'Iwobi', 16: 'Elneny', 17: 'Xhaka', 18: 'Mkhitaryan', 19: 'Nelson', 20: 'Maitland-Niles', 21: 'Welbeck', 22: 'Lacazette', 23: 'Aubameyang', 450: 'Torreira', 451: 'Guendouzi', 542: 'Martínez', 545: 'Smith-Rowe', 546: 'Medley', 552: 'Nketiah', 563: 'Saka', 566: 'Willock', 24: 'Begovic', 25:
'Boruc', 26: 'Francis', 27: 'Steve Cook', 28: 'Daniels', 29: 'Adam Smith', 30: 'Mings', 31: 'Aké', 32: 'Simpson', 34: 'Pugh', 35: 'Surman', 36: 'Stanislas', 37: 'Gosling', 38: 'Ibe', 39: 'Lewis Cook', 40: 'Fraser', 41: 'Hyndman', 42: 'Brooks', 43: 'Wilson', 44: 'Mousset', 45: 'King', 46: 'Defoe', 473: 'Rico', 494: 'Lerma', 47: 'Ryan', 48: 'Dunk', 49: 'Duffy', 50: 'Bruno', 51: 'Bong', 52: 'Suttner', 53: 'Schelotto', 54: 'Balogun', 55: 'Knockaert', 56: 'Stephens', 57: 'Kayal', 58: 'March', 59: 'Groß', 60: 'Pröpper', 61: 'Izquierdo', 62: 'Murray', 63: 'Hemed', 64: 'Baldock', 65: 'Locadia', 66: 'Andone', 179: 'Button', 439: 'Steele', 440: 'Bernardo', 466: 'Bissouma', 476: 'Jahanbakhsh', 503: 'Montoya', 551: 'Gyokeres', 559: 'White', 67: 'Heaton', 68: 'Pope', 69: 'Lowton', 70: 'Mee', 71: 'Ward', 72: 'Tarkowski', 73: 'Long', 74: 'Taylor', 75: 'Bardsley', 76: 'Gudmundsson', 77: 'Defour', 78: 'Hendrick', 79: 'Brady', 80: 'Westwood', 81: 'Walters', 82: 'Cork', 83: 'Lennon', 84: 'Vokes', 85: 'Barnes', 86: 'Wells', 87: 'Wood', 485: 'Lindegaard', 490: 'Hart', 491: 'Gibson', 495: 'Vydra', 534: 'McNeil', 564: 'Koiki', 33: 'Arter', 88: 'Etheridge', 89: 'Smithies', 90: 'Morrison', 91: 'Bamba', 92: 'Ecuele Manga', 93: 'Connolly', 94: 'Bennett', 95: 'Peltier', 96: 'Jazz Richards', 97: 'Halford', 98: 'Ralls', 99: 'Hoilett', 100: 'Mendez-Laing', 101: 'Paterson', 102: 'Gunnarsson', 103: 'Pilkington', 104: 'Damour', 105: 'Ward', 106: 'Murphy', 107: 'Zohore', 108: 'Bogle', 109: 'Madine', 110: 'Reid', 467: 'Cunningham', 504: 'Camarasa', 535: 'Harris', 537: 'Brian Murphy', 111: 'Courtois', 112: 'Caballero', 113: 'Azpilicueta', 114: 'Cahill', 115: 'Alonso', 116: 'David Luiz', 117: 'Moses', 118: 'Rüdiger', 119: 'Christensen', 120: 'Zappacosta', 121: 'Emerson', 122: 'Hazard', 123: 'Fàbregas', 124: 'Willian', 125: 'Pedro', 126: 'Kanté', 127: 'Barkley', 128: 'Drinkwater', 129: 'Bakayoko', 130: 'Ampadu', 131: 'Hudson-Odoi', 132: 'Loftus-Cheek', 133: 'Giroud', 134: 'Morata', 459: 'Jorginho', 496: 'Abraham', 498: 'Kepa', 499: 'Kovacic', 135: 'Hennessey', 136: 'Speroni', 137: 'Guaita', 138: 'Tomkins', 139: 'Dann', 140: 'Kelly', 141: 'Schlupp', 142: 'van Aanholt', 143: 'Sakho', 144: 'Riedewald', 145: 'Wan-Bissaka', 146: 'Puncheon', 147: 'McArthur', 148: 'Sako', 149: 'Townsend', 150: 'Milivojevic', 151: 'Zaha', 152: 'Benteke', 153: 'Sørloth', 412: 'Kouyaté', 441: 'Ward', 442: 'Jach', 443: 'Souaré', 444: 'Wickham', 477: 'Kaikai', 486: 'Meyer', 505: 'Ayew', 154: 'Pickford', 155: 'Stekelenburg', 156: 'Baines', 157: 'Coleman', 158: 'Jagielka', 159: 'Williams', 160: 'Holgate', 161: 'Kenny', 162: 'Keane', 163: 'Martina', 164: 'Walcott', 165: 'Bolasie', 166: 'McCarthy', 167: 'Schneiderlin', 168: 'Gueye', 169: 'Davies', 170: 'Lookman', 171: 'Klaassen', 172: 'Sigurdsson', 173: 'Vlasic', 174: 'Baningime', 175: 'Calvert-Lewin', 176: 'Niasse', 177: 'Tosun', 393: 'Richarlison', 469: 'Mirallas', 470: 'Sandro', 484: 'Digne', 506: 'Mina', 507: 'Bernard', 508: 'André Gomes', 526: 'Zouma', 528: 'Dowell', 532: 'Browning', 9: 'Chambers', 178: 'Bettinelli', 180: 'Ream', 181: 'Odoi', 182: 'Cairney', 183: 'McDonald', 184: 'Sessegnon', 185: 'Johansen', 186: 'Ayité', 187: 'Kebano', 188: 'Fonte', 189: 'Kamara', 455: 'Le Marchand', 456: 'Seri', 460: 'Christie', 474: 'Fabri', 478: 'Schürrle', 480: 'Mitrovic', 487: 'Mawson', 509: 'Rico', 510: 'Bryan', 511: 'Fosu-Mensah', 512: 'Zambo Anguissa', 513: 'Vietto', 521: 'Cissé', 541: 'Steven Sessegnon', 190: 'Hamer', 191: 'Lössl', 192: 'Schindler', 193: 'Löwe', 194: 'Smith', 195: 'Zanka', 196: 'Malone', 197: 'Hadergjonaj', 198: 'Kongolo', 199: 'Mooy', 200: 'van La Parra', 201: 'Hogg', 202: 'Billing', 203: 'Ince', 204: 'Williams', 205: 'Sabiri', 206: 'Pritchard', 207: 'Sobhi', 208: 'Bacuna', 209: 'Quaner', 210: 'Depoitre', 211: 'Mounie', 212: 'Kachunga', 461: 'Durm', 471: 'Diakhaby', 514: 'Mbenza', 522: 'Stankovic', 533: 'Schofield', 213: 'Schmeichel', 214: 'Jakupovic', 215: 'Morgan', 216: 'Fuchs', 217: 'Simpson', 218: 'Benalouane', 219: 'Chilwell', 220: 'Maguire', 221: 'Pereira', 222: 'Evans', 224: 'Albrighton', 225: 'Gray', 226: 'Amartey', 227: 'Ndidi', 228: 'Iborra', 229: 'James', 230: 'Choudhury', 231: 'Silva', 232: 'Diabaté', 233: 'Maddison', 234: 'Vardy', 235: 'Okazaki', 236: 'Iheanacho', 457: 'Ward', 492: 'Ghezzal', 501: 'Benkovic', 515: 'Söyüncü', 527: 'Mendy', 558: 'Knight', 237: 'Mignolet', 238: 'Karius', 239: 'Lovren', 240: 'Moreno', 241: 'Clyne', 242: 'Gomez', 243: 'Matip', 244: 'Klavan', 245: 'Alexander-Arnold', 246: 'van Dijk', 247: 'Robertson', 248: 'Chamberlain', 249: 'Henderson', 250: 'Lallana', 251: 'Mané', 252: 'Wijnaldum', 253: 'Salah', 254: 'Milner', 255: 'Fabinho', 256: 'Keita', 257: 'Firmino', 259: 'Solanke',
462: 'Shaqiri', 468: 'Alisson', 472: 'Sturridge', 543: 'Origi', 557: 'Camacho', 223: 'Mahrez', 260: 'Ederson', 261: 'Bravo', 262: 'Stones', 263: 'Kompany', 264: 'Otamendi', 265: 'Walker', 266: 'Danilo', 267: 'Mendy', 268: 'Laporte', 269: 'Delph', 270: 'Sterling', 271: 'David Silva', 272: 'Fernandinho', 273: 'De Bruyne', 274: 'Gündogan', 275: 'Sané', 276: 'Bernardo Silva', 277: 'Foden', 278: 'Diaz', 279: 'Zinchenko', 280: 'Agüero', 281: 'Jesus', 529: 'Muric', 282: 'De Gea', 283: 'Romero', 284: 'Smalling', 285: 'Jones', 286: 'Shaw', 287: 'Rojo', 288: 'Valencia', 289: 'Darmian', 290: 'Blind', 291: 'Bailly', 292: 'Lindelöf', 293: 'Young', 294:
'Dalot', 295: 'Sánchez', 296: 'Matic', 297: 'Mata', 298: 'Fellaini', 299: 'Herrera', 300: 'Lingard', 301: 'Martial', 302: 'Pogba', 303: 'McTominay', 304: 'Fred',
305: 'Rashford', 306: 'Lukaku', 481: 'Pereira', 520: 'Grant', 568: 'Angel Gomes', 307: 'Elliot', 308: 'Darlow', 309: 'Dubravka', 310: 'Yedlin', 311: 'Clark', 312: 'Lascelles', 313: 'Dummett', 314: 'Lejeune', 315: 'Manquillo', 316: 'Mbemba', 317: 'Ritchie', 318: 'Shelvey', 319: 'Diamé', 320: 'Atsu', 321: 'Murphy', 322: 'Merino', 323: 'Hayden', 324: 'Ki Sung-yueng', 325: 'Gayle', 326: 'Pérez', 327: 'Joselu', 458: 'Kenedy', 482: 'Schär', 488: 'Muto', 493: 'Rondón', 516: 'Fernández', 530: 'Sterry', 531: 'Longstaff', 553: 'Woodman', 258: 'Ings', 328: 'McCarthy', 329: 'Forster', 330: 'Bertrand', 331: 'Yoshida', 332: 'Cédric', 333: 'Stephens', 334: 'McQueen', 335: 'Bednarek', 336: 'Hoedt', 337: 'Davis', 338: 'Ward-Prowse', 339: 'Oriol Romeu', 340: 'Højbjerg', 341: 'Boufal', 342: 'Sims', 343: 'Redmond', 344: 'Lemina', 345: 'Armstrong', 346: 'Elyounoussi', 347: 'Long', 348: 'Austin', 349: 'Gabbiadini', 350: 'Carrillo', 452: 'Gunn', 463: 'Vestergaard', 479: 'Targett', 550: 'Obafemi', 556: 'Valery', 562: 'Johnson', 565: 'Ramsay', 351: 'Lloris', 352: 'Vorm', 353: 'Alderweireld', 354: 'Rose', 355: 'Vertonghen', 356: 'Davies', 357: 'Trippier', 358: 'Walker-Peters', 359: 'Sánchez', 360: 'Aurier', 361: 'Dier', 362: 'Dembélé', 363: 'Lamela', 364: 'Eriksen', 365: 'Alli', 366: 'Wanyama', 367: 'Son', 368: 'Winks', 369: 'Sissoko', 370: 'Lucas Moura', 371: 'Llorente', 372: 'Kane', 497: 'Nkoudou', 523: 'Amos', 536: 'Gazzaniga', 538: 'Whiteman', 540: 'Skipp', 549: 'Foyth', 561: 'Eyoma', 373: 'Gomes', 374: 'Dahlberg', 375: 'Kaboul', 376: 'Cathcart', 377: 'Prödl', 378: 'Holebas', 379: 'Britos', 380: 'Kabasele', 381: 'Janmaat', 382: 'Mariappa', 383: 'Kiko Femenía', 384: 'Zeegelaar', 385: 'Navarro', 386: 'Masina', 387: 'Chalobah', 388: 'Cleverley', 389: 'Capoue', 390: 'Doucouré', 391: 'Pereyra', 392: 'Hughes', 394: 'Deulofeu', 395: 'Gray', 396: 'Deeney', 397: 'Okaka', 398: 'Sinclair', 445: 'Foster', 446: 'Sema', 517: 'Quina', 524: 'Success', 544: 'Wilmot', 554: 'Peñaranda', 399: 'Adrián', 400: 'Fabianski', 401: 'Reid', 402: 'Cresswell', 403: 'Ogbonna', 404: 'Byram', 405: 'Zabaleta', 406: 'Rice', 407: 'Oxford', 408: 'Fredericks', 409: 'Diop', 410: 'Masuaku', 411: 'Noble', 413: 'Obiang', 414: 'Lanzini', 415: 'Antonio', 416: 'Fernandes', 417: 'Arnautovic', 418: 'Carroll', 419: 'Chicharito', 420: 'Hugill', 447: 'Wilshere', 453: 'Yarmolenko', 464: 'Balbuena', 465: 'Felipe Anderson', 489: 'Snodgrass', 502: 'Lucas', 518:
'Carlos Sánchez', 539: 'Diangana', 547: 'Powell', 548: 'Coventry', 560: 'Xande Silva', 421: 'Ruddy', 422: 'Norris', 423: 'Boly', 424: 'Douglas', 425: 'Doherty', 426: 'Saïss', 427: 'Bennett', 428: 'Batth', 429: 'Miranda', 430: 'Coady', 431: 'Vinagre', 432: 'Jota', 433: 'Neves', 434: 'Cavaleiro', 435: 'Costa', 436: 'Enobakhare', 437: 'Jiménez', 438: 'Bonatini', 448: 'Gibbs-White', 449: 'Mir', 454: 'Patrício', 475: 'Moutinho', 483: 'Jonny', 500: 'Traoré', 519: 'Dendoncker', 525: 'Hause', 555: 'Giles', 567: 'Kilman'}


id1=input('Insert your id:\n')
id2=input('Insert id of your nemesis:\n')
user1=set()
user2=set()

u1 = 'https://fantasy.premierleague.com/drf/entry/'+id1
source = requests.get(u1).text
soup = BeautifulSoup(source,'lxml') 
jsonText = soup.find('p').getText()
data=json.loads(jsonText)
data = data['entry']
name1 = data['player_first_name'] + ' ' + data['player_last_name']


u2 = 'https://fantasy.premierleague.com/drf/entry/'+id2
source = requests.get(u2).text
soup = BeautifulSoup(source,'lxml') 
jsonText = soup.find('p').getText()
data=json.loads(jsonText)
data = data['entry']
name2 = data['player_first_name'] + ' ' + data['player_last_name']


url = 'https://fantasy.premierleague.com/drf/entry/' + id1 + '/event/' + str(19) + '/picks'
source = requests.get(url).text
soup = BeautifulSoup(source,'lxml') 
jsonText = soup.find('p').getText()
    
data = json.loads(jsonText)
data = data['picks']
for player in data:
    playerId = player['element']
    user1.add(playerId)

url = 'https://fantasy.premierleague.com/drf/entry/' + id2 + '/event/' + str(19) + '/picks'
source = requests.get(url).text
soup = BeautifulSoup(source,'lxml') 
jsonText = soup.find('p').getText()
    
data = json.loads(jsonText)
data = data['picks']
for player in data:
    playerId = player['element']
    user2.add(playerId)


p1Has = list(user1.difference(user2))
p2Has = list(user2.difference(user1))


line = '{:>12}  {:>12}  {:>12}'.format('You have:', '\t\t\t\t', 'Nemesis has:')
print(line)   

line = '{:>12}  {:>12}  {:>12}'.format('('+name1+')', '\t\t\t\t', '('+name2+')')
print(line)
print('_________________________________________________________________')
for i in range(len(p1Has)):
    line_new = '{:>12}  {:>12}  {:>12}'.format(playerIdToName[p1Has[i]], '\t\t\t\t', playerIdToName[p2Has[i]])
    print(line_new)   