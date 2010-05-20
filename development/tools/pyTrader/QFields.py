class QFields:
	def __init__(self):
		self.username = ''
		self.timeStamp = False
		self.lastTrade = False
		self.change = False
		self.changePercent = False
		self.changeArrow = False
		self.askPrice = False
		self.askVolume = False
		self.bidPrice = False
		self.bidVolume = False
		self.high = False
		self.low = False
		self.volume = False
		self.orderBook = False

	def __str__(self):
		actionString = 'Action: q\n'
		authorizationString = 'Authorization: %s\n' % (self.username)
		QFieldsString = 'QFields: l, b, a\n\n'
	
		QFieldsRequest = actionString + \
			authorizationString + \
			QFieldsString
		
		return QFieldsRequest
