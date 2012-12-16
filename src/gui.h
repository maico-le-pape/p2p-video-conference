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


#ifndef GUI_H
#define GUI_H

#include "user.h"
#include <QWidget>
#include <QGridLayout>
#include <QMap>
#include <QTimer>

class VideoConferenceP2P;

using namespace Epyx;

class GUI : public QWidget {
    Q_OBJECT
public:
    GUI(VideoConferenceP2P* vc);
    void addUser( User* u);
    void start();
    
private slots:
    void update();

private:
    VideoConferenceP2P* conference;
    QGridLayout* layout;
    QMap<User*, QLabel*> videos;
    QTimer* timer;
    int line = 0;
    int column = 0;
};

#endif // GUI_H
