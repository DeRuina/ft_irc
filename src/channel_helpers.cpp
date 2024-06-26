#include "Channel.hpp"
#include "Server.hpp"

std::vector<Client *> Channel::get_clients() const
{
	return (this->clients);
}

Client *Channel::get_client(std::string const &nickname)
{
	for (const auto &client : clients)
	{
		if (client->get_nickname() == nickname)
			return (client);
	}
	return (nullptr);
}

Client *Channel::get_client(Client *client)
{
	for (const auto &c : clients)
	{
		if (c == client)
			return (c);
	}
	return (nullptr);
}

void Channel::add_client(Client *client)
{
	if (get_client(client->get_nickname()) != nullptr)
	{
		std::cerr << "Client already in channel" << std::endl;
		return;
	}
	this->clients.push_back(client);
}

void Channel::remove_client(std::string const &nickname)
{
	Client *client = get_client(nickname);
	if (!client)
	{
		std::cerr << "Client not in channel" << std::endl;
		return;
	}
	this->clients.erase(std::remove(this->clients.begin(), this->clients.end(), client), this->clients.end());
	remove_invite(client);
	remove_op(client);
}
void Channel::remove_client(Client *client)
{
	if (!get_client(client))
	{
		std::cerr << "Client not in channel" << std::endl;
		return;
	}
	this->clients.erase(std::remove(this->clients.begin(), this->clients.end(), client), this->clients.end());
	remove_invite(client);
	remove_op(client);
}

/// OPS ///

std::vector<Client *> Channel::get_ops() const
{
	return (this->ops);
}

Client *Channel::get_op(std::string const &nickname)
{
	for (const auto &op : ops)
	{
		if (op->get_nickname() == nickname)
			return (op);
	}
	return (nullptr);
}

Client *Channel::get_op(Client *client)
{
	for (const auto &op : ops)
	{
		if (op == client)
			return (op);
	}
	return (nullptr);
}

void Channel::add_op(Client *client)
{
	if (get_op(client->get_nickname()) != nullptr)
	{
		std::cerr << "Client already op" << std::endl;
		return;
	}
	this->ops.push_back(client);
}

void Channel::remove_op(std::string const &nickname)
{
	Client *op = get_op(nickname);
	if (!op)
	{
		std::cerr << "Client not op" << std::endl;
		return;
	}
	this->ops.erase(std::remove(this->ops.begin(), this->ops.end(), op), this->ops.end());
}

void Channel::remove_op(Client *client)
{
	if (get_op(client) == NULL)
	{
		std::cerr << "Client not op" << std::endl;
		return;
	}
	this->ops.erase(std::remove(this->ops.begin(), this->ops.end(), client), this->ops.end());
}

/// INVITES ///

Client *Channel::get_invite(std::string const &nickname)
{
	for (const auto &invite : invite_list)
	{
		if (invite->get_nickname() == nickname)
			return (invite);
	}
	return (nullptr);
}

Client *Channel::get_invite(Client *client)
{
	for (const auto &invite : invite_list)
	{
		if (invite == client)
			return (invite);
	}
	return (nullptr);
}

void Channel::add_invite(Client *client)
{
	if (get_invite(client->get_nickname()) != nullptr)
	{
		std::cerr << "Client already invited" << std::endl;
		return;
	}
	this->invite_list.push_back(client);
}

void Channel::remove_invite(std::string const &nickname)
{
	Client *client = get_invite(nickname);
	if (!client)
	{
		std::cerr << "Client not invited" << std::endl;
		return;
	}
	this->invite_list.erase(std::remove(this->invite_list.begin(), this->invite_list.end(), client), this->invite_list.end());
}

void Channel::remove_invite(Client *client)
{
	if (get_invite(client) == NULL)
	{
		std::cerr << "Client not invited" << std::endl;
		return;
	}
	this->invite_list.erase(std::remove(this->invite_list.begin(), this->invite_list.end(), client), this->invite_list.end());
}

/// GETTERS ///

unsigned char Channel::get_modes()
{
	return (this->modes);
}

std::string Channel::get_topic() const
{
	return (this->topic_str);
}

/// SETTERS ///

void Channel::set_key(Client *commander, std::string const &key)
{
	if (!get_op(commander))
	{
		std::cerr << "Client could not set key: not an op" << std::endl;
		return;
	}
	this->key = key;
}

void Channel::set_topic(std::string topic)
{
	this->topic_str = topic;
}

void Channel::set_limit(Client *commander, unsigned int limit)
{
	if (!get_op(commander))
	{
		std::cerr << "Client could not set limit: not an op" << std::endl;
		return;
	}
	this->limit = limit;
}

/// INVITE CHECK ///

bool Channel::invite_check(Client *client)
{
	if (this->modes & MODE_I) // if channel is invite only
	{
		if (get_invite(client) != nullptr)
			return (true); // if client is in invite list
		return (false);	   // if client is not in invite list
	}
	return (true); // if channel is not invite only
}

bool Channel::key_check(std::string const &key) // todo: check if this is correct
{
	if (this->modes & MODE_K) // if channel is key protected
	{
		if (this->key == key)
			return (true); // if client has the correct key
		return (false);	   // if client has the wrong key
	}
	return (true); // if channel is not key protected
}

bool Channel::limit_check()
{
	if (this->modes & MODE_L) // if channel has a user limit
	{
		if (this->clients.size() < this->limit)
			return (true); // if the channel is not full
		return (false);	   // if the channel is full
	}
	return (true); // if channel has no user limit
}

/// MODES ///

void Channel::add_mode(char const &mode)
{
	if (mode == 'i')
		this->modes |= MODE_I;
	else if (mode == 'k')
		this->modes |= MODE_K;
	else if (mode == 'l')
		this->modes |= MODE_L;
	else
		std::cerr << "Unknown mode" << std::endl;
}

void Channel::remove_mode(char const &mode)
{
	if (mode == 'i')
		this->modes &= ~MODE_I;
	else if (mode == 'k')
		this->modes &= ~MODE_K;
	else if (mode == 'l')
		this->modes &= ~MODE_L;
	else
		std::cerr << "Unknown mode" << std::endl;
}

void Channel::broadcast(std::string const &message)
{
	std::cout << "Broadcasting: " << message << std::endl;
	server.send_response(rType::ChannelToClients, "P0INTL655", name, message);
}

void Channel::broadcast(Client *sender, std::string const &message)
{
	std::cout << "Broadcasting: " << message << std::endl;
	server.send_response(rType::ClientToChannel, sender->get_nickname(), name, message);
}
bool Channel::is_client_in_channel(std::string const &nickname)
{
	for (size_t i = 0; i < this->clients.size(); i++)
		if (clients[i]->get_nickname() == nickname)
			return (true);
	return (false);
}

std::string Channel::get_channel_name()
{
	return (this->name);
}

bool Channel::is_empty()
{
	if (this->get_clients().size() == 0)
		return true;
	return false;
}
