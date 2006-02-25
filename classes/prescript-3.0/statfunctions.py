# This module provides a number of standard mathematical functions,
# including:
# mean, mode, median...

import sys,math

def Mean(dataArray):
   sum=0.0
   for i in xrange(len(dataArray)): sum=sum+dataArray[i]
   return sum/len(dataArray)


def record_data( fn, list ):
   f = open( fn, 'w' )
   list.sort()
   f.write( `list` )
   f.close()
    
    
# SD:
#
# Return the Standard Deviation of the list of numbers, according to:
#
#     SD = sqrt( sigma(sqr(x-xBAR)) / (n-1) )
#
# Also has the ability to apply a band-pass filter to remove non-representative values,
# where this is appropriate.  A limit of 'None' means there is no limit in that direction
def SD(list, x_bar, maxval=None, minval=None):

   sum = 0
   n = 0
   newlist = []
   
   # Apply filter and calculate average
   for i in xrange(len(list)):
      item = list[i]

      # only consider this item if it is in range
      if (maxval == None or item <= maxval) and (minval == None or item >= minval):
         sum = sum + item
         n = n + 1
         newlist.append( item )

   # x_bar = float(sum)/n  # the average of newlist
   sum = 0
   for i in xrange(len(newlist)):
      t = newlist[i]-x_bar
      sum = sum + (t*t)

   return math.sqrt( sum/(len(newlist)-1) )


# Returns the statistical mode from a sorted list
def Mode(dataArray):
   return Mode2(dataArray)[0]  # legacy; return only the mode

def Mode2(list):
    list.sort()

    histogram=[]
    thisF = 0
    thisVal = list[0]

    # compress the input list down to frequency,value pairs
    for i in xrange(len(list)):
	if list[i] == thisVal:  thisF = thisF + 1  # found another 'thisVal
	else:
	    histogram.append( (thisF,thisVal) )
	    thisF = 1
	    thisVal = list[i]
    
    histogram.append( (thisF,thisVal) ) # the last pair will not have been written yet
    histogram.sort()  # sort on frequency

#    sys.stderr.write( "i="+`list`+"\no="+`histogram`+"\n" )

    # the mode will be the last in the list
    
    mode = histogram[-1][1]

    if len(histogram) == 1:  mode2 = mode
    else:
	first = list.index(mode)  # first occurance of mode
	for i in xrange(first, len(list)):
	    if list[i] != mode:
		last = i-1
		break
	else:   last = i

	# determine which end is closer
	if first == 0: firstDiff= None
	else: firstDiff = abs(list[first]-list[first-1])

	if last == len(list)-1: lastDiff= None
	else: lastDiff = abs(list[last+1]-list[last])

	# now return the closest end

	if firstDiff == None or lastDiff == None:
	    if firstDiff == None and lastDiff != None:
		mode2 = list[last+1]
	    elif firstDiff != None and lastDiff == None:
		mode2 = list[first-1]
	    else:
		mode2 = mode
	else:
	    if first < lastDiff: mode2 = list[first-1]
	    else: mode2 = list[last+1]

    return (mode,mode2)
	    
    

    
def oldMode(dataArray):

    histogram=[]
    h_index=0
    count=1

    histogram.append(dataArray[0])

    current=dataArray[0]
    
    for i in range(1,len(dataArray)):

	if (current==dataArray[i]):
	    count=count+1
	else:
	    histogram.append(count)
	    h_index=h_index+2
	    histogram.append(dataArray[i])
	    count=1

	current=dataArray[i]

    histogram.append(count)

    max=0
    mode=0

    for i in range(len(histogram)/2):
	if (max < histogram[2*i+1]):
	    max=histogram[2*i+1]
	    mode=histogram[2*i]

	elif (max == histogram[2*i+1]):
	    mode=(mode+histogram[2*i]) / 2
	
    return mode


#  modeXval:
#
#	Finds the value in the given sorted list which has the given frequency
#  This should actually return a list of elements since there is a one-to-many
#  relationship between frequency and value.  
#
#  For now, this function returns a list of only element -- the largest value
#  in the list that has the given mode
#
#  Note, the list must be sorted in ascending order
def modeXval(mode, list):
    if len(list) == 0:
       raise "StatsError","Empty list in call to modeXval"  
       return None

    if mode == 0:
       raise "StatsError", "Searching for value where mode = 0 yields an infinite list"
       return None

    i = -1		# the iterator -- is negative because we traverse the list in reverse order
    thismode = 0	# this is the mode of the current value
    val	= list[i]	# this is the value for which the mode is being currently calculated

    for i in range( -1, -len(list)-1, -1 ):  # examine each element of list
	if list[i] == val:
	   thismode = thismode + 1
	else:
	   if thismode == mode:  # have we reached our target yet?
	      return [val]	      # if yes, return the target

	   val = list[i]
	   thismode = 1


    # if we got down here, then the target (val) isn't in list.
    return []
	




# Returns the statistical mean from a sorted list -- there are also probably
# better ways of achieving this statistic..
def Median(dataArray):
   index=len(dataArray)/2
   if math.fmod(index, 2)==0.0:
       median=(dataArray[index]+dataArray[index+1])/2
   else:
       median = dataArray[index]

   return median


# This procedure returns the average of the next num values
def LocalAverage(dataArray, num, columnThreshold, modeLinespace):
    sum=0.0
    count=0

    for i in range(num-1):
	if ((dataArray[i]=='?') or (dataArray >= columnThreshold) or (dataArray[i]==modeLinespace)):
	    break
	
	sum=sum+dataArray[i]
	count=count+1

    if (sum !=0):
	return (sum/count)
    else:
	return 0.0
    

    
