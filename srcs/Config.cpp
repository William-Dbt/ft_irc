#include <iostream>
#include <fstream>
#include "Config.hpp"

Config::Config() {
	setFileConfig();
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

	this->_config[NSERVERNAME] = SERVERNAME;
	this->_config[NVERSION] = VERSION;
	this->_config[NMOTDFILE] = MOTDFILE;
	this->_config[NMAXUSERS] = MAXUSERS;
	this->_config[NTIMEOUT] = TIMEOUT;
}

bool	Config::checkMinimumConfig() {
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
			return false;
		}
	}
	return true;
}

void	Config::setFileConfig() {
	std::fstream	file;
	std::string		name;
	std::string		value;
	std::string		buffer;
	char			readBuffer[254];

	if (!this->_configNames.size()) {
		this->_configNames.push_back(NSERVERNAME);
		this->_configNames.push_back(NVERSION);
		this->_configNames.push_back(NMOTDFILE);
		this->_configNames.push_back(NMAXUSERS);
		this->_configNames.push_back(NTIMEOUT);
	}
	if (this->_config.size())
		this->_config.clear();

	file.open(CONFIGFILE, std::ifstream::in);
	if (!file.is_open()) {
		error("Error: an error occured while opening config file.");
		return ;
	}
	while (file.getline(readBuffer, sizeof(readBuffer))) {
		buffer = readBuffer;
		name = buffer.substr(0, buffer.find(' '));
		value = buffer.substr(buffer.rfind(' ') + 1);
		this->_config[name] = value;
	}
	if (this->_config.size() < TOTALCONFIGS)
		error("Error: not enough config entries are given.");

	checkMinimumConfig();
}
