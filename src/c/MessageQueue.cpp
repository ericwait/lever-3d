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

#include "MessageQueue.h"
#include "comdef.h"

MessageQueue::MessageQueue()
{
	queueMutex = CreateMutex(NULL,FALSE,NULL);
	validQueue = true;
}

MessageQueue::~MessageQueue()
{
	if (queueMutex!=NULL)
		CloseHandle(queueMutex);

	queueMutex = NULL;
}

Message MessageQueue::getNextMessage()
{
	DWORD waitTime = INFINITE;
	Message msgOut;

#ifdef _DEBUG
	waitTime = 36000;
#endif // _DEBUG
	
	DWORD waitTerm = WaitForSingleObject(queueMutex,waitTime);
	if (waitTerm==WAIT_TIMEOUT)
	{
		throw std::runtime_error("Could not acquire mutex for message queue!");
	}

	if (messages.empty())
	{
		msgOut.command = "null";
		msgOut.message = "";
		msgOut.val = 0.0;
	}
	else
	{
		msgOut = messages.front();
		messages.pop();
	}

	ReleaseMutex(queueMutex);

	return msgOut;
}

void MessageQueue::addMessage(std::string command, double val)
{
	Message msgIn;
	msgIn.command = command;
	msgIn.message = "";
	msgIn.val = val;

	addMessage(msgIn);
}

void MessageQueue::addMessage(std::string command, std::string message)
{
	Message msgIn;
	msgIn.command = command;
	msgIn.message = message;
	msgIn.val = 0.0;

	addMessage(msgIn);
}

void MessageQueue::addMessage(std::string command, std::string message, double val)
{
	Message msgIn;
	msgIn.command = command;
	msgIn.message = message;
	msgIn.val = val;

	addMessage(msgIn);
}

void MessageQueue::addMessage(Message message)
{
	if (!validQueue) return;

	DWORD waitTime = INFINITE;

#ifdef _DEBUG
	waitTime = 36000;
#endif // _DEBUG

	DWORD waitTerm = WaitForSingleObject(queueMutex,waitTime);
	if (waitTerm==WAIT_TIMEOUT)
	{
		throw std::runtime_error("Could not acquire mutex for message queue!");
	}

	messages.push(message);

	ReleaseMutex(queueMutex);
}

void MessageQueue::addErrorMessage(HRESULT hr)
{
	_com_error err(hr);
	LPCTSTR errMsg = err.ErrorMessage();

	Message msgIn;
	msgIn.command = "error";
	msgIn.message = errMsg;
	msgIn.val = hr;

	addMessage(msgIn);
}

void MessageQueue::addErrorMessage(std::string message)
{
	Message msgIn;
	msgIn.command = "error";
	msgIn.message = message;
	msgIn.val = -1.0;

	addMessage(msgIn);
}

void MessageQueue::clear()
{
	DWORD waitTime = INFINITE;

#ifdef _DEBUG
	waitTime = 36000;
#endif // _DEBUG

	DWORD waitTerm = WaitForSingleObject(queueMutex,waitTime);
	if (waitTerm==WAIT_TIMEOUT)
	{
		throw std::runtime_error("Could not acquire mutex for message queue!");
	}

	while (!messages.empty())
	{
		messages.pop();
	}

	validQueue = false;

	ReleaseMutex(queueMutex);
}

std::vector<Message> MessageQueue::flushQueue()
{
	DWORD waitTime = INFINITE;
	Message msgOut;

#ifdef _DEBUG
	waitTime = 36000;
#endif // _DEBUG

	DWORD waitTerm = WaitForSingleObject(queueMutex,waitTime);
	if (waitTerm==WAIT_TIMEOUT)
	{
		throw std::runtime_error("Could not acquire mutex for message queue!");
	}

	std::vector<Message> queueOut;
	if (messages.empty())
	{
		Message none;
		none.command = "null";
		none.message = "";
		none.val = -1;

		queueOut.push_back(none);
	}
	else
	{
		while (!messages.empty())
		{
			queueOut.push_back(messages.front());
			messages.pop();
		}
	}

	ReleaseMutex(queueMutex);

	return queueOut;
}

