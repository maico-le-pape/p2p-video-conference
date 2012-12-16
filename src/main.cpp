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
#include <QApplication>

/**
 * @brief ...
 * @details For the test we suppose we have n clients with
 * ip 127.0.0.1:(1000+i) where x in 2 to (n+1).
 * @param argv second and third aguments must be number_of_clients client_id
 **/
int main ( int argc, char*argv[] )
{
    QApplication app(argc, argv);
    
    if ( argc != 3 ) {
        std::cout << "Use : videoconferencep2p n k" << std::endl;
        std::cout << "n is the total number of clients "
                  "and k the number of the current client."
                  "The first client is number 0" << std::endl;
        return EXIT_FAILURE;
    }

    Epyx::Thread::init();
    Epyx::log::init ( Epyx::log::CONSOLE );

    int clients_number = std::atoi ( argv[1] );
    int client_id = std::atoi ( argv[2] );

    Epyx::log::debug << "Program launched"  <<  Epyx::log::endl;

    VideoConferenceP2P vc ( SockAddress ( "127.0.0.1", 10000 + client_id ) );

    for ( int i = 0; i < clients_number; i++ ) {
        if ( i != client_id ) {
            vc.add ( "User number " + boost::lexical_cast<std::string> ( i ),
                     SockAddress ( "127.0.0.1", 10000 + i ) );
            /* Epyx::log::info << "Other client number " << i << " created" <<
                            Epyx::log::endl; */
        }
    }
    //vc.printUsers();
    vc.start();
    app.exec();
    sleep(50);
    Epyx::log::debug << "Program ended"  <<  Epyx::log::endl;
    Epyx::log::flushAndQuit();
}
