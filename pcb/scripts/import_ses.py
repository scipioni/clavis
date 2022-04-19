import sys                                                                                                                                                                                                                                           
import pcbnew      

inputfile = sys.argv[1]
inputses = sys.argv[2]

    
board = pcbnew.LoadBoard(inputfile)
#pcbnew.ExportSpecctraDSN(board,outputfile)                                                                                                                                    

