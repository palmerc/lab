class StaticData:
	"""Static Data Request Object"""
	def __init__(self):
		self.username = ''
		self.feedNumber = ''
		self.tickerSymbol = ''
		self.language = 'EN'

	def __str__(self):
		actionString = 'Action: StatData\n'
		authorizationString = 'Authorization: %s\n' % (self.username)
		securityString = 'SecOid: %s/%s\n' % (self.feedNumber, self.tickerSymbol)
		languageString = 'Language: %s\n\n' % (self.language)

		staticDataRequest = actionString + \
			authorizationString + \
			securityString + \
			languageString

		return staticDataRequest
