package answers;

import java.util.*;

// line Item is good following srp
class LineItem {
    String sku;
    int quantity;
    double unitPrice;

    LineItem(String sku, int quantity, double unitPrice) {
        this.sku = sku;
        this.quantity = quantity;
        this.unitPrice = unitPrice;
    }
}


interface ITax {
    double getTaxedAmount(double subTotalAfterDiscount);
}

class GST implements ITax {

    @Override
    public double getTaxedAmount(double subTotalAfterDiscount) {
        return subTotalAfterDiscount*0.18;
    }
}


interface IDiscount {
    double getDiscount(double subTotal);
}

class FlatOFF implements IDiscount {
    double v;
    FlatOFF(double v) {
        this.v=  v;
    }
    @Override
    public double getDiscount(double subTotal) {
        return v;
    }
}

class PercentageOFF implements IDiscount {

    private double percentage;

    PercentageOFF(double percentage) {
        this.percentage = percentage;
    }
    @Override
    public double getDiscount(double subTotal) {
        return (subTotal*percentage)/100;
    }
}

class Invoice {
    private List<LineItem> items;
    private List<IDiscount> discounts;
    private String email;
    private double tax;
    private double subTotal;
    private double discountTotal;

    public double getGrandTotal() {
        return subTotal - discountTotal + tax;
    }

    public double getTax() {
        return tax;
    }

    public void setTax(double tax) {
        this.tax = tax;
    }

    public double getSubTotal() {
        return subTotal;
    }

    public void setSubTotal(double subTotal) {
        this.subTotal = subTotal;
    }

    public double getDiscountTotal() {
        return discountTotal;
    }

    public void setDiscountTotal(double discountTotal) {
        this.discountTotal = discountTotal;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public List<IDiscount> getDiscounts() {
        return discounts;
    }

    public void setDiscounts(List<IDiscount> discounts) {
        this.discounts = discounts;
    }

    public List<LineItem> getItems() {
        return items;
    }

    public void setItems(List<LineItem> items) {
        this.items = items;
    }

    @Override
    public String toString() {
        return "Invoice{" +
                "items=" + items +
                ", discounts=" + discounts +
                ", email='" + email + '\'' +
                ", tax=" + tax +
                ", subTotal=" + subTotal +
                ", discountTotal=" + discountTotal +
                '}';
    }
}

interface IRender {
    String render(Invoice invoice);
}

class PDFRender implements IRender {

    @Override
    public String render(Invoice invoice) {
        // rendering inline (pretend PDF)
        StringBuilder pdf = new StringBuilder();
        pdf.append("INVOICE\n");
        for (LineItem it : invoice.getItems()) {
            pdf.append(it.sku).append(" x").append(it.quantity).append(" @ ").append(it.unitPrice).append("\n");
        }
        pdf.append("Subtotal: ").append(invoice.getSubTotal()).append("\n")
                .append("Discounts: ").append(invoice.getDiscountTotal()).append("\n")
                .append("Tax: ").append(invoice.getTax()).append("\n")
                .append("Total: ").append(invoice.getGrandTotal()).append("\n");

        return pdf.toString();
    }
}

class InvoiceService {
    private ITax taxService;
    private IRender renderService;
    private ISender senderService;

    InvoiceService(ITax taxService, IRender renderService, ISender senderService) {
        this.taxService = taxService;
        this.renderService = renderService;
        this.senderService = senderService;
    }

    String process(Invoice invoice) {

        // pricing
        double subtotal = 0.0;
        for (LineItem it : invoice.getItems()) subtotal += it.unitPrice * it.quantity;

        double discountTotal = 0.0;
        for(IDiscount discount : invoice.getDiscounts()) {
            discountTotal += discount.getDiscount(discountTotal);
        }

        double tax = taxService.getTaxedAmount(subtotal-discountTotal);

        invoice.setDiscountTotal(discountTotal);
        invoice.setTax(tax);
        invoice.setSubTotal(subtotal);

        String pdf = renderService.render(invoice);

        senderService.send(invoice);

        return pdf;
    }
}

interface ISender {
    void send(Invoice invoice);
}

class EmailSender implements ISender {

    @Override
    public void send(Invoice invoice) {
        System.out.println("[SMTP] Sending invoice to " + invoice.getEmail() + "...");
    }
}

public class BetterInvoiceSRPOCP {

    public static void main(String[] args) {
        InvoiceService svc = new InvoiceService(new GST(), new PDFRender(), new EmailSender());
        List<LineItem> items = Arrays.asList(
                new LineItem("BOOK-001", 2, 500.0),
                new LineItem("USB-DRIVE", 1, 799.0)
        );
        List<IDiscount> discounts = Arrays.asList(
                new PercentageOFF(10.0),
                new FlatOFF(100.0)
        );
        Invoice invoice = new Invoice();
        invoice.setEmail("customer@example.com");
        invoice.setDiscounts(discounts);
        invoice.setItems(items);
        System.out.println(svc.process(invoice));
    }
}
