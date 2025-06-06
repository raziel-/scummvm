/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 *
 */

/*
 * This code is based on IBXM mod player
 *
 * Copyright (c) 2015, Martin Cameron
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the
 * following conditions are met:
 *
 * * Redistributions of source code must retain the above
 * copyright notice, this list of conditions and the
 * following disclaimer.
 *
 * * Redistributions in binary form must reproduce the
 * above copyright notice, this list of conditions and the
 * following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * * Neither the name of the organization nor the names of
 *  its contributors may be used to endorse or promote
 *  products derived from this software without specific
 *  prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef AUDIO_MODS_MOD_XM_S3M_H
#define AUDIO_MODS_MOD_XM_S3M_H

namespace Common {
class SeekableReadStream;
}

namespace Audio {

class AudioStream;

/**
 * Factory function for ModXmS3mStream streams. Reads all data from the
 * given ReadStream and creates an AudioStream from this. No reference
 * to the 'stream' object is kept, so you can safely delete it after
 * invoking this factory.
 *
 * This stream may be infinitely long if the mod contains a loop.
 *
 * @param stream			the ReadStream from which to read the tracker sound data
 * @param disposeAfterUse	whether to delete the stream after use
 * @param initialPos		initial track to start playback from
 * @param interpolation		interpolation effect level
 */
RewindableAudioStream *makeModXmS3mStream(Common::SeekableReadStream *stream,
		DisposeAfterUse::Flag disposeAfterUse,
		int initialPos = 0,
		int interpolation = 0);

/**
 * Check if the stream is one of the supported formats
 */
bool probeModXmS3m(Common::SeekableReadStream *stream);

} // End of namespace Audio

#endif
