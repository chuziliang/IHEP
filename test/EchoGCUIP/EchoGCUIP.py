import sys

gcu=sys.argv[1]
print("k7 address is 10.7."+str(int(gcu)//256)+"."+str(int(gcu)%256))
print("s6 address is 10.6."+str(int(gcu)//256)+"."+str(int(gcu)%256))