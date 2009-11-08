import sys, os

dloadDirectory = '/Users/palmerc/specBenchmarkCSVs/'

class SpecMark:
    def __init__(self, filename):
        self.filename = filename
        self.runs = []
        self.hardware = []
        self.software = []
        
    def __str__(self):
        output = []
        
        hardwareList = []
        for key, value in self.hardware:
            if key == "":
                hardwareList[-1] = '"' + hardwareList[-1].replace('"', '') + ' ' + value.replace('"', '') + '"'
            else:
                hardwareList.append(value)
        
        softwareList = []
        for key, value in self.software:
            if key == "":
                softwareList[-1] = '"' + softwareList[-1].replace('"', '') + ' ' + value.replace('"', '') + '"'
            else:
                softwareList.append(value)
        
        for run in self.runs:
            valuesList = []
            name = run['currentBenchmark']
            description = run['description']
            baseRunTime = run['baseRunTime']
            peakRunTime = run['peakRunTime']
            valuesList.extend([self.filename, name, description, baseRunTime, peakRunTime])
            valuesList.extend(hardwareList)
            valuesList.extend(softwareList)
            runString = ",".join(valuesList)
            
            output.append(runString)
        
        
        return "\n".join(output)
        
    def addRun(self, name, baseRunTime, peakRunTime, description):
        individualRun = { 'currentBenchmark' : name,
                         'description' : description,
                         'baseRunTime' : baseRunTime,
                         'peakRunTime' : peakRunTime }
        self.runs.append(individualRun)
        
    def addHardware(self, key, value):
        self.hardware.append((key, value))
    
    def addSoftware(self, key, value):
        self.software.append((key, value))
    

def stateMachine(state, line):
    if state == "initial" and line.startswith("valid,0"):
        return "invalid"
    elif state == "initial" and line.startswith("valid,1"):
        return "valid"
    elif state == "valid" and line == '"Full Results Table"':
        return "results"
    elif state == "results" and line == '"Selected Results Table"':
        return "selected"
    elif state == "selected" and line == 'HARDWARE':
        return "hardware"
    elif state == "hardware" and line == 'SOFTWARE':
        return "presoftware"
    elif state == "presoftware" and line == '':
        return "software"
    elif state == "software" and line == '':
        return "final"
    else:
        return state
    
def parseLine(sm, state, line):
    if state == "results":
        linebits = line.split(',')
        if len(linebits) < 12:
            return

        name = linebits[0]
        baseRunTime = linebits[2] 
        peakRunTime = linebits[7]
        description = linebits[11]

        sm.addRun(name, baseRunTime, peakRunTime, description)        
    elif state == "hardware":
        linebits = line.split(',')
        if len(linebits) < 2:
            return
        key = linebits[0]
        value = linebits[1]

        sm.addHardware(key, value)
    elif state == "software":
        linebits = line.split(',')
        if len(linebits) < 2:
            return
        key = linebits[0]
        value = linebits[1]
        
        sm.addSoftware(key, value)

smList = []
count = 0
directoryListing = os.listdir(dloadDirectory)
for fileName in directoryListing:
    if not fileName.endswith('csv'):
        continue
    state = "initial"
    lines = 0
    fin = open(dloadDirectory + fileName, 'r')
    sm = SpecMark(fileName)
    for line in fin:
        line = line.strip()
        state = stateMachine(state, line)
        if state == "invalid":
            break
        parseLine(sm, state, line)
        lines += 1

    if state != "invalid":
        smList.append(sm)
        count += 1

for sm in smList:
    print sm

print "Finished processing", count, "valid runs."