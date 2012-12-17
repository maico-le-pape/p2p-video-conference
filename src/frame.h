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


#ifndef FRAME_H
#define FRAME_H

#include <QImage>
#include <core/common.h>
#include <boost/date_time/posix_time/ptime.hpp>
#include <functional>

class Frame {
    typedef boost::posix_time::ptime ptime;
public:
    Frame ( const Epyx::byte_str& data, const ptime& timestamp );
    bool operator< ( const Frame& B ) const;
    QImage getImage() const;
    ptime getTime() const;
    void setDelay(unsigned int delay);
    
private:
    QImage image;
    ptime timestamp;
};

struct FrameCompare : public std::binary_function<Frame*, Frame*, bool>
{
    bool operator()(const Frame* a, const Frame* b) {
	return !(*a < *b);
    }
};

#endif // FRAME_H
