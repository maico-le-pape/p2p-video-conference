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


GUI::GUI ( VideoConferenceP2P* vc ) : conference ( vc ),
    layout ( new QGridLayout() )
{
   /* for ( int i = 0; i < 3; ++i ) {
        labels[i] = new QLabel ( tr ( "Line %1:" ).arg ( i + 1 ) );
        lineEdits[i] = new QLineEdit;
        layout->addWidget ( labels[i], i + 1, 0 );
        layout->addWidget ( lineEdits[i], i + 1, 1 );
    } */
}

void GUI::run()
{

   // QWidget



}

