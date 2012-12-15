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


#include "sender.h"
#include "videoconferencep2p.h"
#include <iostream>
#include "boost/lexical_cast.hpp"

Sender::Sender ( VideoConferenceP2P* vc ) : conference ( vc )
{

}

void Sender::run()
{
    ifstream indata;
    const int initial = 9458;
    const int final = 10906;
    int size;
    char * memblock;

    for ( int i = initial; i <= final; i++ ) {
        indata.open ( "frames/Picture" +
                      boost::lexical_cast<std::string> ( i ) +
                      ".jpg", ios::binary|ios::ate );
        if ( !indata.is_open() ) {
            Epyx::log::debug << "Error: File not opened" << Epyx::log::endl;
            continue;
        }

        size = indata.tellg();
        memblock = new char [size];
        indata.seekg ( 0, ios::beg );
        indata.read ( memblock, size );
        indata.close();


        std::vector<FragmentPacket> list =
            FragmentManager::cut ( memblock, size );
        map< SockAddress, User > users = conference->getUsers();

        for ( unsigned int i = 0; i < list.size(); i++ ) {
            FragmentPacket fp = list[i];

            for ( auto dest = users.begin() ; dest != users.end(); dest++ ) {
                fp.source = conference->host;
                const byte_str packet = fp.build ();
                dest->second.send ( packet.data() , packet.length() );

                // Epyx::log::debug << fp << Epyx::log::endl;
            }
        }
        usleep(41000);
    }
}
