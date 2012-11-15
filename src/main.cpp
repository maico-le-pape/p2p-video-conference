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

#include <iostream>
#include "videoconferencep2p.h"
#include "net/sockaddress.h"
#include "core/log.h"
#include "boost/lexical_cast.hpp"

/**
 * @brief ...
 * @details For the test we suppose we have n clients with ip 127.0.0.x:4242
 * where x in 2 to (n+1).
 * @param argv second and third aguments must be number_of_clients client_id
 **/
int main ( int argc, char*argv[] )
{
    Epyx::Thread::init();
    Epyx::log::init ( Epyx::log::CONSOLE );

    if ( argc != 3 ) {
        std::cout << "Use : videoconferencep2p n k" << std::endl;
        std::cout << "n is the total number of clients "
                  "and k the number of the current client" <<
                  std::endl;
    }

    int clients_number = std::atoi ( argv[1] );
    int client_id = std::atoi ( argv[2] );

    Epyx::log::debug << "Read " << clients_number << " " <<client_id <<
                     Epyx::log::endl;

    VideoConferenceP2P vc =
        VideoConferenceP2P (
            SockAddress ( std::string ( "127.0.0." ) +
                          boost::lexical_cast<std::string> ( client_id ), 4242 )
        );

    for ( int i = 0; i < clients_number; i++ ) {
        if ( i != client_id ) {
            vc.add ( "User number " +
                     boost::lexical_cast<std::string> ( i ),
                     SockAddress ( std::string ( "127.0.0." ) +
                                   boost::lexical_cast<std::string> ( i + 2 ),
                                   4242
                                 ) );
            Epyx::log::info << "Other client number " << i << " created" <<
                            Epyx::log::endl;
        }
    }

    Epyx::log::flushAndQuit();
}


