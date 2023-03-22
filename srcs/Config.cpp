#include <iostream>
#include <fstream>
#include <cstdlib>
#include "utils.hpp"
#include "Config.hpp"

Config::Config() {
	setFileConfig();
	this->_config["user_mods"] = "aiwros";
	this->_config["channel_mods"] = "OovaimnqpsrtklbeI";
	this->_config["config_file"] = CONFIGFILE;
}

Config::~Config() {}

void	Config::error(std::string error) {
	printConfigLog(error);
	printConfigLog("Config set to default config.");
	setDefaultConfig();
}

void	Config::setDefaultConfig() {
	if (this->_config.size())
		this->_config.clear();

	this->_config[VAR_SERVERNAME] = SERVERNAME;
	this->_config[VAR_VERSION_SERVER] = VERSION_SERVER;
	this->_config[VAR_MOTDFILE] = MOTDFILE;
	this->_config[VAR_MAXUSERS] = MAXUSERS;
	this->_config[VAR_PINGDELAY] = PINGDELAY;
	this->_config[VAR_TIMEOUT] = TIMEOUT;
	this->_config[VAR_OPERUSERNAME] = OPERUSERNAME;
	this->_config[VAR_OPERPASSWORD] = OPERPASSWORD;
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
		this->_configNames.push_back(VAR_VERSION_SERVER);
		this->_configNames.push_back(VAR_MOTDFILE);
		this->_configNames.push_back(VAR_MAXUSERS);
		this->_configNames.push_back(VAR_PINGDELAY);
		this->_configNames.push_back(VAR_TIMEOUT);
		this->_configNames.push_back(VAR_OPERUSERNAME);
		this->_configNames.push_back(VAR_OPERPASSWORD);
	}
	file.open(CONFIGFILE, std::ifstream::in);
	if (!file.is_open()) {
		error("An error occured while opening config file.");
		return ;
	}
	while (file.getline(readBuffer, sizeof(readBuffer))) {
		buffer = readBuffer;
		name = buffer.substr(0, buffer.find(' '));
		value = buffer.substr(buffer.find('=') + 2);
		this->_config.erase(name);
		this->_config[name] = value;
	}
	file.close();
	if (this->_config.size() < TOTALCONFIGS)
		error("Not enough config entries are given.");

	name = VAR_MAXUSERS;
	do {
		if (atoi(this->_config[name].c_str()) < 0) {
			this->_config[name] = intToString(abs(atoi(this->_config[name].c_str())));
			printConfigLog(name + " seems to be negative, absolute value is taken.");
		}
		if (name == VAR_MAXUSERS)
			name = VAR_PINGDELAY;
		else if (name == VAR_PINGDELAY)
			name = VAR_TIMEOUT;
		else
			name = "done";
	} while (name != "done");
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
