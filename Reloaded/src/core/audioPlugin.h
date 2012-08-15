/**
 * Reloaded, an Amstrad CPC emulator
 * Copyright (C) 2011  cpcsdk crew
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _AUDIOPLUGIN_H_
#define _AUDIOPLUGIN_H_

#include <stdint.h>
#include <cstdlib>

#include "log.h"
//extern uint8_t *pbSndBuffer;
//uint8_t *pbSndBuffer;
//extern uint8_t *pbSndBufferPtr;
//uint8_t *pbSndBufferPtr;

class t_CPC;
class t_PSG;

class AudioPlugin
{
    public:
        AudioPlugin() {}
		virtual ~AudioPlugin() {}
        virtual int init(t_CPC& cpc, t_PSG& psg) = 0;
        virtual void shutdown() = 0;
        virtual int update() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual uint8_t* getBuffer() = 0;
		virtual void lock() {}
		virtual void unlock() {}
    protected:
        t_CPC* cpc;
        t_PSG* psg;
};

class NullAudioPlugin : public AudioPlugin
{
    public:
        NullAudioPlugin() {}
        int init(t_CPC& cpc, t_PSG& psg) {
				InfoLogMessage("[NullAudio Plugin] Open.");
				return 0;
		}
        void shutdown() {}
        int update() {return 0;}
        void pause() {}
        void resume() {}
        uint8_t* getBuffer() {return NULL;}
};

#endif /* #ifndef _AUDIOPLUGIN_H_ */
