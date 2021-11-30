/*
 * \brief  OSS emulation to Audio_out and Audio_in file systems
 * \author Josef Soentgen
 * \date   2018-10-25
 */

/*
 * Copyright (C) 2018-2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <audio_in_session/connection.h>
#include <audio_out_session/connection.h>
#include <base/registry.h>
#include <base/signal.h>
#include <os/vfs.h>
#include <util/xml_generator.h>
#include <vfs/dir_file_system.h>
#include <vfs/readonly_value_file_system.h>
#include <vfs/single_file_system.h>
#include <vfs/value_file_system.h>

/* libc includes */
#include <sys/soundcard.h>


static constexpr size_t _audio_in_stream_packet_size { Audio_in::PERIOD * Audio_in::SAMPLE_SIZE };

static constexpr size_t _audio_out_stream_packet_size { Audio_out::PERIOD * Audio_out::SAMPLE_SIZE };

/*
 * One packet cannot be allocated because of the ring buffer
 * implementation.
 */
static constexpr size_t _audio_in_stream_size { (Audio_in::QUEUE_SIZE - 1) *
                                                 _audio_in_stream_packet_size };

/*
 * One packet cannot be allocated because of the ring buffer
 * implementation, another packet cannot be allocated after
 * the stream is reset by 'Audio_out::Session_client::start()'.
 */
static constexpr size_t _audio_out_stream_size { (Audio_out::QUEUE_SIZE - 2) *
                                                 _audio_out_stream_packet_size };

namespace Vfs { struct Oss_file_system; }


struct Vfs::Oss_file_system
{
	using Name = String<32>;

	struct Audio;

	struct Data_file_system;
	struct Local_factory;
	struct Compound_file_system;
};


struct Vfs::Oss_file_system::Audio
{
	public:

		struct Info
		{
			unsigned  channels;
			unsigned  format;
			unsigned  sample_rate;
			unsigned  ifrag_total;
			unsigned  ifrag_size;
			unsigned  ifrag_avail;
			unsigned  ifrag_bytes;
			unsigned  ofrag_total;
			unsigned  ofrag_size;
			unsigned  ofrag_avail;
			unsigned  ofrag_bytes;
			long long optr_samples;
			unsigned  optr_fifo_samples;
			unsigned  play_underruns;

			Readonly_value_file_system<unsigned>  &_channels_fs;
			Readonly_value_file_system<unsigned>  &_format_fs;
			Readonly_value_file_system<unsigned>  &_sample_rate_fs;
			Value_file_system<unsigned>           &_ifrag_total_fs;
			Value_file_system<unsigned>           &_ifrag_size_fs;
			Readonly_value_file_system<unsigned>  &_ifrag_avail_fs;
			Readonly_value_file_system<unsigned>  &_ifrag_bytes_fs;
			Value_file_system<unsigned>           &_ofrag_total_fs;
			Value_file_system<unsigned>           &_ofrag_size_fs;
			Readonly_value_file_system<unsigned>  &_ofrag_avail_fs;
			Readonly_value_file_system<unsigned>  &_ofrag_bytes_fs;
			Readonly_value_file_system<long long> &_optr_samples_fs;
			Readonly_value_file_system<unsigned>  &_optr_fifo_samples_fs;
			Value_file_system<unsigned>           &_play_underruns_fs;

			Info(Readonly_value_file_system<unsigned>  &channels_fs,
			     Readonly_value_file_system<unsigned>  &format_fs,
			     Readonly_value_file_system<unsigned>  &sample_rate_fs,
			     Value_file_system<unsigned>           &ifrag_total_fs,
			     Value_file_system<unsigned>           &ifrag_size_fs,
			     Readonly_value_file_system<unsigned>  &ifrag_avail_fs,
			     Readonly_value_file_system<unsigned>  &ifrag_bytes_fs,
			     Value_file_system<unsigned>           &ofrag_total_fs,
			     Value_file_system<unsigned>           &ofrag_size_fs,
			     Readonly_value_file_system<unsigned>  &ofrag_avail_fs,
			     Readonly_value_file_system<unsigned>  &ofrag_bytes_fs,
			     Readonly_value_file_system<long long> &optr_samples_fs,
			     Readonly_value_file_system<unsigned>  &optr_fifo_samples_fs,
			     Value_file_system<unsigned>           &play_underruns_fs)
			:
				channels              { 0 },
				format                { 0 },
				sample_rate           { 0 },
				ifrag_total           { 0 },
				ifrag_size            { 0 },
				ifrag_avail           { 0 },
				ifrag_bytes           { 0 },
				ofrag_total           { 0 },
				ofrag_size            { 0 },
				ofrag_avail           { 0 },
				ofrag_bytes           { 0 },
				optr_samples          { 0 },
				optr_fifo_samples     { 0 },
				play_underruns        { 0 },
				_channels_fs          { channels_fs },
				_format_fs            { format_fs },
				_sample_rate_fs       { sample_rate_fs },
				_ifrag_total_fs       { ifrag_total_fs },
				_ifrag_size_fs        { ifrag_size_fs },
				_ifrag_avail_fs       { ifrag_avail_fs },
				_ifrag_bytes_fs       { ifrag_bytes_fs },
				_ofrag_total_fs       { ofrag_total_fs },
				_ofrag_size_fs        { ofrag_size_fs },
				_ofrag_avail_fs       { ofrag_avail_fs },
				_ofrag_bytes_fs       { ofrag_bytes_fs },
				_optr_samples_fs      { optr_samples_fs },
				_optr_fifo_samples_fs { optr_fifo_samples_fs },
				_play_underruns_fs    { play_underruns_fs }
			{ }

			void update()
			{
				_channels_fs         .value(channels);
				_format_fs           .value(format);
				_sample_rate_fs      .value(sample_rate);
				_ifrag_total_fs      .value(ifrag_total);
				_ifrag_size_fs       .value(ifrag_size);
				_ifrag_avail_fs      .value(ifrag_avail);
				_ifrag_bytes_fs      .value(ifrag_bytes);
				_ofrag_total_fs      .value(ofrag_total);
				_ofrag_size_fs       .value(ofrag_size);
				_ofrag_avail_fs      .value(ofrag_avail);
				_ofrag_bytes_fs      .value(ofrag_bytes);
				_optr_samples_fs     .value(optr_samples);
				_optr_fifo_samples_fs.value(optr_fifo_samples);
				_play_underruns_fs   .value(play_underruns);
			}

			void print(Genode::Output &out) const
			{
				char buf[512] { };

				Genode::Xml_generator xml(buf, sizeof(buf), "oss", [&] () {
					xml.attribute("channels",          channels);
					xml.attribute("format",            format);
					xml.attribute("sample_rate",       sample_rate);
					xml.attribute("ifrag_total",       ifrag_total);
					xml.attribute("ifrag_size",        ifrag_size);
					xml.attribute("ifrag_avail",       ifrag_avail);
					xml.attribute("ifrag_bytes",       ifrag_bytes);
					xml.attribute("ofrag_total",       ofrag_total);
					xml.attribute("ofrag_size",        ofrag_size);
					xml.attribute("ofrag_avail",       ofrag_avail);
					xml.attribute("ofrag_bytes",       ofrag_bytes);
					xml.attribute("optr_samples",      optr_samples);
					xml.attribute("optr_fifo_samples", optr_fifo_samples);
					xml.attribute("play_underruns",    play_underruns);
				});

				Genode::print(out, Genode::Cstring(buf));
			}
		};

	private:

		Audio(Audio const &);
		Audio &operator = (Audio const &);

		bool _audio_out_started { false };
		bool _audio_in_started  { false };

		enum { CHANNELS = 2, };
		const char *_channel_names[CHANNELS] = { "front left", "front right" };

		Genode::Constructible<Audio_out::Connection> _out[CHANNELS];
		Genode::Constructible<Audio_in::Connection>  _in { };

		Info &_info;
		Readonly_value_file_system<Info, 512> &_info_fs;

		size_t _read_sample_offset  { 0 };
		size_t _write_sample_offset { 0 };

	public:

		Audio(Genode::Env &env,
		      Info        &info,
		      Readonly_value_file_system<Info, 512> &info_fs)
		:
			_info         { info },
			_info_fs      { info_fs }
		{
			for (int i = 0; i < CHANNELS; i++) {
				try {
					_out[i].construct(env, _channel_names[i], false, false);
				} catch (...) {
					Genode::error("could not create Audio_out channel ", i);
					throw;
				}
			}

			try {
				_in.construct(env, "left");
			} catch (...) {
				Genode::error("could not create Audio_in channel");
				throw;
			}

			_info.channels    = CHANNELS;
			_info.format      = (unsigned)AFMT_S16_LE;
			_info.sample_rate = Audio_out::SAMPLE_RATE;
			_info.ifrag_size  = 2048;
			_info.ifrag_total = _audio_in_stream_size / _info.ifrag_size;
			_info.ifrag_avail = 0;
			_info.ifrag_bytes = 0;
			_info.ofrag_size  = 2048;
			_info.ofrag_total = _audio_out_stream_size / _info.ofrag_size;
			_info.ofrag_avail = _info.ofrag_total;
			_info.ofrag_bytes = _info.ofrag_avail * _info.ofrag_size;
			_info.update();
			_info_fs.value(_info);
		}

		void out_alloc_sigh(Genode::Signal_context_capability sigh)
		{
			_out[0]->alloc_sigh(sigh);
		}

		void out_progress_sigh(Genode::Signal_context_capability sigh)
		{
			_out[0]->progress_sigh(sigh);
		}

		void in_progress_sigh(Genode::Signal_context_capability sigh)
		{
			_in->progress_sigh(sigh);
		}

		void in_overrun_sigh(Genode::Signal_context_capability sigh)
		{
			_in->overrun_sigh(sigh);
		}

		void pause()
		{
			for (int i = 0; i < CHANNELS; i++) {
				_out[i]->stop();
			}

			_audio_out_started = false;
		}

		unsigned queued() const
		{
			return _out[0]->stream()->queued();
		}

		void update_info_ofrag_avail_from_optr_fifo_samples()
		{
//int ofrag_bytes_old = _info.ofrag_bytes;
			_info.ofrag_bytes = (_info.ofrag_total * _info.ofrag_size) -
			                    ((_info.optr_fifo_samples + _write_sample_offset) *
			                     CHANNELS * sizeof(int16_t));
//Genode::log("ofrag_bytes diff: ", (int)_info.ofrag_bytes - ofrag_bytes_old);
			_info.ofrag_avail = _info.ofrag_bytes / _info.ofrag_size;
#if 0
Genode::log("fifo samples: ", _info.optr_fifo_samples,
            ", ofrag_total: ", _info.ofrag_total,
            ", ofrag_avail: ", _info.ofrag_avail,
            ", ofrag_bytes: ", _info.ofrag_bytes);
#endif
		}

		/*
		 * Handle Audio_out progress signal.
		 *
		 * Returns true if at least one stream packet is available.
		 */
		bool handle_out_progress()
		{
			unsigned fifo_samples_new = queued() * Audio_out::PERIOD;

			if ((fifo_samples_new >= Audio_out::PERIOD) &&
			    (_write_sample_offset != 0)) {
				/* an allocated packet is part of the queued count,
				   but might not have been submitted yet */
				fifo_samples_new -= Audio_out::PERIOD;
			}

#if 0
Genode::log("handle_out_progress(): queued(): ", queued(),
            ", fifo_samples_new; ", fifo_samples_new,
            ", optr_fifo_samples: ", _info.optr_fifo_samples);
#endif

			if (fifo_samples_new == _info.optr_fifo_samples) {
				/*
				 * This is usually the progress signal for the first
				 * packet after 'start()', which is invalid.
				 */
				return false;
			}

			/*
			 * The queue count can wrap from 0 to 255 if packets are not
			 * submitted fast enough.
			 */

			if ((fifo_samples_new == 0) ||
			    (fifo_samples_new > _info.optr_fifo_samples)) {

//				if (_write_sample_offset == 0)
					pause();

				/* TODO: submit or clear partially filled packet */
				_write_sample_offset = 0;
#if 1
static int underrun_count = 0;
underrun_count++;
//Genode::warning("vfs_oss: underrun ", underrun_count);
Genode::trace("vfs_oss: underrun ", underrun_count);
#endif
				if (fifo_samples_new > _info.optr_fifo_samples) {
					_info.play_underruns++;
					fifo_samples_new = 0;
				}
			}

			_info.optr_fifo_samples = fifo_samples_new;
			update_info_ofrag_avail_from_optr_fifo_samples();
			_info.update();
			_info_fs.value(_info);

			return true;
		}

		void update_info_ifrag_avail()
		{
			unsigned max_queued = (_info.ifrag_total * _info.ifrag_size) /
			                      _audio_in_stream_packet_size;
			unsigned queued = _in->stream()->queued();	
#if 0
Genode::log("update_info_ifrag_avail(): queued: ", _in->stream()->queued(),
            ", max_queued: ", max_queued);
Genode::log("update_info_ifrag_avail(): pos: ", _in->stream()->pos(),
            ", tail: ", _in->stream()->tail());
#endif
			if (queued > max_queued) {
				/*
				 * Reset tail pointer to end of configured buffer
				 * to stay in bounds of the configuration.
				 */
				unsigned pos = _in->stream()->pos();
				for (unsigned int i = 0; i < max_queued; i++)
					_in->stream()->increment_position();
#if 0
Genode::log("update_info_ifrag_avail() a: queued: ", _in->stream()->queued(),
            ", max_queued: ", max_queued);
Genode::log("update_info_ifrag_avail() a: pos: ", _in->stream()->pos(),
            ", tail: ", _in->stream()->tail());
#endif
				_in->stream()->reset();
#if 0
Genode::log("update_info_ifrag_avail() b: queued: ", _in->stream()->queued(),
            ", max_queued: ", max_queued);
Genode::log("update_info_ifrag_avail() b: pos: ", _in->stream()->pos(),
            ", tail: ", _in->stream()->tail());
#endif
				_in->stream()->pos(pos);
#if 0
Genode::log("update_info_ifrag_avail() c: queued: ", _in->stream()->queued(),
            ", max_queued: ", max_queued);
Genode::log("update_info_ifrag_avail() c: pos: ", _in->stream()->pos(),
            ", tail: ", _in->stream()->tail());
#endif
			}

#if 0
Genode::log("update_info_ifrag_avail() 2: queued: ", _in->stream()->queued(),
            ", max_queued: ", max_queued);
Genode::log("update_info_ifrag_avail() 2: pos: ", _in->stream()->pos(),
            ", tail: ", _in->stream()->tail());
#endif
			_info.ifrag_bytes = min(_in->stream()->queued() * _audio_in_stream_packet_size,
			                        _info.ifrag_total * _info.ifrag_size);
			_info.ifrag_avail = _info.ifrag_bytes / _info.ifrag_size;	
			_info.update();
			_info_fs.value(_info);
//Genode::log("_info.ifrag_bytes new: ", _info.ifrag_bytes);
		}

		/*
		 * Handle Audio_in progress signal.
		 *
		 * Returns true if at least one stream packet is available.
		 */
		bool handle_in_progress()
		{
//Genode::log("handle_in_progress()");
			update_info_ifrag_avail();
			return _info.ifrag_avail > 0;
		}

		void handle_in_overrun()
		{
//Genode::warning(__func__);
/* XXX: VirtualBox does not restart the stream when ISPACE is empty */
//			_in->stop();
//			_audio_in_started = false;
		}

		bool read(char *buf, file_size buf_size, file_size &out_size)
		{
//Genode::log("read(): ", buf_size);

#if 0
//Genode::log("vfs");
			/* dummy implementation with audible noise for testing */

			for (file_size i = 0; i < buf_size / sizeof(int16_t) / CHANNELS; i++) {
				for (int c = 0; c < CHANNELS; c++) {
					((int16_t*)buf)[(i * CHANNELS) + c] = (((i*2)+1) << 8) | (((i*2)+0) & 0xff);
				}
			}
			

			out_size = buf_size;
#else
//Genode::log("Audio_in");
			out_size = 0;

			if (!_audio_in_started) {
				_in->start();
				_audio_in_started = true;
			}

			unsigned samples_to_read = buf_size / CHANNELS / sizeof(int16_t);

//Genode::log("read(): samples_to_read: ", samples_to_read);

			if (samples_to_read == 0) {
				/* invalid argument */
				return false;
			}

			Audio_in::Stream *stream = _in->stream();

			unsigned samples_read = 0;

			/* packet loop */

			for (;;) {
				unsigned stream_pos = stream->pos();	
//Genode::log("read(): packet loop for, pos: ", stream->pos(), ", tail: ", stream->tail(), ", queued: ", stream->queued());

				Audio_in::Packet *p = stream->get(stream_pos);

				if (!p || !p->valid()) {
//Genode::log("read(): packet invalid");
					update_info_ifrag_avail();
					return true;
				}

//Genode::log("read(): packet valid");

				/* sample loop */

				for (;;) {

					if (samples_read == samples_to_read) {
						update_info_ifrag_avail();
						return true;
					}

					for (unsigned c = 0; c < CHANNELS; c++) {
						unsigned const buf_index = out_size / sizeof(int16_t);
						((int16_t*)buf)[buf_index] = p->content()[_read_sample_offset] * 32768;
#if 1
static int count = 0;
if ((count == 0) && (((int16_t*)buf)[buf_index] != 0)) {
Genode::trace("vfs_oss read: ", _read_sample_offset, ": i: ", ((int16_t*)buf)[buf_index],
              ", f: ", p->content()[_read_sample_offset]);
//Genode::log("vfs_oss read: ", _read_sample_offset, ", i: ", ((int16_t*)buf)[buf_index],
//              ", f: ", p->content()[_read_sample_offset]);
//Genode::trace("buf[", buf_index, "] = ", ((int16_t*)buf)[buf_index]);
count++;
}
#endif
						out_size += sizeof(int16_t);
					}

					samples_read++;

					_read_sample_offset++;
//Genode::log("read(): samples_read: ", samples_read, ", _read_sample_offset: ", _read_sample_offset, ", period: ", Audio_in::PERIOD);
					if (_read_sample_offset == Audio_in::PERIOD) {
						p->invalidate();
						p->mark_as_recorded();
						stream->increment_position();
						_read_sample_offset = 0;
						break;
					}
				}
			}
#endif
			return true;
		}

		bool write(char const *buf, file_size buf_size, file_size &out_size)
		{
			using namespace Genode;

			out_size = 0;

			if (_info.ofrag_bytes == 0) {
//Genode::log("buffer full, blocking");
				throw Vfs::File_io_service::Insufficient_buffer();
			}

			bool block_write = false;

			if (buf_size > _info.ofrag_bytes) {
				buf_size = _info.ofrag_bytes;
				block_write = true;
			}

//Genode::log("vfs_oss: write(): buf_size new: ", buf_size);

			unsigned stream_samples_to_write = buf_size / CHANNELS / sizeof(int16_t);

//Genode::log("vfs_oss: write(): stream_samples_to_write: ", stream_samples_to_write);

			if (stream_samples_to_write == 0) {
				/* invalid argument */
				return false;
			}

			if (!_audio_out_started) {
				_audio_out_started = true;
//Genode::log("vfs_oss: write(): queued() 0: ", queued());
				_out[0]->start();
				_out[1]->start();
//Genode::log("vfs_oss: write(): queued() 1: ", queued());

			}

			unsigned stream_samples_written = 0;

			/* packet loop */

			for (;;) {
//Genode::log("vfs_oss: write(): packet loop");
				Audio_out::Packet *lp = nullptr;

//if (_write_sample_offset != 0) {
//Genode::log("vfs_oss: write(): _write_sample_offset: ", _write_sample_offset);
//}

				if (_write_sample_offset == 0) {
#if 0
Genode::log("pre alloc: pos: ", _out[0]->stream()->pos(),
            ", tail: ", _out[0]->stream()->tail(),
            ", queued: ", _out[0]->stream()->queued());
#endif

					for (;;) {
						try {
							lp = _out[0]->stream()->alloc();
							break;
						}
						catch (...) {
							/* this can happen on underrun */
							error("stream full",
							      " queued: ", _out[0]->stream()->queued(),
							      " pos: ",    _out[0]->stream()->pos(),
							      " tail: ",   _out[0]->stream()->tail()
							);
							_out[0]->stream()->reset();
						}
					}
#if 0
Genode::log("post alloc: pos: ", _out[0]->stream()->pos(),
            ", tail: ", _out[0]->stream()->tail(),
            ", queued: ", _out[0]->stream()->queued());
#endif
	            } else {
	            	/*
	            	 * Look up the previously allocated packet.
	            	 * The tail pointer got incremented after allocation,
	            	 * so we need to decrement by 1.
	            	 */
	            	unsigned const tail =
						(_out[0]->stream()->tail() +
						 Audio_out::QUEUE_SIZE - 1) %
						Audio_out::QUEUE_SIZE;
					lp = _out[0]->stream()->get(tail);
#if 0
Genode::log("lookup: pos: ", _out[0]->stream()->pos(),
            ", tail: ", _out[0]->stream()->tail(),
            ", queued: ", _out[0]->stream()->queued());
#endif
	            }
//Genode::log("lp: ", lp);
				unsigned const pos    = _out[0]->stream()->packet_position(lp);
//Genode::log("vfs_oss: write(): pos: ", pos);
				Audio_out::Packet *rp = _out[1]->stream()->get(pos);

				float *dest[CHANNELS] = { lp->content(), rp->content() };

				/* sample loop */

				for (;;) {
//Genode::log("vfs_oss: write(): sample loop");

					for (unsigned c = 0; c < CHANNELS; c++) {
						unsigned const buf_index = out_size / sizeof(int16_t);
//Genode::log("buf_index: ", buf_index);
						int16_t src_sample = ((int16_t const*)buf)[buf_index];
//Genode::log("src_sample: ", src_sample);

//Genode::log("_write_sample_offset: ", _write_sample_offset);
						dest[c][_write_sample_offset] = ((float)src_sample) / 32768.0f;

#if 1
static int count = 0;

#if 0
if (count == 0) {
	src_sample = 128;
}
#endif

if ((count == 0) && (src_sample != 0)) {
	Genode::trace("vfs_oss write: ", _write_sample_offset, ": i: ", src_sample,
	              ", f: ", dest[c][_write_sample_offset]);
//	Genode::log("vfs_oss write: ", _write_sample_offset, ", i: ", src_sample,
//	            ", f: ", dest[c][_write_sample_offset]);
	count++;
}
#endif


						out_size += sizeof(int16_t);
//Genode::log("out_size: ", out_size);
					}

					stream_samples_written++;

//Genode::log("stream_samples_written: ", stream_samples_written);


					_write_sample_offset++;
					if (_write_sample_offset == Audio_out::PERIOD) {
						_info.optr_samples += Audio_out::PERIOD;
						_info.optr_fifo_samples += Audio_out::PERIOD;
						_out[0]->submit(lp);
						_out[1]->submit(rp);
//Genode::log("vfs_oss: write(): submitted, queued(): ", queued());
						_write_sample_offset = 0;
						if (stream_samples_written != stream_samples_to_write)
							break;
					}

					if (stream_samples_written == stream_samples_to_write) {
//Genode::log("vfs_oss: write(): done: out_size: ", out_size, ", block_write: ", block_write);

						/* update info */

#if 0
for (unsigned int i = 0; i < Audio_out::PERIOD; i++) {
Genode::log("content[", i, "]: ", dest[0][i]);
}
#endif

#if 0
Genode::log("vfs_oss: write(): stream_samples_written: ", stream_samples_written,
            ", fifo_samples: ", _info.optr_fifo_samples,
            ", _write_sample_offset: ", _write_sample_offset);
#endif
						update_info_ofrag_avail_from_optr_fifo_samples();
						_info.update();
						_info_fs.value(_info);

						if (block_write) { throw Vfs::File_io_service::Insufficient_buffer(); }

						return true;
					}
				}
			}

			return true;
		}
};


class Vfs::Oss_file_system::Data_file_system : public Single_file_system
{
	private:

		Data_file_system(Data_file_system const &);
		Data_file_system &operator = (Data_file_system const &);

		Genode::Entrypoint &_ep;
		Audio              &_audio;

		struct Oss_vfs_handle : public Single_vfs_handle
		{
			Audio &_audio;

			bool blocked = false;

			Oss_vfs_handle(Directory_service      &ds,
			                    File_io_service   &fs,
			                    Genode::Allocator &alloc,
			                    int                flags,
			                    Audio             &audio)
			:
				Single_vfs_handle { ds, fs, alloc, flags },
				_audio { audio }
			{ }

			Read_result read(char *buf, file_size buf_size, file_size &out_count) override
			{
				bool success = _audio.read(buf, buf_size, out_count);
//Genode::log("read(): out_count: ", out_count);
				if (success) {
					if (out_count == 0) {
						blocked = true;
						return READ_QUEUED;
					}
					return READ_OK;
				}
				return READ_ERR_INVALID;
			}

			Write_result write(char const *buf, file_size buf_size,
			                   file_size &out_count) override
			{
				try {
					return _audio.write(buf, buf_size, out_count) ? WRITE_OK : WRITE_ERR_INVALID;
				} catch (Vfs::File_io_service::Insufficient_buffer) {
					blocked = true;
					return WRITE_OK;
				}
			}

			bool read_ready() override
			{
				return true;
			}
		};

		using Registered_handle = Genode::Registered<Oss_vfs_handle>;
		using Handle_registry   = Genode::Registry<Registered_handle>;

		Handle_registry _handle_registry { };

		Genode::Io_signal_handler<Vfs::Oss_file_system::Data_file_system> _audio_out_progress_sigh {
			_ep, *this, &Vfs::Oss_file_system::Data_file_system::_handle_audio_out_progress };

		Genode::Io_signal_handler<Vfs::Oss_file_system::Data_file_system> _audio_in_progress_sigh {
			_ep, *this, &Vfs::Oss_file_system::Data_file_system::_handle_audio_in_progress };

		Genode::Io_signal_handler<Vfs::Oss_file_system::Data_file_system> _audio_in_overrun_sigh {
			_ep, *this, &Vfs::Oss_file_system::Data_file_system::_handle_audio_in_overrun };

		void _handle_audio_out_progress()
		{
			if (_audio.handle_out_progress()) {
				/* at least one stream packet is available */
				_handle_registry.for_each([this] (Registered_handle &handle) {
					if (handle.blocked) {
						handle.blocked = false;
						handle.io_progress_response();
					}
				});
			}
		}

		void _handle_audio_in_progress()
		{
//Genode::log("_handle_audio_in_progress()");
			if (_audio.handle_in_progress()) {
				/* at least one stream packet is available */
				_handle_registry.for_each([this] (Registered_handle &handle) {
					if (handle.blocked) {
						handle.blocked = false;
						handle.io_progress_response();
					}
				});
			}
		}

		void _handle_audio_in_overrun()
		{
			_audio.handle_in_overrun();
		}

	public:

		Data_file_system(Genode::Entrypoint &ep,
		                 Audio              &audio,
		                 Name         const &name)
		:
			Single_file_system { Node_type::CONTINUOUS_FILE, name.string(),
			                     Node_rwx::ro(), Genode::Xml_node("<data/>") },

			_ep    { ep },
			_audio { audio }
		{
			_audio.out_progress_sigh(_audio_out_progress_sigh);
			_audio.in_progress_sigh(_audio_in_progress_sigh);
			_audio.in_overrun_sigh(_audio_in_overrun_sigh);
		}

		static const char *name()   { return "data"; }
		char const *type() override { return "data"; }

		/*********************************
		 ** Directory service interface **
		 *********************************/

		Open_result open(char const  *path, unsigned flags,
		                 Vfs_handle **out_handle,
		                 Allocator   &alloc) override
		{
			if (!_single_file(path)) {
				return OPEN_ERR_UNACCESSIBLE;
			}

			try {
				*out_handle = new (alloc)
					Registered_handle(_handle_registry, *this, *this, alloc, flags,
					                  _audio);
				return OPEN_OK;
			}
			catch (Genode::Out_of_ram)  { return OPEN_ERR_OUT_OF_RAM; }
			catch (Genode::Out_of_caps) { return OPEN_ERR_OUT_OF_CAPS; }
		}

		/********************************
		 ** File I/O service interface **
		 ********************************/

		Ftruncate_result ftruncate(Vfs_handle *, file_size) override
		{
			return FTRUNCATE_OK;
		}

		bool check_unblock(Vfs_handle *, bool, bool wr, bool) override
		{
			return wr;
		}
};


struct Vfs::Oss_file_system::Local_factory : File_system_factory
{
	using Label = Genode::String<64>;
	Label const _label;
	Name  const _name;

	Vfs::Env &_env;

	Readonly_value_file_system<unsigned>  _channels_fs          { "channels", 0U };
	Readonly_value_file_system<unsigned>  _format_fs            { "format", 0U };
	Readonly_value_file_system<unsigned>  _sample_rate_fs       { "sample_rate", 0U };
	Value_file_system<unsigned>           _ifrag_total_fs       { "ifrag_total", 0U };
	Value_file_system<unsigned>           _ifrag_size_fs        { "ifrag_size", 0U} ;
	Readonly_value_file_system<unsigned>  _ifrag_avail_fs       { "ifrag_avail", 0U };
	Readonly_value_file_system<unsigned>  _ifrag_bytes_fs       { "ifrag_bytes", 0U };
	Value_file_system<unsigned>           _ofrag_total_fs       { "ofrag_total", 0U };
	Value_file_system<unsigned>           _ofrag_size_fs        { "ofrag_size", 0U} ;
	Readonly_value_file_system<unsigned>  _ofrag_avail_fs       { "ofrag_avail", 0U };
	Readonly_value_file_system<unsigned>  _ofrag_bytes_fs       { "ofrag_bytes", 0U };
	Readonly_value_file_system<long long> _optr_samples_fs      { "optr_samples", 0LL };
	Readonly_value_file_system<unsigned>  _optr_fifo_samples_fs { "optr_fifo_samples", 0U };
	Value_file_system<unsigned>           _play_underruns_fs    { "play_underruns", 0U };

	Audio::Info _info { _channels_fs, _format_fs, _sample_rate_fs,
	                    _ifrag_total_fs, _ifrag_size_fs,
	                    _ifrag_avail_fs, _ifrag_bytes_fs,
	                    _ofrag_total_fs, _ofrag_size_fs,
	                    _ofrag_avail_fs, _ofrag_bytes_fs,
	                    _optr_samples_fs, _optr_fifo_samples_fs,
	                    _play_underruns_fs };

	Readonly_value_file_system<Audio::Info, 512> _info_fs { "info", _info };

	Audio _audio { _env.env(), _info, _info_fs };

	Genode::Watch_handler<Vfs::Oss_file_system::Local_factory> _ifrag_total_handler {
		_ifrag_total_fs, "/ifrag_total",
		_env.alloc(),
		*this,
		&Vfs::Oss_file_system::Local_factory::_ifrag_total_changed };

	Genode::Watch_handler<Vfs::Oss_file_system::Local_factory> _ifrag_size_handler {
		_ifrag_size_fs, "/ifrag_size",
		_env.alloc(),
		*this,
		&Vfs::Oss_file_system::Local_factory::_ofrag_size_changed };

	Genode::Watch_handler<Vfs::Oss_file_system::Local_factory> _ofrag_total_handler {
		_ofrag_total_fs, "/ofrag_total",
		_env.alloc(),
		*this,
		&Vfs::Oss_file_system::Local_factory::_ofrag_total_changed };

	Genode::Watch_handler<Vfs::Oss_file_system::Local_factory> _ofrag_size_handler {
		_ofrag_size_fs, "/ofrag_size",
		_env.alloc(),
		*this,
		&Vfs::Oss_file_system::Local_factory::_ofrag_size_changed };

	Genode::Watch_handler<Vfs::Oss_file_system::Local_factory> _play_underruns_handler {
		_play_underruns_fs, "/play_underruns",
		_env.alloc(),
		*this,
		&Vfs::Oss_file_system::Local_factory::_play_underruns_changed };

	static constexpr size_t _ifrag_total_min { 2 };
	static constexpr size_t _ifrag_size_min { _audio_in_stream_packet_size };
	static constexpr size_t _ifrag_total_max { _audio_in_stream_size / _ifrag_size_min };
	static constexpr size_t _ifrag_size_max { _audio_in_stream_size / _ifrag_total_min };

	static constexpr size_t _ofrag_total_min { 2 };
	static constexpr size_t _ofrag_size_min { _audio_out_stream_packet_size };
	static constexpr size_t _ofrag_total_max { _audio_out_stream_size / _ofrag_size_min };
	static constexpr size_t _ofrag_size_max { _audio_out_stream_size / _ofrag_total_min };

	/********************
	 ** Watch handlers **
	 ********************/

	void _ifrag_total_changed()
	{
		unsigned ifrag_total_new = _ifrag_total_fs.value(); 

		ifrag_total_new = Genode::max(ifrag_total_new, _ifrag_total_min);
		ifrag_total_new = Genode::min(ifrag_total_new, _ifrag_total_max);

		if (ifrag_total_new * _info.ifrag_size > _audio_in_stream_size)
			_info.ifrag_size = 1 << Genode::log2(_audio_in_stream_size / ifrag_total_new);

		_info.ifrag_total = ifrag_total_new;
		_info.ifrag_avail = 0;
		_info.ifrag_bytes = 0;
Genode::log("_ifrag_total_changed(): ", _info.ifrag_total, ", ", _info.ifrag_bytes);
		_info.update();
		_info_fs.value(_info);
	}

	void _ifrag_size_changed()
	{
		unsigned ifrag_size_new = _ifrag_size_fs.value(); 

		ifrag_size_new = Genode::max(ifrag_size_new, _ifrag_size_min);
		ifrag_size_new = Genode::min(ifrag_size_new, _ifrag_size_max);

		if (ifrag_size_new * _info.ifrag_total > _audio_in_stream_size) {
			_info.ifrag_total = _audio_in_stream_size / ifrag_size_new;
			_info.ifrag_avail = 0;
			_info.ifrag_bytes = 0;
Genode::log("_ifrag_size_changed(): ", _info.ifrag_total, ", ", _info.ifrag_bytes);
		}

		_info.ifrag_size = ifrag_size_new;

		_info.update();
		_info_fs.value(_info);
	}

	void _ofrag_total_changed()
	{
		unsigned ofrag_total_new = _ofrag_total_fs.value(); 

		ofrag_total_new = Genode::max(ofrag_total_new, _ofrag_total_min);
		ofrag_total_new = Genode::min(ofrag_total_new, _ofrag_total_max);

		if (ofrag_total_new * _info.ofrag_size > _audio_out_stream_size)
			_info.ofrag_size = 1 << Genode::log2(_audio_out_stream_size / ofrag_total_new);

		_info.ofrag_total = ofrag_total_new;
		_info.ofrag_avail = ofrag_total_new;
		_info.ofrag_bytes = ofrag_total_new * _info.ofrag_size;
Genode::log("_ofrag_total_changed(): ", _info.ofrag_total, ", ", _info.ofrag_bytes);
		_info.update();
		_info_fs.value(_info);
	}

	void _ofrag_size_changed()
	{
		unsigned ofrag_size_new = _ofrag_size_fs.value(); 

		ofrag_size_new = Genode::max(ofrag_size_new, _ofrag_size_min);
		ofrag_size_new = Genode::min(ofrag_size_new, _ofrag_size_max);

		if (ofrag_size_new * _info.ofrag_total > _audio_out_stream_size) {
			_info.ofrag_total = _audio_out_stream_size / ofrag_size_new;
			_info.ofrag_avail = _info.ofrag_total;
			_info.ofrag_bytes = _info.ofrag_total * _info.ofrag_size;
Genode::log("_ofrag_size_changed(): ", _info.ofrag_total, ", ", _info.ofrag_bytes);
		}

		_info.ofrag_size = ofrag_size_new;

		_info.update();
		_info_fs.value(_info);
	}

	void _play_underruns_changed()
	{
		/* reset counter */

		_info.play_underruns = 0;

		_info.update();
		_info_fs.value(_info);
	}

	static Name name(Xml_node config)
	{
		return config.attribute_value("name", Name("oss"));
	}

	Data_file_system _data_fs;

	Local_factory(Vfs::Env &env, Xml_node config)
	:
		_label   { config.attribute_value("label", Label("")) },
		_name    { name(config) },
		_env     { env },
		_data_fs { _env.env().ep(), _audio, name(config) }
	{ }

	Vfs::File_system *create(Vfs::Env&, Xml_node node) override
	{
		if (node.has_type("data")) {
			return &_data_fs;
		}

		if (node.has_type("info")) {
			return &_info_fs;
		}

		if (node.has_type(Readonly_value_file_system<unsigned>::type_name())) {

			if (_channels_fs.matches(node)) {
				return &_channels_fs;
			}

			if (_sample_rate_fs.matches(node)) {
				return &_sample_rate_fs;
			}

			if (_ifrag_avail_fs.matches(node)) {
				return &_ifrag_avail_fs;
			}

			if (_ifrag_bytes_fs.matches(node)) {
				return &_ifrag_bytes_fs;
			}

			if (_ofrag_avail_fs.matches(node)) {
				return &_ofrag_avail_fs;
			}

			if (_ofrag_bytes_fs.matches(node)) {
				return &_ofrag_bytes_fs;
			}

			if (_format_fs.matches(node)) {
				return &_format_fs;
			}

			if (_optr_samples_fs.matches(node)) {
				return &_optr_samples_fs;
			}

			if (_optr_fifo_samples_fs.matches(node)) {
				return &_optr_fifo_samples_fs;
			}
		}

		if (node.has_type(Value_file_system<unsigned>::type_name())) {

			if (_ifrag_total_fs.matches(node)) {
				return &_ifrag_total_fs;
			}

			if (_ifrag_size_fs.matches(node)) {
				return &_ifrag_size_fs;
			}

			if (_ofrag_total_fs.matches(node)) {
				return &_ofrag_total_fs;
			}

			if (_ofrag_size_fs.matches(node)) {
				return &_ofrag_size_fs;
			}

			if (_play_underruns_fs.matches(node)) {
				return &_play_underruns_fs;
			}
		}

		return nullptr;
	}
};


class Vfs::Oss_file_system::Compound_file_system : private Local_factory,
                                                   public  Vfs::Dir_file_system
{
	private:

		using Name = Oss_file_system::Name;

		using Config = String<1024>;
		static Config _config(Name const &name)
		{
			char buf[Config::capacity()] { };

			/*
			 * By not using the node type "dir", we operate the
			 * 'Dir_file_system' in root mode, allowing multiple sibling nodes
			 * to be present at the mount point.
			 */
			Genode::Xml_generator xml(buf, sizeof(buf), "compound", [&] () {

				xml.node("data", [&] () {
					xml.attribute("name", name); });

				xml.node("dir", [&] () {
					xml.attribute("name", Name(".", name));
					xml.node("info", [&] () { });

					xml.node("readonly_value", [&] {
						xml.attribute("name", "channels");
					});

					xml.node("readonly_value", [&] {
							 xml.attribute("name", "sample_rate");
					});

					xml.node("readonly_value", [&] {
						xml.attribute("name", "format");
					});

					xml.node("value", [&] {
						xml.attribute("name", "ifrag_total");
					});

					xml.node("value", [&] {
						 xml.attribute("name", "ifrag_size");
					});

					xml.node("readonly_value", [&] {
						 xml.attribute("name", "ifrag_avail");
					});

					xml.node("readonly_value", [&] {
						 xml.attribute("name", "ifrag_bytes");
					});

					xml.node("value", [&] {
						xml.attribute("name", "ofrag_total");
					});

					xml.node("value", [&] {
						 xml.attribute("name", "ofrag_size");
					});

					xml.node("readonly_value", [&] {
						 xml.attribute("name", "ofrag_avail");
					});

					xml.node("readonly_value", [&] {
						 xml.attribute("name", "ofrag_bytes");
					});

					xml.node("readonly_value", [&] {
						 xml.attribute("name", "optr_samples");
					});

					xml.node("readonly_value", [&] {
						 xml.attribute("name", "optr_fifo_samples");
					});

					xml.node("value", [&] {
						 xml.attribute("name", "play_underruns");
					});
				});
			});

			return Config(Genode::Cstring(buf));
		}

	public:

		Compound_file_system(Vfs::Env &vfs_env, Genode::Xml_node node)
		:
			Local_factory { vfs_env, node },
			Vfs::Dir_file_system { vfs_env,
			                       Xml_node(_config(Local_factory::name(node)).string()),
			                       *this }
		{ }

		static const char *name() { return "oss"; }

		char const *type() override { return name(); }
};


extern "C" Vfs::File_system_factory *vfs_file_system_factory(void)
{
	struct Factory : Vfs::File_system_factory
	{
		Vfs::File_system *create(Vfs::Env &env, Genode::Xml_node config) override
		{
			return new (env.alloc())
				Vfs::Oss_file_system::Compound_file_system(env, config);
		}
	};

	static Factory f;
	return &f;
}
