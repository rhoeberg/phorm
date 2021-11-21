static int paCallback( const void *inBuffer, void *outBuffer,
					   unsigned long framesPerBuffer,
					   const PaStreamCallbackTimeInfo *timeInfo,
					   PaStreamCallbackFlags flags,
					   void *uData)
{
    AudioMixer *mixer = (AudioMixer*)uData;
    short *out = (short*)outBuffer;
    (void) inBuffer;
    
    for (int i = 0; i < framesPerBuffer; i++) {
		short outLeft = 0;
		short outRight = 0;
		for(int v = 0; v < VOICE_AMOUNT; v++) {
			AudioVoice *voice = &mixer->voices[v];
			if(voice->status == AUDIO_PLAYING) {
				if(voice->pos >= voice->length) {
					voice->status = AUDIO_FREE;
				}
				else {
					outLeft += voice->buffer[voice->pos++];
					outRight += voice->buffer[voice->pos++];
				}
			}
		}
		*out++ = outLeft;
		*out++ = outRight;
    }
    return 0;
}

bool audioInitialize()
{
	_audioState = (AudioState*)malloc(sizeof(AudioState));

    //
    // load vorbis file
    //
    // stb_vorbis_decode_filename returns number of frames 
    // to get total amount of samples we have to multiple with number
    // of channels
    int sampleRate;
    int channelCount;
    _audioState->sampleAmount = stb_vorbis_decode_filename("assets\\test.ogg", &channelCount, &sampleRate, &_audioState->buffer);
    printf("decoded vorbis file\n");
    printf("samplerate: %d\n", sampleRate);
    printf("channels: %d\n", channelCount);
    printf("sampleAmount: %d\n", _audioState->sampleAmount);

    //
    // initialize mixer
    //
    for(int i = 0; i < VOICE_AMOUNT; i++) {
		_audioState->mixer.voices[i].status = AUDIO_FREE;
    }

    //
    // initialize and start Pa stream
    //
    PaError err;
    err = Pa_Initialize();
    if(err != paNoError) {
		printf("port audio error, could not initialize\n");
		return false;
    }
    
    err = Pa_OpenDefaultStream( &_audioState->stream,
								0, // no input channels
								2, // stereo output
								paInt16, // 32 bit float output
								sampleRate, 
								64, //frames per buffer
								paCallback,
								&_audioState->mixer );
    if(err != paNoError) {
		printf("port audio error, coudl not open stream\n");
		return false;
    }

    err = Pa_StartStream(_audioState->stream);
    if(err != paNoError) {
    	printf("port audio error, could not start stream\n");
    }
    
    return true;
}

bool audioClose()
{
    PaError err = Pa_StopStream( _audioState->stream );
    if(err != paNoError) {
		printf("port audio error, could not stop stream\n");
		return false;
    }
    err = Pa_CloseStream(_audioState->stream);
    if(err != paNoError) {
		printf("portaudio error, could not close stream\n");
		return false;
    }

	free(_audioState);

    return true;
}

void playAudio(int n)
{
    _audioState->mixer.voices[n].buffer = _audioState->buffer;
    _audioState->mixer.voices[n].pos = 0;
    _audioState->mixer.voices[n].length = _audioState->sampleAmount * 2;
    _audioState->mixer.voices[n].status = AUDIO_PLAYING;
}
