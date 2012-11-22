/*
    Copyright 2012 <copyright holder> <email>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef USER_H
#define USER_H

#include <string>
#include "net/sockaddress.h"
#include "net/udpsocket.h"
#include "boost/shared_ptr.hpp"

using namespace std;
using namespace Epyx;

class User {

public:
    User ( string, SockAddress );
    string getName() const;
    SockAddress getAddress() const;
    string getIpStr();
    unsigned short int getDelay() const;
    void updateDelay ( unsigned short int delay );
    void send(const void *data, int size);

private:
    string name;
    SockAddress address;
    unsigned short int delay;
    boost::shared_ptr<UDPSocket> socket;

};

#endif // USER_H
