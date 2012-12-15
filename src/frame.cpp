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


#include "frame.h"
#include <QByteArray>
#include <QImageReader>
#include <QBuffer>

Frame::Frame ( const unsigned char* data, int size )
{
    QByteArray message = QByteArray::fromRawData (
                             reinterpret_cast<const char * > ( data ),
                             size );
    QBuffer buffer ( &message );
    QImageReader in ( &buffer, "JPG" );
    image = in.read();
}

bool Frame::operator< ( const Frame& B ) const
{
    return true;
}

