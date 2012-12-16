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


#include "gui.h"
#include "videoconferencep2p.h"
#include <QLabel>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <rttmanager.h>

const int usersPerLine = 3;
const int updateDelay = 1000 / 24;

using namespace boost::posix_time;

GUI::GUI ( VideoConferenceP2P* vc ) : conference ( vc ),
    layout ( new QGridLayout() ),
    timer ( new QTimer )
{
    setLayout ( layout );
    timer->setInterval ( updateDelay );
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( update() ) );
}

void GUI::addUser ( User* u )
{
    AutoResizeImageView* video = new AutoResizeImageView;
    layout->addWidget ( video, line, column );
    QString nameString  =
        QString::fromStdString ( u->getName() );
    QLabel* name = new QLabel ( nameString );
    log::debug << "Adding user " << nameString.toStdString() << log::endl;
    layout->addWidget ( name, line + 1, column++ );

    if ( column == usersPerLine ) {
        line += 2;
        column = 0;
    }

    videos.insert ( u, video );
}

void GUI::start()
{
    timer->start();
}

void GUI::update()
{
    ptime maxTime = microsec_clock::local_time();
    maxTime = maxTime +
              milliseconds ( conference->getRTTManager()->getMaxDelay() );

    for ( auto it = videos.begin(); it != videos.end(); it++ )
    {
	QImage image = it.key()->getLatestFrame(maxTime);
	if(!image.isNull())
	    it.value()->setImage(image);
    }
}
