// OALAudio.hpp
// KlayGE OpenAL声音引擎 头文件
// Ver 3.2.0
// 版权所有(C) 龚敏敏, 2003-2006
// Homepage: http://klayge.sourceforge.net
//
// 3.2.0
// 改进了OALMusicBuffer中线程的使用 (2006.4.29)
//
// 2.0.0
// 初次建立 (2003.7.7)
//
// 修改记录
/////////////////////////////////////////////////////////////////////////////////

#ifndef _OALAUDIO_HPP
#define _OALAUDIO_HPP

#define KLAYGE_LIB_NAME KlayGE_AudioEngine_OpenAL
#include <KlayGE/config/auto_link.hpp>

#include <KlayGE/PreDeclare.hpp>

#include <al/al.h>
#include <al/alc.h>

#include <vector>
#include <windows.h>

#include <boost/utility.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include <KlayGE/Audio.hpp>

namespace KlayGE
{
	ALenum Convert(AudioFormat format);
	float3 VecToALVec(float3 const & v);
	float3 ALVecToVec(float3 const & v);

	// 声音缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	class OALSoundBuffer : boost::noncopyable, public SoundBuffer
	{
	public:
		OALSoundBuffer(AudioDataSourcePtr const & dataSource, uint32_t numSource, float volume);
		~OALSoundBuffer();

		void Play(bool loop = false);
		void Stop();

		void Volume(float vol);

		bool IsPlaying() const;

		float3 Position() const;
		void Position(float3 const & v);
		float3 Velocity() const;
		void Velocity(float3 const & v);
		float3 Direction() const;
		void Direction(float3 const & v);

	private:
		void DoReset();
		std::vector<ALuint>::iterator FreeSource();

	private:
		std::vector<ALuint>	sources_;
		ALuint			buffer_;

		float3		pos_;
		float3		vel_;
		float3		dir_;
	};

	// 音乐缓冲区
	/////////////////////////////////////////////////////////////////////////////////
	class OALMusicBuffer : boost::noncopyable, public MusicBuffer
	{
	public:
		OALMusicBuffer(AudioDataSourcePtr const & dataSource, uint32_t bufferSeconds, float volume);
		~OALMusicBuffer();

		void Volume(float vol);

		bool IsPlaying() const;

		float3 Position() const;
		void Position(float3 const & v);
		float3 Velocity() const;
		void Velocity(float3 const & v);
		float3 Direction() const;
		void Direction(float3 const & v);

		void LoopUpdateBuffer();

	private:
		void DoReset();
		void DoPlay(bool loop);
		void DoStop();

	private:
		ALuint					source_;
		std::vector<ALuint>		bufferQueue_;

		bool		loop_;

		bool stopped_;
		boost::condition play_cond_;
		boost::mutex play_mutex_;
		boost::shared_ptr<boost::thread> play_thread_;
	};

	// 管理音频播放
	/////////////////////////////////////////////////////////////////////////////////
	class OALAudioEngine : boost::noncopyable, public AudioEngine
	{
	public:
		OALAudioEngine();
		~OALAudioEngine();

		std::wstring const & Name() const;

		float3 GetListenerPos() const;
		void SetListenerPos(float3 const & v);
		float3 GetListenerVel() const;
		void SetListenerVel(float3 const & v);
		void GetListenerOri(float3& face, float3& up) const;
		void SetListenerOri(float3 const & face, float3 const & up);
	};
}

#endif		// _OALAUDIO_HPP
