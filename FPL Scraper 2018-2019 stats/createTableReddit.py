from bs4 import BeautifulSoup
import requests
import json

""" top1k = {498: 37, 145: 901, 115: 816, 357: 669, 427: 299, 122: 995, 233: 607, 393: 531, 22: 215, 43: 400, 280: 767, 454: 465, 264: 2, 13: 25, 105: 67, 260: 285, 247: 468, 425: 554, 40: 473, 367: 39, 372: 266, 151: 149, 190: 306, 102: 4, 20: 2, 142: 30, 432: 7, 370: 154, 270: 72, 453: 5, 45: 80, 136: 45, 245: 255, 480: 407, 267: 82, 253: 318, 234: 106, 155: 54, 80: 21, 182: 24, 47: 48, 483: 40, 433: 105, 131: 32, 275: 15, 437: 136, 179: 40, 56: 65, 32: 4, 423: 23, 276: 54, 251: 392, 445: 112, 380: 3, 135: 177, 258: 116, 202: 81, 72: 23, 378: 63, 76: 61, 468: 39, 413: 10, 284: 3, 473: 11, 55: 39, 138: 20, 392: 54, 478: 31, 125: 15, 23: 23, 435: 5, 141: 7, 286: 66, 164: 35, 89: 2, 417: 144, 518: 1, 113: 7, 396: 12, 451: 21, 257: 22, 400: 99, 14: 30, 74: 4, 25: 28, 189: 69, 271: 83, 221: 89, 490: 111, 95: 61, 140: 16, 310: 13, 340: 180, 439: 9, 268: 58, 116: 22, 459: 19, 269: 3, 183: 6, 215: 1, 363: 10, 274: 12, 16: 7, 391: 28, 24: 45, 332: 14, 282: 7, 4: 46, 306: 14, 410: 30, 302: 27, 83: 12, 484: 4, 494: 1, 387: 8, 265: 77, 351: 8, 499: 2, 5: 31, 27: 22, 178: 7, 504: 7, 338: 1, 334: 1, 62: 19, 456: 3, 29: 5, 88:
8, 52: 10, 327: 10, 154: 11, 242: 4, 246: 7, 220: 15, 328: 31, 500: 10, 147: 3, 167: 13, 335: 29, 236: 5, 254: 14, 524: 9, 213: 12, 57: 2, 354: 6, 172: 26, 347: 3, 405: 2, 94: 4, 26: 1, 309: 2, 395: 4, 537: 2, 304: 1, 330: 7, 513: 2, 458: 11, 447: 5, 49: 19, 109: 5, 42: 5, 126: 23, 28: 8, 339: 3, 219: 18, 300: 1, 124:
4, 475: 5, 199: 1, 312: 3, 93: 3, 98: 2, 31: 2, 252: 2, 527: 3, 184: 2, 422: 4, 103: 1, 133: 2, 355: 1, 465: 5, 277: 1, 509: 1, 191: 2, 209: 10, 226: 2, 522: 7, 262: 1, 53: 3, 177: 3, 352: 1, 91: 1, 38: 1, 48: 2, 58: 2, 313: 2, 292: 1, 96: 3, 7: 2, 186: 1, 137: 1, 295: 1, 70: 3, 365: 4, 225: 2, 376: 2, 146: 2, 272: 1,
450: 1, 157: 1, 430: 1, 218: 3, 298: 2, 291: 1, 175: 1, 174: 1, 15: 2, 303: 2, 162: 2, 517: 1, 356: 1, 359: 1, 409: 1, 516: 1, 143: 1, 353: 3, 168: 1, 2: 1, 153: 1, 34: 2, 428: 1, 346: 1, 35: 1, 288: 1, 344: 1, 383: 1, 464: 1, 195: 1}
 """
top1k = {154: 30, 247: 744, 425: 626, 245: 393, 378: 14, 122: 631, 393: 691, 270: 628, 370: 10, 475: 2, 493: 19, 24: 30, 280: 720, 484: 160, 43: 503, 47: 139, 115: 837, 145: 752, 253: 567, 101: 37, 417: 723, 179: 76, 427: 243, 56: 49, 260: 282, 49: 73, 301: 225, 40: 349, 234: 37, 190: 355, 102: 9, 524: 88, 191: 8, 340: 125, 437: 411, 284: 5, 468: 80, 275: 146, 251: 257, 465: 217, 32: 3, 454: 265, 269: 14, 405: 12, 394: 15, 258: 48, 498: 30, 8: 2, 246: 68, 113: 14, 58:
3, 372: 24, 490: 56, 504: 60, 464: 80, 400: 290, 155: 72, 539: 45, 95: 33, 367: 8, 105: 27, 458: 69, 106: 26, 268: 238, 202: 60, 221: 77, 151: 36, 480: 81, 410: 27, 143: 1, 116: 53, 538: 2, 512: 2, 16: 5, 219: 51, 136: 38, 124: 20, 494: 4, 135: 71, 431: 14, 157: 10, 189: 99, 118: 13, 192: 17, 172: 64, 110: 2, 483: 9, 62: 77, 522: 9, 167: 6, 409: 14, 23: 46, 262: 19, 451: 11, 147: 5, 423: 20, 533: 5, 80: 12, 209: 19, 252: 4, 383: 19, 357: 51, 233: 26, 271: 89, 45: 4, 392: 19, 328: 17, 433: 35, 363: 6, 518: 1, 257: 7, 422: 9, 57: 20, 445: 64, 127: 45, 265: 32, 310: 29, 22: 27, 162: 29, 456: 3, 391: 35, 435: 4, 186: 1, 364: 5, 478: 12, 140: 9, 150: 2, 76: 22, 267: 16, 303: 7, 84: 6, 516: 6, 42: 54, 413: 11, 213: 7, 462: 16, 125: 4, 439: 15, 210: 3, 459: 6, 93: 3, 208: 3, 25: 36, 55: 6, 48: 57, 225: 10, 387: 3, 537: 2, 242: 15, 332: 8, 276: 9, 141: 1, 327: 3, 126: 17, 4: 10, 52: 5, 182: 5, 142: 4, 131: 33, 149: 3, 432: 1, 27: 6, 347: 1, 365: 6, 28: 2, 411: 1, 293: 3, 199: 3, 223: 3, 109: 5, 313: 1, 239: 8, 351: 6, 336: 1, 335: 17, 31: 2, 195: 3, 339: 8, 382: 1, 178: 2, 292: 5, 146: 6, 326: 1, 312: 4, 70: 2, 286: 5, 88: 4, 215: 1, 183: 3, 201: 5, 527: 4, 164: 2, 134: 5, 98: 1, 5: 1, 197: 1, 282: 3, 302: 4, 361: 2, 274: 3, 94: 1, 305: 2, 509: 1, 13: 1, 389: 4, 220: 1, 206: 2, 448: 2, 353: 2, 355: 1, 29: 1, 205: 1, 211: 1, 2: 1, 396: 1, 26: 1, 354: 1, 153: 1, 309: 2, 50: 1, 34: 1, 428: 1, 334: 1, 230: 1, 519: 1, 72: 1, 226: 1, 138: 2, 359: 1, 196: 1, 356: 1, 412: 1, 51: 1, 542: 2, 489: 1, 83: 1, 297: 1, 254: 1, 506: 2, 508: 1, 549: 1, 376: 2, 406: 1, 380: 1}

first60=[]
sorted1k = sorted(top1k.items(), key=lambda kv: kv[1], reverse=True)
for i in range(60):
    first60.append(sorted1k[i])

top10k = {498: 320, 145: 8931, 115: 8078, 357: 6003, 427: 3503, 122: 9855, 233: 5780, 393: 5388, 22: 1877, 43: 3453, 280: 7595, 454: 3880, 264: 12, 13: 195, 105: 519, 260: 2662, 247: 5287, 425: 4198, 40: 4577, 367: 316, 372: 3201, 151: 1721, 190: 2938, 102: 63, 20: 13, 142: 347, 432: 49, 370: 1765, 270: 658, 453: 77, 45: 702, 136: 554, 245: 2745, 480: 4249, 267: 950, 253: 2853, 234: 921, 155: 607, 80: 195, 182: 240, 47: 470, 483: 288, 433: 1154, 131: 307, 275: 122, 437: 1248, 179: 662, 56: 737, 32: 42, 423: 297, 276: 633, 251: 4163, 445: 1016, 380: 39, 135: 1673, 258: 1284, 202: 738, 72: 239, 378: 743, 76: 834, 468: 543, 413: 111, 284: 42, 473: 77, 55: 418, 138: 277, 392: 428, 478: 253, 125: 145, 23: 236, 435: 48, 141: 86, 286: 835, 164: 465, 89: 2, 417: 1450, 518: 4, 113: 108, 396: 142, 451: 258,
257: 310, 400: 1246, 14: 230, 74: 29, 25: 422, 189: 694, 271: 736, 221: 646, 490: 1230, 95: 581, 140: 154, 310: 120, 340: 1933, 439: 74, 268: 750, 116: 222, 459: 288, 269: 31, 183: 47, 215: 3, 363: 118, 274: 116, 16: 73, 391: 277, 24: 454, 332: 179, 282: 150, 4: 440, 306: 123, 410: 283, 302: 230, 83: 138, 484: 52, 494: 27, 387: 56, 265: 716, 351: 59, 499: 12, 5: 261, 27: 233, 178: 79, 504: 50, 338: 1, 334: 4, 62: 144, 456: 143, 29: 49, 88: 112, 52: 92, 327: 93, 154: 161, 242: 49, 246: 112, 220: 177, 328: 285, 500: 39, 147: 33, 167: 120, 335: 347, 236: 46, 254: 156, 524: 116, 213: 117, 57: 6, 354: 70, 172: 171, 347: 19, 405: 32, 94:
22, 26: 4, 309: 24, 395: 36, 537: 14, 304: 9, 330: 84, 513: 27, 458: 86, 447: 17, 49: 214, 109: 35, 42: 26, 126: 253, 28: 135, 339: 41, 219: 200, 300: 16, 124:
41, 475: 97, 199: 6, 312: 21, 93: 21, 98: 28, 31: 44, 252: 8, 527: 70, 184: 14, 422: 79, 103: 9, 133: 27, 355: 43, 465: 63, 277: 3, 509: 3, 191: 39, 209: 153, 226: 6, 522: 92, 262: 13, 53: 36, 177: 22, 352: 7, 91: 21, 38: 1, 48: 37, 58: 3, 313: 8, 292: 14, 96: 21, 7: 10, 186: 9, 137: 4, 295: 13, 70: 72, 365: 14, 225: 54, 376: 19, 146: 19, 272: 19, 450: 24, 157: 4, 430: 10, 218: 8, 298: 23, 291: 2, 175: 8, 174: 2, 15: 23, 303: 17, 162: 20, 517: 1, 356: 4, 359: 8, 409: 16, 516: 4, 143: 22, 353: 44, 168: 1, 2: 37, 153: 4, 34: 6, 428: 9, 346: 3, 35: 4, 288: 7, 344: 7, 383: 5, 464: 12, 195: 35, 1: 25, 118: 23, 39: 2, 389: 9, 18: 40, 196: 13, 54: 2, 149: 23, 408: 4, 51: 2, 106: 8, 69: 13, 533: 15, 471: 1, 394: 3, 381: 5, 534: 1, 158: 3, 492: 19, 107: 8, 448: 5, 536: 6, 434: 4, 411: 6, 46: 1, 17: 32, 150: 25, 438: 8, 519: 11, 206: 2, 188: 3, 84: 13, 192: 16, 512: 20, 59: 4, 6: 2, 201: 7, 160: 6, 479: 1, 503: 7, 336: 9, 364: 9, 505: 2, 222: 1, 37: 2, 243: 2, 67: 1, 362: 3, 506: 1, 526: 1, 216: 2, 361: 9, 329: 8, 193: 1, 525: 3, 472: 1, 406: 6, 343: 5, 9: 6, 104: 3, 324: 1, 86: 8, 440: 1, 273: 4, 92: 1, 399: 6,
85: 4, 305: 6, 390: 11, 421: 3, 532: 1, 474: 10, 293: 1, 36: 4, 205: 1, 180: 2, 108: 1, 198: 2, 495: 3, 481: 2, 441: 1, 134: 2, 181: 1, 416: 1, 493: 1, 161: 3,
326: 1, 197: 2, 507: 1, 156: 3, 217: 1, 331: 4, 87: 2, 12: 4, 239: 1, 227: 3, 82: 6, 223: 2, 187: 2, 66: 2, 348: 1, 41: 1, 511: 4, 256: 1, 307: 2, 491: 1, 200:
1, 323: 2, 208: 1, 382: 1, 384: 1, 231: 1, 99: 2, 71: 1, 73: 2, 317: 3, 487: 2, 19: 2, 460: 1, 119: 1, 130: 1, 345: 2, 311: 3, 60: 1, 316: 1, 463: 2, 90: 3, 402: 1, 538: 1, 318: 1, 230: 1, 224: 1, 333: 1, 30: 1, 489: 2, 319: 1, 412: 1, 204: 1}

first60_2=[]
for (playerId,own) in first60:
    first60_2.append((playerId,top1k[playerId]))

url = 'https://fantasy.premierleague.com/drf/bootstrap-static'    

playerIdToName = {}
playerIdToTSB = {}

source = requests.get(url).text
soup = BeautifulSoup(source,'lxml') 
jsonText = soup.find('p').getText()
    
data = json.loads(jsonText)
data = data['elements']

for playerInfo in data:
    playerIdToName[playerInfo['id']] = playerInfo['web_name']
    playerIdToTSB[playerInfo['id']] = playerInfo['selected_by_percent']  

# Table
outFile = open('PostGW14table.txt', 'a', encoding='utf8')

representation = []
for i in range(60):
    p = first60[i]
    q = first60_2[i]    
    tup = (playerIdToName[p[0]], str(p[1]/10), str(q[1]/100), playerIdToTSB[p[0]])
    representation.append(tup)
    line = '|'.join(tup)
    line = '|'+line+'|'
    print(line,file=outFile)
    