#include<iostream>
#include<string>
#include<vector>

using namespace std;

class NotifyService{
    public:
        virtual void sendNotification() = 0;
};

struct User{
    string email;
    string phone;
};

class SmtpMailer : public NotifyService{
    string email;
    string templ;
    string body;
   public:
    SmtpMailer(const string& templ, const string& email , const string& body ):templ(templ),body(body), email(email){}
    void sendNotification() override{
        cout << "[SMTP] template=" << templ << " to=" << email << " body=" << body << "\n";
    }
};

class IOtpSender{
    protected:
        string code;
    public:
        IOtpSender(){code = "random 6 digit code genrator";}
        virtual void sendOtp() = 0;
};

class TwilioClient : public IOtpSender{
    string phone;
    public:
        TwilioClient(const string& phn) : IOtpSender(),phone(phn){}
        void sendOtp() override{
            cout << "[Twilio] OTP " << code << " -> " << phone << "\n";
        }
};

class SignUpService{
    vector<NotifyService*> notifiers;
    vector<IOtpSender*> verifiers;
    
    public:
        SignUpService(const vector<NotifyService*> &nt,const vector<IOtpSender*> &ver):notifiers(move(nt)),verifiers(move(ver)){}
        bool signUp(const User& usr){
            if(usr.email.empty())return false;
            for(auto notify: notifiers){
                notify->sendNotification();
            }
            for(auto verify: verifiers){
                verify->sendOtp();
            }
            return true;
        }      
};

int main(){
    User user{"user@example.com", "+15550001111"};
    SmtpMailer smtpMailer("welcome", user.email, "Welcome!");
    TwilioClient twilioClient(user.phone);
    vector<NotifyService*> notifiers({&smtpMailer});
    vector<IOtpSender*> verfifiers({&twilioClient});
    SignUpService svc(notifiers,verfifiers);
    svc.signUp(user);
    return 0;
}