// 01-invoice-srp-ocp.cpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

struct LineItem {
    string sku;
    int quantity{0};
    double unitPrice{0.0};
};

class InvoiceCalculator{
        vector<LineItem> items;
        double subtotal = 0.0;
    public:
        InvoiceCalculator( const vector<LineItem> &a) : items(a){
            for (auto& it : items) subtotal += it.unitPrice * it.quantity;
        }
        virtual double getDiscount() const = 0; 
    
        double getSubtotal() const{
            return subtotal;
        }

        virtual double getTax() const = 0;

        virtual double getGrand() const = 0;

        vector<LineItem> getItems() const{
            return items;
        }
};

class PercentDiscountInvoiceCalculator : public InvoiceCalculator{
    double discount;
    public:
        PercentDiscountInvoiceCalculator( const vector<LineItem> &a, const double &val) : InvoiceCalculator(a),discount(val) {}
        double getDiscount() const override{
            double subtotal = getSubtotal();
            double discount_total = subtotal*(discount/100.0);
            return discount_total;
        }

        double getTax() const override{
            double subtotal = getSubtotal();
            double discount_val = getDiscount();
            return (subtotal-discount_val)*0.18;
        }

        double getGrand() const override{
            double subtotal = getSubtotal();
            double discount_val = getDiscount();
            double tax = getTax();
            return (subtotal - discount_val + tax);
        }
        
};

class FlatDiscountInvoiceCalculator : public InvoiceCalculator{
    double flatDiscount;
    public:
        FlatDiscountInvoiceCalculator(const vector<LineItem> &a,const double &amt) : InvoiceCalculator(a), flatDiscount(amt){}
        double getDiscount() const override{
            return flatDiscount;
        }

        double getTax() const override{
            double subtotal = getSubtotal();
            return (subtotal-flatDiscount)*0.18;
        }

        double getGrand() const override{
            double subtotal = getSubtotal();
            double tax = getTax();
            return (subtotal - flatDiscount + tax);
        }

};

class NoDiscountInvoiceCalculator : public InvoiceCalculator{
    public:
        NoDiscountInvoiceCalculator(const vector<LineItem> &a) : InvoiceCalculator(a) {}
        double getDiscount() const override{
            return (double)(0.0);
        }

        double getTax() const override{
            double subtotal = getSubtotal();
            return (subtotal)*0.18;
        }

        double getGrand() const override{
            double subtotal = getSubtotal();
            double tax = getTax();
            return (subtotal + tax);
        }
};
class IInvoiceRenderer{
    public:
        virtual string renderInvoice(const InvoiceCalculator &invoice) = 0;
};
class InvoiceRenderer : public IInvoiceRenderer{
    ostringstream pdf;
    public:
        string renderInvoice(const InvoiceCalculator &invoice) override{
            pdf << "INVOICE\n";
            vector<LineItem> items = invoice.getItems();
            for (auto& it : items) {
                pdf << it.sku << " x" << it.quantity << " @ " << it.unitPrice << "\n";
            }
            pdf << "Subtotal: " << invoice.getSubtotal() << "\n"
                << "Discounts: " << invoice.getDiscount() << "\n"
                << "Tax: " << invoice.getTax() << "\n"
                << "Total: " << invoice.getGrand() << "\n";
            return pdf.str();
        }
};

class InvoiceNotifier{
    string address;
    public:
        InvoiceNotifier(const string &s):address(s){}
        virtual void send() = 0;
        string getAddress() const{
            return address;
        }
};
class EmailNotifier : public InvoiceNotifier{
    public:
        EmailNotifier(const string &s):InvoiceNotifier(s){}
        void send(){
            string email = getAddress();   
            if(!email.empty()){
                 cout << "[SMTP] Sending invoice to " << email << "...\n";
            }
        }
};

class IInvoiceLogger{
    public:
        virtual void log(const InvoiceCalculator &invoice,const InvoiceNotifier &notify) = 0;
};

class InvoiceLogging : public IInvoiceLogger{
    public:
        void log(const InvoiceCalculator &invoice,const InvoiceNotifier &notify) override{
         cout << "[LOG] Invoice processed for " << notify.getAddress() << " total=" << invoice.getGrand() << "\n";
        }
};

class InvoiceService{
    public:
        void processInvoice(InvoiceCalculator &invoice, InvoiceNotifier &notify , IInvoiceLogger &log, IInvoiceRenderer &render){
            string invoiceText = render.renderInvoice(invoice);
            cout<<invoiceText<<"\n";
            notify.send();
            log.log(invoice,notify);
        }      
};

int main() {
    InvoiceService service;
    vector<LineItem> items = { {"ITEM-001", 2, 150.0}, {"ITEM-002", 1, 200.0} };
    PercentDiscountInvoiceCalculator calc(items, 10.0);
    EmailNotifier mail("customer@example.com");
    InvoiceLogging logger;
    InvoiceRenderer renderer;
    service.processInvoice(calc,mail,logger,renderer);
    return 0;
}
