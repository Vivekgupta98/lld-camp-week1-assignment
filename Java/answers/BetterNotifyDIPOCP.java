package answers;

interface IMailer {
    void send(String templ, String to, String body);
}

interface IOTP {
    void sendOTP(String phone, String code);
}

class SmtpMailer implements IMailer {

    @Override
    public void send(String templ, String to, String body) {
        System.out.println("[SMTP] template=" + templ + " to=" + to + " body=" + body);
    }
}

class TwilioClient implements IOTP {

    @Override
    public void sendOTP(String phone, String code) {
        System.out.println("[Twilio] OTP " + code + " -> " + phone);
    }
}

class User {
    String email;
    String phone;
    User(String email, String phone) { this.email = email; this.phone = phone; }
}

class SignUpService {
    private IMailer mailer;
    private IOTP sms;

    SignUpService(IMailer mailer, IOTP sms) {
        this.mailer = mailer;
        this.sms = sms;
    }

    boolean signUp(User u){
        if (u.email == null || u.email.isEmpty()) return false;
        // pretend DB save here…

        mailer.send("welcome", u.email, "Welcome!");
        sms.sendOTP(u.phone, "123456");
        return true;
    }
}

public class BetterNotifyDIPOCP {
    public static void main(String[] args) {
        SignUpService svc = new SignUpService(new SmtpMailer(), new TwilioClient());
        svc.signUp(new User("user@example.com", "+15550001111"));
    }
}
