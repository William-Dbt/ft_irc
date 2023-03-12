#include <iostream>
#include <fstream>
#include "Config.hpp"

Config::Config() {
	setFileConfig();
	this->_config["user_mods"] = "aiwro0s";
	this->_config["channel_mods"] = "0ovaimnqpsrtklbeI";
}

Config::~Config() {}

void	Config::error(std::string error) {
	std::cerr << "Error: " << error << '\n';
	std::cerr << "\tConfig set to default config." << std::endl;
	setDefaultConfig();
}

void	Config::setDefaultConfig() {
	if (this->_config.size())
		this->_config.clear();

	this->_config[VAR_SERVERNAME] = SERVERNAME;
	this->_config[VAR_VERSION] = VERSION;
	this->_config[VAR_MOTDFILE] = MOTDFILE;
	this->_config[VAR_MAXUSERS] = MAXUSERS;
	this->_config[VAR_PINGDELAY] = PINGDELAY;
	this->_config[VAR_TIMEOUT] = TIMEOUT;
}

void	Config::checkMinimumConfig() {
	std::string							buffer;
	std::vector<std::string>::iterator	it;

	for (it = this->_configNames.begin(); it != this->_configNames.end(); it++) {
		try {
			this->_config.at((*it));
		}
		catch (std::exception & e) {
			buffer = "the config \'" + (*it);
			buffer += "\' is missing.";
			error(buffer);
			return ;
		}
	}
}

void	Config::setFileConfig() {
	std::fstream	file;
	std::string		name;
	std::string		value;
	std::string		buffer;
	char			readBuffer[254];

	if (!this->_configNames.size()) {
		this->_configNames.push_back(VAR_SERVERNAME);
		this->_configNames.push_back(VAR_VERSION);
		this->_configNames.push_back(VAR_MOTDFILE);
		this->_configNames.push_back(VAR_MAXUSERS);
		this->_configNames.push_back(VAR_PINGDELAY);
		this->_configNames.push_back(VAR_TIMEOUT);
	}
	/*if (this->_config.size())
		this->_config.clear();*/

	file.open(CONFIGFILE, std::ifstream::in);
	if (!file.is_open()) {
		error("Error: an error occured while opening config file.");
		return ;
	}
	while (file.getline(readBuffer, sizeof(readBuffer))) {
		buffer = readBuffer;
		name = buffer.substr(0, buffer.find(' '));
		value = buffer.substr(buffer.rfind(' ') + 1);
		this->_config.erase(name);
		this->_config[name] = value;
	}
	file.close();
	if (this->_config.size() < TOTALCONFIGS)
		error("Error: not enough config entries are given.");

	checkMinimumConfig();
}

std::string	Config::get(std::string configName) {
	std::string	buffer;

	try {
		buffer = this->_config.at(configName);
	}
	catch (std::exception & e) {}

	return (buffer);
}
