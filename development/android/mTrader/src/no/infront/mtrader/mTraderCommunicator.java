package no.infront.mtrader;

public class mTraderCommunicator {
	String EOL = "";
	
	String login = "Action: login" + EOL +
						"Authorization: " + username + "/" + password + EOL +
						"Platform: Android" + EOL +
						"Client: mTrader" + EOL +
						"Protocol: 2.0" + EOL +
						"VerType: " + version + EOL +
						"ConnType: Socket" + EOL +
						"Streaming: 1" + EOL +
						"QFields: " + qFields + EOL + EOL;
						
	String logout = "Action: Logout" + EOL + EOL;

	String addSecurity = "Action: addSec" + EOL +
						"Authorization: " + username + EOL +
						"Search: " + tickerSymbol + EOL +
						"mCode: " + mCode + EOL + EOL;

	String removeSecurity = "Action: remSec" + EOL +
						"Authorization: " + username + EOL +
						"SecOid: " + feedTicker + EOL + EOL;

	String staticData = "Action: StatData" + EOL +
						"Authorization: " + username + EOL +
						"SecOid: " + feedTicker + EOL +
						"Language: " + "EN" + EOL + EOL;

	String streaming = "Action: q" + EOL +
						"Authorization: " + username + EOL +
						"SecOid: " + feedTicker + EOL +
						"QFields: " + qFields + EOL + EOL;

	String historicTrades = "Action: HistTrades" + EOL +
						"Authorization: " + username + EOL +
						"SecOid: " + feedTicker + EOL +
						"Index: " + index + EOL +
						"Count: " + count + EOL +
						"Columns: " + "TPVAY" + EOL + EOL;

	String chart = "Action: Chart" +
						"Authorization: " + username + EOL +
						"SecOid: " + feedTicker + EOL +
						"Period: " + period + EOL +
						"ImgType: " + imgType + EOL +
						"Width: " + width + EOL +
						"Height: " + height + EOL +
						"Orient: " + orientation + EOL + EOL; // (A)uto, (H)orizontal, and (V)ertical

	String newsBody = "Action: NewsBody" + EOL +
						"Authorization: " + username + EOL +
						"NewsID: " + newsId + EOL + EOL;

	String newsHeadlines = "Action: NewsListFeeds" + EOL +
						"Authorization: " + username + EOL +
						"NewsFeeds: " + mCode + EOL +
						"Days: 30" + EOL +
						"MaxCount: 50" + EOL + EOL;

	String newsHeadlinesForSymbol = "Action: NewsList" + EOL +
						"Authorization: " + username + EOL +
						"SecOid: " + feedTicker + EOL +
						"Days: 30" + EOL +
						"MaxCount: 50" + EOL + EOL;
	
	String search = "Action: incSearch" + EOL +
						"Search: " + symbol + EOL +
						"MaxHits: 50" + EOL + EOL;
}