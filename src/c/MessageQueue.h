////////////////////////////////////////////////////////////////////////////////////
//
//     Copyright 2014 Andrew Cohen, Eric Wait, and Mark Winter
//
//     This file is part of LEVER 3-D - the tool for 5-D stem cell segmentation,
//     tracking, and lineaging. See http://bioimage.coe.drexel.edu 'software' section
// 	  for details.
//
//     LEVER 3-D is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by the Free
//     Software Foundation, either version 3 of the License, or (at your option) any
//     later version.
// 
//     LEVER 3-D is distributed in the hope that it will be useful, but WITHOUT ANY
//     WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
//     A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with LEVer in file "gnu gpl v3.txt".  If not, see 
//     <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <queue>
#include <vector>
#include "windows.h"

struct Message
{
	std::string command;
	std::string message;
	double val;
};

class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

	Message getNextMessage();
	void addMessage(std::string command, double val);
	void addMessage(std::string command, std::string message);
	void addMessage(std::string command, std::string message, double val);
	void addErrorMessage(HRESULT hr);
	void addErrorMessage(std::string message);
	void clear();
	std::vector<Message> flushQueue();

private:
	void addMessage(Message message);
	bool validQueue;
	HANDLE queueMutex;
	std::queue<Message> messages;
};