class Login:
	"""Represents an authentication message"""

	def __init__(self):
		self.username = ''
		self.password = ''
		self.platform = ''
		self.client = ''
		self.version = ''
		self.protocol = ''
		self.connection = 'socket'
		self.streaming = 1


	def __str__(self):
		actionString = 'Action: Login\n'
		authorizationString = 'Authorization: %s/%s\n' % (self.username, self.password)
		platformString = 'Platform: %s\n' % (self.platform)
		clientString = 'Client: %s\n' % (self.client)
		protocolString = 'Protocol: %s\n' % (self.protocol)
		versionString = 'VerType: %s\n' % (self.version)
		connectionString = 'ConnType: %s\n' % (self.connection)
		streamingString = 'Streaming: %d\n\n' % (self.streaming)

		loginRequest = actionString + \
			authorizationString + \
			platformString + \
			clientString + \
			protocolString + \
			versionString + \
			connectionString + \
			streamingString

		return loginRequest
