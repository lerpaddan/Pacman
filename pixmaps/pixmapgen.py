
#pixmapgen.py

import sys
from PIL import Image

INNACCESIBLE = 0x000000
EMPTY        = 0x0000ff
WALL         = 0xffffff
GHOST        = 0xff00ff
ORIGIN       = 0x00ff00
PORTAL       = 0x00ffff
SPAWN        = 0xff0000
SPEED        = 0xffff00
FREEZE       = 0xff7f00


def get_letter(hex):
        #print("\t\tHandling hex " + str(hex))
        
        if(hex == EMPTY):
                return 'E'
        elif(hex == WALL):
                return 'W'
        elif(hex == GHOST):
                return 'G'
        elif(hex == ORIGIN):
                return 'O'
        elif(hex == PORTAL):
                return 'P'
        elif(hex == SPAWN):
                return 'S'
        elif(hex == SPEED):
                return 'C'
        elif(hex == FREEZE):
                return 'F'
        else:
                return 'I'
        
        
def handle_file(name):
        print("Generating " + name + ".txt")
        inf  = Image.open(name, "r")
        w, h = inf.size
        data = list(inf.getdata())
        outf = open(name + ".txt", "wb")
                
        for y in range(0, 32):
                for x in range(0, 64):
                        hex = (data[w*y + x][0] << 16) | (data[y*w + x][1] << 8) | data[y*w + x][2]
                
                        letter = get_letter(hex)
                        outf.write(letter + ',')
                
                outf.write('\n')
                
                
        inf.close()
        outf.close()
        print("Finished generating" + name + ".txt")
        


def main(argc, argv):
        #print("In main")
        for i in range(1, argc):
                print
                handle_file(argv[i])


main(len(sys.argv), sys.argv)
