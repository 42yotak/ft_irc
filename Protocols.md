https://modern.ircdocs.horse/
---

<!-- '?' == 명시되진 않았는데 아마 이럴거같음 -->
PASS		PASS <password>																		(PASS 0000)
->			no reply

NICK		NICK <nickname>																		(NICK Mike)
->			:<source> NICK <nickname>													(:yuhwang NICK babo)

USER		USER <username> 0 * <realname>										(USER yotak 0 * yuje)
->			no reply (Welcome protocols)

PING		PING <token> 																			(PING hi)
->			PONG <server> <token>															(PONG PokemonGo hi)

QUIT		QUIT [<reason>]																		(QUIT :it's too boring)
->			:<source> QUIT :Quit: <reason>										(:yotak QUIT :Quit: IRC sooo EZ)


JOIN		JOIN <channel>{,<channel>} [<key>{,<key>}]				(JOIN #foo,#bar)
->			:<source> JOIN <channel>													(:yuhwang JOIN #foo)

PART		PART <channel>{,<channel>} [<reason>]							(PART #foo,#bar :going to eat something)
->			:<source> PART <channel> [<reason>]?							(:yuhwang PART #foo)

KICK		KICK <channel> <user> *( "," <user> ) [<comment>]	(KICK #42seoul yuhwang :too stupid)
->			:<source> KICK <channel> <user> [<comment>]?			(:yotak KICK #42seoul yuhwang :too stupid)


PRIVMSG	PRIVMSG <target>{,<target>} <text to be sent>			(PRIVMSG #42seoul,yotak :Hello World!)
->			:<source> PRIVMSG <target> :<text>								(:yuhwang PRIVMSG yotak :Hello World!)

NOTICE	NOTICE <target>{,<target>} <text to be sent>			(NOTICE #42seoul,yotak :Hello World!)
->			no reply
