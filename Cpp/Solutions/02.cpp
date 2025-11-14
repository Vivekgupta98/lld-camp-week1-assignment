#include<iostream>
#include<string>
#include<stdexcept>

using namespace std;

class Player{
    public:
        virtual ~Player() = default;
        virtual void play(const string& source) = 0;
        virtual void pause() = 0;
};

class AudioPlayer : public Player{
    bool playing{false};
    public:
        void play(const string& source) override{
            (void)source;
            playing = true;
        }
        void pause() override{
            playing = false;
        }
        void download(const string& url){
            (void)url;
        }
        bool isPlaying() const {return playing;}
};

int main(){
    return 0;
}