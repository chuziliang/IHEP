#!/bin/env python

import random # For randint
import sys # For sys.argv and sys.exit
import uhal
import struct
#import gcu
import datetime
import time
import os
from subprocess import call
from tkinter import filedialog as fd

    
if __name__ == '__main__':
    #main(sys.argv[1:])
   
    
    #os.chdir("../Binary")
    os.system('pwd')
    filetypes = (
        ('binary file', '*.bin'),
        ('All files', '*.*')
        )
    filename = fd.askopenfilename(
                title = 'Open a binary file',
                initialdir='~/Tests/GCU_SPMT/Binary',
                filetypes=filetypes)
    
    print filename
    
    #call(["New_Data_Readout_201109", filename])
    # filetypes = (
        # ('pdf file', '*.pdf'),
        # ('All files', '*.*')
        # )
    # filename = fd.askopenfilename(
                # title = 'Open a pdf file',
                # initialdir='~/Tests/GCU_SPMT/Binary',
                # filetypes=filetypes)
    
    call(["acroread", filename])
    # os.system('ls')
    # cmd = "New_Data_Readout_201109 "+baseFileName
    # #os.system('pwd')
    # print cmd
    # call(["New_Data_Readout_201109", baseFileName])
    # #os.system(cmd)
    # #os.system('mv *.bin ../Runs')
    # #os.system('mv *.pdf ../Runs')
    
    # #os.system('pwd')
    # #os.system('ls -la')
    print "... success!"
    
	
