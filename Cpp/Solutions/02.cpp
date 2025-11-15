#include<iostream>
#include<string>
#include<stdexcept>

using namespace std;

class IPlayable {
    protected:
    bool playing{false};
    public:
        virtual ~IPlayable() = default;
        virtual void play(const string& source) = 0;
        virtual void pause() = 0;
        bool isPlaying() const { return playing; }
        
};

class IStreamable{
    public:
     virtual void streamLive(const string& url) = 0;
};

class IRecordable{
    public:
        virtual void record(const string& destination) = 0;
};

class IDownloadable{
    public:
        virtual void download(const string& resource) = 0;
};
class AudioPlayer : public  IDownloadable, public IPlayable{
    
    public:
        void play(const string& source) override{
            (void)source;
            playing=true;
        }
        void pause() override {playing = false;}
        void download(const string& resource) override{
            (void)resource;
        }

};

class CameraStreamPlayer : public IStreamable, public IRecordable,public IPlayable{
    bool liveStarted{false};
    public:
        void streamLive(const string& url) override { (void)url; liveStarted = true; }
        void play(const string& src) override {
            (void)src;
            cout<<"Camera is playing";
            streamLive(src);
            playing = true;
        }
        void pause() override{
            playing = false;
        }
        
        bool isLive() const { return liveStarted; }
        void record(const string& dest) override { (void)dest; /* pretend */ }

};



int main(){
    IPlayable* p = new AudioPlayer();
    p->play("song.mp3");
    cout << "Audio playing: " << boolalpha << p->isPlaying() << "\n";
    p->pause();

    p = new CameraStreamPlayer();
    p->play("rtsp://camera");

    return 0;
}