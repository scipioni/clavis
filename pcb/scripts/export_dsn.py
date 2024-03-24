import sys                                                                                                                                                                                                                                           
from pcbnewTransition import pcbnew

inputfile = sys.argv[1]
if len(sys.argv) > 1:
    outputfile = sys.argv[2]
else:
    inputfile.replace('.kicad_pcb','.dsn')
    
board = pcbnew.LoadBoard(inputfile)
pcbnew.ExportSpecctraDSN(board,outputfile)                                                                                                                                    

