#ifndef _PISERVER_H_
#define _PISERVER_H_

#include <iostream>
#include <Python.h>
#include <QApplication>


#include <XmlReader.h>
#include <Classify.h>
#include <Network.h>

#include <ClientQuickStart.h>
#include <TerminalModule.hpp>

struct ServerWork;
struct ServerAddress;

TerminalModule * TerminalModule::instance;

#endif