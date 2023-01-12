#pragma once

#include <string>
#include <vector>

class Client;

/* PASS 461 462 464 */
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
/* NICK 431 432 433 436 */
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NICKCOLLISION 436
/* USER 461 462 */
/* PING 461 409 */
#define ERR_NOORIGIN 409
/* QUIT X */
/* JOIN 461 403 405 475 474 473 476 332 333 353 366 */
#define ERR_NEEDMOREPARAMS 461
#define ERR_NOSUCHCHANNEL 403
#define ERR_TOOMANYCHANNELS 405
#define ERR_BADCHANNELKEY 475
#define ERR_BANNEDFROMCHAN 474
#define ERR_CHANNELISFULL 471
#define ERR_INVITEONLYCHAN 473
#define ERR_BADCHANMASK 476
#define RPL_TOPIC 332
#define RPL_TOPICWHOTIME 333
#define RPL_NAMREPLY 353
#define RPL_ENDOFNAMES 366
/* PART 461 403 442 */
#define ERR_NOTONCHANNEL 442
/* KICK 461 403 482 441 442 */
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_USERNOTINCHANNEL 441
/* PRIVMSG 401 402 404 407 411 412 413 414 301 */
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYTARGETS 407
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_NOTOPLEVEL 413
#define ERR_WILDTOPLEVEL 414
#define RPL_AWAY 301

#define ERR_NOPRIVILEGES 481

class Command {
	public:
		Command();
		~Command();

		void welcomeProtocol(std::vector<std::string> cmds, Client *client);
		void cmdPass(std::vector<std::string> cmd, Client *client);
		void cmdNick(std::vector<std::string> cmd, Client *client);
		void cmdUser(std::vector<std::string> cmd, Client *client);
		void cmdPing(std::vector<std::string> cmd, Client *client);
		void cmdQuit(std::vector<std::string> cmd, Client *client);

		void cmdJoin(std::vector<std::string> cmd, Client *client);
		void cmdPart(std::vector<std::string> cmd, Client *client);
		void cmdKick(std::vector<std::string> cmd, Client *client);

		void cmdNotice(std::vector<std::string> cmd, Client *client);
		void cmdPrivmsg(std::vector<std::string> cmd, Client *client);
};